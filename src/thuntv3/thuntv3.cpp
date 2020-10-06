#include "gameplay.cpp"

// After the game ended, remove the user on the current users list...
ACTION thuntv3::intgame(name username)
{
    require_auth(username);
    // Create a record in the table if the player doesn't exist in our app yet
    auto itr = _users.find(username.value);
    check(itr == _users.end(), "Error : Either User has Initialized a Game or has an Existing Game");
    // missing check balances
    if (itr == _users.end())
    {
        _users.emplace(username, [&](auto &new_users) {
            new_users.username = username;
            new_users.game_id = gen_gameid(); // generate user game_id
        });
    }
}

ACTION thuntv3::setpanel(name username, vector<uint8_t> panelset)
{
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist");
    // check if the user has existing game, else cancel start new game
    check(user.game_data.status == INITIALIZED, "Error: Has an existing game, can't start a new game.");
    check(user.game_data.panel_set.empty(), "Error: Game Panel Already Set.");

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

ACTION thuntv3::destination(name username, uint8_t destination)
{
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist");
    // check if the user has existing game, else cancel start new game
    check(user.game_data.status == INITIALIZED, "Error: Has an existing game, can't start a new game.");
    check(user.game_data.destination == 0, "Error: Game Destination Already Set.");

    _users.modify(user, username, [&](auto &modified_user) {
        modified_user.game_data.destination = destination;
        modified_user.game_data.prize = destination;
    });
}

ACTION thuntv3::setenemy(name username, uint8_t enemy_count)
{
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist");
    // check if the user has existing game, else cancel start new game
    check(user.game_data.status == INITIALIZED, "Error: Has an existing game, can't start a new game.");
    check(user.game_data.enemy_count == 0, "Error: Numbers of Enemies Already Set.");

    _users.modify(user, username, [&](auto &modified_user) {
        modified_user.game_data.enemy_count = enemy_count;
    });
}

ACTION thuntv3::gamestart(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist");
    auto itr = _users.find(username.value);
    check(itr != _users.end(), "Error: Game Doesn't Exist.");
    check(user.game_data.status == INITIALIZED, "Error: Has an existing game, can't start a new game.");
    check(user.game_data.destination != MAP_DEFAULT, "Error: Numbers of Enemies Not Set.");
    check(user.game_data.enemy_count != 0, "Error: Numbers of Enemies Not Set.");

    _users.modify(itr, username, [&](auto &modified_user) {
        modified_user.game_data.isready = true;
        modified_user.game_data.status = ONGOING;
    });
}

ACTION thuntv3::slcttile(name username, uint8_t index)
{
    require_auth(username);
    auto user = _users.find(username.value);

    // check if game is started, game status and if tile is not open
    // check(user->game_data.isready == true, "Error: Not Yet Ready To Start Game.");
    check(user->game_data.status == ONGOING, "Error: Either game has ended or not yet configured.");
    check(user->game_data.panel_set.at(index).isopen == UNOPENED, "Error: Tile already opened!");
    _users.modify(user, username, [&](auto &modified_user) {
        game game_data = modified_user.game_data;

        // generate if treasure or pirate
        uint8_t wintiles = (16 - game_data.enemy_count) * 100; // base on provided win chance calculations
        uint8_t genres = rndm(100);
        uint8_t winchance = wintiles / (PANEL_SIZE - game_data.win_count); // base on provided win chance calculations
        if (genres < winchance)
        {
            double odds = (double)game_data.unopentile / ((double)game_data.unopentile - (double)game_data.enemy_count);

            float intprize = (game_data.prize * odds) * 0.98;
            game_data.panel_set.at(index).iswin = 1;
            game_data.panel_set.at(index).isopen = 1;
            game_data.unopentile--;
            game_data.win_count++; // count number of chest found
            game_data.prize = intprize;
        }
        else
        {
            // game_data.tilelist.push(index);
            game_data.unopentile--;
            game_data.enemy_count--;
            game_data.panel_set.at(index).isopen = 1;
            game_data.prize = PRIZE_DEFAULT;

            // remaining prizes
            int random_result;
            int remaining_prizes = game_data.unopentile - game_data.enemy_count;
            while (remaining_prizes > 0)
            {
                random_result = rndm(15);
                if (game_data.panel_set.at(random_result).isopen == 0)
                {
                    game_data.panel_set.at(random_result).isopen = 1;
                    game_data.panel_set.at(random_result).iswin = 1;
                    remaining_prizes--;
                }
            }

            for (int i = 0; i < PANEL_SIZE; i++)
            {
                if (game_data.panel_set.at(i).isopen == 0 && game_data.panel_set.at(i).iswin == 0)
                {
                    game_data.panel_set.at(i).isopen = 1;
                    // if (game_data.panel_set.at(i).iswin == 0)
                }
            }
            game_data.status = 2;
            // addhistory(user);
            // game_update(username);
        }
        modified_user.game_data = game_data;
    });
}

ACTION thuntv3::withdraw(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist");

    check(user.game_data.status == ONGOING, "Error: Either game has ended or not yet configured.");
    check(user.game_data.enemy_count == user.game_data.enemy_count, "Error: You found an enemy.");
    check(user.game_data.unopentile != PANEL_SIZE, "Error: You have to open a tile first.");
    // initialized using the existing data
    // addhistory(user);
    // game_update(username);
}

ACTION thuntv3::end(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "Error: User doesn't exist");
    _users.erase(user);
    // check(user.game_data.status == ONGOING, "Game does not exist.");
    // Note: It will continue even if theres an error adding at history.
    // if (user.game_data.status == DONE || (user.game_data.status == ONGOING && user.game_data.explore_count != 0)){}
}