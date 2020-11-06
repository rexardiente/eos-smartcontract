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

void ghostquest::genstat(ghost &initghost) // function for generating monster/s status
{

    // int overallstat = initghost.status - initghost.hitpoints;
    // int level = initghost.ghost_level;
    initghost.attack = 25 + rng(49) + 1;
    initghost.defense = 25 + rng(49) + 1;
    initghost.speed = 25 + rng(49) + 1;
    initghost.luck = 25 + rng(49) + 1;
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
    // } while (overallstat < (initghost.attack + game_data.character.at(i).defense + game_data.character.at(i).speed + game_data.character.at(i).luck));
    // if (overallstat < sum)
    // {
    //     i--;
    // }
    // switch (game_data.character.at(i).ghost_class)
    // {
    // case 1:
    //     game_data.character.at(i).attack = 25 + (10 * level) + rng(50 - (10 * level));
    //     game_data.character.at(i).defense = 25 + rng(game_data.character.at(i).attack - 25);
    //     game_data.character.at(i).speed = overallstat - game_data.character.at(i).attack - game_data.character.at(i).defense;
    //     game_data.character.at(i).luck = overallstat - game_data.character.at(i).attack - game_data.character.at(i).defense - game_data.character.at(i).speed;
    //     break;
    // case 2:
    //     game_data.character.at(i).defense = 25 + (10 * level) + rng(50 - (10 * level));
    //     game_data.character.at(i).attack = ;
    //     game_data.character.at(i).speed = ;
    //     game_data.character.at(i).luck = ;
    //     break;
    // case 3:
    //     game_data.character.at(i).speed = 25 + (10 * level) + rng(50 - (10 * level));
    //     game_data.character.at(i).defense = ;
    //     game_data.character.at(i).attack = ;
    //     game_data.character.at(i).luck = ;
    //     break;
    // case 4:
    //     game_data.character.at(i).luck = 25 + (10 * level) + rng(50 - (10 * level));
    //     game_data.character.at(i).defense = ;
    //     game_data.character.at(i).speed = ;
    //     game_data.character.at(i).attack = ;
    //     break;
    // }
}

void ghostquest::battle(ghost &ghost1, ghost &ghost2) // function for generating monster/s status
{
    int numberofrounds = 0;
    while (ghost1.hitpoints > 0 && ghost2.hitpoints > 0)
    {
        if (ghost1.speed > ghost2.speed)
        {
            check(ghost2.hitpoints >= 1, "Your enemy[2] character no longer have hitpoints.");
            check(ghost1.status = 1, "Your character has either won or lost already.");
            damage_step(ghost1, ghost2);
            if (ghost2.hitpoints <= 0)
            {
                ghost2.hitpoints = 0;
                ghost2.status = 3;
                ghost1.status = 2;
            }
            numberofrounds++;
            print(numberofrounds);
            print("XX");
            print(ghost1.hitpoints, ghost2.hitpoints);
            print("XX");
            check(ghost1.hitpoints >= 1, "Your enemy[1] character no longer have hitpoints.");
            check(ghost2.status = 1, "Your character has either won or lost already.");
            damage_step(ghost2, ghost1);
            if (ghost1.hitpoints <= 0)
            {
                ghost1.hitpoints = 0;
                ghost1.status = 3;
                ghost2.status = 2;
            }
            numberofrounds++;
            print(numberofrounds);
            print("XX");
            print(ghost1.hitpoints, ghost2.hitpoints);
            print("XX");
        }
        else
        {
            check(ghost1.hitpoints >= 1, "Your enemy[1] character no longer have hitpoints.");
            check(ghost2.status = 1, "Your character has either won or lost already.");
            damage_step(ghost2, ghost1);
            if (ghost1.hitpoints <= 0)
            {
                ghost1.hitpoints = 0;
                ghost1.status = 3;
                ghost2.status = 2;
            }
            numberofrounds++;
            print(numberofrounds);
            print("XX");
            print(ghost1.hitpoints, ghost2.hitpoints);
            print("XX");
            check(ghost2.hitpoints >= 1, "Your enemy[2] character no longer have hitpoints.");
            check(ghost1.status = 1, "Your character has either won or lost already.");
            damage_step(ghost1, ghost2);
            if (ghost2.hitpoints <= 0)
            {
                ghost2.hitpoints = 0;
                ghost2.status = 3;
                ghost1.status = 2;
            }
            numberofrounds++;
            print(numberofrounds);
            print("XX");
            print(ghost1.hitpoints, ghost2.hitpoints);
            print("XX");
        }
    }
}

void ghostquest::damage_step(ghost &ghost1, ghost &ghost2) // function for generating monster/s status
{

    int chance = ghost1.luck / 4;
    int luck = rng(99) + 1;
    int fnldmg = 0;
    print(luck);
    print("XX");
    // int getdmgwdt = ghost1.attack / 16 + 1;
    // int dmgwdt = rng(getdmgwdt * 2);
    // int fnldmgwdt = getdmgwdt - dmgwdt;
    int dmgred = (ghost1.attack * ghost2.defense) / 100;
    if (luck <= chance)
    {
        fnldmg = ghost1.attack;
        print("HI");
    }
    else
    {
        fnldmg = ghost1.attack - dmgred;
        print("HELLO");
    }
    // int fnldmg = ghost1.attack;
    ghost2.hitpoints = ghost2.hitpoints - fnldmg;
    print(fnldmg);
    print("XX");

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