#include "gameplay.cpp"

// function for accepting bet
ACTION mahjonghilo::acceptbet(name username, asset quantity)
{
    require_auth(_self);
    auto itr = _users.find(username.value);
    // check(itr == _users.end(), "Either User has Initialized a Game or has an Existing Game");
    auto user = _users.find(username.value);
    _users.modify(user, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.hi_lo_balance.amount += quantity.amount;
    });
}

ACTION mahjonghilo::initialize(name username)
{
    require_auth(_self);
    // Ensure this action is authorized by the player
    if (_users.find(username.value) == _users.end())
    {
        _users.emplace(_self, [&](auto &new_user) { new_user.username = username; });
    }

    auto itr = _users.find(username.value);
    check(itr != _users.end(), "User or Game Doesn't Exist.");
    uint64_t id = 0;
    auto size = transaction_size();
    char buf[size];
    check(size == read_transaction(buf, size), "read_transaction failed");
    checksum256 h = sha256(buf, size);
    auto hbytes = h.extract_as_byte_array();
    string hash_string = checksum256_to_string(hbytes, hbytes.size()); // convert txID arr to string
    check(itr->game_data.status != 1, "Continue or finish your current game..");
    _users.modify(itr, _self, [&](auto &modified_user) {
        modified_user.game_count += 1;
        game &game_data = modified_user.game_data;
        const int game_count = modified_user.game_count;
        game_data.game_id = hash_string.substr(0, 30) + to_string(rng(100));
        game_data.status = ONGOING;
        int num1 = game_count % 16;
        int num2 = game_count % 4;
        if (num1 > 0 && num1 < 5)
        {
            game_data.prevalent_wind = EAST;
        }
        else if (num1 > 4 && num1 < 9)
        {
            game_data.prevalent_wind = SOUTH;
        }
        else if (num1 > 8 && num1 < 13)
        {
            game_data.prevalent_wind = WEST;
        }
        else
        {
            game_data.prevalent_wind = NORTH;
        }
        if (num2 == 1)
        {
            game_data.seat_wind = EAST;
        }
        else if (num2 == 2)
        {
            game_data.seat_wind = SOUTH;
        }
        else if (num2 == 3)
        {
            game_data.seat_wind = WEST;
        }
        else
        {
            game_data.seat_wind = NORTH;
        }

        // game_data.standard_tile = game_data.current_tile;
        const auto hilo_tile = table_deck.at(gettile(game_data));
        get_odds(game_data, hilo_tile.value);
    });
}

ACTION mahjonghilo::starttrial(name username, int numgames, vector<int> idx)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(user.game_data.status == INITIALIZED || user.game_data.status == ONTRIAL, "Cannot do a trial. Initialize a new game.");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.status = ONTRIAL;
        game_data.deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
        // int num1 = numgames % 16;
        // int num2 = numgames % 4;
        // if (num1 > 0 && num1 < 5)
        // {
        //     game_data.prevalent_wind = EAST;
        // }
        // else if (num1 > 4 && num1 < 9)
        // {
        //     game_data.prevalent_wind = SOUTH;
        // }
        // else if (num1 > 8 && num1 < 13)
        // {
        //     game_data.prevalent_wind = WEST;
        // }
        // else
        // {
        //     game_data.prevalent_wind = NORTH;
        // }
        // if (num2 == 1)
        // {
        //     game_data.seat_wind = EAST;
        // }
        // else if (num2 == 2)
        // {
        //     game_data.seat_wind = SOUTH;
        // }
        // else if (num2 == 3)
        // {
        //     game_data.seat_wind = WEST;
        // }
        // else
        // {
        //     game_data.seat_wind = NORTH;
        // }
        int size5 = game_data.hand_player.size();
        if (game_data.hand_player.size() != 0)
        {
            for (int i = 0; i < size5; i++)
            {
                game_data.hand_player.erase(game_data.hand_player.begin());
            }
        }
        else
        {
            if (game_data.winning_hand.size() != 0)
            {
                int size = game_data.winning_hand.size();
                for (int i = 0; i < size; i++)
                {
                    game_data.winning_hand.erase(game_data.winning_hand.begin());
                }
                game_data.pair_count = 0;
                game_data.pung_count = 0;
                game_data.chow_count = 0;
                game_data.final_score = 0;
                int size2 = game_data.score_check.size();
                game_data.deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
                for (int i = 0; i < size2; i++)
                {
                    game_data.score_check.erase(game_data.score_check.begin());
                }
                int size3 = game_data.score_type.size();
                for (int i = 0; i < size3; i++)
                {
                    game_data.score_type.erase(game_data.score_type.begin());
                }
            }
            else
            {
                int size = game_data.hand_player.size();
                for (int i = 0; i < size; i++)
                {
                    game_data.hand_player.erase(game_data.hand_player.begin());
                }
            }
            for (int i = 0; i < idx.size(); i++)
            {
                game_data.hand_player.insert(game_data.hand_player.begin(), game_data.deck_player[idx[i] - 1]); // Assign the tile to the first empty slot in the hand
            }
            sorthand(game_data.hand_player);
            winhand_check(game_data, game_data.hand_player);
            if (game_data.winnable == 1)
            {
                transferhand(game_data, game_data.hand_player.size());
                sorteye(game_data.winning_hand, game_data.eye_idx);
                getscore(game_data, game_data.winning_hand);
                sorthand(game_data.score_check);
                print("Well played!");
                // print(game_data.final_score);
            }
            else
            {
                print("Your hand didn't win..");
            }
        }
        game_data.winnable = 0;
    });
}

