#include "ghostquest.hpp"
#include <ctime>
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

void ghostquest::genstat(ghost &initghost) // function for generating monster/s status
{

    initghost.attack = 25 + rng(50);
    initghost.defense = 25 + rng(50);
    initghost.speed = 25 + rng(50);
    initghost.luck = 25 + rng(50);
    int sum = initghost.attack + initghost.defense + initghost.speed + initghost.luck + initghost.hitpoints;
    if (sum > 199 && sum < 286)
    {
        initghost.ghost_level = 1;
    }
    else if (sum > 285 && sum < 311)
    {
        initghost.ghost_level = 2;
    }
    else if (sum > 310 && sum < 341)
    {
        initghost.ghost_level = 3;
    }
    else if (sum > 340 && sum < 386)
    {
        initghost.ghost_level = 4;
    }
    else
    {
        initghost.ghost_level = 5;
    }
    if (initghost.attack > initghost.defense)
    {
        if (initghost.attack > initghost.speed && initghost.attack > initghost.luck)
        {
            initghost.ghost_class = 1;
        }
        else if (initghost.speed > initghost.luck)
        {
            initghost.ghost_class = 3;
        }
        else
        {
            initghost.ghost_class = 4;
        }
    }
    else
    {
        if (initghost.defense > initghost.speed && initghost.defense > initghost.luck)
        {
            initghost.ghost_class = 2;
        }
        else if (initghost.speed > initghost.luck)
        {
            initghost.ghost_class = 3;
        }
        else
        {
            initghost.ghost_class = 4;
        }
    }
}

void ghostquest::battle(ghost &ghost1, ghost &ghost2) // function for generating monster/s status
{
    int numberofrounds = 0;
    while (ghost1.hitpoints > 0 && ghost2.hitpoints > 0)
    {
        if (ghost1.speed > ghost2.speed)
        {
            check(ghost2.hitpoints >= 1, "Your or your enemy[2] character no longer have hitpoints.[1]");
            check(ghost1.status == 3, "Your character has either won or lost already.[1]");
            damage_step(ghost1, ghost2);
            if (ghost2.hitpoints <= 0)
            {
                ghost2.hitpoints = 0;
                ghost2.status = LOSER;
                if (ghost2.character_life == 1)
                {
                    ghost2.status = ELIMINATED;
                    ghost2.character_life = 0;
                }
                else
                {
                    ghost2.character_life -= 1;
                }
                ghost1.status = WINNER;
                ghost1.character_life += 1;
                ghost1.battle_count += 1;
                ghost2.battle_count += 1;
            }
            numberofrounds += 1;
            print(ghost2.status);
            check(ghost1.hitpoints >= 1, "Your or your enemy[1] character no longer have hitpoints.[2]");
            check(ghost2.status == 3, "Your character has either won or lost already.[2]");
            damage_step(ghost2, ghost1);
            if (ghost1.hitpoints <= 0)
            {
                ghost1.hitpoints = 0;
                ghost1.status = LOSER;
                if (ghost1.character_life == 1)
                {
                    ghost1.status = ELIMINATED;
                    ghost1.character_life = 0;
                }
                else
                {
                    ghost1.character_life -= 1;
                }
                ghost2.status = WINNER;
                ghost2.character_life += 1;
                ghost1.battle_count += 1;
                ghost2.battle_count += 1;
            }
            numberofrounds += 1;
        }
        else
        {
            check(ghost1.hitpoints >= 1, "Your or your enemy[1] character no longer have hitpoints.[3]");
            check(ghost2.status == 3, "Your character has either won or lost already.[3]");
            damage_step(ghost2, ghost1);
            if (ghost1.hitpoints <= 0)
            {
                ghost1.hitpoints = 0;
                ghost1.status = LOSER;
                if (ghost1.character_life == 1)
                {
                    ghost1.status = ELIMINATED;
                    ghost1.character_life = 0;
                }
                else
                {
                    ghost1.character_life -= 1;
                }
                ghost2.status = WINNER;
                ghost2.character_life += 1;
                ghost1.battle_count += 1;
                ghost2.battle_count += 1;
            }
            numberofrounds += 1;
            print(ghost1.status);
            check(ghost2.hitpoints >= 1, "Your or your enemy[2] character no longer have hitpoints.[4]");
            check(ghost1.status == 3, "Your character has either won or lost already.[4]");
            damage_step(ghost1, ghost2);
            if (ghost2.hitpoints <= 0)
            {
                ghost2.hitpoints = 0;
                ghost2.status = LOSER;
                if (ghost2.character_life == 1)
                {
                    ghost2.status = ELIMINATED;
                    ghost2.character_life = 0;
                }
                else
                {
                    ghost2.character_life -= 1;
                }
                ghost1.status = WINNER;
                ghost1.character_life += 1;
                ghost1.battle_count += 1;
                ghost2.battle_count += 1;
            }
            numberofrounds += 1;
        }
    }
}

void ghostquest::damage_step(ghost &ghost1, ghost &ghost2) // function for generating monster/s status
{

    int chance = ghost1.luck / 4;
    int luck = rng(99) + 1;
    int fnldmg = 0;
    int getdmgwdt = ghost1.attack / 16 + 1;
    int dmgwdt = rng(getdmgwdt);
    int dmgred = (ghost1.attack * ghost2.defense) / 100;
    if (luck <= chance)
    {
        if (luck % 2 == 0)
        {
            fnldmg = ghost1.attack + dmgwdt;
        }
        else
        {
            fnldmg = ghost1.attack - dmgwdt;
        }
    }
    else
    {
        if (luck % 2 == 0)
        {
            fnldmg = ghost1.attack - dmgred + dmgwdt;
        }
        else
        {
            fnldmg = ghost1.attack - dmgred - dmgwdt;
        }
    }
    ghost2.hitpoints = ghost2.hitpoints - fnldmg;

    // std::string battle_log = ghost2.ghost_id + std::to_string(ghost2.key) + " took " + std::to_string(fnldmg) + " damage from" + std::to_string(ghost1.ghost_id) + std::to_string(ghost1.key);
    // eosio::print(ghost2.ghost_id + ghost2.key + " took " + fnldmg + " damage from" + ghost1.ghost_id + ghost1.key + "\n");
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