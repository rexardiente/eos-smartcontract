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

ACTION mahjonghilo::startgame(name username, int numgames)
{
    // Ensure this action is authorized by the player
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == INITIALIZED, "Game already started");
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
        game_data.status = ONGOING;
        game_data.hi_lo_prize.amount = 10000;
        int num1 = numgames % 16;
        int num2 = numgames % 4;
        if (num1 > 0 && num1 < 5)
        {
            game_data.prevalent_wind = EAST;
        }
        else if (num1 > 4 && num1 < 9)
        {
            game_data.prevalent_wind = SOUTH;
        }
        else if (num1 > 8 && num1 < 13)
        {
            game_data.prevalent_wind = WEST;
        }
        else
        {
            game_data.prevalent_wind = NORTH;
        }
        if (num2 == 1)
        {
            game_data.seat_wind = EAST;
        }
        else if (num2 == 2)
        {
            game_data.seat_wind = SOUTH;
        }
        else if (num2 == 3)
        {
            game_data.seat_wind = WEST;
        }
        else
        {
            game_data.seat_wind = NORTH;
        }
        gettile(game_data);
        game_data.standard_tile = game_data.current_tile;
        const auto hilo_tile = table_deck.at(game_data.standard_tile);
        get_odds(game_data, hilo_tile.value);
    });
}

ACTION mahjonghilo::starttrial(name username, vector<int> idx)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == INITIALIZED || user.game_data.status == ONTRIAL, "Cannot do a trial. Initialize a new game.");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        if (game_data.status == INITIALIZED)
        {
            game_data.status = ONTRIAL;
        }
        else
        {
            if (game_data.winning_hand.size() != 0)
            {
                int size = game_data.winning_hand.size();
                for (int i = 0; i < size; i++)
                {
                    game_data.winning_hand.erase(game_data.winning_hand.begin());
                }
                game_data.pair_count = 0;
                game_data.pung_count = 0;
                game_data.chow_count = 0;
            }
            else
            {
                int size = game_data.hand_player.size();
                for (int i = 0; i < size; i++)
                {
                    game_data.hand_player.erase(game_data.hand_player.begin());
                }
            }
        }
        for (int i = 0; i < idx.size(); i++)
        {
            game_data.hand_player.insert(game_data.hand_player.begin(), game_data.deck_player[idx[i] - 1]); // Assign the tile to the first empty slot in the hand
        }
        sorthand(game_data.hand_player);
        winhand_check(game_data, game_data.hand_player);
        // game_data.winning_hand = temp_hand;
        if (game_data.hand_player.size() == 0)
        {
            print("Well played!");
            getscore(game_data, game_data.winning_hand);
            sorthand(game_data.score_check);
            // print(game_data.final_score);
        }
        else
        {
            print("Your hand didn't win..");
        }
    });
}

ACTION mahjonghilo::playhilo(name username, int option)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.discarded_tiles.size() < 20, "Max number of draws reached.");
    check(user.game_data.hand_player.size() < (14 + user.game_data.kong_count - user.game_data.reveal_kong.size()), "Discard a tile to draw a new one.");
    check(user.game_data.status == ONGOING, "Game may haven't started yet, may on an ongoing trial, or may already ended.");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        gettile(game_data);
        const auto current_tile = table_deck.at(game_data.current_tile);
        if (option != 0 && game_data.hi_lo_prize.amount != 0)
        {
            const auto hilo_tile = table_deck.at(game_data.standard_tile);
            hilo_step(game_data, hilo_tile.value, current_tile.value, option);
        }
        game_data.standard_tile = game_data.current_tile;
        get_odds(game_data, current_tile.value);
    });
}

ACTION mahjonghilo::discardtile(name username, int idx)
{
    require_auth(username);

    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == ONGOING, "Game may haven't started yet, may on an ongoing trial, or may already ended.");
    check(user.game_data.discarded_tiles.size() < 19, "Your hand is for declaration(win/lose).");
    check(user.game_data.hand_player.size() == (14 + user.game_data.kong_count - user.game_data.reveal_kong.size()), "Have a complete hand before discarding a tile.");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.discarded_tiles.insert(game_data.discarded_tiles.begin(), game_data.hand_player[idx]);
        game_data.hand_player.erase(game_data.hand_player.begin() + idx); // Remove the card from the hand
    });
}

ACTION mahjonghilo::dclrkong(name username, vector<int> idx)
{
    require_auth(username);

    auto &user = _users.get(username.value, "User doesn't exist");
    vector<tile> kongtile{};
    check(user.game_data.status == ONGOING, "Game may haven't started yet, may on an ongoing trial, or may already ended.");
    check(user.game_data.hand_player.size() == (14 + user.game_data.kong_count - user.game_data.reveal_kong.size()), "Must have a complete hand to declare a kong.");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        for (int i = 0; i < 4; i++)
        {
            kongtile[i] = table_deck.at(game_data.hand_player[idx[i]]);
        }
        // const auto kongtile1 = table_deck.at(game_data.hand_player[idx[0]]);
        // const auto kongtile2 = table_deck.at(game_data.hand_player[idx[1]]);
        // const auto kongtile3 = table_deck.at(game_data.hand_player[idx[2]]);
        // const auto kongtile4 = table_deck.at(game_data.hand_player[idx[3]]);
        int check1 = pair_check(kongtile[0], kongtile[1]);
        int check2 = pair_check(kongtile[2], kongtile[3]);
        check(check1 == 1 && check2 == 1, "Tiles are not of the same suit and value.");
        if (check1 == check2)
        {
            for (int i = 0; i < 4; i++)
            {
                game_data.reveal_kong.insert(game_data.reveal_kong.begin(), game_data.hand_player[idx[i]]);
            }
            for (int i = 3; i >= 0; i--)
            {
                game_data.hand_player.erase(game_data.hand_player.begin() + idx[i]);
            }
            game_data.kong_count += 1;
        }
        else
        {
            print("Tiles are not of the same suit and value.");
        }
        sorthand(game_data.reveal_kong);
    });
}

ACTION mahjonghilo::dclrwinhand(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == ONGOING, "Game may haven't started yet, may on an ongoing trial, or may already ended.");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        winhand_check(game_data, game_data.hand_player);
        if (game_data.hand_player.size() == 0)
        {
            vector<uint8_t> temp_hand = game_data.winning_hand;
            sorteye(temp_hand, game_data.eye_idx);
            getscore(game_data, temp_hand);
            for (int i = 0; i < game_data.reveal_kong.size(); i++)
            {
                game_data.winning_hand.insert(game_data.winning_hand.end(), game_data.reveal_kong[i]);
            }
            for (int i = 0; i < game_data.reveal_kong.size(); i++)
            {
                game_data.reveal_kong.erase(game_data.reveal_kong.begin());
            }
            sorteye(temp_hand, game_data.eye_idx);
            // getscore(game_data, temp_hand);
            print("Well played!");
        }
        else
        {
            print("Your hand didn't win..");
        }
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