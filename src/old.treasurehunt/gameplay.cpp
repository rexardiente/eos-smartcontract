#include "treasure.v2.hpp"
#include <eosio/system.hpp>

// random users win with based on win limit = 4
uint16_t treasurev2::iswinning(const vector<TilePrize> &tile_prizes, const uint8_t &win_count)
{
    const uint8_t unopened = 16 - tile_prizes.size();

    // has equal or less 4 wins and has available panel equal to win limit
    const int win_rate = rng(10);
    // check remaining tile and win is equal

    // if (unopened == (4 - win_count))
    //     return destination * multiplier();
    // if (win_rate > 6)
    //     return destination * multiplier();
    if (unopened == (4 - win_count))
        return multiplier();
    if (win_rate > 6)
        return multiplier();
    else
        return 0;
}

// chest multiplier
uint64_t treasurev2::multiplier()
{
    const int genChest = rng(1000);
    const int genTier = rng(100);
    if (genTier < 26)
    {
        return 1;
    }
    else if (genTier >= 26 && genTier <= 50)
    {
        return 2;
    }
    else if (genTier >= 51 && genTier <= 75)
    {
        if (genChest >= 1 && genChest <= 700)
        {
            return 3;
        }
        else
        {
            return 4;
        }
    }
    else
    {
        if (genChest >= 1 && genChest <= 700)
        {
            return 4;
        }
        else if (genChest >= 701 && genChest <= 850)
        {
            return 8;
        }
        else if (genChest >= 851 && genChest <= 950)
        {
            return 16;
        }
        else if (genChest >= 951 && genChest <= 990)
        {
            return 32;
        }
        else if (genChest >= 991 && genChest <= 999)
        {
            return 64;
        }
        else
        {
            return 128;
        }
    }
}

uint64_t treasurev2::gen_gameid()
{
    // get current time
    uint64_t current_time = current_time_point().elapsed._count;
    // get current size of the table
    int size = std::distance(_users.begin(), _users.end());

    return (size + rng(1000)) + current_time;
}

int treasurev2::rng(const int &range)
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



void treasurev2::game_update(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist");
    auto iterator = _users.find(username.value);

    _users.modify(user, username, [&](auto &modified_user) {
        // check the current total win and limit is already reached, change the game status, add to history
        game game_data = modified_user.game_data;

        game_data.status = ONGOING;
        modified_user.game_data = game_data;

    });


}