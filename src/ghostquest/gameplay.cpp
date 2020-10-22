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
        "genmonst"_n,
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

void ghostquest::genstat(ghost character)
{
    int overall_stat = 100 + character.ghost_level * 25;
    switch (character.ghost_class)
    {
    case 1:
        character.defense = (character.ghost_level * 5) + 10 + rng(10);
        character.speed = (character.ghost_level * 5) + 10 + rng(10);
        character.luck = (character.ghost_level * 5) + 10 + rng(10);
        character.attack = overall_stat - (character.defense + character.speed + character.luck);
        break;
    case 2:
        character.attack = (character.ghost_level * 5) + 10 + rng(10);
        character.speed = (character.ghost_level * 5) + 10 + rng(10);
        character.luck = (character.ghost_level * 5) + 10 + rng(10);
        character.defense = overall_stat - (character.attack + character.speed + character.luck);
        break;
    case 3:
        character.defense = (character.ghost_level * 5) + 10 + rng(10);
        character.attack = (character.ghost_level * 5) + 10 + rng(10);
        character.luck = (character.ghost_level * 5) + 10 + rng(10);
        character.speed = overall_stat - (character.defense + character.attack + character.luck);
        break;
    case 4:
        character.defense = (character.ghost_level * 5) + 10 + rng(10);
        character.speed = (character.ghost_level * 5) + 10 + rng(10);
        character.attack = (character.ghost_level * 5) + 10 + rng(10);
        character.luck = overall_stat - (character.defense + character.speed + character.attack);
        break;
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