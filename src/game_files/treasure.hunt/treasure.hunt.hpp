#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;
class [[eosio::contract("treasure.hunt")]] treasurehunt : public contract {

  private:
    struct [[eosio::table]] user_info {
      uint16_t user_identifier;
      name username;
      asset user_balance;
      auto primary_key() const { return username.value; }
    };

    struct [[eosio::table]] map_info {
      uint16_t map_identifier;
      name map_name;

      uint64_t primary_key() const { return map_identifier; }
    };

    typedef eosio::multi_index<name("maps"), map_info> maps_table;

    maps_table _maps;


    enum game_status: int8_t {
      ONGOING     = 0,
      PLAYER_WON   = 1,
      PLAYER_LOST  = -1
    };

    typedef eosio::multi_index<name("users"), user_info> users_table;

    users_table _users;

  public:
    treasurehunt( name receiver, name code, datastream<const char*> ds ):contract(receiver, code, ds),
                       _users(receiver, receiver.value),
                       _seed(receiver, receiver.value) {}

    [[eosio::action]]
    void login(name username);

    [[eosio::action]]
    void startgame(name username);

    [[eosio::action]]
    void endgame(name username);

    [[eosio::action]]
    void nextround(name username)
};
