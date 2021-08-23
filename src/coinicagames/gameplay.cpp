#include <eosio/transaction.hpp>
#include "coinicagames.hpp"

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
    character->second.character_life = DEFAULT;
    character->second.hitpoints = DEFAULT;
    character->second.attack = DEFAULT;
    character->second.defense = DEFAULT;
    character->second.speed = DEFAULT;
    character->second.luck = DEFAULT;
    character->second.prize = DEFAULT;
}

// Simple Pseudo Random Number Algorithm, randomly pick a number within 0 to n-1
int coinicagames::random(const int range) {
  // Find the existing seed
  auto seed_iterator = _seeds.begin();

  // Initialize the seed with default value if it is not found
  if (seed_iterator == _seeds.end()) {
    seed_iterator = _seeds.emplace( _self, [&]( auto& seed ) { });
  }

  // Generate new seed value using the existing seed value
  int prime = 65537;
  auto new_seed_value = (seed_iterator->value + current_time_point().elapsed.count()) % prime;
  
  // Store the updated seed value in the table
  _seeds.modify( seed_iterator, _self, [&]( auto& s ) {
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

void coinicagames::showremainingtile(thgamedata &game_data)
{
    check(game_data.status == TH_DONE, "Game hasn't ended yet.");
    int available_tile = game_data.unopentile - game_data.enemy_count;
    for (size_t i = 0; i < TH_PANEL_SIZE; i++) {
        if (game_data.panel_set.at(i).isopen == 0) {
            if ((random(TH_PANEL_SIZE) % 2) && available_tile > 0) {
                game_data.panel_set.at(i).iswin = 1;
                available_tile --;
            }
            game_data.panel_set.at(i).isopen = 1;
        }
    }
}

double coinicagames::generateprize(thgamedata thgamedata)
{
    double game_prize = thgamedata.prize;
    double odds = calculateodds(thgamedata);
    game_prize = (game_prize * odds) * 0.98;
    return game_prize;
}

double coinicagames::calculateodds(thgamedata thgamedata)
{
    double rem_panel = (double)thgamedata.unopentile - (double)thgamedata.enemy_count;
    return ((double)thgamedata.unopentile) / (double)rem_panel;
}

double coinicagames::maxprize(thgamedata thgamedata)
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

void coinicagames::gameupdate(thgamedata &game_data)
{
    if (game_data.status == TH_ONGOING)
    {
        if(game_data.win_count!=16-game_data.enemy_count)
        {
            game_data.nextprize = generateprize(game_data);
            game_data.odds = calculateodds(game_data);
        }
        else
        {
            game_data.nextprize = DEFAULT;
            game_data.odds = DEFAULT;
        }
    }
    if (game_data.status == TH_DONE) {
        showremainingtile(game_data);
        game_data.prize = DEFAULT;
        game_data.nextprize = DEFAULT;
        game_data.odds = DEFAULT;
        game_data.unopentile = DEFAULT; // reset unopentile to empty
    }
}