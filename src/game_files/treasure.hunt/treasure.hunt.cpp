/*
--------------------------------------------------------------
Filename: treasure.hunt.cpp
Purpose: this file contains tables and functions for the TH game.
--------------------------------------------------------------
*/

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include "treasure.hunt.hpp"

using namespace eosio;

void treasurehunt::loguser(name username) {
  // Ensure this action is authorized by the player
  require_auth(username);
 // usr_index usrs(get_first_receiver(), get_first_receiver().value);
  log_index login(get_first_receiver(), get_first_receiver().value);
  // Create a record in the table if the player doesn't exist in our app yet
  auto itr = login.find(username.value);
  if (itr == login.end()) {
    itr = login.emplace(username,  [&](auto& new_user) {
      new_user.username = username;
    });
  }
}

void treasurehunt::startgame(name username) {
  // Ensure this action is authorized by the player
  require_auth(username);

  auto& user = login.get(username.value, "User doesn't exist");

  login.modify(user, username, [&](auto& modified_user) {
    // Create a new game
    game game_data;
    // Assign the newly created game to the player
    modified_user.game_data = game_data;
  });
}
