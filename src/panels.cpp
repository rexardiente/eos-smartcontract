/*
-------------------------------------------------------------------------------
File name: panels.cpp
Creator: Brett A. Harvey
Purpose: the main program for executing all of the contract code.
-------------------------------------------------------------------------------
*/

#include <eosio/eosio.hpp>

/* Including external files for execution. */
#include "game_files/game.rank/game.rank.hpp"
#include "game_files/game.rank/game.rank.cpp"
#include "game_files/games/games.hpp"
#include "game_files/game.rank/games.cpp"
#include "game_files/overall/overall.hpp"
#include "game_files/overall/overall.cpp"
#include "smart_contract_mechanics/payable/payable.cpp"
#include "token_files/donut.token/donut.token.hpp"
#include "token_files/donut.token/donut.token.cpp"

class panels : public eosio::contract {
private:
  gamerank gamerank_controller;
  games games_controller;
  overall overall_controller;
  payable payable_controller;
  donutToken  donutToken_controller;

public:
  panels(self)
  :eosio::contract(self),
  gamerank_controller(self),
  games_controller(self),
  overall_controller(self),
  payable_controller(self),
  donutToken_controller(self) {

    /* game-related actions */
    /*-------------------------------------------------------------------------*/
    /// @abi action DELETE
    void delete(uint64_t id) {
      gamerank_controller.del(id);
    }

    /// @abi action ADD
    void add(uint64_t id, std::vector<user> data, uint64_t created_at) {
      gamerank_controller.add(id, data, created_at);
    }

    /// @abi action EDIT
    void edit(uint64_t id, std::vector<user> data, uint64_t created_at) {
      gamerank_controller.edit(id, data, created_at);
    }

    /* --- Configuration Actions --- */
    /// @abi action ADD CONFIG
    void addconfig(const uint64_t id) {
      games_controller(id);
    }

    /// @abi action EDIT CONFIG
    void editconfig(const uint64_t id) {
      games_controller(id);
    }

    /// @abi action DELETE CONFIG
    void delconfig(const uint64_t id) {
      games_controller(id);
    }

    /// @abi action DELETE
    void del(const uint64_t id) {
      games_controller.del(id);
    }

    /// @abi action ADD
    void add(const uint64_t id, std::string title, std::string description) {
      games_controller.add(id, title, description);
    }

    /// @abi action EDIT
    void edit(const uint64_t id, std::string name, std::string description) {
      games_controller.edit(id, name, description);
    }

    /* -- Overall functions -- */
    /// @abi action DELETE
    void del(const uint64_t id) {
      overall_controller.del(id);
    }

    /// @abi action ADD
    void add(uint64_t id, std::vector<user> data, uint64_t created_at) {
      overall_controller.add(id, data, created_at);
    }

    /// @abi action EDIT
    void edit(uint64_t id, std::vector<user> data, uint64_t created_at) {
      overall_controller.edit(id, data, created_at);
    }

    /* sc-mechanics-related actions */
    /*-------------------------------------------------------------------------*/
    /// @abi action DEPOSIT
    void deposit(name from, name to, eosio::asset quantity, std::string memo) {
      payable_controller.deposit(from, to, quantity, memo);
    }

    /// @abi action WITHDRAW
    void withdraw(name acc) {
      payable_controller.withdraw(acc);
    }

    /* token-related actions */
    /*-----------------------------;--------------------------------------------*/
    /// @abi action CREATE TOKEN
    void create(const name& token_issuer, const asset&  maximum_supply) {
      require_auth(name("panels")); // Only the panels account can authorize this action. 
      donutToken_controller.create(token_issuer, max_supply);
    }

    /// @abi action ISSUE TOKEN
    void issue(const name& token_receiver, const asset& token_amount, const string& memo_add) {
      require_auth(name("panels")); // Only the panels account can authorize this action. 
      donutToken_controller.issue(token_receiver, token_amount, memo_add);
    }

    /// @abi action TRANSFER TOKEN
    void transfer(const name& token_sender, const name& token_receiver,
      const asset& token_amount, const string&  memo_add) {
      donutToken_controller.transfer(token_sender, token_receiver, token_amount, memo_add);
    }
  }
}
