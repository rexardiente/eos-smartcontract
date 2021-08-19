#include "gameplay.cpp"
#include <string>

ACTION coinicagames::thinitialize(int id)
{
    require_auth(_self);
    auto itr = _thunts.find(id);
    check(itr == _thunts.end(), "Error : Either User has Initialized a Game or has an Existing Game");

    if (itr == _thunts.end())
    {
        thunts_table thuntstbl(_self, id);
        // string hash_string = checksum256_to_string_hash();
        // uint64_t gameid = userstbl.available_primary_key();

        _thunts.emplace(_self, [&](auto &new_thunts) {
            new_thunts.id = id;
            // new_thunts.game_id = hash_string;
        });
    }
}

ACTION coinicagames::thsetpanel(int id, vector<uint8_t> panelset)
{
    require_auth(_self);
    // auto &user = _thunts.get(username.value, "User doesn't exist");
    auto itr = _thunts.find(id);
    // check if the user has existing game, else cancel start new game
    check(itr != _thunts.end(), "User doesn't exist.");
    check(itr->thgame_data.status == TH_INITIALIZED, "Has an existing game, can't start a new game.");
    check(itr->thgame_data.panel_set.empty(), "Game Panel Already Set.");

    _thunts.modify(itr, _self, [&](auto &modified_thunt) {
        vector<tile> new_panel_set;
        for (int i = 0; i < TH_PANEL_SIZE; i++)
        {
            tile new_tile;
            new_tile.key = panelset[i];
            new_panel_set.insert(new_panel_set.end(), new_tile);
        }

        modified_thunt.thgame_data.panel_set = new_panel_set;
    });
}

ACTION coinicagames::thdstination(int id, uint8_t destination)
{
    require_auth(_self);
    auto itr = _thunts.find(id);
    // check if the user has existing game, else cancel start new game
    check(itr != _thunts.end(), "User doesn't exist.");
    check(itr->thgame_data.status == TH_INITIALIZED, "Has an existing game, can't start a new game.");
    check(destination %10== 0 || destination ==1, "Please input valid destination.");
    check(destination <=20, "Please input valid destination.");
    // check(user.thgame_data.destination == 0, "Game Destination Already Set.");

    _thunts.modify(itr, _self, [&](auto &modified_thunt) {
        modified_thunt.thgame_data.destination = destination;
    });
}

ACTION coinicagames::thsetenemy(int id, uint8_t enemy_count)
{
    require_auth(_self);
    // auto &user = _thunts.get(username.value, "User doesn't exist");
    auto itr = _thunts.find(id);
    check(itr != _thunts.end(), "User doesn't exist.");
    check(enemy_count>0&&enemy_count<16, "Please input valid enemy count.");
    check(itr->thgame_data.status == TH_INITIALIZED, "Has an existing game, can't start a new game.");
    check(itr->thgame_data.destination > 0, "Set destination first.");
    check(itr->thgame_data.enemy_count < TH_PANEL_SIZE, "Can't have enemy greater than or equal to 16");

    _thunts.modify(itr, _self, [&](auto &modified_thunt) {
        modified_thunt.thgame_data.enemy_count = enemy_count;
    });
}

// Note: Game Start will be triggered using Notification..
ACTION coinicagames::thgamestart(int id, double quantity)
{
    require_auth(_self);

    // auto &user = _thunts.get(username.value, "User doesn't exist");
    auto itr = _thunts.find(id);

    check(itr != _thunts.end(), "Game Doesn't Exist.");
    check(itr->thgame_data.status == TH_INITIALIZED, "Has an existing game, can't start a new game.");
    // check(itr->thgame_data.destination == (quantity / 10000), "Deposited amount does not match the selected destination.");
    // check(user.thgame_data.destination != MAP_DEFAULT, "Destination Not Set.");
    // check(user.thgame_data.enemy_count != 0, "Numbers of Enemies Not Set.");
    auto size = transaction_size();
    char buf[size];
    check(size == read_transaction(buf, size), "read_transaction failed");
    checksum256 h = sha256(buf, size);
    auto hbytes = h.extract_as_byte_array();
    string hash_string = checksum256_to_string(hbytes, hbytes.size()); // convert txID arr to string
    _thunts.modify(itr, _self, [&](auto &modified_thunt) {
        thgame &thgame_data = modified_thunt.thgame_data;
        thgame_data.prize = quantity;
        thgame_data.game_id = hash_string.substr(0, 30) + to_string(rng(100));
        gameupdate(thgame_data);
    });
}

