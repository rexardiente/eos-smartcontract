#include "gameplay.cpp"

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

ACTION ghostquest::summoncount(name username, uint64_t summoncount)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    // check(user.game_data.destination > 0, "Set destination first.");

    _users.modify(user, username, [&](auto &modified_user) {
        modified_user.game_data.summon_count = summoncount;
    });
}

ACTION ghostquest::battlelimit(name username, uint64_t battlelimit)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    check(user.game_data.summon_count > 0, "Set number of summons first.");

    _users.modify(user, username, [&](auto &modified_user) {
        modified_user.game_data.character.battle_limit = battlelimit;
    });
}

ACTION ghostquest::genmonst(name username, asset quantity)
{
    require_auth(_self);

    auto &user = _users.get(username.value, "User doesn't exist");
    auto itr = _users.find(username.value);

    check(itr != _users.end(), "Game Doesn't Exist.");
    check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    check(itr->game_data.destination == (quantity.amount / 10000), "Deposited amount does not match the selected destination.");
    // check(user.game_data.destination != MAP_DEFAULT, "Destination Not Set.");
    // check(user.game_data.enemy_count != 0, "Numbers of Enemies Not Set.");

    _users.modify(itr, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.prize = quantity;
        gameupdate(game_data);
    });
}

ACTION ghostquest::end(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(user.game_data.status == DONE, "End your existing game first.");
    _users.erase(user);
}