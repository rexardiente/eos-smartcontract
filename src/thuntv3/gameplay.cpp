#include "thuntv3.hpp"
#include <eosio/transaction.hpp>

// Wallet Destination = {0:1, 1:10, 2:50}
// memo must have "Selected Wallet Destination: 0"
// void thuntv3::ondeposit(name from,
//                         name to,
//                         asset quantity,
//                         string memo)
// {
//     if (from == _self)
//     {
//         // we're sending money, do nothing additional
//         return;
//     }

//     check(to == _self, "Not to our contract");
//     check(quantity.symbol.is_valid(), "Invalid quantity");
//     check(quantity.amount > 0, "Only positive quantity allowed");

//     check(quantity.symbol == eosio_symbol(), "Invalid EOS Token");
//     eosio::print("Update current Ticket Balance.");

//     // Get Selected Destination
//     int findCharPos = memo.find(":");
//     string str_dest = memo.substr(findCharPos + 2); // get from "live" to the end
//     uint64_t newdestination = stoi(str_dest);

//     // Set 1EOS as default wallet
//     if (findCharPos < 0 || newdestination > 2)
//     {
//         newdestination = 0;
//     }

//     uint64_t convertedquantity = quantity.amount / 10000;

//     ticket_add(from, convertedquantity, newdestination);
// }

// void thuntv3::check_asset(asset quantity, uint64_t minvalue, uint8_t nonfloat)
// {
//     check(quantity.symbol.is_valid(), "invalid symbol name");
//     check(quantity.symbol == eosio_symbol(), "token symbol not match");
//     check(quantity.is_valid(), "invalid quantity");
//     check(quantity.amount > 0, "quantity must > 0");
//     check(quantity.amount >= minvalue, "quantity not reach minimum allowed");
//     if (nonfloat > 0)
//         check((quantity.amount % 10000) == 0, "quantity must integer");
// }

uint64_t thuntv3::gen_gameid()
{
    // get current time
    uint64_t current_time = current_time_point().elapsed._count;
    // get current size of the table
    int size = std::distance(_users.begin(), _users.end());

    return (size + rndm(1000)) + current_time;
}

int thuntv3::rndm(const int &range)
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

float thuntv3::roundoff(float value)
{
    float result = (int)(value * 100 + .5);
    return (float)result / 100;
}

// void thuntv3::genresult(name username, const uint8_t &enemy_count, const uint8_t &unopentile)
// {
//     require_auth(username);
//     auto &user = _users.get(username.value, "Error: User doesn't exist");
//     _users.modify(user, username, [&](auto &modified_user) {
//         game game_data = modified_user.game_data;

//     });
// }

// void thuntv3::addhistory(user user_data)
// {
//     name username = user_data.username;
//     require_auth(username);

//     uint64_t game_id = user_data.game_id;
//     auto itr = _history.find(game_id);

//     // check if the game_id doesn't exist
//     if (itr == _history.end())
//     {
//         _history.emplace(username, [&](auto &new_history) {
//             history game_history;

//             game_history.game_id = game_id;
//             game_history.username = user_data.username;
//             game_history.game_data = user_data.game_data;

//             new_history = game_history;
//         });
//     }
// }

void thuntv3::game_update(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist");
    auto iterator = _users.find(username.value);

    _users.modify(user, username, [&](auto &modified_user) {
        // check the current total win and limit is already reached, change the game status, add to history
        game game_data = modified_user.game_data;

        game_data.isready = false;
        modified_user.game_data = game_data;

        game_data.panel_set = modified_user.game_data.panel_set;

        // Update isopen and iswin to default
        for (uint8_t i = 0; i < PANEL_SIZE; i++)
        {
            game_data.panel_set[i].isopen = IS_OPEN_PANEL;
            game_data.panel_set[i].iswin = IS_WIN_PANEL;
        }

        // reuse current game user destination
        game_data.destination = modified_user.game_data.destination;
        game_data.status = INITIALIZED;

        modified_user.game_data = game_data;
        modified_user.game_id = gen_gameid(); // generate new game ID

        // // Todo: If Explorer Count is Empty End the game and add history..
        // if (game_data.win_count == 4 || game_data.explore_count == 0)
        // {
        //     modified_user.game_data.status = DONE;
        //     // addhistory(modified_user);
        // }
    });
}