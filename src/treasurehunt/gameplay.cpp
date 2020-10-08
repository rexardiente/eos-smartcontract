#include "treasurehunt.hpp"
#include <eosio/transaction.hpp>

// Wallet Destination = {0:1, 1:10, 2:20}
// memo must have "Selected Wallet Destination: 0"
void treasurehunt::ondeposit(name from,
                             name to,
                             asset quantity,
                             string memo)
{
    if (from == _self)
    {
        return; // we're sending money, do nothing additional
    }
    check(to == _self, "Not to our contract");
    check(quantity.symbol.is_valid(), "Invalid quantity");
    check(quantity.amount > 0, "Only positive quantity allowed");
    check(quantity.symbol == treasurehunt_symbol, "Invalid EOS Token");

    print("Wallet Transfer Successful");
    initialize(from);
}

void treasurehunt::initialize(name username)
{
    require_auth(username);
    // Create a record in the table if the player doesn't exist in our app yet
    auto itr = _users.find(username.value);
    check(itr == _users.end(), "Error : Either User has Initialized a Game or has an Existing Game");
    // missing check balances
    if (itr == _users.end())
    {
        _users.emplace(username, [&](auto &new_users) {
            new_users.username = username;
            new_users.game_id = generategameid(); // generate user game_id
        });
    }
}

void treasurehunt::sendwithdraw(name to, int prize)
{
    require_auth(_self);

    action{
        permission_level{get_self(), "active"_n},
        eosio_token,
        "transfer"_n,
        std::make_tuple(get_self(), to, asset(prize, treasurehunt_symbol), std::string("Congratulations!!!"))}
        .send();
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