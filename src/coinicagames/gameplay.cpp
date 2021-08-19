#include "coinicagames.hpp"
#include <string>


void coinicagames::calculate_prize(map<string, character>::iterator &character) // generate prize after battle
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

void coinicagames::eliminated_withdrawn(map<string, character>::iterator &character) // disable characters by removing stats
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

int coinicagames::rng(const int &range)
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

string coinicagames::checksum256_to_string_hash()
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

// 15 ALL
// 5 enemy
// 10
// 0 tile = lost

void coinicagames::showremainingtile(thgame &thgame_data)
{
    check(thgame_data.status == TH_DONE, "Game hasn't ended yet.");
    int available_tile = thgame_data.unopentile - thgame_data.enemy_count;
    // vector<thtile> panel_set = thgame_data.panel_set;
    // thgame_data.panel_set.size()
    for (size_t i = 0; i < TH_PANEL_SIZE; i++) {
        //  uint8_t key; // panel index(panel_idx)
        // uint8_t isopen = TH_IS_OPEN_PANEL;
        // uint8_t iswin = TH_IS_WIN_PANEL;
        if (thgame_data.panel_set.at(i).isopen == 0) {
            // check 

            int prime = 65537;
            int old_seed = 15;
            auto new_seed_value = (old_seed + current_time_point().elapsed.count()) % prime;
            // Store the updated seed value in the table
            old_seed = new_seed_value;
            // Get the random result in desired range
            if ((new_seed_value % TH_PANEL_SIZE) && available_tile > 0) {
                thgame_data.panel_set.at(i).iswin = 1;
                available_tile --;
            }
            thgame_data.panel_set.at(i).isopen = 1;
        }
    }
    // while (available_tile > 0)
    // {
    //     int random_result = rng(15);
    //     if (thgame_data.panel_set.at(random_result).isopen == 0)
    //     {
    //         thgame_data.panel_set.at(random_result).isopen = 1;
    //         thgame_data.panel_set.at(random_result).iswin = 1;
    //         available_tile--;
    //     }
    // }
    // // open all panels and show treasures and enemies
    // for (int i = 0; i < TH_PANEL_SIZE; i++)
    // {
    //     if (thgame_data.panel_set.at(i).isopen == 0 && thgame_data.panel_set.at(i).iswin == 0)
    //     {
    //         thgame_data.panel_set.at(i).isopen = 1;
    //     }
    // }
}

double coinicagames::generateprize(thgame thgamedata)
{
    double game_prize = thgamedata.prize;
    double odds = calculateodds(thgamedata);
    game_prize = (game_prize * odds) * 0.98;
    return game_prize;
}

double coinicagames::calculateodds(thgame thgamedata)
{
    double rem_panel = (double)thgamedata.unopentile - (double)thgamedata.enemy_count;
    return ((double)thgamedata.unopentile) / (double)rem_panel;
}

double coinicagames::maxprize(thgame thgamedata)
{
    double game_prize = thgamedata.prize;
    float rem_panel = thgamedata.panel_set.size() - thgamedata.enemy_count;
    for (int i = 0; i < rem_panel; i++)
    {
        float odds = (float)(thgamedata.panel_set.size() - i) / (float)(rem_panel - i);
        float current_prize = (game_prize * odds) * 0.98;

        game_prize = current_prize;
    }

    return game_prize;
}

void coinicagames::gameupdate(thgame &thgame_data)
{

    if (thgame_data.status == TH_INITIALIZED)
    {
        thgame_data.maxprize = maxprize(thgame_data);
        thgame_data.status = TH_ONGOING;
    }

    if (thgame_data.status == TH_ONGOING)
    {
        if(thgame_data.win_count!=16-thgame_data.enemy_count)
        {
        thgame_data.nextprize = generateprize(thgame_data);
        thgame_data.odds = calculateodds(thgame_data);
        }
        else
        {
        thgame_data.nextprize = TH_DEFAULT;
        thgame_data.odds = TH_DEFAULT;
        }
        
    }
    if (thgame_data.status == TH_DONE)
    {
        showremainingtile(thgame_data);
        thgame_data.prize = TH_DEFAULT;
        thgame_data.nextprize = TH_DEFAULT;
        thgame_data.odds = TH_DEFAULT;
        thgame_data.unopentile = TH_DEFAULT; // reset unopentile to empty
    }
}