/*
-------------------------------------------------------------------------------
File name: main.cpp
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

class main : public eosio::contract, public control_base {
private:

public:
/* player-related actions */
/*-------------------------------------------------------------------------*/
/// @abi action

}
