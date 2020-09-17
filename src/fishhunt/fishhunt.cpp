#include "gameplay.cpp"

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
void fishhunt::playerticket(name username, uint64_t amount)
{
  require_auth(username);
  auto itr = _tickets.find(username.value);

  if (itr == _tickets.end())
  {
    _tickets.emplace(username, [&](auto &new_ticket) {
      new_ticket.username = username;
      new_ticket.balance = amount;
    });
  }
  else
  {
    _tickets.modify(itr, username, [&](auto &modify_ticket) {
      uint64_t new_balance;

      new_balance = modify_ticket.balance + amount;
      modify_ticket.balance = new_balance;
    });
  }
}
void fishhunt::destlake(name username, uint8_t lakechoice)
{
  require_auth(username);
  auto &user = _users.get(username.value, "Error: User doesn't exist");
  // check if the user has existing game, else cancel start new game
  check(user.game_data.status == INITIALIZED, "Error: Has an existing game, can't start a new game.");
  check(user.game_data.destination == 0, "Error: Game Destination Already Set.");

  _users.modify(user, username, [&](auto &modified_user) {
    modified_user.game_data.destination = lakechoice;
  });
}
void fishhunt::setuppanel(name username, vector<fishhunt::Tile> panel_set)
{
  require_auth(username);
  auto &user = _users.get(username.value, "Error: User doesn't exist");
  // check if the user has existing game, else cancel start new game
  check(user.game_data.status == INITIALIZED, "Error: Has an existing game, can't start a new game.");
  check(user.game_data.panels.empty(), "Error: Game Panel Already Set.");

  _users.modify(user, username, [&](auto &modified_user) {
    modified_user.game_data.panels = panel_set;

    modified_user.total_win = 0; // Find another way to track users total win..
    // modified_user.tickets = ticket_balance(username); // create another table for tickets
  });
}
void fishhunt::renew(name username)
{
    //username log
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist");
    check(!user.game_data.tile_prizes.empty(), "Error: Please Open atleast 1 panel to renew the map.");

    _users.modify(user, username, [&](auto &modified_user) {
        game game_data;
        game_data.panels = modified_user.game_data.panels;
        for (uint8_t i = 0; i < modified_user.game_data.panels.size(); i++)
        {
            // Update only into UNOPENED value..
            game_data.panels[i].isopen = UNOPENED;
        }
        // reuse current game user destination
        game_data.destination = modified_user.game_data.destination;
        game_data.status = ONGOING;

        // generate new game ID
        modified_user.game_id = gen_gameid();
        modified_user.game_data = game_data;
    });
}