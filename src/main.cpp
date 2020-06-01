#include <eosio/eosio.hpp>

#define user_name name

#include "game_files/game.rank/game.rank.cpp"

class [[eosio::contract("main")]] main : public eosio::contract
{
private:
  gamerank gamerank_controller(name receiver, name code, datastream<const char *> ds);

public:
  using contract::contract;
  main(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

  [[eosio::action]] void add(uint64_t id, std::vector<gamerank::user> data, uint64_t created_at)
  {
    gamerank_controller.add(id, data, created_at);
  }
};