ACTION coinicagames::thopentile(int id, uint8_t index)
{
    require_auth(_self);
    // auto user = _thunts.find(user_id);
    auto itr = _thunts.find(id);

    check(itr->thgame_data.win_count != (TH_PANEL_SIZE - itr->thgame_data.enemy_count), "You already found all treasures, feel free to withdraw.");
    check(itr->thgame_data.status == TH_ONGOING, "Either game has ended or not yet configured.");
    check(itr->thgame_data.panel_set.at(index).isopen == TH_UNOPENED, "Tile already opened!");

    _thunts.modify(itr, _self, [&](auto &modified_thunt) {
        thgame &thgame_data = modified_thunt.thgame_data;

        float available = TH_PANEL_SIZE - thgame_data.enemy_count - thgame_data.win_count;
        float chance = available / (TH_PANEL_SIZE - thgame_data.win_count) * 100;

        // out of 100, if generated result is lesser than win chance, it means win
        // if (chance < 101)
        if(chance < rng(100))
        {
            thgame_data.prize = generateprize(thgame_data);
            thgame_data.panel_set[index].iswin = 1;
            thgame_data.win_count++; // count number of chest found
            thgame_data.unopentile--;
        }
        else
        {
            thgame_data.status = DONE;
        }
        thgame_data.panel_set[index].isopen = 1;
        gameupdate(thgame_data);
    });
}

ACTION coinicagames::thend(int id)
{
    require_auth(_self);
    // auto &user = _thunts.get(username.value, "User doesn't exist");
    auto itr = _thunts.find(id);
    // check(user.thgame_data.status == DONE, "End your existing game first.");
    _thunts.erase(itr);
}

ACTION coinicagames::thwithdraw(int id)
{
    require_auth(_self);
    // auto user = _thunts.find(user_id);
    auto itr = _thunts.find(id);
    check(itr->thgame_data.win_count > 0, "You have not found any treasure yet.");
    check(itr->thgame_data.prize > 0, "You've already lost.");
    check(itr->thgame_data.status == ONGOING, "Game has ended and prize is already transferred.");

    _thunts.modify(itr, _self, [&](auto &modified_thunt) {
        thgame &thgame_data = modified_thunt.thgame_data;
        thgame_data.prize = TH_DEFAULT;
        thgame_data.status = TH_DONE;
        // std::string feedback = name{username}.to_string() + ": opened tile " + std::to_string(index) + " -> " + (thgame_data.panel_set.at(index).iswin == 1 ? "Win" : "Lost");
        // eosio::print(feedback + "\n");
    });
    // std::string feedback = "TH Withdraw: " + name{username}.to_string() + " recieved " + std::to_string(itr->thgame_data.prize);

    // action{
    //     permission_level{_self, "active"_n},
    //     eosio_token,
    //     "transfer"_n,
    //     std::make_tuple(_self, username, itr->thgame_data.prize, feedback)}
    //     .send();
}

// ACTION coinicagames::thsettledpay(int id, asset prize, string memo)
// {
//     require_auth(_self);
//     // auto &user = _thunts.get(username.value, "User doesn't exist");
//     auto itr = _thunts.find(id);
//     check(itr->thgame_data.status == ONGOING, "Game has ended and prize is already transferred.");

//     _thunts.modify(itr, _self, [&](auto &modified_thunt) {
th//         game &thgame_data = modified_thunt.thgame_data;
//         thgame_data.status = DONE;
//         gameupdate(thgame_data);
//     });
// }

ACTION coinicagames::thautoplay(int id, vector<uint8_t> to_open_panel)
{
    require_auth(_self);
    // auto user = _thunts.find(user_id);
    auto itr = _thunts.find(id);

    // check if game is started, game status and if tile is not open
    check(itr->thgame_data.status == ONGOING, "Either game has ended or not yet configured.");

    _thunts.modify(itr, _self, [&](auto &modified_thunt) {
        thgame &thgame_data = modified_thunt.thgame_data;

        for (size_t i = 0; i < to_open_panel.size(); i++)
        {
            tile &tile = thgame_data.panel_set[to_open_panel[i]];

            // check if panel is available if NO do nothing..
            if (tile.isopen == TH_IS_OPEN_PANEL)
            {
                tile.isopen = 1;
                float available = TH_PANEL_SIZE - thgame_data.enemy_count - thgame_data.win_count;
                float chance = available / (PANEL_SIZE - thgame_data.win_count) * 100;
                if (rng(100) < chance)
                {
                    thgame_data.prize = generateprize(thgame_data);
                    thgame_data.win_count++; // count number of chest found
                    tile.iswin = 1;
                }
                else
                {
                    thgame_data.status = TH_DONE;
                    thgame_data.nextprize = TH_DEFAULT;
                    thgame_data.prize = TH_DEFAULT;
                    thgame_data.odds = TH_DEFAULT;
                    break;
                }
                thgame_data.unopentile--;
                thgame_data.odds = calculateodds(thgame_data);
                thgame_data.nextprize = generateprize(thgame_data);
            }
        }
    });
}

ACTION coinicagames::thdel(int size)
{
    require_auth(_self);
    auto itr = _thunts.begin();
    while (itr != _thunts.end())
    {
        itr = _thunts.erase(itr);
    }
}