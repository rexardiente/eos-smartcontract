#include <eosio/eosio.hpp>
#include "../game_files/game.rank/game.rank.hpp"

using namespace eosio;

class [[eosio::contract("main")]] main : public contract
{

public:
  using contract::contract;
  main(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

  /* ----- GAME-RELATED FUNCTIONS AND ACTIONS ----- */ 
  /* -- GAME.RANK DELETE -- */
    [[eosio::action]] void del(uint64_t id, std::vector<gamerank::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    gamerank::del_action delaction("game.rank"_n, {get_self(), "active"_n});
    delaction.send(id);
  };

  /* -- GAME.RANK ADD -- */
    [[eosio::action]] void add(uint64_t id, std::vector<gamerank::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    gamerank::add_action addaction("game.rank"_n, {get_self(), "active"_n});
    addaction.send(id, data, createdAt);
  };

  /* -- GAME.RANK EDIT -- */
  [[eosio::action]] void edit(uint64_t id, std::vector<gamerank::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    gamerank::edit_action editaction("game.rank"_n, {get_self(), "active"_n});
    editaction.send(id, data, createdAt);
  };
};
