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

void mahjonghilo::gettile(vector<uint8_t> &deck, vector<uint8_t> &hand)
{
    int deck_tile_idx = rng(deck.size()); // Pick a random tile from the deck

    int first_empty_slot = -1; // Find the first empty slot in the hand
    for (int i = 0; i <= hand.size(); i++)
    {
        auto id = hand[i];
        if (table_deck.at(id).type == EMPTY)
        {
            first_empty_slot = i;
            break;
        }
    }
    // check(first_empty_slot != -1, "No empty slot in the player's hand");
    hand.insert(hand.begin(), deck[deck_tile_idx]); // Assign the tile to the first empty slot in the hand
    deck.erase(deck.begin() + deck_tile_idx);       // Remove the tile from the deck
}
float mahjonghilo::get_odds(int value)
{
    float sametiles, odds;
    if (value == 10)
    {
        sametiles = 15;
    }
    else
    {
        sametiles = 11;
    }
    if (option == 0)
    {
        odds = ((sametiles + 1) * (value - 1)) / 135;
    }
    else if (option == 1)
    {
        odds = (sametiles) / 135;
    }
    else
    {
        odds = ((sametiles + 1) * (11 - value)) / 135;
    }
    return odds * 100;
}

int mahjonghilo::hi_lo_step(int prev_tile, int current_tile)
{
    if (prev_tile > current_tile)
    {
        return 0;
    }
    else if (prev_tile == current_tile)
    {
        return 1;
    }
    else
    {
        return 2;
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