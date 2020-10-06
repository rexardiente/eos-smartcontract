#include "treasurehunt.hpp"
#include <eosio/transaction.hpp>

// Wallet Destination = {0:1, 1:10, 2:50}
// memo must have "Selected Wallet Destination: 0"
void treasurehunt::ontransfernotification(name from,
                                          name to,
                                          asset quantity,
                                          string memo)
{
    if (from == _self)
    {
        // we're sending money, do nothing additional
        return;
    }

    check(to == _self, "Not to our contract");
    check(quantity.symbol.is_valid(), "Invalid quantity");
    check(quantity.amount > 0, "Only positive quantity allowed");

    check(quantity.symbol == eosio_symbol(), "Invalid EOS Token");

    // Your Code here...
}

uint64_t treasurehunt::generategameid()
{
    // get current time
    uint64_t current_time = current_time_point().elapsed._count;
    // get current size of the table
    int size = std::distance(_users.begin(), _users.end());

    return (size + rng(1000)) + current_time;
}

int treasurehunt::rng(const int &range)
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

float treasurehunt::roundoff(float value)
{
    float result = (int)(value * 100 + .5);
    return (float)result / 100;
}

// void treasurehunt::game_update(name username)
// {
//     require_auth(username);
//     auto &user = _users.get(username.value, "Error: User doesn't exist");
//     auto iterator = _users.find(username.value);

//     _users.modify(user, username, [&](auto &modified_user) {
//         // check the current total win and limit is already reached, change the game status, add to history
//         game game_data = modified_user.game_data;

//         game_data.isready = false;
//         modified_user.game_data = game_data;

//         game_data.panel_set = modified_user.game_data.panel_set;

//         // Update isopen and iswin to default
//         for (uint8_t i = 0; i < PANEL_SIZE; i++)
//         {
//             game_data.panel_set[i].isopen = IS_OPEN_PANEL;
//             game_data.panel_set[i].iswin = IS_WIN_PANEL;
//         }

//         // reuse current game user destination
//         game_data.destination = modified_user.game_data.destination;
//         game_data.status = INITIALIZED;

//         modified_user.game_data = game_data;
//         modified_user.game_id = generategameid(); // generate new game ID
//     });
// }
