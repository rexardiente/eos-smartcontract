#include <eosio/eosio.hpp>
#include "../game_files/game.rank/game.rank.hpp"
#include "../game_files/games/games.hpp"
#include "../game_files/overall/overall.hpp"
#include "../token_files/donut.token/donut.token.hpp"

#include <eosio/asset.hpp>
// #include <eosio.cdt/eosio.hpp>

using namespace eosio;

class [[eosio::contract("main")]] main : public contract
{

public:
  using contract::contract;
  main(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

  /* ----- GAME-RELATED FUNCTIONS AND ACTIONS ----- */
  // GAMES.CPP
  // @abi action
  /* -- Add Configuration  -- */
  [[eosio::action]] void addconfig(uint64_t id)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    games::add_config addconfig("games"_n, {get_self(), "active"_n});
    addconfig.send(id);
  };

  // @abi action
  /* -- Edit Configuration  -- */
  [[eosio::action]] void editconfig(uint64_t id)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    games::edit_config editconfig("games"_n, {get_self(), "active"_n});
    editconfig.send(id);
  };

  // @abi action
  /* -- Delete Configuration  -- */
  [[eosio::action]] void delconfig(uint64_t id)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    games::del_config delconfig("games"_n, {get_self(), "active"_n});
    delconfig.send(id);
  };

  // @abi action
  /* -- Add Action -- */
  [[eosio::action]] void addgame(uint64_t id, std::string title, std::string description)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    games::add_action addaction("games"_n, {get_self(), "active"_n});
    addaction.send(id, title, description);
  };

  // @abi action
  /* -- Edit Action  -- */
  [[eosio::action]] void editgame(uint64_t id, std::string title, std::string description)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    games::edit_action editaction("games"_n, {get_self(), "active"_n});
    editaction.send(id, title, description);
  };

  // @abi action
  /* -- Delete Action  -- */
  [[eosio::action]] void delgame(uint64_t id)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    games::del_action delaction("games"_n, {get_self(), "active"_n});
    delaction.send(id);
  };

  // GAME RANK
  // @abi action
  /* -- Add Action -- */
  [[eosio::action]] void addgamerank(uint64_t id, std::vector<gamerank::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    gamerank::add_action addaction("game.rank"_n, {get_self(), "active"_n});
    addaction.send(id, data, createdAt);
  };

  // @abi action
  /* -- Edit Action -- */
  [[eosio::action]] void editgamerank(uint64_t id, std::vector<gamerank::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    gamerank::edit_action editaction("game.rank"_n, {get_self(), "active"_n});
    editaction.send(id, data, createdAt);
  };

  // @abi action
  /* -- Delete Action -- */
  [[eosio::action]] void delgamerank(uint64_t id, std::vector<gamerank::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    gamerank::del_action delaction("game.rank"_n, {get_self(), "active"_n});
    delaction.send(id);
  };

  // OVERALL.CPP
  // @abi action
  /* -- Add Action(Overall)  -- */
  [[eosio::action]] void addoverall(uint64_t id, std::vector<overall::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    overall::add_action addaction("overall"_n, {get_self(), "active"_n});
    addaction.send(id, data, createdAt);
  };

  /* -- Edit Action(Overall)   -- */
  
  [[eosio::action]] void editoverall(uint64_t id, std::vector<overall::user> data, uint64_t createdAt)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    overall::edit_action edit_overall("overall"_n, {get_self(), "active"_n});
    edit_overall.send(id, data, createdAt);
  };

  /* -- Edit Action (Overall)  -- */
  [[eosio::action]] void deloverall(uint64_t id)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    overall::del_action del_overall("overall"_n, {get_self(), "active"_n});
    del_overall.send(id);
  };
  

  // TOKEN CONTRACT
  /* -- Create Action -- */
  [[eosio::action]] void create(const name& issuer, const asset&  max_supply)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    donuttoken::create_action create("create"_n, {get_self(), "active"_n});
    create.send(issuer, max_supply);
  };

  /* -- Issue Action -- */
  [[eosio::action]] void issue(name to, asset quantity, string memo)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    donuttoken::issue_action issue("issue"_n, {get_self(), "active"_n});
    issue.send(to, quantity, memo);
  };

  /* -- Transfer Action -- */
  [[eosio::action]] void transfer(eosio::name from, eosio::name to, eosio::asset quantity, std::string memo)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    donuttoken::transfer_action transfer("transfer"_n, {get_self(), "active"_n});
    transfer.send(from ,to, quantity, memo);
  };

  /* -- Burn Action -- */
  [[eosio::action]] void burn(eosio::asset quantity, std::string memo)
  {
    // Check if the user is authorized to action
    check(has_auth(get_self()), "Only users with admin access are authorized to execute this function.");
    require_auth(get_self());

    donuttoken::burn_action burn("burn"_n, {get_self(), "active"_n});
    burn.send(quantity, memo);
  };
};