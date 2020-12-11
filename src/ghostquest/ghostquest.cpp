#include "gameplay.cpp"

// function for initializing the a game
ACTION ghostquest::initialize(name username)
{
    require_auth(username);
    // Create a record in the table if the player doesn't exist in our app yet
    auto itr = _users.find(username.value);

    // users_table userstbl(_self, username.value);
    // uint64_t gameid = _users.available_primary_key();
    check(itr == _users.end(), "Error : Either User has Initialized a Game or has an Existing Game");

    if (itr == _users.end())
    {
        _users.emplace(_self, [&](auto &new_users) {
            new_users.username = username;
            // new_users.game_id = _users.available_primary_key();
            // new_users.game_id = generategameid(); // generate user game_id
        });
    }
}

ACTION ghostquest::genchar(name username, asset quantity, int limit) // generate characters after transfer transaction
{
    require_auth(_self);

    auto &user = _users.get(username.value, "User doesn't exist");
    auto itr = _users.find(username.value);
    int counter = user.game_data.character.size();
    check(itr != _users.end(), "Game Doesn't Exist.");
    _users.modify(itr, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        for (int i = counter; i < (counter + (quantity.amount / 10000)); i++) // summon character/characters and hitpoints
        {
            ghost new_ghost;
            // new_ghost.ghost_id = modified_user.game_id + i;
            new_ghost.owner = user.username;
            new_ghost.prize.amount = 10000;
            new_ghost.battle_limit = limit;
            new_ghost.status = SUMMONED;
            new_ghost.character_life = 1;
            new_ghost.status = STANDBY;
            new_ghost.initial_hp = 100 + rng(50);
            gen_stat(new_ghost); // generate status for character
            game_data.character.insert(game_data.character.end(), pair<int, ghost>(i + 1, new_ghost));
        }
    });
}

ACTION ghostquest::addlife(name username, asset quantity, int key) // add life/lives after transfer transaction
{
    require_auth(_self);

    auto &user = _users.get(username.value, "User doesn't exist");
    auto characters = user.game_data.character;
    map<int, ghost>::iterator itr = characters.find(key);
    check(itr->second.character_life > 0, "Character cease to exist.");
    itr->second.character_life += quantity.amount / 10000;

    _users.modify(user, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character = characters;
    });
}

ACTION ghostquest::battle(vector<pair<int, name>> &players, string gameid) // battle action
{
    require_auth(_self);

    battle_history current_battle;
    const uint64_t time_executed = current_time_point().elapsed._count; // track current time of execution..
    auto &player1 = _users.get(players[0].second.value, "User doesn't exist.");
    auto &player2 = _users.get(players[1].second.value, "User doesn't exist.");

    map<int, ghost> player1_characters = player1.game_data.character;
    map<int, ghost> player2_characters = player2.game_data.character;

    vector<map<int, ghost>::iterator> itr{player1_characters.find(players[0].first), player2_characters.find(players[1].first)};
    itr[0]->second.hitpoints = itr[0]->second.initial_hp;
    itr[1]->second.hitpoints = itr[1]->second.initial_hp;

    check(itr[0]->second.match_history.find(itr[1]->second.ghost_id) == itr[0]->second.match_history.end(), "Enemy already fought before.");
    check(itr[1]->second.match_history.find(itr[0]->second.ghost_id) == itr[1]->second.match_history.end(), "Enemy already fought before.");
    // check(itr[0]->second.character_life == itr[1]->second.character_life, "Match not allowed.");
    check(itr[0]->second.battle_count <= itr[0]->second.battle_limit, "Battle limit reached.");
    check(itr[1]->second.battle_count <= itr[1]->second.battle_limit, "Battle limit reached.");

    current_battle.game_id = gameid;
    current_battle.time_executed = time_executed;
    battle_step(itr[0], itr[1], current_battle);
    // update each players battle history..
    for_each(itr.begin(), itr.end(), [&](map<int, ghost>::iterator &n) {
        n->second.match_history.insert(n->second.match_history.end(), pair<uint64_t, battle_history>(n->second.ghost_id == itr[0]->second.ghost_id ? itr[1]->second.ghost_id : itr[0]->second.ghost_id, current_battle));
        n->second.last_match = time_executed;
    });

    if (itr[0]->second.character_life > player1.game_data.character.at(players[0].first).character_life)
    {
        itr[0]->second.match_history.at(itr[1]->second.ghost_id).isWin = true;
        itr[1]->second.match_history.at(itr[0]->second.ghost_id).isWin = false;
    }
    else
    {
        itr[1]->second.match_history.at(itr[0]->second.ghost_id).isWin = true;
        itr[0]->second.match_history.at(itr[1]->second.ghost_id).isWin = false;
    }
    itr[0]->second.match_history.at(itr[1]->second.ghost_id).enemy = players[1].second;
    itr[1]->second.match_history.at(itr[0]->second.ghost_id).enemy = players[0].second;
    // itr[0]->second.last_match = current_time_point().elapsed._count; // time of the last match
    // itr[1]->second.last_match = current_time_point().elapsed._count; // time of the last match
    // itr[1]->second.ghost_id
    // if (n->second.ghost_id == itr[0]->second.ghost_id)
    //     n->second.match_history.at(itr[1]->second.ghost_id).enemy = players[1].second;
    // else
    //     n->second.match_history.at(itr[0]->second.ghost_id).enemy = players[0].second;

    _users.modify(player1, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character = player1_characters;
    });
    _users.modify(player2, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character = player2_characters;
    });
}

ACTION ghostquest::settledpay(name username, asset prize, string memo)
{
    require_auth(_self);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(user.game_data.status == ONGOING, "Game has ended and prize is already transferred.");
}

ACTION ghostquest::withdraw(name username, int key)
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
    map<int, ghost>::iterator itr = characters.find(key);
    eliminated_withdrawn(itr); // modify withdrawn character
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character = characters;
    });
}

ACTION ghostquest::eliminate(name username, int key) // generate stats of monsters after transfer transaction
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