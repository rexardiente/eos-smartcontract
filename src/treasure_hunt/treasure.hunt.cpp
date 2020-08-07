/*
--------------------------------------------------------------
Filename: treasurehunt.cpp
Purpose: This is the source file for the TH game contract.
--------------------------------------------------------------
*/

#include "treasurehunt.hpp"
using namespace eosio;

void treasurehunt::login(name username) {
  // Ensure this action is authorized by the player
  require_auth(username);
  // Create a record in the table if the player doesn't exist in our app yet
  auto itr = _users.find(username.value);
  if (itr == _users.end()) {
    itr = _users.emplace(username,  [&](auto& new_user) {
      new_user.username = username;
    });
  }
}

void treasurehunt::startgame(name username, vector<uint8_t> panel_set) {
  // Ensure this action is authorized by the player
  require_auth(username);
  auto& user = _users.get(username.value, "User doesn't exist");
  // check if user has already login or exist in the game
  if (user.game_data.status != ONGOING) {
    _users.modify(user, username, [&](auto& user) {
      // Initialize game table
      game game;
      // update game_data details..
      game.game_data.ticket_player = ???;
      game.game_data.ticket_ai = ???;
      game.game_data.map_player = ???;
      // add game_data into user table..
      user.game_data = game_data;
    });
  } else endgame(username);
}

void treasurehunt::setsail(name username) {
  require_auth(username);
  auto& user = _users.get(username.value, "User doesn't exist");
  _users.modify(user, username, [&](auto& user) {
    // option oother codes here...
    user.set_sail = READY;
  });
}

void treasurehunt::reset(name username, vector<uint8_t> panel_set) {
  // Ensure this action is authorized by the player
  require_auth(username);
  // Get the user and reset the game
  auto& user = _users.get(username.value, "User doesn't exist");
  _users.modify(user, username, [&](auto& modified_user) {
    modified_user.game_data = game();
    // update game_data details..
    modified_user.game_data.ticket_player = ???;
    modified_user.game_data.ticket_ai = ???;
    modified_user.game_data.map_player = ???;
    // add game_data into user table..
    modified_user.game_data = game_data;
  });
}

void treasurehunt::generateprize(name username, uint64_t selected_panel) {
   // Ensure this action is authorized by the player
  require_auth(username);
  // Initialize user_info table (which has the game data and user information).
  auto& user = _users.get(username.value, "User doesn't exist");
  // verify game status if player is suitable to take the action.
  // If all the validations success, then modify users_info..
  _users.modify(user, username, [&](auto& user) {
    user.set_sail = ONHOLD;
    calculatePrize(user, selected_panel);
  }
}

// Generate Prize based on RNG and occurence rate..
void treasurehunt::calculateprize(user user_data, uint64_t selected_panel) {
  // How can we generate the fairness of the game?
  // So, we need two things which is `Prize set and Tier Set` table for tracking..
  auto& prize_tracker = _prize.get(user_data.username, "User has no existing game.");
  auto& tier_tracker = _tier.get(user_data.username, "User has no existing game.");
  // to calculate prize tracker use the sample Table of calculation on Telegram..
  // use the same idea for calculating Tier set..
  // update the current_data object based on prize and tier set calculations
  //result based on prize_tracker and tier_tracker//
  uint64_t final_prize = ???;
  user_data.game_data; // add new the final_prize..
  gameStatus(user_data);
}

// Game status will update prizes that already won and some other
void treasurehunt::gamestatus(user user_data) {
  game& game_data = user.game_data;
  // Now update all other details of the game
  // won prizes, available prizes, available tickets and etc..
}

void treasurehunt::endgame(name username) {
  // Ensure this action is authorized by the player
  require_auth(username);
  // Get the user and reset the game
  auto& user = _users.get(username.value, "User doesn't exist");
  _users.modify(user, username, [&](auto& modified_user) {
    modified_user.game_data = game();
  });
}

//RNG Library..
int treasurehunt::random(const int range) {
  // Find the existing seed
  auto seed_iterator = _seed.begin();

  // Initialize the seed with default value if it is not found
  if (seed_iterator == _seed.end()) {
    seed_iterator = _seed.emplace( _self, [&]( auto& seed ) { });
  }

  // Generate new seed value using the existing seed value
  int prime = 65537;
  auto new_seed_value = (seed_iterator->value + current_time_point().elapsed.count()) % prime;

  // Store the updated seed value in the table
  _seed.modify( seed_iterator, _self, [&]( auto& s ) {
    s.value = new_seed_value;
  });

  // Get the random result in desired range
  int random_result = new_seed_value % range;
  return random_result;
}
