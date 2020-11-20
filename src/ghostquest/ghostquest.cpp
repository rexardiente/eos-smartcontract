#include "gameplay.cpp"
// #include <chrono>

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
    // check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");

    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.summon_count = summoncount;
        // vector<ghost> new_character;                                                            // summon monster/monsters
        for (int i = game_data.monster_count + 1; i < (1 + game_data.monster_count + summoncount); i++) // summon monster/monsters and hitpoints
        {

            ghost new_ghost;
            new_ghost.ghost_id = user.username;
            new_ghost.prize.amount = 10000;
            new_ghost.battle_limit = battlelimit;
            new_ghost.status = SUMMONED;
            new_ghost.character_life = 1;
            new_ghost.initial_hp = 100 + rng(50);
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
        game_data.character.at(1).hitpoints = game_data.character.at(1).initial_hp;
        game_data.character.at(4).status = INBATTLE;
        game_data.character.at(4).hitpoints = game_data.character.at(4).initial_hp;
        battle(game_data.character.at(1), game_data.character.at(4));
        // game_data.character.at(1).last_battle = std::chrono::high_resolution_clock::now();
        // game_data.character.at(2).last_battle = std::chrono::high_resolution_clock::now();
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