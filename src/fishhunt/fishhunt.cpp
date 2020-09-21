#include "gameplay.cpp"

using namespace eosio;

// After the game ended, remove the user on the current users list...
void fishhunt::initgames(name username)
{
  require_auth(username);
  // Create a record in the table if the player doesn't exist in our app yet
  auto itr = _users.find(username.value);
  check(itr == _users.end(), "Error: Either User Has Initialized or Existing Game.");

  if (itr == _users.end())
  {
    _users.emplace(username, [&](auto &new_users) {
      new_users.username = username;
    });
  }
}
void fishhunt::sethuntfish(name username, bool ready)
{

  require_auth(username);
  auto itr = _users.find(username.value);
  check(itr != _users.end(), "Error: Game Doesn't Exist.");
  check(itr->game_data.set_hunt == false, "Error: Set Hunt Status Already Triggered.");

  _users.modify(itr, username, [&](auto &modified_user) {
    modified_user.game_data.set_hunt = ready;
  });
}
void fishhunt::destlake(name username, uint8_t lakechoice)
{
  //username log
  require_auth(username);
}

void fishhunt::end(name username)
{
  require_auth(username);
  auto &user = _users.get(username.value, "Error : User does not exist.");
  auto iterator = _users.find(username.value);

  _users.erase(iterator);
  // _users.modify(user, username, [&](auto& modified_user) {
  //     modified_user.game_data = game();
  //     });
}