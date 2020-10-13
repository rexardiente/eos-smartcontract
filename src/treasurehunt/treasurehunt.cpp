#include "gameplay.cpp"

ACTION treasurehunt::initialize(name username)
{
    require_auth(username);
    // Create a record in the table if the player doesn't exist in our app yet
    auto itr = _users.find(username.value);

    users_table userstbl(_self, username.value);
    uint64_t gameid = userstbl.available_primary_key();

    check(itr == _users.end(), "Error : Either User has Initialized a Game or has an Existing Game");

    if (itr == _users.end())
    {
        _users.emplace(_self, [&](auto &new_users) {
            new_users.username = username;
            new_users.game_id = gameid;
            // new_users.game_id = generategameid(); // generate user game_id
        });
    }
}

ACTION treasurehunt::setpanel(name username, vector<uint8_t> panelset)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check if the user has existing game, else cancel start new game
    check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    check(user.game_data.panel_set.empty(), "Game Panel Already Set.");

    _users.modify(user, username, [&](auto &modified_user) {
        game game_data = modified_user.game_data;
        vector<tile> new_panel_set;

        for (int i = 0; i < PANEL_SIZE; i++)
        {
            tile new_tile;
            new_tile.key = panelset[i];
            new_panel_set.insert(new_panel_set.end(), new_tile);
        }
        game_data.panel_set = new_panel_set;
        modified_user.game_data = game_data;
        // modified_user.tickets = ticket_balance(username); // create another table for tickets
    });
}

ACTION treasurehunt::destination(name username, uint8_t destination)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check if the user has existing game, else cancel start new game
    check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    check(user.game_data.destination == 0, "Game Destination Already Set.");

    _users.modify(user, username, [&](auto &modified_user) {
        modified_user.game_data.destination = destination;
    });
}

ACTION treasurehunt::setenemy(name username, uint8_t enemy_count)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check if the user has existing game, else cancel start new game
    check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    check(user.game_data.enemy_count == 0, "Numbers of Enemies Already Set.");

    _users.modify(user, username, [&](auto &modified_user) {
        modified_user.game_data.enemy_count = enemy_count;
    });
}

// Note: Game Start will be triggered using Notification..
ACTION treasurehunt::gamestart(name username, asset quantity)
{
    require_auth(_self);

    auto &user = _users.get(username.value, "User doesn't exist");
    auto itr = _users.find(username.value);

    check(itr != _users.end(), "Game Doesn't Exist.");
    check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    check(itr->game_data.destination == (quantity.amount / 10000), "Deposited amount does not match the selected destination.");
    // check(user.game_data.destination != MAP_DEFAULT, "Destination Not Set.");
    // check(user.game_data.enemy_count != 0, "Numbers of Enemies Not Set.");

    _users.modify(itr, username, [&](auto &modified_user) {
        modified_user.game_data.prize = quantity;
    });

    gameupdate(username);
}

ACTION treasurehunt::opentile(name username, uint8_t index)
{
    require_auth(username);
    auto user = _users.find(username.value);
    // check if game is started, game status and if tile is not open
    check(user->game_data.win_count < (16 - user->game_data.enemy_count), "You already found all treasures.");
    check(user->game_data.status == ONGOING, "Either game has ended or not yet configured.");
    check(user->game_data.panel_set.at(index).isopen == UNOPENED, "Tile already opened!");

    _users.modify(user, username, [&](auto &modified_user) {
        game game_data = modified_user.game_data;
        // generate if treasure or pirate

        game_data.panel_set.at(index).isopen = 1;
        uint8_t genres = rng(100);
        float wintiles = 16 - game_data.enemy_count - game_data.win_count; // base on provided win chance calculations
        float winchance = wintiles / (16 - game_data.win_count);           // base on provided win chance calculations
        if (genres < 100)                                                  // out of 100, if generated result is lesser than win chance, it means win
        {
            game_data.panel_set.at(index).iswin = 1;
            game_data.win_count++; // count number of chest found
            game_data.prize = generateprize(game_data);
            game_data.unopentile--;
        }
        else
        {
            game_data.status = DONE;
            game_data.unopentile--;
            game_data = showremainingtile(game_data);
            game_data.prize.amount = 0;
        }

        modified_user.game_data = game_data;

        std::string feedback = name{username}.to_string() + ": opened tile " + std::to_string(index) + " -> " + (game_data.panel_set.at(index).iswin == 1 ? "Win" : "Lost");
        eosio::print(feedback + "\n");
    });

    gameupdate(username);
}

ACTION treasurehunt::end(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == DONE, "End your existing game first.");
    _users.erase(user);
}

ACTION treasurehunt::withdraw(name username)
{
    require_auth(username);
    auto user = _users.find(username.value);
    check(user->game_data.status == ONGOING, "Game has ended and prize is already transferred.");
    check(user->game_data.win_count > 0, "You have not found any treasure yet.");

    std::string feedback = "TH Withdraw: " + name{username}.to_string() + " recieved " + std::to_string(user->game_data.prize.amount);

    action{
        permission_level{_self, "active"_n},
        eosio_token,
        "transfer"_n,
        std::make_tuple(_self, username, user->game_data.prize, feedback)}
        .send();
}

ACTION treasurehunt::settledpay(name username, asset prize, string memo)
{
    require_auth(_self);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == ONGOING, "Game has ended and prize is already transferred.");

    _users.modify(user, username, [&](auto &modified_user) {
        game game_data = modified_user.game_data;
        modified_user.game_data = showremainingtile(game_data);
        modified_user.game_data.status = DONE;
    });
}