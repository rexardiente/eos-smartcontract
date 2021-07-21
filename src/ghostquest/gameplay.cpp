#include "ghostquest.hpp"
#include <string>


void ghostquest::calculate_prize(map<string, character>::iterator &character) // generate prize after battle
{
    float house_edge;
    float init_prize = character->second.character_life;
    if (character->second.battle_count < 21)
    {
        house_edge = init_prize * 0.06;
    }
    else if (character->second.battle_count > 20 && character->second.battle_count < 41)
    {
        house_edge = init_prize * 0.07;
    }
    else if (character->second.battle_count > 40 && character->second.battle_count < 61)
    {
        house_edge = init_prize * 0.08;
    }
    else if (character->second.battle_count > 60 && character->second.battle_count < 81)
    {
        house_edge = init_prize * 0.09;
    }
    else
    {
        house_edge = init_prize * 0.1;
    }
    character->second.prize = init_prize - house_edge;
}

void ghostquest::eliminated_withdrawn(map<string, character>::iterator &character) // disable characters by removing stats
{
    character->second.status = 6;
    character->second.character_life = 0;
    // character->second.initial_hp = 0;
    character->second.hitpoints = 0;
    // character->second.character_class = 0;
    // character->second.character_level = 0;
    character->second.attack = 0;
    character->second.defense = 0;
    character->second.speed = 0;
    character->second.luck = 0;
    character->second.prize = 0;
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

string ghostquest::checksum256_to_string_hash()
{   
    auto size = transaction_size();
    char buf[size];
    check(size == read_transaction(buf, size), "read_transaction failed");
    checksum256 sha = sha256(buf, size);
    auto hbytes = sha.extract_as_byte_array();
    std::string hash_id;

    const char *to_hex = "0123456789abcdef";
    for (uint32_t i = 0; i < hbytes.size(); ++i) { (hash_id += to_hex[(hbytes[i] >> 4)]) += to_hex[(hbytes[i] & 0x0f)]; }
    return hash_id;
}

// void ghostquest::ondeposit(name from,
//                            name to,
//                            asset quantity,
//                            string memo)
// {
//     if (from == _self)
//     {
//         if (memo.find(HAS_ON_SETTLE_PAY) != std::string::npos)
//         {
//             onsettledpay(to, quantity, memo);
//         }

//         // we're sending money, do nothing additional
//         return;
//     }
//     check(to == _self, "Not to our contract");
//     check(quantity.symbol.is_valid(), "Invalid quantity");
//     check(quantity.amount > 0, "Only positive quantity allowed");
//     check(quantity.symbol == ghostquest_symbol, "Invalid EOS Token");

//     std::string str = memo.substr(9);
//     if (memo.find("ADD_LIFE") != std::string::npos)
//     {
//         int key = stoi(str);
//         set_add_life(from, quantity, key);
//     }
//     else
//     {
//         int limit = stoi(str);
//         summon_ready(from, quantity, limit);
//     }
// }


// void ghostquest::onsettledpay(name username, asset quantity, string memo)
// {
//     require_auth(_self);
//     action(
//         permission_level{_self, "active"_n},
//         _self,
//         "settledpay"_n,
//         std::make_tuple(username, quantity, memo))
//         .send();
// }

// void ghostquest::gen_stat(ghost &initial_ghost) // function for generating monster/s status
// {
//     initial_ghost.attack = 25 + rng(50);
//     initial_ghost.defense = 25 + rng(50);
//     initial_ghost.speed = 25 + rng(50);
//     initial_ghost.luck = 25 + rng(50);
//     int sum = initial_ghost.attack + initial_ghost.defense + initial_ghost.speed + initial_ghost.luck + initial_ghost.hitpoints;
//     if (sum > 199 && sum < 286)
//     {
//         initial_ghost.ghost_level = 1;
//     }
//     else if (sum > 285 && sum < 311)
//     {
//         initial_ghost.ghost_level = 2;
//     }
//     else if (sum > 310 && sum < 341)
//     {
//         initial_ghost.ghost_level = 3;
//     }
//     else if (sum > 340 && sum < 386)
//     {
//         initial_ghost.ghost_level = 4;
//     }
//     else
//     {
//         initial_ghost.ghost_level = 5;
//     }
//     if (initial_ghost.attack > initial_ghost.defense)
//     {
//         if (initial_ghost.attack > initial_ghost.speed && initial_ghost.attack > initial_ghost.luck)
//         {
//             initial_ghost.ghost_class = 1;
//         }
//         else if (initial_ghost.speed > initial_ghost.luck)
//         {
//             initial_ghost.ghost_class = 3;
//         }
//         else
//         {
//             initial_ghost.ghost_class = 4;
//         }
//     }
//     else
//     {
//         if (initial_ghost.defense > initial_ghost.speed && initial_ghost.defense > initial_ghost.luck)
//         {
//             initial_ghost.ghost_class = 2;
//         }
//         else if (initial_ghost.speed > initial_ghost.luck)
//         {
//             initial_ghost.ghost_class = 3;
//         }
//         else
//         {
//             initial_ghost.ghost_class = 4;
//         }
//     }
// }

// void ghostquest::battle_step(map<int, ghost>::iterator &ghost1, map<int, ghost>::iterator &ghost2) // battle process
// {
//     check(ghost1->second.character_life > 0 && ghost2->second.character_life > 0, "Your or your enemy character can not battle.");
//     int numberofrounds = 1;
//     while (ghost1->second.hitpoints > 0 && ghost2->second.hitpoints > 0)
//     {
//         if (ghost1->second.speed > ghost2->second.speed) // determine which monster attack first
//         {
//             damage_step(ghost1, ghost2, numberofrounds); // perform damage calculation
//             numberofrounds += 1;
//             if (ghost2->second.hitpoints > 0)
//             {
//                 damage_step(ghost2, ghost1, numberofrounds); // perform damage calculation
//                 numberofrounds += 1;
//                 if (ghost1->second.hitpoints == 0)
//                 {
//                     result_step(ghost1, ghost2); // generate result(status=winner/loser/eliminated) if either character reached 0 hitpoints
//                 }
//             }
//             else
//             {
//                 result_step(ghost2, ghost1); // generate result(status=winner/loser/eliminated) if either character reached 0 hitpoints
//             }
//         }
//         else
//         {
//             damage_step(ghost2, ghost1, numberofrounds); // perform damage calculation
//             numberofrounds += 1;
//             if (ghost1->second.hitpoints > 0)
//             {
//                 damage_step(ghost1, ghost2, numberofrounds); // perform damage calculation
//                 numberofrounds += 1;
//                 if (ghost2->second.hitpoints == 0)
//                 {
//                     result_step(ghost2, ghost1); // generate result(status=winner/loser/eliminated) if either character reached 0 hitpoints
//                 }
//             }
//             else
//             {
//                 result_step(ghost1, ghost2); // generate result(status=winner/loser/eliminated) if either character reached 0 hitpoints
//             }
//         }
//     }
//     calculate_prize(ghost1); // calculate prize after battle
//     calculate_prize(ghost2); // calculate prize after battle
// }

// void ghostquest::damage_step(map<int, ghost>::iterator &attacker, map<int, ghost>::iterator &defender, int round) // perform damage calculation
// {

//     int chance = attacker->second.luck / 4;
//     int luck = rng(99) + 1;
//     int fnldmg = 0;
//     int getdmgwdt = attacker->second.attack / 16 + 1;
//     int dmgwdt = rng(getdmgwdt);
//     int dmgred = (attacker->second.attack * defender->second.defense) / 100;
//     if (luck <= chance) // determine critical and damage width for final damage dealt
//     {
//         if (luck % 2 == 0)
//         {
//             fnldmg = attacker->second.attack + dmgwdt;
//         }
//         else
//         {
//             fnldmg = attacker->second.attack - dmgwdt;
//         }
//     }
//     else
//     {
//         if (luck % 2 == 0)
//         {
//             fnldmg = attacker->second.attack - dmgred + dmgwdt;
//         }
//         else
//         {
//             fnldmg = attacker->second.attack - dmgred - dmgwdt;
//         }
//     }
//     defender->second.hitpoints = ((defender->second.hitpoints - fnldmg) < 0) ? 0 : defender->second.hitpoints - fnldmg; // Hitpoints reduction
//     print("Round " + std::to_string(round) + ". Character of " + std::to_string(defender->second.owner_id) + " took " + std::to_string(fnldmg) + " damage from character of " + std::to_string(attacker->second.owner_id) + " .. ");
// }

// void ghostquest::result_step(map<int, ghost>::iterator &loser, map<int, ghost>::iterator &winner) // modify status and other data values for both characters
// {

//     loser->second.status = LOSER;
//     if (loser->second.character_life == 1)
//     {
//         loser->second.status = ELIMINATED;
//         eliminated_withdrawn(loser);
//     }
//     else
//     {
//         loser->second.character_life -= 1;
//     }
//     winner->second.status = WINNER;
//     winner->second.character_life += 1;
//     loser->second.battle_count += 1;
//     winner->second.battle_count += 1;
// }

