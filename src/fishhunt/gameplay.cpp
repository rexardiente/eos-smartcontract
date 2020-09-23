#include "fishhunt.hpp"
#include <eosio/system.hpp>

uint16_t fishhunt::iswinning(const vector<fishPrize> &Fish_prizes, const uint8_t &win_count, Fish fishcatch)
{
    const uint8_t unopened = 16 - Fish_prizes.size();
    const int win_rate = rng(10);
    // has equal or less 6 wins and has available panel equal to win limit
    if (unopened == (4 - win_count))
        return multiplier(fishcatch);
    if (win_rate > 6)
        return multiplier(fishcatch);
    else
        return 0;
}

//fish multiplier
uint64_t fishhunt::multiplier(Fish fishcatch)
{
    double_t size;
    if (fishcatch.key == 'A')
    {
        do
        {
            size = rng(15);
        } while (size < 5);
        return 0.5, size;
    }
    else if (fishcatch.key == 'B')
    {
        do
        {
            size = rng(30);
        } while (size < 10);
        return 0.5, size;
    }
    else if (fishcatch.key == 'B')
    {
        do
        {
            size = rng(30);
        } while (size < 10);
        return 0.5;
    }
}

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

uint64_t fishhunt::gen_gameid()
{
    // get current time
    uint64_t current_time = current_time_point().elapsed._count;
    // get current size of the table
    int size = std::distance(_users.begin(), _users.end());

    return (size + rng(1000)) + current_time;
}

int fishhunt::rng(const int range)
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

int fishhunt::rngtwo(const int range)
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
    // _seeds.modify(seed_iterator, _self, [&](auto &s) {
    //     s.value = new_seed_value;
    // });

    // Get the random result in desired range
    int random_result = new_seed_value % range;
    return random_result;
}

int64_t fishhunt::ticket_balance(name username)
{
    require_auth(username);
    auto &ticket = _tickets.get(username.value, "Ticket doesn't exist");
    return ticket.balance;
}

void fishhunt::ticket_update(name username, bool isdeduction, uint64_t amount)
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

void fishhunt::game_update(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist.");
    auto iterator = _users.find(username.value);

    _users.modify(user, username, [&](auto &modified_user) {
        // check the current total win and limit is already reached, change the game status, add to history
        game game_data = modified_user.game_data;

        //Change SetHuntFish status
        game_data.set_hunt = false;
        game_data.status = ONGOING;
        modified_user.game_data = game_data;

        // Todo: If Explorer Count is Empty End the game and add history..
        if (game_data.win_count == 6 || game_data.explore_count == 0)
        {
            modified_user.game_data.status = DONE;
        }
    })
}