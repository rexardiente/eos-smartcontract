#include "gameplay.cpp"
#include <string>

ACTION treasurehunt::initialize(int id)
{
    require_auth(_self);
    auto itr = _users.find(id);
    check(itr == _users.end(), "Error : Either User has Initialized a Game or has an Existing Game");

    if (itr == _users.end())
    {
        users_table userstbl(_self, id);
        // string hash_string = checksum256_to_string_hash();
        // uint64_t gameid = userstbl.available_primary_key();

        _users.emplace(_self, [&](auto &new_users) {
            new_users.id = id;
            // new_users.game_id = hash_string;
        });
    }
}

ACTION treasurehunt::setpanel(int id, vector<uint8_t> panelset)
{
    require_auth(_self);
    // auto &user = _users.get(username.value, "User doesn't exist");
    auto itr = _users.find(id);
    // check if the user has existing game, else cancel start new game
    check(itr != _users.end(), "User doesn't exist.");
    check(itr->game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    check(itr->game_data.panel_set.empty(), "Game Panel Already Set.");

    _users.modify(itr, _self, [&](auto &modified_user) {
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

ACTION treasurehunt::destination(int id, uint8_t destination)
{
    require_auth(_self);
    auto itr = _users.find(id);
    // check if the user has existing game, else cancel start new game
    check(itr != _users.end(), "User doesn't exist.");
    check(itr->game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    check(destination %10== 0 || destination ==1, "Please input valid destination.");
    check(destination <=20, "Please input valid destination.");
    // check(user.game_data.destination == 0, "Game Destination Already Set.");

    _users.modify(itr, _self, [&](auto &modified_user) {
        modified_user.game_data.destination = destination;
    });
}

ACTION treasurehunt::setenemy(int id, uint8_t enemy_count)
{
    require_auth(_self);
    // auto &user = _users.get(username.value, "User doesn't exist");
    auto itr = _users.find(id);
    check(itr != _users.end(), "User doesn't exist.");
    check(enemy_count>0&&enemy_count<16, "Please input valid enemy count.");
    check(itr->game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    check(itr->game_data.destination > 0, "Set destination first.");
    check(itr->game_data.enemy_count < PANEL_SIZE, "Can't have enemy greater than or equal to 16");

    _users.modify(itr, _self, [&](auto &modified_user) {
        modified_user.game_data.enemy_count = enemy_count;
    });
}

// Note: Game Start will be triggered using Notification..
ACTION treasurehunt::gamestart(int id, double quantity)
{
    require_auth(_self);

    // auto &user = _users.get(username.value, "User doesn't exist");
    auto itr = _users.find(id);

    check(itr != _users.end(), "Game Doesn't Exist.");
    check(itr->game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    // check(itr->game_data.destination == (quantity / 10000), "Deposited amount does not match the selected destination.");
    // check(user.game_data.destination != MAP_DEFAULT, "Destination Not Set.");
    // check(user.game_data.enemy_count != 0, "Numbers of Enemies Not Set.");
    auto size = transaction_size();
    char buf[size];
    check(size == read_transaction(buf, size), "read_transaction failed");
    checksum256 h = sha256(buf, size);
    auto hbytes = h.extract_as_byte_array();
    string hash_string = checksum256_to_string(hbytes, hbytes.size()); // convert txID arr to string
    _users.modify(itr, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.prize = quantity;
        game_data.game_id = hash_string.substr(0, 30) + to_string(rng(100));
        gameupdate(game_data);
    });
}

ACTION treasurehunt::opentile(int id, uint8_t index)
{
    require_auth(_self);
    // auto user = _users.find(user_id);
    auto itr = _users.find(id);

    check(itr->game_data.win_count != (PANEL_SIZE - itr->game_data.enemy_count), "You already found all treasures, feel free to withdraw.");
    check(itr->game_data.status == ONGOING, "Either game has ended or not yet configured.");
    check(itr->game_data.panel_set.at(index).isopen == UNOPENED, "Tile already opened!");

    _users.modify(itr, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        tile &tile = game_data.panel_set[index];

        tile.isopen = 1;
        float available = PANEL_SIZE - game_data.enemy_count - game_data.win_count;
        float chance = available / (PANEL_SIZE - game_data.win_count) * 100;

        // out of 100, if generated result is lesser than win chance, it means win
        if (chance < 101)
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

ACTION treasurehunt::end(int id)
{
    require_auth(_self);
    // auto &user = _users.get(username.value, "User doesn't exist");
    auto itr = _users.find(id);
    // check(user.game_data.status == DONE, "End your existing game first.");
    _users.erase(itr);
}

ACTION treasurehunt::withdraw(int id)
{
    require_auth(_self);
    // auto user = _users.find(user_id);
    auto itr = _users.find(id);
    check(itr->game_data.win_count > 0, "You have not found any treasure yet.");
    check(itr->game_data.prize > 0, "You've already lost.");
    check(itr->game_data.status == ONGOING, "Game has ended and prize is already transferred.");

    _users.modify(itr, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.prize = EOS_DEFAULT;
        game_data.status = DONE;
        // std::string feedback = name{username}.to_string() + ": opened tile " + std::to_string(index) + " -> " + (game_data.panel_set.at(index).iswin == 1 ? "Win" : "Lost");
        // eosio::print(feedback + "\n");
    });
    // std::string feedback = "TH Withdraw: " + name{username}.to_string() + " recieved " + std::to_string(itr->game_data.prize);

    // action{
    //     permission_level{_self, "active"_n},
    //     eosio_token,
    //     "transfer"_n,
    //     std::make_tuple(_self, username, itr->game_data.prize, feedback)}
    //     .send();
}

// ACTION treasurehunt::settledpay(int id, asset prize, string memo)
// {
//     require_auth(_self);
//     // auto &user = _users.get(username.value, "User doesn't exist");
//     auto itr = _users.find(id);
//     check(itr->game_data.status == ONGOING, "Game has ended and prize is already transferred.");

//     _users.modify(itr, _self, [&](auto &modified_user) {
//         game &game_data = modified_user.game_data;
//         game_data.status = DONE;
//         gameupdate(game_data);
//     });
// }

ACTION treasurehunt::autoplay(int id, vector<uint8_t> to_open_panel)
{
    require_auth(_self);
    // auto user = _users.find(user_id);
    auto itr = _users.find(id);

    // check if game is started, game status and if tile is not open
    check(itr->game_data.status == ONGOING, "Either game has ended or not yet configured.");

    _users.modify(itr, _self, [&](auto &modified_user) {
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
                    game_data.nextprize = EOS_DEFAULT;
                    game_data.prize = EOS_DEFAULT;
                    game_data.odds = EOS_DEFAULT;
                    break;
                }
                game_data.unopentile--;
                game_data.odds = calculateodds(game_data);
                game_data.nextprize = generateprize(game_data);
            }
        }
    });
}

ACTION treasurehunt::del(int size)
{
    require_auth(_self);
    auto itr = _users.begin();
    while (itr != _users.end())
    {
        itr = _users.erase(itr);
    }
}