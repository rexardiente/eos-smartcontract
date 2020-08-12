/*
--------------------------------------------------------------
Filename: treasurehunt.cpp
Purpose: This is the source file containing functions for the TH game contract.
--------------------------------------------------------------
*/

#include "treasure.hunt.hpp"
using namespace eosio;

/* --------------------------------------------------------------------
Function name: Start game
Parameters: name username, vector<uint8_t> panel_set
Purpose: Starts the game for a specific user and panel set.
-------------------------------------------------------------------- */
void treasurehunt::startgame(name username,uint8_t  selected_map_player) {
  // Ensure this action is authorized by the player
  require_auth(username);
  auto& user = _users.get(username.value, "User doesn't exist");
  // check if user has already login or exist in the game
  if (user.game_data.status != ONGOING) {
    _users.modify(user, username, [&](auto& user) {
      // Initialize game table
      game game_data;
      // update game_data details..
      generatePrize(username, 3);
      user.game_data.hand_player = {0, 0, 0,0,0};
      user.game_data.status = ONGOING;
      user.game_data.ticket_lost_player = 0;
      user.game_data.ticket_player = 1;
      user.game_data.selected_map_player = selected_map_player;
      user.game_data.map_player[1] = 1;
      // add game_data into user table..
      user.game_data = game_data;
    });
  } else {
    endgame(username);
  }
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
  /*
  // Ensure this action is authorized by the player
  require_auth(username);

  auto& user = _users.get(username.value, "User doesn't exist");
  game& game_data = user.game_data;

  if (user.game_data.status == ONGOING) return true;
  return false;
  */
}

/* --------------------------------------------------------------------
Function name: Reset game
Parameters: name username, vector<uint8_t> panel_set
Purpose: Resets the game to the default game settings.
-------------------------------------------------------------------- */
void treasurehunt::reset_game(name username) {
  // Ensure this action is authorized by the player
  require_auth(username);

  // Get the user and modify game data to reset the game.
  auto& user = _users.get(username.value, "User doesn't exist");

  _users.modify(user, username, [&](auto& modified_user) {
    // Initialize game table
    game game_data;

    // revert all game details to default values.
    user.game_data.ticket_player = 1;
    user.game_data.ticket_ai = 1;
    user.game_data.map_player = {1,2,3,4,7,8,9,10,11,12,13,14,15,16};
    user.game_data.map_ai = {1,2,3,4,7,8,9,10,11,12,13,14,15,16};
    user.game_data.hand_player = {0,0,0,0};
    user.game_data.hand_ai = {0,0,0,0};
    user.game_data.selected_map_player = 0;
    user.game_data.selected_map_ai = 0;
    user.game_data.ticket_lost_player = 0;
    user.game_data.ticket_lost_ai = 0;
    user.game_data.status = ONGOING;

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
  require_auth(username);
  auto& user = _users.get(username.value, "User doesn't exist");
  game game_data = user.game_data;

  // Now update all other details of the game
  _users.modify(user, username, [&](auto& modified_user) {
    modified_user.game_data = game();
    modified_user.game_data.status = ONGOING;
    modified_user.game_data = game_data;
  });
}

/* --------------------------------------------------------------------
Function name: Game status
Parameters: name username, user user_data
Purpose: Game status will update prizes that already won and some other.
-------------------------------------------------------------------- */
void treasurehunt::gamestatus(name username) {
  // Ensure this action is authorized by the player
  require_auth(username);
  auto& user = _users.get(username.value, "User doesn't exist");
  game game_data = user.game_data;
  // Now update all other details of the game
  _users.modify(user, username, [&](auto& modified_user) {
    modified_user.game_data = game();
    // update game_data details..
    modified_user.game_data.status = ONGOING;
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
    // Initialize game table
    game game_data;
    // option other codes here...
    user.game_data.setsail = READY;
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
void treasurehunt::calculatePrize(name username,uint64_t results) {
  double finalprize = 0.00;

  //ramdom tier_results
  int tier_results = random(results);

  //results is random prize results from generate Prize
  if (results <= 350) {
     //gameid=7;
     if (tier_results <= 70) finalprize = tier_results * 01.00;
     else finalprize = tier_results * 0.20;

  } else if (results <= 700) {
    if (tier_results <= 70) finalprize = tier_results * 01.00;
    else finalprize = tier_results * 0.20;

  } else if (results <= 850) {
    if (tier_results <= 30) finalprize = tier_results * 1.00;
    else if (tier_results <= 50) finalprize = tier_results * 0.75;
    else if (tier_results < 80) finalprize = tier_results * 0.20;
    else finalprize = tier_results * 0.15;

  } else if (results <= 950) {
    if (tier_results <= 30) finalprize = tier_results * 1.00;
    else if (tier_results <= 50) finalprize=tier_results * 0.65;
    else if (tier_results < 80) finalprize=tier_results * 0.15;
    else finalprize = tier_results * 0.12;

  } else if (results <= 990) {
    if (tier_results <= 30) finalprize = tier_results * 1.00;
    else if (tier_results <= 50) finalprize = tier_results * 0.60;
    else if (tier_results < 80) finalprize = tier_results * 0.12;
    else finalprize = tier_results * 0.10;

  } else if (results <= 999) {
    if (tier_results <= 30) finalprize = tier_results * 1.00;
    else if (tier_results <= 50) finalprize = tier_results * 0.50;
    else if (tier_results < 80) finalprize = tier_results * 0.30;
    else finalprize = tier_results * 0.05;

  } else {
    if (tier_results <= 30) finalprize = tier_results * 1.00;
    else if (tier_results <= 50) finalprize = tier_results * 0.40;
    else if (tier_results < 80) finalprize = tier_results * 0.25;
    else finalprize = tier_results * 0.01;
  }

  game game_data; // add new the final_prize..
 // gameStatus(username);
}

/* --------------------------------------------------------------------
Function name: Generate panel
Parameters: name username, uint64_t selected_panel
Purpose: Generates the user's prize.
-------------------------------------------------------------------- */
void treasurehunt::generatePrize(name username, uint8_t selected_map_player) {
   // Ensure this action is authorized by the player
  require_auth(username);
  // Initialize user_info table (which has the game data and user information).
  auto& user = _users.get(username.value, "User doesn't exist");
  // verify game status if player is suitable to take the action.
  // If all the validations success, then modify users_info..
    int results = random(selected_map_player);
  _users.modify(user, username, [&](auto& user) {
    //user.set_sail = ONHOLD;
    calculatePrize(username,results);
  });
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
