#include "treasure.v2.hpp"
#include <eosio/system.hpp>
#include <eosio/time.hpp>

// template <typename T>
// const bool contains(vector<T> vec, T element) {
//     if (std::find(vec.begin(), vec.end(), element) != vec.end())
//         return true;
//     return false;
// }

uint64_t treasurev2::gen_gameid() {
    // get current time
    uint64_t current_time = current_time_point().elapsed._count;
    // get current size of the table
    int size = std::distance(_users.begin(), _users.end());

    return (size + rng(1000)) + current_time;
}


int treasurev2::rng(const int range) {
    // Find the existing seed
    auto seed_iterator = _seeds.begin();

    // Initialize the seed with default value if it is not found
    if (seed_iterator == _seeds.end()) {
        seed_iterator = _seeds.emplace(_self, [&](auto& seed) {});
    }

    // Generate new seed value using the existing seed value
    int prime = 65537;
    auto new_seed_value = (seed_iterator->value + current_time_point().elapsed.count()) % prime;

    // Store the updated seed value in the table
    _seeds.modify(seed_iterator, _self, [&](auto& s) {
        s.value = new_seed_value;
        });

    // Get the random result in desired range
    int random_result = new_seed_value % range;
    return random_result;
};

void treasurev2::purchase(name username, uint64_t amount) {
    require_auth(username);
    auto itr = _tickets.find(username.value);

    if (itr == _tickets.end())
    {
        _tickets.emplace(username, [&](auto& new_ticket) {
            new_ticket.username = username;
            new_ticket.balance = amount;
            });
    }
    else
    {
        _tickets.modify(itr, username, [&](auto& modify_ticket) {
            uint64_t new_balance;

            new_balance = modify_ticket.balance + amount;
            modify_ticket.balance = new_balance;
            });
    }
}

int64_t treasurev2::ticket_balance(name username) {
    require_auth(username);

    auto& ticket = _tickets.get(username.value, "Ticket doesn't exist");
    return ticket.balance;
};