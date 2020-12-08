#include "gameplay.cpp"

// function for initializing the a game
ACTION ghostquest::initialize(name username)
{
    require_auth(username);
    // Create a record in the table if the player doesn't exist in our app yet
    auto itr = _users.find(username.value);

    users_table userstbl(_self, username.value);
    uint64_t gameid = userstbl.available_primary_key();

    check(itr == _users.end(), "Error : Either User has Initialized a Game or has an Existing Game");

    if (itr == _users.end())
    {
        _users.emplace(_self, [&](auto &new_users) {
            new_users.username = username;
            new_users.game_id = gameid;
            // new_users.game_id = generategameid(); // generate user game_id
        });
    }
}

// function for setting number of summons of monster
ACTION ghostquest::summoncount(name username, uint64_t summoncount, uint64_t battlelimit)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");

    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.summon_count = summoncount;
    });
}

ACTION ghostquest::getstat(name username, asset quantity) // generate stats of monsters after transfer transaction
{
    require_auth(_self);

    auto &user = _users.get(username.value, "User doesn't exist");
    auto itr = _users.find(username.value);

    check(itr != _users.end(), "Game Doesn't Exist.");
    check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    check(itr->game_data.summon_count == (quantity.amount / 10000), "Deposited amount does not match the selected number of summons.");
    _users.modify(itr, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        // vector<ghost> new_character;                                                            // summon monster/monsters
        for (int i = game_data.monster_count + 1; i < (1 + game_data.monster_count + game_data.summon_count); i++) // summon monster/monsters and hitpoints
        {
            ghost new_ghost;
            new_ghost.owner = user.username;
            new_ghost.prize.amount = 10000;
            // new_ghost.battle_limit = 5;
            new_ghost.status = SUMMONED;
            new_ghost.character_life = 1;
            new_ghost.status = STANDBY;
            new_ghost.initial_hp = 100 + rng(50);
            genstat(new_ghost);
            game_data.character.insert(game_data.character.end(), pair<int, ghost>(i, new_ghost));
        }

        game_data.monster_count += game_data.summon_count;
        game_data.summon_count = 0;
    });
}

// ACTION ghostquest::setaddlife(name username, ghost &ghostsel, int life_to_add) // generate stats of monsters after transfer transaction
// {
//     require_auth(_self);

//     auto &user = _users.get(username.value, "User doesn't exist");
//     auto itr = _users.find(username.value);

//     check(itr != _users.end(), "Game Doesn't Exist.");
//     check(ghostsel.character_life > 0, "Character cease to exist..");
//     check(ghostsel.status == IDLE, "Character cease to exist or currently in battle.");
//     // check(life_to_add == (quantity.amount / 10000), "Deposited amount does not match the desired life to add..");

//     _users.modify(itr, username, [&](auto &modified_user) {
//         game &game_data = modified_user.game_data;
//         game_data.temp_add_life = life_to_add;
//     });
// }

ACTION ghostquest::battle(name username1, name username2, int ghost1_key, int ghost2_key) // function for generating monster/s status
{
    require_auth(_self);
    auto &user1 = _users.get(username1.value, "User doesn't exist.");
    auto &user2 = _users.get(username2.value, "User doesn't exist.");
    auto characters1 = user1.game_data.character;
    map<int, ghost>::iterator itr1 = characters1.find(ghost1_key);
    auto characters2 = user2.game_data.character;
    map<int, ghost>::iterator itr2 = characters2.find(ghost2_key);
    itr1->second.hitpoints = itr1->second.initial_hp;
    itr2->second.hitpoints = itr2->second.initial_hp;
    battle_step(itr1, itr2);
    itr1->second.last_match = current_time_point().elapsed._count;
    itr2->second.last_match = current_time_point().elapsed._count;
    _users.modify(user1, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;

        game_data.character = characters1;
        if (itr1->second.status == ELIMINATED)
        {
            game_data.monster_count -= 1;
        }
        else
        {
            game_data.monster_count = game_data.monster_count;
        }
    });
    _users.modify(user2, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character = characters2;
        if (itr2->second.status == ELIMINATED)
        {
            game_data.monster_count -= 1;
        }
        else
        {
            game_data.monster_count = game_data.monster_count;
        }
    });
}

ACTION ghostquest::settledpay(name username, asset prize, string memo)
{
    require_auth(_self);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(user.game_data.status == ONGOING, "Game has ended and prize is already transferred.");

    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.monster_count -= 1;
    });
}

ACTION ghostquest::withdraw(name username, int key)
{
    require_auth(username);
    auto user = _users.find(username.value);
    // check(user->game_data.character.at(idx).prize.amount > 0, "Ghost doesn't exist.");
    std::string feedback = "GQ Withdraw: " + name{username}.to_string() + " received " + std::to_string(user->game_data.character.find(key)->second.prize.amount);
    action{
        permission_level{_self, "active"_n},
        eosio_token,
        "transfer"_n,
        std::make_tuple(_self, username, user->game_data.character.find(key)->second.prize, feedback)}
        .send();
    auto characters = user->game_data.character;
    map<int, ghost>::iterator itr = characters.find(key);
    eliminated_withdrawn(itr);
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character = characters;
    });
}

ACTION ghostquest::end(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(user.game_data.status == DONE, "End your existing game first.");
    _users.erase(user);
}