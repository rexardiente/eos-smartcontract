#include "ghostquest.hpp"
#include <string>

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

    std::string str = memo.substr(9);
    if (memo.find("ADD_LIFE") != std::string::npos)
    {
        set_add_life(from, quantity, str);
    }
    else
    {
        int limit = stoi(str);
        summon_ready(from, quantity, limit);
    }
}

void ghostquest::summon_ready(name username, asset quantity, int limit) // trigger summoning after transfer transaction
{
    require_auth(username);

    action( //generate characters
        permission_level{_self, "active"_n},
        _self,
        "genchar"_n,
        std::make_tuple(username, quantity, limit))
        .send();
}

void ghostquest::set_add_life(name username, asset quantity, string key) // trigger adding life after transfer transaction
{
    require_auth(username);

    action( // add life
        permission_level{_self, "active"_n},
        _self,
        "addlife"_n,
        std::make_tuple(username, quantity, key))
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

void ghostquest::gen_stat(ghost &initial_ghost) // function for generating monster/s status
{
    initial_ghost.attack = 25 + rng(50);
    initial_ghost.defense = 25 + rng(50);
    initial_ghost.speed = 25 + rng(50);
    initial_ghost.luck = 25 + rng(50);
    int sum = initial_ghost.attack + initial_ghost.defense + initial_ghost.speed + initial_ghost.luck + initial_ghost.hitpoints;
    if (sum > 199 && sum < 286)
    {
        initial_ghost.ghost_level = 1;
    }
    else if (sum > 285 && sum < 311)
    {
        initial_ghost.ghost_level = 2;
    }
    else if (sum > 310 && sum < 341)
    {
        initial_ghost.ghost_level = 3;
    }
    else if (sum > 340 && sum < 386)
    {
        initial_ghost.ghost_level = 4;
    }
    else
    {
        initial_ghost.ghost_level = 5;
    }
    if (initial_ghost.attack > initial_ghost.defense)
    {
        if (initial_ghost.attack > initial_ghost.speed && initial_ghost.attack > initial_ghost.luck)
        {
            initial_ghost.ghost_class = 1;
        }
        else if (initial_ghost.speed > initial_ghost.luck)
        {
            initial_ghost.ghost_class = 3;
        }
        else
        {
            initial_ghost.ghost_class = 4;
        }
    }
    else
    {
        if (initial_ghost.defense > initial_ghost.speed && initial_ghost.defense > initial_ghost.luck)
        {
            initial_ghost.ghost_class = 2;
        }
        else if (initial_ghost.speed > initial_ghost.luck)
        {
            initial_ghost.ghost_class = 3;
        }
        else
        {
            initial_ghost.ghost_class = 4;
        }
    }
}

void ghostquest::battle_step(map<string, ghost>::iterator &ghost1, map<string, ghost>::iterator &ghost2, battle_history &current_battle) // battle process
{

    check(ghost1->second.character_life > 0 && ghost2->second.character_life > 0, "Your or your enemy character can not battle.");
    int numberofrounds = 1;
    while (ghost1->second.hitpoints > 0 && ghost2->second.hitpoints > 0)
    {
        if (ghost1->second.speed > ghost2->second.speed) // determine which monster attack first
        {
            damage_step(ghost1, ghost2, numberofrounds, current_battle); // perform damage calculation
            numberofrounds += 1;
            if (ghost2->second.hitpoints > 0)
            {
                damage_step(ghost2, ghost1, numberofrounds, current_battle); // perform damage calculation
                numberofrounds += 1;
                if (ghost1->second.hitpoints == 0)
                {
                    result_step(ghost1, ghost2, current_battle); // generate result(status=winner/loser/eliminated) if either character reached 0 hitpoints
                }
            }
            else
            {
                result_step(ghost2, ghost1, current_battle); // generate result(status=winner/loser/eliminated) if either character reached 0 hitpoints
            }
        }
        else
        {
            damage_step(ghost2, ghost1, numberofrounds, current_battle); // perform damage calculation
            numberofrounds += 1;
            if (ghost1->second.hitpoints > 0)
            {
                damage_step(ghost1, ghost2, numberofrounds, current_battle); // perform damage calculation
                numberofrounds += 1;
                if (ghost2->second.hitpoints == 0)
                {
                    result_step(ghost2, ghost1, current_battle); // generate result(status=winner/loser/eliminated) if either character reached 0 hitpoints
                }
            }
            else
            {
                result_step(ghost1, ghost2, current_battle); // generate result(status=winner/loser/eliminated) if either character reached 0 hitpoints
            }
        }
    }
    calculate_prize(ghost1); // calculate prize after battle
    calculate_prize(ghost2); // calculate prize after battle
}

void ghostquest::damage_step(map<string, ghost>::iterator &attacker, map<string, ghost>::iterator &defender, int round, battle_history &current_battle) // perform damage calculation
{
    int chance = attacker->second.ghost_class == 4 ? (attacker->second.luck / 3) : (attacker->second.luck / 4);
    int luck = rng(99) + 1;
    int fnldmg = 0;
    int getdmgwdt = attacker->second.attack / 16 + 1;
    int dmgwdt = rng(getdmgwdt);
    int dmgred = (attacker->second.attack * defender->second.defense) / 100;
    if (luck <= chance) // determine critical and damage width for final damage dealt
    {
        if (luck % 2 == 0)
        {
            fnldmg = attacker->second.attack + dmgwdt;
        }
        else
        {
            fnldmg = attacker->second.attack - dmgwdt;
        }
    }
    else
    {
        if (luck % 2 == 0)
        {
            fnldmg = attacker->second.attack - dmgred + dmgwdt;
        }
        else
        {
            fnldmg = attacker->second.attack - dmgred - dmgwdt;
        }
    }
    defender->second.hitpoints = ((defender->second.hitpoints - fnldmg) < 0) ? 0 : defender->second.hitpoints - fnldmg; // Hitpoints reduction
    std::string battlelog = "Round " + std::to_string(round) + " : Character of " + name{defender->second.owner}.to_string() + " took " + std::to_string(fnldmg) + " damage from character of " + name{attacker->second.owner}.to_string() + ".";
    current_battle.gameplay_log.push_back(battlelog);
}

void ghostquest::result_step(map<string, ghost>::iterator &loser, map<string, ghost>::iterator &winner, battle_history &current_battle) // modify status and other data values for both characters
{
    loser->second.status = LOSER;
    (loser->second.character_life == 1) ? ({loser->second.status = ELIMINATED; eliminated_withdrawn(loser); })
                                        : ({ loser->second.character_life -= 1; return; });
    winner->second.status = WINNER;
    winner->second.character_life += 1;
    loser->second.battle_count += 1;
    winner->second.battle_count += 1;
    std::string victorylog = " Battle Outcome : Character of " + name{winner->second.owner}.to_string() + " won against character of" + name{loser->second.owner}.to_string();
    current_battle.gameplay_log.push_back(victorylog);
}

void ghostquest::calculate_prize(map<string, ghost>::iterator &ghost) // generate prize after battle
{
    float house_edge;
    float init_prize = ghost->second.character_life * 10000;
    if (ghost->second.battle_count < 21)
    {
        house_edge = init_prize * 0.06;
    }
    else if (ghost->second.battle_count > 20 && ghost->second.battle_count < 41)
    {
        house_edge = init_prize * 0.07;
    }
    else if (ghost->second.battle_count > 40 && ghost->second.battle_count < 61)
    {
        house_edge = init_prize * 0.08;
    }
    else if (ghost->second.battle_count > 60 && ghost->second.battle_count < 81)
    {
        house_edge = init_prize * 0.09;
    }
    else
    {
        house_edge = init_prize * 0.1;
    }
    ghost->second.prize.amount = init_prize - house_edge;
}

void ghostquest::eliminated_withdrawn(map<string, ghost>::iterator &ghost) // disable characters by removing stats
{
    ghost->second.status = 6;
    ghost->second.character_life = 0;
    ghost->second.initial_hp = 0;
    ghost->second.hitpoints = 0;
    ghost->second.ghost_class = 0;
    ghost->second.ghost_level = 0;
    ghost->second.attack = 0;
    ghost->second.defense = 0;
    ghost->second.speed = 0;
    ghost->second.luck = 0;
    ghost->second.prize.amount = 0;
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