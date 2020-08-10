#include "treasure.v2.hpp"
#include <eosio/system.hpp>

int treasurev2::rng(const int range) {
    // Find the existing seed
    auto seed_iterator = _seed.begin();

    // Initialize the seed with default value if it is not found
    if (seed_iterator == _seed.end()) {
        seed_iterator = _seed.emplace(_self, [&](auto& seed) {});
    }

    // Generate new seed value using the existing seed value
    int prime = 65537;
    auto new_seed_value = (seed_iterator->value + current_time_point().elapsed.count()) % prime;

    // Store the updated seed value in the table
    _seed.modify(seed_iterator, _self, [&](auto& s) {
        s.value = new_seed_value;
        });

    // Get the random result in desired range
    int random_result = new_seed_value % range;
    return random_result;
};
