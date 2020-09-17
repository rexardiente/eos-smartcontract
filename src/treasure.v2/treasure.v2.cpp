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
void treasurev2::setpanel(name username, vector<Tile> panel_set)
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

void treasurev2::setexplr(name username, uint16_t explore_count)
{
  require_auth(username);
  auto &user = _users.get(username.value, "Error: User doesn't exist");
  // check if the user has existing game, else cancel start new game
  check(user.game_data.status == INITIALIZED, "Error: Has an existing game, can't start a new game.");
  check(user.game_data.explore_count == 0, "Error: Game Explore Count Already Set.");

  _users.modify(user, username, [&](auto &modified_user) {
    game game_data = modified_user.game_data;

    uint64_t user_balance = ticket_balance(username);
    check(user_balance >= (explore_count * game_data.destination) && user_balance > 0, "Error: No enough balance to play the game.");

    game_data.explore_count = explore_count;

    modified_user.game_data = game_data;
    game_update(username);
  });
}

void treasurev2::renew(name username)
{
  require_auth(username);
  auto &user = _users.get(username.value, "Error: User doesn't exist");
  // Check if user has no balancecheck(ticket_balance(username) != 0, "Error: 0 balance, Can't renew the map.");
  check(!user.game_data.tile_prizes.empty(), "Error: Please Open atleast 1 panel to renew the map.");

  _users.modify(user, username, [&](auto &modified_user) {
    // Before renewing current user game data, add it to the history..
    // and check if the current game is inserted into the history or else fail.
    addhistory(modified_user);
    auto itr = _users.find(username.value);
    auto history = _history.find(itr->game_id);
    check(history != _history.end(), "Error: Adding to History. Please try again!");

    // initialized using the current data
    // generate new panel with unopened as default value..
    game game_data;
    game_data.panels = modified_user.game_data.panels;
    for (uint8_t i = 0; i < modified_user.game_data.panels.size(); i++)
    {
      // Update only into UNOPENED value..
      game_data.panels[i].isopen = UNOPENED;
    }
    // reuse current game user destination
    game_data.destination = modified_user.game_data.destination;
    game_data.explore_count = modified_user.game_data.explore_count;
    game_data.status = ONGOING;

    // generate new game ID
    modified_user.game_id = gen_gameid();
    modified_user.game_data = game_data;
  });
}

void treasurev2::setsail(name username, bool ready)
{
  require_auth(username);
  auto itr = _users.find(username.value);
  check(itr != _users.end(), "Error: Game Doesn't Exist.");
  check(itr->game_data.set_sail == false, "Error: Setsail Status Already Triggered.");

  _users.modify(itr, username, [&](auto &modified_user) {
    modified_user.game_data.set_sail = ready;
  });
}

// panel_idx is based on the index of vector panel_set
// Example: [2,4,1,6,10]
// where tile = 4 is on index/panel_idx = 1
void treasurev2::genprize(name username, uint8_t panel_idx)
{
  require_auth(username);
  auto user = _users.find(username.value);

  check(ticket_balance(username) != 0, "Error: No enough balance to play the game.");
  check(user->game_data.set_sail == true, "Error: Not Yet Ready To Set Sail.");
  check(user->game_data.status == ONGOING, "Error: Either game has ended or not yet configured.");
  check(user->game_data.win_count < 4, "Error: No more prizes available, Game already ended.");
  check(user->game_data.explore_count > 0, "Error: You've Reached Game Explorer Count.");

  _users.modify(user, username, [&](auto &modified_user) {
    game game_data = modified_user.game_data;

    // check if tile not opened
    check(game_data.panels.at(panel_idx).isopen == UNOPENED, "Error: Tile already opened!");
    // deduct current explorer count
    game_data.explore_count -= 1;

    // calculate and generate prize
    uint16_t prize = calculate_prize(game_data.tile_prizes, game_data.win_count, game_data.destination);

    // if prize is > 0, user win.
    if (prize > 0)
      game_data.win_count += 1;

    game_data.tile_prizes.insert(game_data.tile_prizes.begin(), {panel_idx, prize});

    // deduct ticket and update tile to open and game status
    ticket_update(username, true, game_data.destination);
    game_data.panels.at(panel_idx).isopen = OPENED;

    modified_user.game_data = game_data;
  });

  // Update Overall Game Status..
  game_update(username);
}

void treasurev2::end(name username)
{
  require_auth(username);
  auto &user = _users.get(username.value, "Error: User doesn't exist");

  // Note: It will continue even if theres an error adding at history.
  if (user.game_data.status == DONE || (user.game_data.status == ONGOING && user.game_data.tile_prizes.size() != 0))
  {
    addhistory(user);
  }

  _users.erase(user);
}

// Purchase new ticket
void treasurev2::purchase(name username, uint64_t amount)
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
