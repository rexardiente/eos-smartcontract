#include "treasurehunt.hpp"
#include <eosio/transaction.hpp>

// Wallet Destination = {0:1, 1:10, 2:50}
// memo must have "Selected Wallet Destination: 0"
void treasurehunt::ondeposit(name from,
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

    print("Wallet Transfer Successful");
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