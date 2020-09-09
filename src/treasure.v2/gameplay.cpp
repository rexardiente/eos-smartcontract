#include "treasure.v2.hpp"
#include <eosio/system.hpp>

// random users win with based on win limit = 4
uint16_t treasurev2::calculate_prize(vector<TilePrize> &tile_prizes, uint8_t &win_count)
{
    const uint8_t unopened = 16 - tile_prizes.size();

    // has equal or less 4 wins and has available panel equal to win limit
    const int win_rate = rng(10);

    // check remaining tile and win is equal
    if (unopened == win_count)
        return rng(20);
    if (win_rate > 7)
        return rng(20);
    else
        return 0;
}

uint64_t treasurev2::gen_gameid()
{
    // get current time
    uint64_t current_time = current_time_point().elapsed._count;
    // get current size of the table
    int size = std::distance(_users.begin(), _users.end());

    return (size + rng(1000)) + current_time;
}

int treasurev2::rng(const int range)
{
    // Find the existing seed
    auto seed_iterator = _seeds.begin();

    // Initialize the seed with default value if it is not found
    if (seed_iterator == _seeds.end())
    {
        seed_iterator = _seeds.emplace(_self, [&](auto &seed) {});
    }

    // Generate new seed value using the existing seed value
    int prime = 65537;
    auto new_seed_value = (seed_iterator->value + current_time_point().elapsed.count()) % prime;

    // Store the updated seed value in the table
    _seeds.modify(seed_iterator, _self, [&](auto &s) {
        s.value = new_seed_value;
    });

    // Get the random result in desired range
    int random_result = new_seed_value % range;
    return random_result;
}

int64_t treasurev2::ticket_balance(name username)
{
    require_auth(username);
    auto &ticket = _tickets.get(username.value, "Ticket doesn't exist");
    return ticket.balance;
}

void treasurev2::ticket_update(name username, bool isdeduction, uint64_t amount)
{
    require_auth(username);
    auto &ticket = _tickets.get(username.value, "Ticket doesn't exist");
    _tickets.modify(ticket, username, [&](auto &modified_ticket) {
        if (isdeduction)
            modified_ticket.balance -= amount;
        else
            modified_ticket.balance += amount;
    });
}

//  Add to history tbl and return true if successfully added
void treasurev2::addhistory(user user_data)
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
    // Double check if added successfully to history
    // auto updated_history = _history.find(game_id); // Bad implementation..
    // if (updated_history != _history.end())
    //     return true;
    // return false;
}

void treasurev2::game_update(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist");

    _users.modify(user, username, [&](auto &modified_user) {
        // check the current total win and limit is already reached, change the game status, add to history
        game game_data = modified_user.game_data;

        // Change Setsail status
        game_data.set_sail = false;
        game_data.status = ONGOING;
        modified_user.game_data = game_data;

        // Todo: If Explorer Count is Empty End the game and add history..
        if (game_data.win_count == 4 || game_data.explore_count == 0)
        {
            modified_user.game_data.status = DONE;

            // add to history and remove to users tbl
            // bool isAddedToHistory = addhistory(modified_user);
            // print(isAddedToHistory);
            // if (!isAddedToHistory)
            //     check(false, "Error: Adding to History. Please try again!");
            // else
            // game_update(username);
        }
    });

    // Check if game has ended...
    // auto itr = _users.find(username.value);
    // if (itr->game_data.status == DONE)
    // {
    //     print("DONE DONE DONE");
    //     _users.erase(itr);
    // }
}