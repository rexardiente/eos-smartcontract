#include <eosio/eosio.hpp>
#include "../game_files/game.rank/game.rank.hpp"
#include "../game_files/games/games.hpp"
#include "../game_files/overall/overall.hpp"

using namespace eosio;

class [[eosio::contract("main")]] main : public contract
{

public:
  using contract::contract;
  main(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

  /* ----- GAME-RELATED FUNCTIONS AND ACTIONS ----- */ 
  // GAMES.CPP
  /* -- Add Configuration  -- */
  [[eosio::action]] void addconfig(uint64_t id)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    games::add_config addconfig("games"_n, {get_self(), "active"_n});
    addconfig.send(id);
  };

  /* -- Edit Configuration  -- */
  [[eosio::action]] void editconfig(uint64_t id)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    games::edit_config editconfig("games"_n, {get_self(), "active"_n});
    editconfig.send(id);
  };

  /* -- Delete Configuration  -- */
  [[eosio::action]] void delconfig(uint64_t id)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    games::del_config delconfig("games"_n, {get_self(), "active"_n});
    delconfig.send(id);
  };

  /* -- Add Action -- */
  [[eosio::action]] void add(uint64_t id, std::string title, std::string description)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    games::add_action addaction("games"_n, {get_self(), "active"_n});
    addaction.send(id, title, description);
  };

  /* -- Edit Action  -- */
  [[eosio::action]] void edit(uint64_t id, std::string title, std::string description)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    games::edit_action editaction("games"_n, {get_self(), "active"_n});
    editaction.send(id, title, description);
  };

  /* -- Delete Action  -- */
  [[eosio::action]] void del(uint64_t id)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    games::del_action delaction("games"_n, {get_self(), "active"_n});
    delaction.send(id);
  };

  // GAME RANK 
  /* -- Add Action -- */
  [[eosio::action]] void add(uint64_t id, std::vector<gamerank::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    gamerank::add_action addaction("game.rank"_n, {get_self(), "active"_n});
    addaction.send(id, data, createdAt);
  };

  /* -- Edit Action -- */
  [[eosio::action]] void edit(uint64_t id, std::vector<gamerank::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    gamerank::edit_action editaction("game.rank"_n, {get_self(), "active"_n});
    editaction.send(id, data, createdAt);
  };

  /* -- Delete Action -- */
  [[eosio::action]] void del(uint64_t id, std::vector<gamerank::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    gamerank::del_action delaction("game.rank"_n, {get_self(), "active"_n});
    delaction.send(id);
  };

  // OVERALL.CPP
  /* -- Add Action(Overall)  -- */
  [[eosio::action]] void add_overall(uint64_t id, std::vector<gamerank::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    overall::add_action add_overall("overall"_n, {get_self(), "active"_n});
    add_overall.send(id, data, createdAt);
  };

  /* -- Edit Action(Overall)   -- */
  [[eosio::action]] void edit_overall(uint64_t id, std::vector<gamerank::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    overall::edit_action edit_overall("overall"_n, {get_self(), "active"_n});
    edit_overall.send(id, data, createdAt);
  };
ss
  /* -- Edit Action (Overall)  -- */
  [[eosio::action]] void del_overall(uint64_t id)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "User with admin access only authorized.");
    require_auth(get_self());

    overall::del_action del_overall("overall"_n, {get_self(), "active"_n});
    del_overall.send(id);
  };
};
