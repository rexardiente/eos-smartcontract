#include "gameplay.cpp"

using namespace eosio;

void treasurev2::hello(name username) {
    // print("Hello:", username.value);
    // int genNum = rng(10);
    // print(genNum);

    // vector<treasurev2::opened_panel> set ={
    //     { 1, UNOPENED },
    //     { 0, UNOPENED },
    //     { 2, OPENED },
    //     { 3, OPENED },
    //     { 4, OPENED },
    //     { 5, UNOPENED },
    //     { 6, UNOPENED },
    //     { 7, UNOPENED },
    //     { 8, UNOPENED },
    //     { 9, UNOPENED },
    //     { 10, UNOPENED },
    //     { 11, UNOPENED },
    //     { 12, UNOPENED },
    //     { 13, UNOPENED },
    //     { 14, UNOPENED },
    //     { 15, UNOPENED } };

    // login or create user
    // authorized(username);
    // Start game
    // startgame(username, MAP_1, EXPLORE_1, set);

    // remove current user..
    // end(username);
    // renew(username, true);
};

void treasurev2::showprize(name username, uint8_t panel_idx) {

};

void treasurev2::end(name username) {
    require_auth(username);
    auto& user = _users.get(username.value, "User doesn't exist");

    auto iterator = _users.find(username.value);
    // check(iterator != _users.end(), "User does not exist");
    // _users.erase(iterator);
    _users.modify(user, username, [&](auto& modified_user) {
        modified_user.game_data = game();
        });
};

// Initialized the game components
void treasurev2::startgame(name username, uint8_t destination, uint16_t explore_count, vector<treasurev2::opened_panel> panel_set) {
    require_auth(username);
    auto& user = _users.get(username.value, "User doesn't exist");

    _users.modify(user, username, [&](auto& modified_users)
        {
            // Initialized game_data
            game game_data;

            // update game data 
            game_data.panels = panel_set;
            game_data.destination = destination;
            game_data.explore_count = explore_count;

            // Prize Results
            // prizeresults(game_data, modified_users);
            game_data.prize_results = modified_users.game_data.prize_results;
            game_data.prize_results.insert(game_data.prize_results.begin(), {
                1, 20
                });

            modified_users.total_win = 0;
            modified_users.tickets = 100; // create another table for tickets

            modified_users.game_data = game_data;
            print("Game Initialized!!! \n");
        });
};

// Common Function "update prize results"
vector<treasurev2::results> treasurev2::prizeresults(game game_data, user modified_users) {
    // game_data.prize_results = modified_users.game_data.prize_results;
    // game_data.prize_results.insert(game_data.prize_results.begin(), {
    //     1, 20
    //     });

    return {};
};

void treasurev2::renew(name username, bool isreset) {
    require_auth(username);
    auto& user = _users.get(username.value, "User doesn't exist");

    _users.modify(user, username, [&](auto& modified_users)
        {
            // Initialized game_data
            game game_data;

            if (!isreset)
            {
                // load default values
                game game_data;
                modified_users.game_data = game_data;
            }
            else
            {
                // initialized using the current data
                // generate new panel with unopened as default value..
                game_data.panels = modified_users.game_data.panels;
                for (uint8_t i = 0; i < modified_users.game_data.panels.size(); i++) {
                    // Update only into UNOPENED value..
                    game_data.panels[i].isopen = UNOPENED;
                }

                // reuse current game user destination
                game_data.destination = modified_users.game_data.destination;
                game_data.explore_count = modified_users.game_data.explore_count;
            }

            modified_users.game_data = game_data;
            print("Panel Successfully Renewed or Reset!\n");
        });
};

// After the game ended, remove the user on the current users list...
void treasurev2::authorized(name username) {
    // require_auth(username);
    // Create a record in the table if the player doesn't exist in our app yet
    auto itr = _users.find(username.value);

    if (itr == _users.end())
    {
        _users.emplace(username, [&](auto& new_users) {
            new_users.username = username;
            });
    }
};
