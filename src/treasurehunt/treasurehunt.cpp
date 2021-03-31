#include "gameplay.cpp"

ACTION treasurehunt::initialize(name username)
{
    require_auth(username);
    auto itr = _users.find(username.value);
    check(itr == _users.end(), "Error : Either User has Initialized a Game or has an Existing Game");

    if (itr == _users.end())
    {
        users_table userstbl(_self, username.value);
        string hash_string = checksum256_to_string_hash();
        // uint64_t gameid = userstbl.available_primary_key();

        _users.emplace(_self, [&](auto &new_users) {
            new_users.username = username;
            new_users.game_id = hash_string;
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
        vector<tile> new_panel_set;
        for (int i = 0; i < PANEL_SIZE; i++)
        {
            tile new_tile;
            new_tile.key = panelset[i];
            new_panel_set.insert(new_panel_set.end(), new_tile);
        }

        modified_user.game_data.panel_set = new_panel_set;
    });
}

ACTION treasurehunt::destination(name username, uint8_t destination)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check if the user has existing game, else cancel start new game
    check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    // check(user.game_data.destination == 0, "Game Destination Already Set.");

    _users.modify(user, username, [&](auto &modified_user) {
        modified_user.game_data.destination = destination;
    });
}

ACTION treasurehunt::setenemy(name username, uint8_t enemy_count)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    check(user.game_data.destination > 0, "Set destination first.");
    check(user.game_data.enemy_count < PANEL_SIZE, "Can't have enemy greater than or equal to 16");

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
        game &game_data = modified_user.game_data;
        game_data.prize = quantity;
        gameupdate(game_data);
    });
}

ACTION treasurehunt::opentile(name username, uint8_t index)
{
    require_auth(username);
    auto user = _users.find(username.value);

    check(user->game_data.win_count != (PANEL_SIZE - user->game_data.enemy_count), "You already found all treasures, feel free to withdraw.");
    check(user->game_data.status == ONGOING, "Either game has ended or not yet configured.");
    check(user->game_data.panel_set.at(index).isopen == UNOPENED, "Tile already opened!");

    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        tile &tile = game_data.panel_set[index];

        tile.isopen = 1;
        float available = PANEL_SIZE - game_data.enemy_count - game_data.win_count;
        float chance = available / (PANEL_SIZE - game_data.win_count) * 100;

        // out of 100, if generated result is lesser than win chance, it means win
        if (rng(100) < chance)
        {
            game_data.prize = generateprize(game_data);
            tile.iswin = 1;
            game_data.win_count++; // count number of chest found
            game_data.unopentile--;
        }
        else
        {
            game_data.status = DONE;
        }

        gameupdate(game_data);
        // std::string feedback = name{username}.to_string() + ": opened tile " + std::to_string(index) + " -> " + (game_data.panel_set.at(index).iswin == 1 ? "Win" : "Lost");
        // eosio::print(feedback + "\n");
    });
}

ACTION treasurehunt::end(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(user.game_data.status == DONE, "End your existing game first.");
    _users.erase(user);
}

ACTION treasurehunt::withdraw(name username)
{
    require_auth(username);
    auto user = _users.find(username.value);
    check(user->game_data.win_count > 0, "You have not found any treasure yet.");
    check(user->game_data.prize.amount > 0, "You've already lost.");
    check(user->game_data.status == ONGOING, "Game has ended and prize is already transferred.");

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
        game &game_data = modified_user.game_data;
        game_data.status = DONE;
        gameupdate(game_data);
    });
}

ACTION treasurehunt::autoplay(name username, vector<uint8_t> to_open_panel)
{
    require_auth(username);
    auto user = _users.find(username.value);

    // check if game is started, game status and if tile is not open
    check(user->game_data.status == ONGOING, "Either game has ended or not yet configured.");

    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;

        for (size_t i = 0; i < to_open_panel.size(); i++)
        {
            tile &tile = game_data.panel_set[to_open_panel[i]];

            // check if panel is available if NO do nothing..
            if (tile.isopen == IS_OPEN_PANEL)
            {
                tile.isopen = 1;
                float available = PANEL_SIZE - game_data.enemy_count - game_data.win_count;
                float chance = available / (PANEL_SIZE - game_data.win_count) * 100;
                if (rng(100) < chance)
                {
                    game_data.prize = generateprize(game_data);
                    game_data.win_count++; // count number of chest found
                    tile.iswin = 1;
                }
                else
                {
                    game_data.status = DONE;
                    game_data.nextprize = DEFAULT_ASSET;
                    game_data.prize = DEFAULT_ASSET;
                    game_data.odds = DEFAULT_ASSET.amount;
                    break;
                }
                game_data.unopentile--;
                game_data.odds = calculateodds(game_data);
                game_data.nextprize = generateprize(game_data);
            }
        }
    });
}