#include "ghostquest.hpp"

void ghostquest::gen_stat(character &ch) // function for generating monster/s status
{
    ch.ATTACK = 25 + rng(50);
    ch.DEFENSE = 25 + rng(50);
    ch.SPEED = 25 + rng(50);
    ch.LUCK = 25 + rng(50);
    int sum = ch.ATTACK + ch.DEFENSE + ch.SPEED + ch.LUCK + ch.HP;

    if (sum > 199 && sum < 286) { ch.LEVEL = 1; }
    else if (sum > 285 && sum < 311) { ch.LEVEL = 2; }
    else if (sum > 310 && sum < 341) { ch.LEVEL = 3; }
    else if (sum > 340 && sum < 386) { ch.LEVEL = 4; } 
    else { ch.LEVEL = 5; }

    if (ch.ATTACK > ch.DEFENSE) 
    {
        if (ch.ATTACK > ch.SPEED && ch.ATTACK > ch.LUCK) { ch.CLASS = 1; }
        else if (ch.SPEED > ch.LUCK) { ch.CLASS = 3; }
        else { ch.CLASS = 4; }
    }
    else 
    {
        if (ch.DEFENSE > ch.SPEED && ch.DEFENSE > ch.LUCK) { ch.CLASS = 2; }
        else if (ch.SPEED > ch.LUCK) { ch.CLASS = 3; }
        else { ch.CLASS = 4; }
    }
}

int ghostquest::rng(const int &range)
{
    auto seed_iterator = _seeds.begin();

    if (seed_iterator == _seeds.end())
    {
        seed_iterator = _seeds.emplace(_self, [&](auto &seed) {});
    }

    int prime = 65537;
    auto new_seed_value = (seed_iterator->value + current_time_point().elapsed.count()) % prime;

    _seeds.modify(seed_iterator, _self, [&](auto &s)
    {
        s.value = new_seed_value;
    });

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

void ghostquest::ondeposit(name from, name to, asset quantity, string memo)
{
    if (from == _self)
    {
        if (memo.find(HAS_ON_SETTLE_PAY) != std::string::npos) { onsettledpay(to, quantity, memo); }
        // we're sending money, do nothing additional
        return;
    }
    check(to == _self, "Not to our contract");
    check(quantity.symbol.is_valid(), "Invalid quantity");
    check(quantity.amount > 0, "Only positive quantity allowed");
    check(quantity.symbol == ghostquest_symbol, "Invalid EOS Token");

    std::string str = memo.substr(9);
    if (memo.find("ADD_LIFE") != std::string::npos) { onaddnewlife(from, quantity, str); }
    else
    {
        int limit = stoi(str);
        oninit(from, quantity, limit);
    }
}

void ghostquest::oninit(name username, asset quantity, int limit) // trigger summoning after transfer transaction
{
    require_auth(username);
    action(permission_level{_self, "active"_n}, _self, "init"_n, std::make_tuple(username, quantity, limit)).send();
}

void ghostquest::onaddnewlife(name username, asset quantity, string key) // trigger adding life after transfer transaction
{
    require_auth(username);
    action(permission_level{_self, "active"_n}, _self, "newlife"_n, std::make_tuple(username, quantity, key)).send();
}

void ghostquest::onsettledpay(name username, asset quantity, string memo)
{
    require_auth(_self);
    action(permission_level{_self, "active"_n}, _self, "settledpay"_n, std::make_tuple(username, quantity, memo)).send();
}

asset ghostquest::calculate_prize(map<string, character>::iterator character) // generate prize after battle
{   
    float house_edge;
    float init_prize = character->second.LIFE * 10000;
    if (character->second.GAME_COUNT < 21) { house_edge = init_prize * 0.06; }
    else if (character->second.GAME_COUNT > 20 && character->second.GAME_COUNT < 41) { house_edge = init_prize * 0.07; }
    else if (character->second.GAME_COUNT > 40 && character->second.GAME_COUNT < 61) { house_edge = init_prize * 0.08; }
    else if (character->second.GAME_COUNT > 60 && character->second.GAME_COUNT < 81) { house_edge = init_prize * 0.09; }
    else { house_edge = init_prize * 0.1;}
    
    return asset(init_prize - house_edge, symbol(MAIN_TOKEN, PRECISION));
}
