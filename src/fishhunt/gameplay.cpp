#include "fishhunt.hpp"
#include <eosio/system.hpp>
void fishhunt::addhistory(user user_data)
{
    name username = user_data.username;
    require_auth(username);

    uint64_t game_id = user_data.game_id;
    auto itr = _history.find(game_id);

    // check if the game_id doesn't exist
    if (itr == _history.end())
    {
        _history.emplace(username, [&](auto &new_history) {
            history game_history;

            game_history.game_id = game_id;
            game_history.username = user_data.username;
            game_history.game_data = user_data.game_data;

            new_history = game_history;
        });
    }
}