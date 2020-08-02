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

void treasurehunt::draw_one_map(vector<uint8_t>& island, vector<uint8_t>& hand) {
  // Pick a random card from the deck
  int deck_map_idx = random(island.size());

  // Find the first empty slot in the hand
  int first_empty_slot = -1;
  for (int i = 0; i <= hand.size(); i++) {
    auto id = hand[i];
    if (map_dict.at(id).type == EMPTY) {
      first_empty_slot = i;
      break;
    }
  }

  check(first_empty_slot != -1, "No empty slot in the player's hand");

  // Assign the card to the first empty slot in the hand
  hand[first_empty_slot] = island[deck_map_idx];

  // Remove the card from the deck
  island.erase(island.begin() + deck_map_idx);
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

int treasurehunt::ai_choose_map(const game& game_data) {
  // The 4th strategy is only chosen in the dire situation
  int available_strategies = 4;
  if (game_data.ticket_ai > 2) available_strategies--;
  int strategy_idx = random(available_strategies);

  // Calculate the score of each card in the AI hand
  int chosen_map_idx = -1;
  int chosen_map_score = std::numeric_limits<int>::min();

  for (int i = 0; i < game_data.hand_ai.size(); i++) {
    const auto ai_map_id = game_data.hand_ai[i];
    const auto ai_map = map_dict.at(ai_map_id);

    if (ai_map.type == EMPTY) continue;
    auto map_score = calculate_ai_map_score(strategy_idx, game_data.ticket_ai, ai_map, game_data.hand_player);

    if (map_score > chosen_map_score) {
      chosen_map_score = map_score;
      chosen_map_idx = i;
    }
  }
  return chosen_map_idx;
}

int treasurehunt::ai_best_map_win_strategy(const int ai_attack_point, const int player_attack_point) {
  eosio::print("Best Island Wins");
  if (ai_attack_point > player_attack_point) return 3;
  if (ai_attack_point < player_attack_point) return -2;
  return -1;
}

int treasurehunt::calculate_ai_map_score(
  const int strategy_idx, const int8_t ticket_ai, const island& ai_map,
  const vector<uint8_t> hand_player) {
    int map_score = 0;

    for (int i = 0; i < hand_player.size(); i++) {
      const auto player_map_id = hand_player[i];
      const auto player_map = map_dict.at(player_map_id);

      int ai_attack_point = calculate_attack_point(ai_map, player_map);
      int player_attack_point = calculate_attack_point(player_map, ai_map);

      // Accumulate the card score based on the given strategy
      switch (strategy_idx) {
        case 0: {
          map_score += ai_best_map_win_strategy(ai_attack_point, player_attack_point);
          break;
        }
        case 1: {
          map_score += ai_min_loss_strategy(ai_attack_point, player_attack_point);
          break;
        }
        case 2: {
          map_score += ai_points_tally_strategy(ai_attack_point, player_attack_point);
          break;
        }
        default: {
          map_score += ai_loss_prevention_strategy(ticket_ai, ai_attack_point, player_attack_point);
          break;
        }
      }
    }
    return map_score;
}

void treasurehunt::update_game_status(user_info& user) {
  game& game_data = user.game_data;

  if (game_data.ticket_ai <= 0) game_data.status = PLAYER_WON;
  else if (game_data.ticket_player <= 0) game_data.status = PLAYER_LOST; // Check the player's HP
  else const auto is_empty_slot = [&](const auto& id) return map_dict.at(id).type == EMPTY;

  bool player_finished = std::all_of(game_data.hand_player.begin(), game_data.hand_player.end(), is_empty_slot);
  bool ai_finished = std::all_of(game_data.hand_ai.begin(), game_data.hand_ai.end(), is_empty_slot);

  if (player_finished || ai_finished) {
    if (game_data.ticket_player > game_data.ticket_ai) game_data.status = PLAYER_WON;
    else game_data.status = PLAYER_LOST;
  }
}

void treasurehunt::resolve_selected_maps(game& game_data) {
  const auto player_map = map_dict.at(game_data.selected_map_player);
  const auto ai_map = map_dict.at(game_data.selected_map_ai);

  if (player_map.type == VOID || ai_map.type == VOID) return;

  int player_attack_point = calculate_attack_point(player_map, ai_map);
  int ai_attack_point =  calculate_attack_point(ai_map, player_map);

  if (player_attack_point > ai_attack_point) {
    int diff = player_attack_point - ai_attack_point;
    game_data.ticket_lost_ai = diff;
    game_data.ticket_ai -= diff;
  } else if (ai_attack_point > player_attack_point) {
    int diff = ai_attack_point - player_attack_point;
    game_data.ticket_lost_player = diff;
    game_data.ticket_player -= diff;
  }
}

int treasurehunt::calculate_attack_point(const island& island1, const island& island2) {
  int result = island1.attack_point;

  //Add elemental compatibility bonus of 1
  if ((island1.type == MAP_A && island2.type == MAP_A) ||
      (island1.type == MAP_B && island2.type == MAP_B) ||
      (island1.type == MAP_C && island2.type == MAP_B)) {
    result++;
  }
  return result;
}

int treasurehunt::ai_min_loss_strategy(const int ai_attack_point, const int player_attack_point) {
  //eosio::print("Minimum Losses");
  if (ai_attack_point > player_attack_point) return 1;
  if (ai_attack_point < player_attack_point) return -4;
  return -1;
}

// AI Points Tally Strategy
int treasurehunt::ai_points_tally_strategy(const int ai_attack_point, const int player_attack_point) {
  //eosio::print("Points Tally");
  return ai_attack_point - player_attack_point;
}

// AI Loss Prevention Strategy
int treasurehunt::ai_loss_prevention_strategy(const int8_t ticket_ai, const int ai_attack_point, const int player_attack_point) {
  //eosio::print("Loss Prevention");
  if (ticket_ai + ai_attack_point - player_attack_point > 0) return 1;
  return 0;
}
