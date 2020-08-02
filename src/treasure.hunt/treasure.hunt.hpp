/*
--------------------------------------------------------------
Filename: treasurehunt.hpp
Purpose: This is a header file for the TH game contract.
--------------------------------------------------------------
*/

#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

#include <eosio/asset.hpp>
using namespace std;
using namespace eosio;

class [[eosio::contract("treasure.hunt")]] treasurehunt : public contract
{
private:
  enum map_type : int8_t
  {
    EMPTY = 0,
    MAP_A = 1,
    MAP_B = 2,
    MAP_C = 3,
    VOID = 4
  };

  struct island
  {
    uint8_t type;
    uint8_t attack_point;
  };

  const map<uint8_t, island> map_dict = {
      {0, {EMPTY, 0}},
      {1, {MAP_A, 1}},
      {2, {MAP_A, 1}},
      {3, {MAP_A, 2}},
      {4, {MAP_A, 2}},
      {5, {MAP_A, 3}},
      {6, {MAP_B, 1}},
      {7, {MAP_B, 1}},
      {8, {MAP_B, 2}},
      {9, {MAP_B, 2}},
      {10, {MAP_B, 3}},
      {11, {MAP_C, 1}},
      {12, {MAP_C, 1}},
      {13, {MAP_C, 2}},
      {14, {MAP_C, 2}},
      {15, {MAP_B, 3}},
      {16, {VOID, 0}}};

  enum game_status : int8_t
  {
    ONGOING = 0,
    PLAYER_WON = 1,
    PLAYER_LOST = -1
  };

  struct game
  {
    int8_t ticket_player = 1;
    int8_t ticket_ai = 1;
    vector<uint8_t> map_player = {1, 2, 3, 4, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    vector<uint8_t> map_ai = {1, 2, 3, 4, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    vector<uint8_t> hand_player = {0, 0, 0, 0};
    vector<uint8_t> hand_ai = {0, 0, 0, 0};
    uint8_t selected_map_player = 0;
    uint8_t selected_map_ai = 0;
    uint8_t ticket_lost_player = 0;
    uint8_t ticket_lost_ai = 0;
    int8_t status = ONGOING;
  };

  struct [[eosio::table]] user
  {
    uint16_t user_ID;
    name username;
    asset user_balance;
    game game_data;

    auto primary_key() const { return username.value; }
  };

  struct [[eosio::table]] ticket
  {
    uint16_t ticket_ID;
    name ticket_name;

    auto primary_key() const { return ticket_ID; }
  };

  struct [[eosio::table]] history
  {
    uint16_t ticket_ID;
    name ticket_name;
    asset user_balance;
    game game_data;

    auto primary_key() const { return ticket_ID; }
  };

  struct [[eosio::table]] seed
  {
    uint64_t key = 1;
    uint32_t value = 1;

    auto primary_key() const { return key; }
  };

  using seed_table = eosio::multi_index<"seed"_n, seed>;
  using users_table = eosio::multi_index<"user"_n, user>;
  using ticket_table = eosio::multi_index<"ticket"_n, ticket>;
  using history_table = eosio::multi_index<"history"_n, history>;

  users_table _user;
  seed_table _seed;
  ticket_table _ticket;
  history_table _history;

  /* --- Common Actions --- */
  void generate_panel();
  void system_fairness_strategy();
  int calculate_map_coins();
  
  int calculate_attack_point(const island &island1, const island &island2);
  void resolve_selected_maps(game & game_data);
  void update_game_status(user & user);
  void new_destination(name username);

  bool has_existing_game(name username);
  void new_explorers(name username, uint16_t number_of_explorers);
  void game_history(name & username);
  void reset_game(name username);

  /* RNG function */
  int random(const int range);

public:
  treasurehunt(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                        _user(receiver, receiver.value),
                                                                        _seed(receiver, receiver.value),
                                                                        _ticket(receiver, receiver.value),
                                                                        _history(receiver, receiver.value) {}

  /* --- Authenticated Actions --- */
  [[eosio::action]] void login(name username);
  [[eosio::action]] void startgame(name username);
  [[eosio::action]] void endgame(name username);
  [[eosio::action]] void playhunt(name username, uint8_t player_map_idx);
};
