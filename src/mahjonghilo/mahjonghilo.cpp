#include "gameplay.cpp"

// function for initializing the a game
ACTION mahjonghilo::initialize(name username)
{
    require_auth(username);
    // Create a record in the table if the player doesn't exist in our app yet
    auto itr = _users.find(username.value);
    check(itr == _users.end(), "Either User has Initialized a Game or has an Existing Game");

    if (itr == _users.end())
    {
        _users.emplace(_self, [&](auto &new_users) {
            new_users.username = username;
        });
    }
}

ACTION mahjonghilo::startgame(name username)
{
    // Ensure this action is authorized by the player
    require_auth(username);

    auto &user = _users.get(username.value, "User doesn't exist");
    uint64_t id = 0;
    auto size = transaction_size();
    char buf[size];
    check(size == read_transaction(buf, size), "read_transaction failed");
    checksum256 h = sha256(buf, size);
    auto hbytes = h.extract_as_byte_array();
    string hash_string = checksum256_to_string(hbytes, hbytes.size()); // convert txID arr to string
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.game_id = hash_string.substr(0, 30) + to_string(rng(100));
        game_data.status = INITIALIZED;
        gettile(game_data.deck_player, game_data.hand_player);
        game_data.standard_tile = game_data.hand_player[0];
    });
}

ACTION mahjonghilo::playhilo(name username, int option)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        gettile(game_data.deck_player, game_data.hand_player);
        const auto hilo_tile = table_deck.at(game_data.hand_player[1]);

        const auto current_tile = table_deck.at(game_data.hand_player[0]);
        hi_lo_step(hilo_tile.tile_value, current_tile.tile_value);
        game_data.standard_tile = game_data.hand_player[0];
    });
}

ACTION mahjonghilo::discardtile(name username, int idx)
{
    require_auth(username);

    auto &user = _users.get(username.value, "User doesn't exist");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.discarded_tiles.insert(game_data.discarded_tiles.begin(), game_data.hand_player[idx]);
        game_data.hand_player.erase(game_data.hand_player.begin() + idx); // Remove the card from the hand
        gettile(game_data.deck_player, game_data.hand_player);
    });
}

ACTION mahjonghilo::settledpay(name username, asset prize, string memo)
{
    require_auth(_self);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == ONGOING, "Game has ended and prize is already transferred.");
}

ACTION mahjonghilo::withdraw(name username)
{
    require_auth(username);
    auto user = _users.find(username.value);
    // action{
    //     permission_level{_self, "active"_n},
    //     eosio_token,
    //     "transfer"_n,
    //     std::make_tuple(_self, username, user.game_data.hi_lo_prize, feedback)}
    //     .send();
}

ACTION mahjonghilo::end(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(user.game_data.status == DONE, "End your existing game first.");
    _users.erase(user);
}