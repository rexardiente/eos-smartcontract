#include "ghostquest.hpp"

void ghostquest::ondeposit(name from,
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
    check(quantity.symbol == ghostquest_symbol, "Invalid EOS Token");
    gameready(from, quantity);
}

void ghostquest::gameready(name username, asset quantity)
{
    require_auth(username);
    action(
        permission_level{_self, "active"_n},
        _self,
        "getstat"_n,
        std::make_tuple(username, quantity))
        .send();
}

void ghostquest::onsettledpay(name username, asset quantity, string memo)
{
    require_auth(_self);
    action(
        permission_level{_self, "active"_n},
        _self,
        "settledpay"_n,
        std::make_tuple(username, quantity, memo))
        .send();
}

void ghostquest::genstat(game &game_data) // function for generating monster/s status
{
    for (int i = 0; i < game_data.summon_count; i++)
    {
        int level = game_data.character.at(i).ghost_level - 1;
        switch (game_data.character.at(i).ghost_class)
        {
        case 1:
            game_data.character.at(i).defense = 25 + rng(7) + (10 * level);
            game_data.character.at(i).speed = 25 + rng(7) + (10 * level);
            game_data.character.at(i).luck = 25 + rng(7) + (10 * level);
            game_data.character.at(i).attack = 25 + (rng(3) + 7) + (10 * level);
            break;
        case 2:
            game_data.character.at(i).attack = 25 + rng(7) + (10 * level);
            game_data.character.at(i).speed = 25 + rng(7) + (10 * level);
            game_data.character.at(i).luck = 25 + rng(7) + (10 * level);
            game_data.character.at(i).defense = 25 + (rng(3) + 7) + (10 * level);
            break;
        case 3:
            game_data.character.at(i).defense = 25 + rng(7) + (10 * level);
            game_data.character.at(i).attack = 25 + rng(7) + (10 * level);
            game_data.character.at(i).luck = 25 + rng(7) + (10 * level);
            game_data.character.at(i).speed = 25 + (rng(3) + 7) + (10 * level);
            break;
        case 4:
            game_data.character.at(i).defense = 25 + rng(7) + (10 * level);
            game_data.character.at(i).speed = 25 + rng(7) + (10 * level);
            game_data.character.at(i).attack = 25 + rng(7) + (10 * level);
            game_data.character.at(i).luck = 25 + (rng(3) + 7) + (10 * level);
            break;
        }
    }
}

int ghostquest::rng(const int &range)
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