ACTION mahjonghilo::playhilo(name username, int option)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    // check(, "Max number of draws reached.");
    check(user.game_data.hand_player.size() < (14 + user.game_data.kong_count - user.game_data.reveal_kong.size()) && user.game_data.discarded_tiles.size() < 20, "Discard a tile to draw a new one.");
    // check(, "Game may haven't started yet, may on an ongoing trial, or may already ended.");
    check(user.game_data.status == ONGOING && user.game_data.hi_lo_balance.amount > 1, "No remaining balance on account");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        // gettile(game_data);
        // float bet = 1.0000;
        game_data.hi_lo_balance.amount -= 10000;
        game_data.standard_tile = game_data.current_tile;
        const auto standard_tile = table_deck.at(game_data.standard_tile);
        const auto current_tile = table_deck.at(gettile(game_data));
        game_data.hi_lo_prize = hilo_step(game_data, standard_tile.value, current_tile.value, option);
        if (game_data.hi_lo_prize != 0)
        {
            game_data.hi_lo_result = WIN;
        }
        else
        {
            game_data.hi_lo_result = LOSE;
        }
        game_data.hi_lo_balance.amount += game_data.hi_lo_prize * 10000;
        // uint8_t tile_var = gettile(game_data);
        // const auto hilo_tile = table_deck.at(standard_tile);
        // hilo_step(game_data, standard_tile.value, current_tile.value, option, bet);
        // game_data.standard_tile = game_data.current_tile;
        get_odds(game_data, current_tile.value);
        if (game_data.hand_player.size() == (14 + user.game_data.kong_count - user.game_data.reveal_kong.size()))
        {
            winhand_check(game_data, game_data.hand_player);
        }
        if (game_data.winnable == 1)
        {
            print("You have a winning hand.");
        }
        else
        {
            if (game_data.discarded_tiles.size() >= 19)
            {
                game_data.status = LOSE;
                print("Draw limits reached.");
            }
            else
            {
                print("Keep playing and try again..");
            }
        }
    });
}

ACTION mahjonghilo::discardtile(name username, int idx)
{
    require_auth(username);

    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == ONGOING, "Game may haven't started yet, may on an ongoing trial, or may already ended.");
    check(user.game_data.discarded_tiles.size() < 19, "Your hand is for declaration(win/lose).");
    check(user.game_data.hand_player.size() == (14 + user.game_data.kong_count - user.game_data.reveal_kong.size()), "Have a complete hand before discarding a tile.");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.discarded_tiles.insert(game_data.discarded_tiles.begin(), game_data.hand_player[idx]);
        game_data.hand_player.erase(game_data.hand_player.begin() + idx); // Remove the card from the hand
    });
}

