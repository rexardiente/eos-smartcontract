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
ACTION ghostquest::summoncount(name username, uint64_t summoncount)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");

    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.summon_count = summoncount;
        // vector<ghost> new_character;                                                            // summon monster/monsters
        for (int i = game_data.monster_count + 1; i < (1 + game_data.monster_count + summoncount); i++) // summon monster/monsters and hitpoints
        {

            ghost new_ghost;
            new_ghost.ghost_id = user.username;
            new_ghost.status = SUMMONED;
            new_ghost.hitpoints = 100 + (rng(49) + 1);
            // int rndmlvl = rng(99) + 1;
            // new_ghost.ghost_class = rng(3) + 1;
            // if (rndmlvl < 15)
            // {
            //     new_ghost.ghost_level = 1;
            //     new_ghost.status = 201 + rng(24);
            // }
            // else if (rndmlvl > 14 && rndmlvl < 45)
            // {
            //     new_ghost.ghost_level = 2;
            //     new_ghost.status = 226 + rng(74);
            // }
            // else if (rndmlvl > 44 && rndmlvl < 85)
            // {
            //     new_ghost.ghost_level = 3;
            //     new_ghost.status = 301 + rng(74);
            // }
            // else if (rndmlvl > 84 && rndmlvl < 96)
            // {
            //     new_ghost.ghost_level = 4;
            //     new_ghost.status = 376 + rng(49);
            // }
            // else
            // {
            //     new_ghost.ghost_level = 5;
            //     new_ghost.status = 426 + rng(24);
            // }
            game_data.character.insert(game_data.character.end(), new_ghost);
        }
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

    _users.modify(itr, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        for (int i = game_data.monster_count; i < (game_data.monster_count + game_data.summon_count); i++)
        {
            game_data.character.at(i).key = i + 1;
            game_data.character.at(i).status = STANDBY;
            genstat(game_data.character.at(i));
        }
        game_data.monster_count += game_data.summon_count;
    });
}

ACTION ghostquest::findmatch(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");

    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.character.at(1).status = INBATTLE;
        game_data.character.at(2).status = INBATTLE;
        battle(game_data.character.at(1), game_data.character.at(2));
    });
}

ACTION ghostquest::settledpay(name username, asset prize, string memo)
{
    require_auth(_self);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == ONGOING, "Game has ended and prize is already transferred.");

    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.status = DONE;
    });
}

ACTION ghostquest::end(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(user.game_data.status == DONE, "End your existing game first.");
    _users.erase(user);
}