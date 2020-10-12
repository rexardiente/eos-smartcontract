#include "treasurehunt.hpp"
#include <eosio/transaction.hpp>

void treasurehunt::ondeposit(name from,
                             name to,
                             asset quantity,
                             string memo)
{
    if (from == _self)
    {
        onsettledpay(to, quantity, memo);
        return; // we're sending money, do nothing additional
    }
    check(to == _self, "Not to our contract");
    check(quantity.symbol.is_valid(), "Invalid quantity");
    check(quantity.amount > 0, "Only positive quantity allowed");
    check(quantity.symbol == treasurehunt_symbol, "Invalid EOS Token");

    gameready(from, quantity);
}

void treasurehunt::onsettledpay(name username, asset quantity, string memo)
{
    require_auth(_self);
    action(
        permission_level{_self, "active"_n},
        _self,
        "settledpay"_n,
        std::make_tuple(username, quantity, memo))
        .send();
}

void treasurehunt::gameready(name username, asset quantity)
{
    require_auth(username);
    action(
        permission_level{_self, "active"_n},
        _self,
        "gamestart"_n,
        std::make_tuple(username, quantity))
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