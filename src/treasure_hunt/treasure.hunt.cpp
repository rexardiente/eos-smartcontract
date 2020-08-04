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
  // usr_index usrs(get_first_receiver(), get_first_receiver().value);
  // log_index login(get_first_receiver(), get_first_receiver().value);
  // Create a record in the table if the player doesn't exist in our app yet
  auto itr = _users.find(username.value);
  if (itr == _users.end()) {
    itr = _users.emplace(username,  [&](auto& new_user) {
      new_user.username = username;
    });
  }
}

void treasurehunt::startgame(name username) {
  // Ensure this action is authorized by the player
  require_auth(username);

  auto& user = _users.get(username.value, "User doesn't exist");

  _users.modify(user, username, [&](auto& modified_user) {
    // Create a new game
    game game_data;
    for (uint8_t i = 0; i < 4; i++) {
      draw_one_map(game_data.map_player, game_data.hand_player);
      draw_one_map(game_data.map_ai, game_data.hand_ai);
    }
    // Assign the newly created game to the player
    modified_user.game_data = game_data;
  });
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

void treasurehunt::nextround(name username) {
  // Ensure this action is authorized by the player
  require_auth(username);

  auto& user = _users.get(username.value, "User doesn't exist");

  // Verify game status
  check(user.game_data.status == ONGOING,
              "nextround: This game has ended. Please start a new one.");
  check(user.game_data.selected_map_player != 0 && user.game_data.selected_map_ai != 0,
               "nextround: Please play a card first.");

  _users.modify(user, username, [&](auto& modified_user) {
    game& game_data = modified_user.game_data;

    // Reset selected card and damage dealt
    game_data.selected_map_player = 0;
    game_data.selected_map_ai = 0;
    game_data.ticket_lost_player = 0;
    game_data.ticket_lost_ai = 0;

    // Draw card for the player and the AI
    if (game_data.map_player.size() > 0) draw_one_map(game_data.map_player, game_data.hand_player);
    if (game_data.map_ai.size() > 0) draw_one_map(game_data.map_ai, game_data.hand_ai);
  });
}

void treasurehunt:: generate_panel() {

}

void treasurehunt::playhunt(name username, uint8_t player_map_idx) {
  // Ensure this action is authorized by the player
  require_auth(username);

  // Checks that selected card is valid
  check(player_map_idx < 4, "playcard: Invalid hand index");

  auto& user = _users.get(username.value, "User doesn't exist");

  // Verify game status is suitable for the player to play a card
  check(user.game_data.status == ONGOING,
               "playmap: This game has ended. Please start a new one");
  check(user.game_data.selected_map_player == 0,
               "playmap: The player has played his Island this turn!");

  _users.modify(user, username, [&](auto& modified_user) {
    game& game_data = modified_user.game_data;

    // Assign the selected card from the player's hand
    game_data.selected_map_player = game_data.hand_player[player_map_idx];
    game_data.hand_player[player_map_idx] = 0;

    // AI picks a card
    int ai_map_idx = ai_choose_map(game_data);
    game_data.selected_map_ai = game_data.hand_ai[ai_map_idx];
    game_data.hand_ai[ai_map_idx] = 0;

    resolve_selected_maps(game_data);

    update_game_status(modified_user);
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
