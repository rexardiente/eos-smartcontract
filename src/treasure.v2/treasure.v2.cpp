#include "gameplay.cpp"

using namespace eosio;

void treasurev2::hello(name username) {
    vector<treasurev2::Tile> panel_set ={
        { 1, UNOPENED },
        { 0, UNOPENED },
        { 2, UNOPENED },
        { 3, UNOPENED },
        { 4, UNOPENED },
        { 5, UNOPENED },
        { 6, UNOPENED },
        { 7, UNOPENED },
        { 8, UNOPENED },
        { 12, UNOPENED },
        { 10, UNOPENED },
        { 11, UNOPENED },
        { 9, UNOPENED },
        { 14, UNOPENED },
        { 15, UNOPENED },
        { 13, UNOPENED } };

    // authorized(username);
    // purchase(username, 200);
    // startgame(username, MAP_1, EXPLORE_1, panel_set);
    end(username);
    // genprize(username, 1);
    // renew(username, true);
}

// panel_idx is based on the index of vector panel_set  
// Example: [2,4,1,6,10]
// where tile = 4 is on index/panel_idx = 1
void treasurev2::genprize(name username, uint8_t panel_idx) {
    require_auth(username);
    auto itr = _users.find(username.value);

    check(ticket_balance(username) != 0, "No enough balance to play the game.");
    check(itr->game_data.win_count <= 4, "Game has ended, and no more prizes available.");

    _users.modify(itr, username, [&](auto& modified_user) {
        game game_data = modified_user.game_data;

        // check if tile not opened
        check(game_data.panels.at(panel_idx).isopen == UNOPENED, "Tile already opened!");

        // calculate and generate prize
        uint16_t prize = calculate_prize(game_data.tile_prizes, game_data.win_count);

        // if prize is > 0, user win.
        if (prize > 0) game_data.win_count += 1;

        game_data.tile_prizes.insert(game_data.tile_prizes.begin(), {
            panel_idx, prize
            });

        // deduct ticket and update tile to open and game status
        ticket_update(username, true, 1);
        game_data.panels.at(panel_idx).isopen = OPENED;

        //  if win limit is already reached, change the game status..
        if (game_data.win_count >= 4) game_data.status = DONE;

        modified_user.game_data = game_data;
        });
}

void treasurev2::end(name username) {
    require_auth(username);
    auto& user = _users.get(username.value, "User doesn't exist");
    auto iterator = _users.find(username.value);
    _users.erase(iterator);
    // _users.modify(user, username, [&](auto& modified_user) {
    //     modified_user.game_data = game();
    //     });
}

// Initialized the game components
void treasurev2::startgame(name username, uint8_t destination, uint16_t explore_count, vector<treasurev2::Tile> panel_set) {
    require_auth(username);

    auto& user = _users.get(username.value, "User doesn't exist");
    uint64_t user_balance = ticket_balance(username);

    // check if the user has existing game, else cancel start new game
    check(user.game_data.status != ONGOING, "Has an existing game, cant start a new game.");

    check(user_balance > explore_count && user_balance > 0, "No enough balance to play the game.");
    _users.modify(user, username, [&](auto& modified_user)
        {
            // Initialized game_data
            game game_data;

            // update game data 
            game_data.panels = panel_set;
            game_data.destination = destination;
            game_data.explore_count = explore_count;
            game_data.status = ONGOING;

            modified_user.total_win = 0;
            // modified_user.tickets = ticket_balance(username); // create another table for tickets
            modified_user.game_id = gen_gameid(); // generate user game_id

            modified_user.game_data = game_data;
        });
}

void treasurev2::renew(name username, bool isreset) {
    require_auth(username);
    auto& user = _users.get(username.value, "User doesn't exist");

    // Before renewing current user game data, add it to the history..
    addhistory(user);

    // and check if the current game is inserted into the history or else fail.
    auto history = _history.find(user.game_id);
    check(history != _history.end(), "Game Cannot be renewed. Please try again!");

    _users.modify(user, username, [&](auto& modified_user)
        {
            // Initialized game_data
            game game_data;

            if (!isreset) {
                // load default values
                modified_user.game_data = game();
            }
            else {
                // initialized using the current data
                // generate new panel with unopened as default value..
                game_data.panels = modified_user.game_data.panels;
                for (uint8_t i = 0; i < modified_user.game_data.panels.size(); i++) {
                    // Update only into UNOPENED value..
                    game_data.panels[i].isopen = UNOPENED;
                }
                // reuse current game user destination
                game_data.destination = modified_user.game_data.destination;
                game_data.explore_count = modified_user.game_data.explore_count;
                game_data.status = ONGOING;
            }

            // generate new game ID
            modified_user.game_id = gen_gameid();
            modified_user.game_data = game_data;
            // print("Panel Successfully Renewed or Reset!\n");
        });
}

// After the game ended, remove the user on the current users list...
void treasurev2::authorized(name username) {
    require_auth(username);
    // Create a record in the table if the player doesn't exist in our app yet
    auto itr = _users.find(username.value);

    if (itr == _users.end())
    {
        _users.emplace(username, [&](auto& new_users) {
            new_users.username = username;
            });
    }
}