ACTION mahjonghilo::dclrkong(name username, vector<int> idx)
{
    require_auth(username);

    auto &user = _users.get(username.value, "User doesn't exist");
    vector<tile> kongtile{};
    check(user.game_data.status == ONGOING, "Game may haven't started yet, may on an ongoing trial, or may already ended.");
    check(user.game_data.hand_player.size() == (14 + user.game_data.kong_count - user.game_data.reveal_kong.size()), "Must have a complete hand to declare a kong.");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        for (int i = 0; i < 4; i++)
        {
            kongtile[i] = table_deck.at(game_data.hand_player[idx[i]]);
        }
        // const auto kongtile1 = table_deck.at(game_data.hand_player[idx[0]]);
        // const auto kongtile2 = table_deck.at(game_data.hand_player[idx[1]]);
        // const auto kongtile3 = table_deck.at(game_data.hand_player[idx[2]]);
        // const auto kongtile4 = table_deck.at(game_data.hand_player[idx[3]]);
        int check1 = pair_check(kongtile[0], kongtile[1]);
        int check2 = pair_check(kongtile[2], kongtile[3]);
        check(check1 == 1 && check2 == 1, "Tiles are not of the same suit and value.");
        if (pair_check(kongtile[0], kongtile[2]) == 1)
        {
            for (int i = 0; i < 4; i++)
            {
                game_data.reveal_kong.insert(game_data.reveal_kong.begin(), game_data.hand_player[idx[i]]);
            }
            for (int i = 3; i >= 0; i--)
            {
                game_data.hand_player.erase(game_data.hand_player.begin() + idx[i]);
            }
            game_data.kong_count += 1;
        }
        else
        {
            print("Tiles are not of the same suit and value.");
        }
        sorthand(game_data.reveal_kong);
    });
}

ACTION mahjonghilo::dclrwinhand(name username)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == ONGOING, "Game may haven't started yet, may be on trial, or may already ended.");
    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        // winhand_check(game_data, game_data.hand_player);
        if (game_data.winnable == 1)
        {
            transferhand(game_data, game_data.hand_player.size());
            vector<uint8_t> temp_hand = game_data.winning_hand;
            sorteye(temp_hand, game_data.eye_idx);
            getscore(game_data, temp_hand);
            for (int i = 0; i < game_data.reveal_kong.size(); i++)
            {
                game_data.winning_hand.insert(game_data.winning_hand.end(), game_data.reveal_kong[i]);
            }
            for (int i = 0; i < game_data.reveal_kong.size(); i++)
            {
                game_data.reveal_kong.erase(game_data.reveal_kong.begin());
            }
            game_data.status = WIN;
            // sorteye(temp_hand, game_data.eye_idx);
            // getscore(game_data, temp_hand);
            print("Well played!");
        }
        // else
        // {
        //     print("Your hand didn't win..");
        // }
        // if (game_data.hand_player.size() == 0)
        // {
        //     vector<uint8_t> temp_hand = game_data.winning_hand;
        //     sorteye(temp_hand, game_data.eye_idx);
        //     getscore(game_data, temp_hand);
        //     for (int i = 0; i < game_data.reveal_kong.size(); i++)
        //     {
        //         game_data.winning_hand.insert(game_data.winning_hand.end(), game_data.reveal_kong[i]);
        //     }
        //     for (int i = 0; i < game_data.reveal_kong.size(); i++)
        //     {
        //         game_data.reveal_kong.erase(game_data.reveal_kong.begin());
        //     }
        //     game_data.status = WIN;
        //     // sorteye(temp_hand, game_data.eye_idx);
        //     // getscore(game_data, temp_hand);
        //     print("Well played!");
        // }
        else
        {
            print("Your hand didn't win..");
        }
    });
}

ACTION mahjonghilo::settledpay(name username, asset prize, string memo)
{
    require_auth(_self);
    auto &user = _users.get(username.value, "User doesn't exist");
    check(user.game_data.status == ONGOING, "Game has ended and prize is already transferred.");
}

ACTION mahjonghilo::withdraw(name username)
{
    require_auth(username);
    auto user = _users.find(username.value);
    check(user->game_data.status != INITIALIZED, "Game has ended and prize is already transferred or you are on trial.");
    check(user->game_data.hi_lo_balance.amount > 0, "You have no prize money.");
    std::string feedback = "MHL Withdraw: " + name{username}.to_string() + " received " + std::to_string(user->game_data.hi_lo_balance.amount); // transfer funds to user
    action{
        permission_level{_self, "active"_n},
        eosio_token,
        "transfer"_n,
        std::make_tuple(_self, username, user->game_data.hi_lo_balance, feedback)}
        .send();

    _users.modify(user, username, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.hi_lo_balance = DEFAULT_ASSET;
    });
}

ACTION mahjonghilo::end(name username)
{
    // require_auth(_self);
    check(has_auth(_self) || has_auth(username), "Unauthorized user");
    auto &user = _users.get(username.value, "User doesn't exist");
    _users.erase(user);
}