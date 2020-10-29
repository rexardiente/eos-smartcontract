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
    check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");

    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.summon_count = summoncount;
        vector<ghost> new_character;          // summon monster/monsters
        for (int i = 0; i < summoncount; i++) // summon monster/monsters, set level and hitpoints
        {

            ghost new_ghost;
            new_ghost.key = i;
            int rndmlvl = rng(99) + 1;
            new_ghost.ghost_class = rng(3) + 1;
            print(rndmlvl);
            if (rndmlvl < 15)
            {
                new_ghost.ghost_level = 1;
                new_ghost.hitpoints = 110;
            }
            else if (rndmlvl > 14 && rndmlvl < 45)
            {
                new_ghost.ghost_level = 2;
                new_ghost.hitpoints = 120;
            }
            else if (rndmlvl > 44 && rndmlvl < 85)
            {
                new_ghost.ghost_level = 3;
                new_ghost.hitpoints = 130;
            }
            else if (rndmlvl > 84 && rndmlvl < 96)
            {
                new_ghost.ghost_level = 4;
                new_ghost.hitpoints = 140;
            }
            else
            {
                new_ghost.ghost_level = 5;
                new_ghost.hitpoints = 150;
            }
            new_character.insert(new_character.end(), new_ghost);
        }
        game_data.character = new_character;
    });
}

// ACTION ghostquest::battlelimit(name username, uint64_t battlelimit)
// {
//     require_auth(username);
//     auto &user = _users.get(username.value, "User doesn't exist");
//     check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
//     check(user.game_data.summon_count > 0, "Set number of summons first.");

//     _users.modify(user, username, [&](auto &modified_user) {
//         modified_user.game_data.character.battle_limit = battlelimit;
//     });
// }

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
        game_data.status = ONGOING;
        genstat(game_data);
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