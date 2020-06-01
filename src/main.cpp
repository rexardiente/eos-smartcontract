#include <eosio/eosio.hpp>

#define user_name name

// #include "game_files/game.rank/game.rank.hpp"
#include "game_files/game.rank/game.rank.cpp"

class [[eosio::contract("panels")]] panels : public eosio::contract
{
private:
  gamerank gamerank_controller(name receiver, name code, datastream<const char *> ds);

public:
  using contract::contract;
  panels(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

  //gamerank_controller(_self) {
  	[[eosio::action]] void add(uint64_t id, std::vector<gamerank::user> data, uint64_t created_at)
  	{
    	gamerank_controller.add(id, data, created_at);
  	}
  //};
};