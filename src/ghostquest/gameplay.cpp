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
    int numberofrounds = 1;
    check(ghost1.character_life > 0 && ghost2.character_life > 0, "Your or your enemy character can not battle.");
    while (ghost1.hitpoints > 0 && ghost2.hitpoints > 0)
    {
        if (ghost1.speed > ghost2.speed)
        {
            damage_step(ghost1, ghost2, numberofrounds);
            numberofrounds += 1;
            if (ghost2.hitpoints > 0)
            {
                damage_step(ghost2, ghost1, numberofrounds);
                numberofrounds += 1;
                if (ghost1.hitpoints == 0)
                {
                    result_step(ghost1, ghost2);
                }
            }
            else
            {
                result_step(ghost2, ghost1);
            }
        }
        else
        {
            damage_step(ghost2, ghost1, numberofrounds);
            numberofrounds += 1;
            if (ghost1.hitpoints > 0)
            {
                damage_step(ghost1, ghost2, numberofrounds);
                numberofrounds += 1;
                if (ghost2.hitpoints == 0)
                {
                    result_step(ghost2, ghost1);
                }
            }
            else
            {
                result_step(ghost1, ghost2);
            }
        }
    }
    calculate_prize(ghost1);
    calculate_prize(ghost2);
}

void ghostquest::damage_step(ghost &ghost1, ghost &ghost2, int round) // function for generating monster/s status
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
    ghost2.hitpoints = ((ghost2.hitpoints - fnldmg) < 0) ? 0 : ghost2.hitpoints - fnldmg;
    print("Round#");
    print(round);
    print("->");
    print("Characterof");
    print(ghost2.ghost_id);
    print("atkey");
    print(ghost2.key);
    print("took");
    print(fnldmg);
    print("damagefromcharacterof");
    print(ghost1.ghost_id);
    print("atkey");
    print(ghost1.key);
    print("...");
    // std::string battle_log = "Round " + to_string(round) + " : Character of " + std::to_string(ghost2.ghost_id) + " took " + std::to_string(fnldmg) + " amount of damage from character of " + std::to_string(ghost1.ghost_id);
    // print(battle_log);
    // std::string battle_log = ghost2.ghost_id + std::to_string(ghost2.key) + " took " + std::to_string(fnldmg) + " damage from" + std::to_string(ghost1.ghost_id) + std::to_string(ghost1.key);
    // eosio::print(ghost2.ghost_id + ghost2.key + " took " + fnldmg + " damage from" + ghost1.ghost_id + ghost1.key + "\n");
}

void ghostquest::calculate_prize(ghost &ghost)
{
    float house_edge;
    float init_prize = ghost.character_life * 10000;
    if (ghost.battle_count < 21)
    {
        house_edge = init_prize * 0.06;
    }
    else if (ghost.battle_count > 20 && ghost.battle_count < 41)
    {
        house_edge = init_prize * 0.07;
    }
    else if (ghost.battle_count > 40 && ghost.battle_count < 61)
    {
        house_edge = init_prize * 0.08;
    }
    else if (ghost.battle_count > 60 && ghost.battle_count < 81)
    {
        house_edge = init_prize * 0.09;
    }
    else
    {
        house_edge = init_prize * 0.1;
    }
    ghost.prize.amount = init_prize - house_edge;
}

void ghostquest::result_step(ghost &ghost1, ghost &ghost2)
{
    ghost1.status = LOSER;
    if (ghost1.character_life == 1)
    {
        ghost1.status = ELIMINATED;
        eliminated_withdrawn(ghost1);
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

void ghostquest::eliminated_withdrawn(ghost &ghost)
{
    ghost.status = 6;
    ghost.character_life = 0;
    ghost.initial_hp = 0;
    ghost.hitpoints = 0;
    ghost.ghost_class = 0;
    ghost.ghost_level = 0;
    ghost.attack = 0;
    ghost.defense = 0;
    ghost.speed = 0;
    ghost.luck = 0;
    ghost.prize.amount = 0;
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