/*
--------------------------------------------------------------
Filename: treasurehunt.cpp
Purpose: This is the source file for the TH game contract.
--------------------------------------------------------------
*/

#include "treasurehunt.hpp"
using namespace eosio;

/*
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
*/

/* --------------------------------------------------------------------
Function name: Start game
Parameters: name username, vector<uint8_t> panel_set
Purpose: Starts the game for a specific user and panel set.
-------------------------------------------------------------------- */
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

/* --------------------------------------------------------------------
Function name: End Game
Parameters: name username
Purpose: Ends the game for a specific user.
-------------------------------------------------------------------- */
void treasurehunt::endgame(name username) {
  // Ensure this action is authorized by the player
  require_auth(username);
  // Get the user and reset the game
  auto& user = _users.get(username.value, "User doesn't exist");
  _users.modify(user, username, [&](auto& modified_user) {
    modified_user.game_data = game();
  });
}

/* --------------------------------------------------------------------
Function name: Has existing game
Parameters: user username
Purpose: Checks to see if a given user has an existing game.
-------------------------------------------------------------------- */
bool treasurehunt::has_existing_game(user username) {
  // Ensure this action is authorized by the player
  require_auth(username);

  auto& user = _users.get(username.value, "User doesn't exist");
  game& game_data = user.game_data;

  if (user.game_data.status == ONGOING) return true;
  return false;
}

/* --------------------------------------------------------------------
Function name: Reset game
Parameters: name username, vector<uint8_t> panel_set
Purpose: Resets the game to the default game settings.
-------------------------------------------------------------------- */
void treasurehunt::reset_game(name username, vector<uint8_t> panel_set) {
  // Ensure this action is authorized by the player
  require_auth(username);

  // Get the user and modify game data to reset the game.
  auto& user = _users.get(username.value, "User doesn't exist");

  _users.modify(user, username, [&](auto& modified_user) {
    modified_user.game_data = game();

    // revert all game details to default values.
    modified_user.ticket_player = 1;
    modified_user.ticket_ai = 1;
    modified_user.map_player = {1,2,3,4,7,8,9,10,11,12,13,14,15,16};
    modified_user.map_ai = {1,2,3,4,7,8,9,10,11,12,13,14,15,16};
    modified_user.hand_player = {0,0,0,0};
    modified_user.hand_ai = {0,0,0,0};
    modified_user.selected_map_player = 0;
    modified_user.selected_map_ai = 0;
    modified_user.ticket_lost_player = 0;
    modified_user.ticket_lost_ai = 0;
    modified_user.status = ONGOING;

    // add game_data into user table..
    modified_user.game_data = game_data;
  });
}

/* --------------------------------------------------------------------
Function name: New destination
Parameters: name username
Purpose: Generates a new destination for a given user.
-------------------------------------------------------------------- */
void treasurehunt::new_destination(name username) {
  
}

/* --------------------------------------------------------------------
Function name: Game history
Parameters: name username
Purpose: Returns the game history for a given user.
-------------------------------------------------------------------- */
void treasurehunt::game_history(name username) {
  // Ensure this action is authorized by the player
  require_auth(username);

  auto& user = _users.get(username.value, "User doesn't exist");
  game& game_data = user.game_data;

  // Now update all other details of the game
  _users.modify(user, username, [&](auto& modified_user) {
    modified_user.game_data = game();
    // update game_data details..
    modified_user.game_data.status = ONGOING;
    modified_user.game_data.ticket_ai = ???;
    modified_user.game_data.map_player = ???;
    // add game_data into user table..
    modified_user.game_data = game_data;
  });

  // won prizes, available prizes, available tickets and etc..

}

/* --------------------------------------------------------------------
Function name: Game status
Parameters: name username, user user_data
Purpose: Game status will update prizes that already won and some other.
-------------------------------------------------------------------- */
void treasurehunt::gamestatus(name username, user user_data) {
  // Ensure this action is authorized by the player
  require_auth(username);

  auto& user = _users.get(username.value, "User doesn't exist");
  game& game_data = user.game_data;

  // Now update all other details of the game
  _users.modify(user, username, [&](auto& modified_user) {
    modified_user.game_data = game();
    // update game_data details..
    modified_user.game_data.status = ONGOING;
    modified_user.game_data.ticket_ai = ???;
    modified_user.game_data.map_player = ???;
    // add game_data into user table..
    modified_user.game_data = game_data;
  });

  // won prizes, available prizes, available tickets and etc..

}

/* --------------------------------------------------------------------
Function name: Set sail
Parameters: name username
Purpose: Modifies the setsail variable to reflect the user ready to begin flagging locations.
-------------------------------------------------------------------- */
void treasurehunt::setsail(name username) {
  require_auth(username);
  auto& user = _users.get(username.value, "User doesn't exist");
  _users.modify(user, username, [&](auto& user) {
    // option oother codes here...
    user.set_sail = READY;
  });
}

/* --------------------------------------------------------------------
Function name: New Explorers
Parameters: name username, uint16_t number_of_explorers
Purpose: Generates new explorers.
-------------------------------------------------------------------- */
void treasurehunt::new_explorers(name username, uint16_t number_of_explorers) {

}

/* --------------------------------------------------------------------
Function name: Next round
Parameters: name username
Purpose:
-------------------------------------------------------------------- */
void treasurehunt::nextround(name username) {

}

/* --------------------------------------------------------------------
Function name: Play hunt
Parameters: name username, uint8_t player_map_idx
Purpose:
-------------------------------------------------------------------- */
void treasurehunt::playhunt(name username, uint8_t player_map_idx) {

}

/* --------------------------------------------------------------------
Function name: Calculate prize
Parameters: user user_data, uint64_t selected_panel
Purpose: Calculates the user's prize based upon the RNG function & occurance rate.
-------------------------------------------------------------------- */
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

/* --------------------------------------------------------------------
Function name: Generate panel
Parameters: name username, uint64_t selected_panel
Purpose: Generates the user's prize.
-------------------------------------------------------------------- */
void treasurehunt::generateprize(name username, uint64_t selected_panel) {
   // Ensure this action is authorized by the player
  require_auth(username);
  // Initialize user_info table (which has the game data and user information).
  auto& user = _users.get(username.value, "User doesn't exist");
  // verify game status if player is suitable to take the action.
  // If all the validations success, then modify users_info..
  _users.modify(user, username, [&](auto& user) {
    user.set_sail = ONHOLD;
    calculateprize(user, selected_panel);
  }
}

/* --------------------------------------------------------------------
Function name: Random
Parameters: int range
Purpose: To generate a random number for a user-specified range of integers.
-------------------------------------------------------------------- */
//RNG Library
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
