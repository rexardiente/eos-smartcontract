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
            new_users.game_id = _users.available_primary_key();
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
            new_ghost.ghost_id = modified_user.game_id + i;
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

ACTION ghostquest::battle(name username1, int ghost1_key, name username2, int ghost2_key) // battle action
{
    require_auth(_self);
    battle_history current_battle;
    auto &user1 = _users.get(username1.value, "User doesn't exist.");
    auto &user2 = _users.get(username2.value, "User doesn't exist.");
    auto characters1 = user1.game_data.character;
    map<int, ghost>::iterator itr1 = characters1.find(ghost1_key);
    auto characters2 = user2.game_data.character;
    map<int, ghost>::iterator itr2 = characters2.find(ghost2_key);
    itr1->second.hitpoints = itr1->second.initial_hp;
    itr2->second.hitpoints = itr2->second.initial_hp;
    check(itr1->second.match_history.find(itr2->second.ghost_id) == itr1->second.match_history.end(), "Enemy already fought before.");
    check(itr2->second.match_history.find(itr1->second.ghost_id) == itr2->second.match_history.end(), "Enemy already fought before.");
    check(itr1->second.character_life == itr2->second.character_life, "Match not allowed.");
    check(itr1->second.battle_count <= itr1->second.battle_limit, "Battle limit reached.");
    check(itr2->second.battle_count <= itr2->second.battle_limit, "Battle limit reached.");
    current_battle.time_start = current_time_point().elapsed._count;
    battle_step(itr1, itr2, current_battle);
    current_battle.time_end = current_time_point().elapsed._count;
    itr1->second.match_history.insert(itr1->second.match_history.end(), pair<uint64_t, battle_history>(itr2->second.ghost_id, current_battle)); // insert enemy into battle history
    itr2->second.match_history.insert(itr2->second.match_history.end(), pair<uint64_t, battle_history>(itr1->second.ghost_id, current_battle)); // insert enemy into battle history
    if (itr1->second.character_life > user1.game_data.character.at(ghost1_key).character_life)
    {
        itr1->second.match_history.at(itr2->second.ghost_id).isWin = true;
        itr2->second.match_history.at(itr1->second.ghost_id).isWin = false;
    }
    else
    {
        itr2->second.match_history.at(itr1->second.ghost_id).isWin = true;
        itr1->second.match_history.at(itr2->second.ghost_id).isWin = false;
    }
    itr1->second.last_match = current_time_point().elapsed._count; // time of the last match
    itr2->second.last_match = current_time_point().elapsed._count; // time of the last match
    _users.modify(user1, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character = characters1;
    });
    _users.modify(user2, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character = characters2;
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