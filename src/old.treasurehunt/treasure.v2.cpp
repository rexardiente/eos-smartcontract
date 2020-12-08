#include "gameplay.cpp"

using namespace eosio;

// After the game ended, remove the user on the current users list...
void treasurev2::init(name username)
{
  require_auth(username);
  // Create a record in the table if the player doesn't exist in our app yet
  auto itr = _users.find(username.value);
  check(itr == _users.end(), "Error: Either User Has Initialized or Existing Game.");

  if (itr == _users.end())
  {
    _users.emplace(username, [&](auto &new_users) {
      new_users.username = username;
      new_users.game_id = gen_gameid(); // generate user game_id
    });
  }
}

// Configure Existing Game - User Settings

void treasurev2::gamestart(name username, vector<Tile> panel_set,uint8_t enemy_count,int64_t bet_amount)
{
  require_auth(username);
  auto &user = _users.get(username.value, "Error: User doesn't exist");
  // check if the user has existing game, else cancel start new game
  check(user.game_data.status == INITIALIZED, "Error: Has an existing game, can't start a new game.");
  check(user.game_data.panels.empty(), "Error: Game Panel Already Set.");
  _users.modify(user, username, [&](auto &modified_user) {  
    modified_user.game_data.panels =panel_set;
     modified_user.game_data.enemy_count = enemy_count;
    modified_user.total_win = 0;
    modified_user.game_data.bet_amount=bet_amount;
  });
}
/*
void treasurev2::setdest(name username, uint8_t destination)
{
  require_auth(username);
  auto &user = _users.get(username.value, "Error: User doesn't exist");
  // check if the user has existing game, else cancel start new game
  check(user.game_data.status == INITIALIZED, "Error: Has an existing game, can't start a new game.");
  check(user.game_data.destination == 0, "Error: Game Destination Already Set.");

  _users.modify(user, username, [&](auto &modified_user) {
    modified_user.game_data.destination = destination;
  });
}
*/


void treasurev2::end(name username)
{
  require_auth(username);
  auto &user = _users.get(username.value, "Error: User doesn't exist");

  // Note: It will continue even if theres an error adding at history.


  _users.erase(user);
}
