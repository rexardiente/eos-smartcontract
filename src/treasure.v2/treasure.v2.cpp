#include "gameplay.cpp"

using namespace eosio;

void treasurev2::genprize(name username, uint8_t panel_idx) {
    require_auth(username);
    auto itr = _users.find(username.value);

    check(ticket_balance(username) > 0, "No enough balance to play the game.");
    _users.modify(itr, username, [&](auto& modified_user) {
        uint16_t prize = rng(10);

        game game_data = modified_user.game_data;

        // it will generate 1 to 10 random result as temp prize

        // find current data..
        // game_data.panels.begin();
        // treasurev2::contains(game_data.panels, panel_idx);

        // vector<opened_panel> panels = game_data.panels;
        // opened_panel opp ={ 1, UNOPENED };

        for (auto gdp : game_data.panels) {
            //  Check the current index and update once found..
            if (gdp.panel_idx == panel_idx) {
                // change to opened tile
                game_data.panels[panel_idx].isopen = OPENED;

                // Add to prize results
                game_data.prize_results.insert(game_data.prize_results.begin(), {
                    panel_idx, prize
                    });
            }
        }

        modified_user.game_data = game_data;
        });
};

void treasurev2::end(name username) {
    require_auth(username);
    auto& user = _users.get(username.value, "User doesn't exist");
    // auto iterator = _users.find(username.value);
    // _users.erase(iterator);
    _users.modify(user, username, [&](auto& modified_user) {
        modified_user.game_data = game();
        });
};

// Initialized the game components
void treasurev2::startgame(name username, uint8_t destination, uint16_t explore_count, vector<treasurev2::opened_panel> panel_set) {
    require_auth(username);

    auto& user = _users.get(username.value, "User doesn't exist");
    uint64_t user_balance = ticket_balance(username);

    check(user_balance > explore_count && user_balance > 0, "No enough balance to play the game.");
    _users.modify(user, username, [&](auto& modified_user)
        {
            // Initialized game_data
            game game_data;

            // update game data 
            game_data.panels = panel_set;
            game_data.destination = destination;
            game_data.explore_count = explore_count;

            modified_user.total_win = 0;
            modified_user.tickets = ticket_balance(username); // create another table for tickets
            modified_user.game_id = gen_gameid(); // generate user game_id

            modified_user.game_data = game_data;
        });
};

void treasurev2::renew(name username, bool isreset) {
    require_auth(username);
    auto& user = _users.get(username.value, "User doesn't exist");

    // Before renewing current user game data, add it to the history..
    // and check if the current game is inserted into the history or else fail.
    addhistory(user);

    auto history = _history.find(user.game_id);
    check(history != _history.end(), "Game Cannot be renewed. Please try again!");

    _users.modify(user, username, [&](auto& modified_user)
        {
            // Initialized game_data
            game game_data;

            if (!isreset) {
                // load default values
                game game_data;
                modified_user.game_data = game_data;
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
            }

            // generate new game ID
            modified_user.game_id = gen_gameid();
            modified_user.game_data = game_data;
            // print("Panel Successfully Renewed or Reset!\n");
        });
};

void treasurev2::addhistory(user user_data) {
    name username = user_data.username;
    require_auth(username);

    uint64_t game_id = user_data.game_id;
    auto itr = _history.find(game_id);

    // check if the game_id doesn't exist
    if (itr == _history.end()) {
        _history.emplace(username, [&](auto& history) {
            history.game_id = game_id;
            history.username = user_data.username;
            history.game_data = user_data.game_data;
            history.game_data.status = DONE;
            });
    }
};

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
};
