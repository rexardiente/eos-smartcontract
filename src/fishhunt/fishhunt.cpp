#include "fishhunt.hpp"

using namespace eosio;

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
void fishhunt::renew(name username)
{
    //username log
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist");
  _users.modify(user, username, [&](auto &modified_user) {
    
  });
}