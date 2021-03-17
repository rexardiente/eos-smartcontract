#include "gameplay.cpp"

// function for initializing the a game
ACTION ghostquest::initialize(name username)
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

ACTION ghostquest::genchar(name username, asset quantity, int limit) // generate characters after transfer transaction
{
    require_auth(_self);
    auto itr = _users.find(username.value);
    check(itr != _users.end(), "User or Game Doesn't Exist.");
    uint64_t id = 0;
    auto size = transaction_size();
    char buf[size];
    check(size == read_transaction(buf, size), "read_transaction failed");
    checksum256 h = sha256(buf, size);
    auto hbytes = h.extract_as_byte_array();
    string hash_string = checksum256_to_string(hbytes, hbytes.size()); // convert txID arr to string

    _users.modify(itr, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        int counter = game_data.character.size();
        auto current_time = current_time_point().time_since_epoch()._count;
        for (int i = counter; i < (counter + (quantity.amount / 10000)); i++) // summon character/characters and hitpoints
        {
            ghost new_ghost;
            string key = hash_string.substr(0, 30) + to_string(i);
            new_ghost.owner = modified_user.username;
            new_ghost.prize.amount = 10000;
            new_ghost.battle_limit = limit;
            new_ghost.character_life = 1;
            new_ghost.status = STANDBY;
            new_ghost.initial_hp = 100 + rng(50);
            new_ghost.created_at = current_time;
            gen_stat(new_ghost); // generate status for character
            game_data.character.insert(game_data.character.end(), pair<string, ghost>(key, new_ghost));
        }
    });
}

ACTION ghostquest::addlife(name username, asset quantity, string key) // add life/lives after transfer transaction
{
    require_auth(_self);

    auto &user = _users.get(username.value, "User doesn't exist");
    auto characters = user.game_data.character;
    map<string, ghost>::iterator itr = characters.find(key);
    check(itr->second.character_life > 0, "Character cease to exist.");
    itr->second.character_life += quantity.amount / 10000;

    _users.modify(user, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character = characters;
    });
}

ACTION ghostquest::battle(string gameid, pair<string, name> winner, pair<string, name> loser, vector<string> logs) // battle action
{
    require_auth(_self);
    check(winner.second != loser.second, "Same Character are not allowed to match");
    check(winner.first != loser.first, "Same Character are not allowed to match");
    auto &winner_player = _users.get(winner.second.value, "User doesn't exist.");
    auto &loser_player = _users.get(loser.second.value, "User doesn't exist.");

    map<string, ghost> winner_player_characters = winner_player.game_data.character;
    map<string, ghost> loser_player_characters = loser_player.game_data.character;

    vector<map<string, ghost>::iterator> itr{winner_player_characters.find(winner.first), loser_player_characters.find(loser.first)};

    check(itr[0]->second.match_history.find(itr[1]->first) == itr[0]->second.match_history.end(), "Enemy already fought before.");
    check(itr[1]->second.match_history.find(itr[0]->first) == itr[1]->second.match_history.end(), "Enemy already fought before.");
    check(itr[0]->second.character_life == itr[1]->second.character_life, "Match not allowed.");
    check(itr[0]->second.battle_count <= itr[0]->second.battle_limit, "Battle limit reached.");
    check(itr[1]->second.battle_count <= itr[1]->second.battle_limit, "Battle limit reached.");

    battle_history current_battle;
    auto time_executed = current_time_point().time_since_epoch()._count;
    current_battle.time_executed = time_executed;
    current_battle.gameplay_log = logs;

    // update each players battle history..
    for_each(itr.begin(), itr.end(), [&](map<string, ghost>::iterator &n) {
        n->second.match_history.insert(n->second.match_history.end(), pair<string, battle_history>(gameid, current_battle));
        n->second.last_match = time_executed;
        n->second.battle_count += 1;
    });

    _users.modify(winner_player, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;

        itr[0]->second.character_life += 1;
        itr[0]->second.match_history.at(gameid).enemy = loser.second;
        itr[0]->second.match_history.at(gameid).enemy_id = loser.first;
        itr[0]->second.match_history.at(gameid).isWin = true;
        calculate_prize(itr[0]);
        game_data.character = winner_player_characters;
    });
    _users.modify(loser_player, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;

        itr[1]->second.match_history.at(gameid).enemy = winner.second;
        itr[1]->second.match_history.at(gameid).enemy_id = winner.first;
        itr[1]->second.match_history.at(gameid).isWin = false;
        itr[1]->second.prize = DEFAULT_ASSET;

        if (itr[1]->second.character_life > 1)
        {
            itr[1]->second.character_life -= 1;
        }
        else
        {
            itr[1]->second.status = ELIMINATED;
            itr[1]->second.character_life = 0;
        }
        game_data.character = loser_player_characters;
    });
}

ACTION ghostquest::settledpay(name username, asset prize, string memo)
{
    require_auth(_self);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(user.game_data.status == ONGOING, "Game has ended and prize is already transferred.");
}

ACTION ghostquest::withdraw(name username, string key)
{
    require_auth(username);
    auto user = _users.find(username.value);
    check(user->game_data.character.find(key)->second.character_life > 0, "Ghost doesn't exist.");
    check(user->game_data.character.find(key)->second.status != 3, "Ghost is currently in battle.");
    check(user->game_data.character.find(key)->second.battle_count > 0, "Ghost has not been in a battle.");
    std::string feedback = "GQ Withdraw: " + name{username}.to_string() + " received " + std::to_string(user->game_data.character.find(key)->second.prize.amount); // transfer funds to user
    action{
        permission_level{_self, "active"_n},
        eosio_token,
        "transfer"_n,
        std::make_tuple(_self, username, user->game_data.character.find(key)->second.prize, feedback)}
        .send();
    auto characters = user->game_data.character;
    map<string, ghost>::iterator itr = characters.find(key);
    itr->second.character_life = 0;
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character = characters;
    });
}

ACTION ghostquest::eliminate(name username, string key) // generate stats of monsters after transfer transaction
{
    require_auth(_self);

    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.character.at(key).status != 3, "Character in battle.");

    _users.modify(user, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character.erase(key);
    });
}

ACTION ghostquest::end(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(user.game_data.status == DONE, "End your existing game first.");
    _users.erase(user);
}