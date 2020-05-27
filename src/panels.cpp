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
    /// @abi action

    /* sc-mechanics-related actions */
    /*-------------------------------------------------------------------------*/
    /// @abi action

    /* token-related actions */
    /*-----------------------------;--------------------------------------------*/
    /// @abi action CREATE TOKEN
    void create(const name& token_issuer, const asset&  maximum_supply) {
      donutToken_controller.create(token_issuer, max_supply);
    }

    /// @abi action ISSUE TOKEN
    void issue(const name& token_receiver, const asset& token_amount, const string& memo_add) {
      donutToken_controller.issue(token_receiver, token_amount, memo_add);
    }

    /// @abi action TRANSFER TOKEN
    void transfer(const name& token_sender, const name& token_receiver,
      const asset& token_amount, const string&  memo_add) {
      donutToken_controller.transfer(token_sender, token_receiver, token_amount, memo_add);
    }
  }























}
