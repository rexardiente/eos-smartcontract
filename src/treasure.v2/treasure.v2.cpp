#include "gameplay.cpp"

void treasurev2::hello(name username) {
    // print("Hello:", username.value);
    // int genNum = rng(10);
    // print(genNum);

    vector<treasurev2::opened_panel> set ={
        { 1, UNOPENED },
        { 0, UNOPENED },
        { 2, UNOPENED },
        { 3, UNOPENED },
        { 4, OPENED },
        { 5, UNOPENED },
        { 6, UNOPENED },
        { 7, UNOPENED },
        { 8, UNOPENED },
        { 9, UNOPENED },
        { 10, UNOPENED },
        { 11, UNOPENED },
        { 12, UNOPENED },
        { 13, UNOPENED },
        { 14, UNOPENED },
        { 15, UNOPENED } };

    // Create a user
    // authorized(username);
    // Start game
    startgame(username, MAP_1, EXPLORE_1, set);
}

// Initialized the game components
void treasurev2::startgame(name username, uint8_t map_selected, uint16_t panels_to_explore, vector<treasurev2::opened_panel> panel_set) {
    require_auth(username);
    auto& user = _user.get(username.value, "User doesn't exist");

    _user.modify(user, username, [&](auto& modified_user)
        {
            // Initialized game_data
            game game_data;

            // update game data 
            game_data.panels = panel_set;
            game_data.map_selected = map_selected;
            game_data.panels_to_explore = panels_to_explore;
            modified_user.total_win = 0;
            modified_user.tickets = 100; // create another table for tickets
            modified_user.game_data = game_data;

            print("Game Initialized!!! \n");
        });
};

void treasurev2::reset(name username) {};

void treasurev2::authorized(name username) {
    // require_auth(username);
    // Create a record in the table if the player doesn't exist in our app yet
    auto itr = _user.find(username.value);

    if (itr == _user.end())
    {
        _user.emplace(username, [&](auto& new_user) {
            new_user.username = username;
            });
        print("User Added");
    }
    else print("User Already Exists \n");
};
void treasurev2::end(name username) {};
