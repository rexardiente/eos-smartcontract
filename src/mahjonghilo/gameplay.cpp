#include "mahjonghilo.hpp"
#include <string>

void mahjonghilo::ondeposit(name from,
                            name to,
                            asset quantity,
                            string memo)
{
    if (from == _self)
    {
        if (memo.find(HAS_ON_SETTLE_PAY) != std::string::npos)
        {
            onsettledpay(to, quantity, memo);
        }

        // we're sending money, do nothing additional
        return;
    }
    check(to == _self, "Not to our contract");
    check(quantity.symbol.is_valid(), "Invalid quantity");
    check(quantity.amount > 0, "Only positive quantity allowed");
    check(quantity.symbol == mahjonghilo_symbol, "Invalid EOS Token");

    // std::string str = memo.substr(9);
    // if (memo.find("ADD_LIFE") != std::string::npos)
    // {
    //     set_add_life(from, quantity, str);
    // }
    // else
    // {
    //     int limit = stoi(str);
    //     summon_ready(from, quantity, limit);
    // }
}

void mahjonghilo::onsettledpay(name username, asset quantity, string memo)
{
    require_auth(_self);
    action(
        permission_level{_self, "active"_n},
        _self,
        "settledpay"_n,
        std::make_tuple(username, quantity, memo))
        .send();
}

void mahjonghilo::gettile(game &gamedata)
{
    int deck_tile_idx = rng(gamedata.deck_player.size()); // Pick a random tile from the deck
    // int deck_tile_idx = 30; // Pick a random tile from the deck

    gamedata.hand_player.insert(gamedata.hand_player.begin(), gamedata.deck_player[deck_tile_idx]); // Assign the tile to the first empty slot in the hand
    gamedata.current_tile = gamedata.deck_player[deck_tile_idx];
    gamedata.deck_player.erase(gamedata.deck_player.begin() + deck_tile_idx); // Remove the tile from the deck
}

void mahjonghilo::get_odds(game &gamedata, int value)
{
    float odds;
    if (value == 1)
    {
        gamedata.low_odds = 0;
    }
    else
    {
        gamedata.low_odds = (135 / ((value - 1) * 12)) * 0.9;
        if (gamedata.low_odds < 1)
        {
            gamedata.low_odds = 1;
        }
    }
    if (value == 10)
    {
        gamedata.draw_odds = (135 / 15) * 0.9;
    }
    else
    {
        gamedata.draw_odds = (135 / 11) * 0.9;
        if (gamedata.draw_odds < 1)
        {
            gamedata.draw_odds = 1;
        }
    }
    if (value == 11)
    {
        gamedata.high_odds = 0;
    }
    else
    {
        gamedata.high_odds = (135 / ((11 - value) * 12)) * 0.9;
        if (gamedata.high_odds < 1)
        {
            gamedata.high_odds = 1;
        }
    }
}

void mahjonghilo::sorthand(vector<uint8_t> &hand)
{
    int n = hand.size(), i, j, temp;
    for (i = 0; i < (n - 1); i++)
    {
        for (j = 0; j < (n - i - 1); j++)
        {
            if (hand[j] > hand[j + 1])
            {
                temp = hand[j];
                hand[j] = hand[j + 1];
                hand[j + 1] = temp;
            }
        }
    }
}

void mahjonghilo::hilo_step(game &gamedata, int prev_tile, int current_tile, int option)
{
    int result;
    if (prev_tile > current_tile)
    {
        result = 1;
    }
    else if (prev_tile == current_tile)
    {
        result = 2;
    }
    else
    {
        result = 3;
    }
    if (result == option)
    {
        if (result == 1)
        {
            gamedata.hi_lo_prize.amount *= gamedata.low_odds;
        }
        else if (result == 2)
        {
            gamedata.hi_lo_prize.amount *= gamedata.draw_odds;
        }
        else
        {
            gamedata.hi_lo_prize.amount *= gamedata.high_odds;
        }
    }
    else
    {
        gamedata.hi_lo_prize.amount = 0;
    }
}

int mahjonghilo::rng(const int &range)
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

string mahjonghilo::checksum256_to_string(std::array<uint8_t, 32UL> arr, size_t size)
{
    std::string r;
    const char *to_hex = "0123456789abcdef";
    for (uint32_t i = 0; i < arr.size(); ++i)
    {
        (r += to_hex[(arr[i] >> 4)]) += to_hex[(arr[i] & 0x0f)];
    }
    return r;
}