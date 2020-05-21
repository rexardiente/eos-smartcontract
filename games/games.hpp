#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("games")]] games : public contract
{
private:
  struct [[eosio::table]] game
  { 
    uint64_t id;
    std::string title;
    std::string description;

    auto primary_key() const { return id; }
    EOSLIB_SERIALIZE(game, (id)(title)(description))
  };

  /* This config table is just example for games configuration
  *  and changes will be made based on gameplay.
  */
  struct [[eosio::table]] config
  {
    uint64_t id; // game table id

    auto primary_key() const { return id; }
    EOSLIB_SERIALIZE(config, (id))
  };

  typedef eosio::multi_index<"games"_n, game> game_index;
  typedef eosio::multi_index<"configs"_n, config> config_index;
  
public:
  using contract::contract;
  games(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}
  
  // Configuration actions
  [[eosio::action]] void addconfig(const uint64_t id);
  [[eosio::action]] void editconfig(const uint64_t id);
  [[eosio::action]] void delconfig(const uint64_t id);

  // Game actions
  [[eosio::action]] void add(const uint64_t id, std::string title, std::string description);
  [[eosio::action]] void edit(const uint64_t id, std::string name, std::string description);
  [[eosio::action]] void del(const uint64_t id);
};