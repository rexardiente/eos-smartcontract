#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
using namespace std;
using namespace eosio;

class [[eosio::contract("treasurehunt")]] treasurehunt : public contract {
  private:
  enum map_type: int8_t {
	  EMPTY = 0,
    MAP_A = 1,
	  MAP_B = 2,
	  MAP_C = 3,
    VOID = 4
  };

  struct island {
    uint8_t type;
    uint8_t attack_point;
  };

  const map <uint8_t, island> map_dict = {
    { 0, { EMPTY, 0 }},
    { 1, { MAP_A, 1 }},
    { 2, { MAP_A, 1 }},
    { 3, { MAP_A, 2 }},
    { 4, { MAP_A, 2 }},
    { 5, { MAP_A, 3 }},
    { 6, { MAP_B, 1 }},
    { 7, { MAP_B, 1 }},
    { 8, { MAP_B, 2 }},
    { 9, { MAP_B, 2 }},
    { 10, { MAP_B, 3 }},
    { 11, { MAP_C, 1 }},
    { 12, { MAP_C, 1 }},
    { 13, { MAP_C, 2 }},
    { 14, { MAP_C, 2 }},
    { 15, { MAP_B, 3 }},
    { 16, { VOID, 0 }}
  };

  enum game_status: int8_t {
    ONGOING     = 0,
    PLAYER_WON   = 1,
    PLAYER_LOST  = -1
  };

  enum set_sail: int8_t {
    NOT_READY   = 0,
    READY       = 1
  };

  enum newDestination: int8_t {
    NO        = 0,
    YES       = 1
  };

  struct game {
    int8_t          ticket_player = 1;
    //int8_t          ticket_ai = 1;
    vector<uint8_t> map_player = {1,2,3,4,7,8,9,10,11,12,13,14,15,16};
    //vector<uint8_t> map_ai = {1,2,3,4,7,8,9,10,11,12,13,14,15,16};
    vector<uint8_t> hand_player = {0,0,0,0};
    //vector<uint8_t> hand_ai = {0,0,0,0};
    uint8_t         selected_map_player = 0;
   // uint8_t         selected_map_ai = 0;
    uint8_t         ticket_lost_player = 0;
   // uint8_t         ticket_lost_ai = 0;
    int8_t          status = ONGOING;
    int8_t          setsail = NOT_READY;
    int8_t          newdestination = NO;
  };

  struct [[eosio::table]] user {
    uint16_t user_ID;
    name username;
    asset user_balance;
    game game_data;
    auto primary_key() const { return username.value; }
  };

  struct [[eosio::table]] ticket {
    uint16_t ticket_ID;
    name ticket_name;
    auto primary_key() const { return ticket_name.value; }
  };

  struct [[eosio::table]] seed {
    uint64_t        key = 1;
    uint32_t        value = 1;
    auto primary_key() const { return key; }
  };

  typedef eosio::multi_index<name("seed"), seed> seed_table;
  typedef eosio::multi_index<name("user"), user> users_table;
  typedef eosio::multi_index<name("ticket"), ticket> ticket_table;

  users_table _users;
  seed_table _seed;
  ticket_table _tickets;

  /* RNG function */
  int random(const int range);

  /* --- Common Actions --- */
  void generate_panel();
  void system_fairness_strategy();
  int calculate_map_coins();
  int calculate_attack_point(const island& island1, const island& island2);
  void resolve_selected_maps(game& game_data);
  void calculatePrize(name username,uint64_t results);
  void generatePrize(name username, uint8_t selected_map_player);
  bool hasexisting(user username);

  public:
  treasurehunt( name receiver, name code, datastream<const char*> ds ):contract(receiver, code, ds),
                       _users(receiver, receiver.value),
                       _seed(receiver, receiver.value),
                       _tickets(receiver, receiver.value) {}

  /* --- Authenticated Actions --- */
  [[eosio::action]]
  void loguser(name username);

  [[eosio::action]]
  void startgame(name username,uint8_t  selected_map_player);

  [[eosio::action]]
  void endgame(name username);

  [[eosio::action]]
  void resetgame(name username);

  [[eosio::action]]
  void destination(name username);

  [[eosio::action]]
  void gamehistory(name username);

  [[eosio::action]]
  void gamestatus(name username);

  [[eosio::action]]
  void setsail(name username);

  [[eosio::action]]
  void newexplorers(name username, uint16_t number_of_explorers);

  [[eosio::action]]
  void nextround(name username);

  [[eosio::action]]
  void playhunt(name username, uint8_t player_map_idx);
};
