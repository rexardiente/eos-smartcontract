#include <eosio/crypto.hpp>
#include "mhlgameplay.cpp"

ACTION mhlgame::mhladdbet(int id, double quantity)
{
    require_auth(_self);
    auto itr = _mjhilos.find(id);
    // check(itr == _mjhilos.end(), "Either User has Initialized a Game or has an Existing Game");
    // auto user = _mjhilos.find(id);
    _mjhilos.modify(itr, _self, [&](auto &modified_mjhilo) {
        mhlgamedata &game_data = modified_mjhilo.game_data;
        game_data.hi_lo_balance += quantity;
    });
}

ACTION mhlgame::mhlinitialze(int id)
{
    require_auth(_self);
    // Ensure this action is authorized by the player
    if (_mjhilos.find(id) == _mjhilos.end())
    {
        _mjhilos.emplace(_self, [&](auto &new_mjhilo) { new_mjhilo.id = id; });
    }

    auto itr = _mjhilos.find(id);
    check(itr != _mjhilos.end(), "User or Game Doesn't Exist.");
    // uint64_t id = 0;
    check(itr->game_data.status != 1, "Continue or finish your current game..");
    check(itr->game_data.option_status == 0, "Bet in place..");
    _mjhilos.modify(itr, _self, [&](auto &modified_mjhilo) {
        modified_mjhilo.game_count += 1;
        mhlgamedata &game_data = modified_mjhilo.game_data;
        const int game_count = modified_mjhilo.game_count;
        // game_data.hi_lo_balance += quantity;
        game_data.game_id = checksum256_to_string_hash().substr(0, 30);
        game_data.status = MHL_ONGOING;
        game_data.bet_status = 1;
        game_data.deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
        game_data.sumofvalue = {12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 12};
        game_data.hi_lo_stake = 0.0000;
        game_data.option_status = 0;
        game_data.hi_lo_result = DEFAULT;
        // game_data.hi_lo_prize = DEFAULT;
        game_data.low_odds = DEFAULT;
        game_data.draw_odds = DEFAULT;
        game_data.high_odds = DEFAULT;
        game_data.winnable = DEFAULT;
        game_data.hand_player = {};
        game_data.discarded_tiles = {};
        game_data.reveal_kong = {};
        game_data.winning_hand = {};
        game_data.score_check = {};
        game_data.score_type = {};
        game_data.pair_count = DEFAULT;
        game_data.pung_count = DEFAULT;
        game_data.chow_count = DEFAULT;
        game_data.kong_count = DEFAULT;
        game_data.draw_count = DEFAULT;
        game_data.final_score = DEFAULT;
        game_data.current_tile = DEFAULT;
        game_data.standard_tile = DEFAULT;
        if(game_data.hi_lo_stake>0)
        {
            game_data.hi_lo_balance += game_data.hi_lo_stake;
        }
        else
        {
            game_data.hi_lo_balance = DEFAULT;
        }
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
        gettile(game_data);
    });
}

ACTION mhlgame::mhlplayhilo(int id, int option)
{
    require_auth(_self);
    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    int checking = 0;
    mhltile checktile = table_deck.at(mjhilo.game_data.standard_tile);
    if (checktile.value == 1 && option == 1)
    {
        checking = 5;
    }
    else if (checktile.value == 11 && option == 3)
    {
        checking = 10;
    }
    else if(mjhilo.game_data.sumofvalue[checktile.value-1]==0 && option == 3)
    {
        checking = 15;
    }
    else
    {
        checking = 1;
    }
    // check(, "Max number of draws reached.");
    check(mjhilo.game_data.hand_player.size() < (14 + mjhilo.game_data.kong_count - mjhilo.game_data.reveal_kong.size()), "Discard a tile to draw a new one.");
    check(mjhilo.game_data.draw_count < 34, " Draw limit reached..");
    check(mjhilo.game_data.status == 1, "No ongoing game.");
    check(mjhilo.game_data.option_status == 1, "No bet yet.");
    check(checking != 5, "Cannot choose low.");
    check(checking != 10, "Cannot choose high.");
    check(checking != 15, "Cannot choose high.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo) {
        mhlgamedata &game_data = modified_mjhilo.game_data;
        float current_stake = game_data.hi_lo_stake;
        // if(game_data.hi_lo_result==2)
        // {
        //     game_data.hi_lo_bet = game_data.hi_lo_stake;
        // }
        // else
        // {
        //     game_data.hi_lo_bet = 1;
        // }
        gettile(game_data);
        if (game_data.hand_player.size() <= 13)
        {
            sorthand(game_data.hand_player);
        }
        game_data.prediction = option;
        // float bet = 1.0000;
        const auto standard_tile = table_deck.at(game_data.standard_tile);
        const auto current_tile = table_deck.at(game_data.current_tile);
        game_data.hi_lo_stake *= hilo_step(game_data, standard_tile.value, current_tile.value);
        if (game_data.hi_lo_stake != 0)
        {
            game_data.hi_lo_result = MHL_WIN;
        }
        else
        {
            game_data.hi_lo_result = MHL_LOSE;
            game_data.hi_lo_bet = current_stake;
        }
        // game_data.hi_lo_winnings += game_data.hi_lo_prize;
        // uint8_t tile_var = gettile(game_data);
        // const auto hilo_tile = table_deck.at(standard_tile);
        // hilo_step(game_data, standard_tile.value, current_tile.value, option, bet);
        // game_data.standard_tile = game_data.current_tile;
        // get_odds(game_data, current_tile.value);
        if (game_data.hand_player.size() == (14 + mjhilo.game_data.kong_count - mjhilo.game_data.reveal_kong.size()))
        {
            // sorthand(game_data.hand_player);
            winhand_check(game_data, game_data.hand_player);
            if (game_data.winnable == 1)
            {
                print("You have a winning hand.");
            }
            else
            {
                if (game_data.draw_count >= 33)
                {
                    game_data.hi_lo_balance += game_data.hi_lo_stake;
                    game_data.hi_lo_stake = 0.0000;
                    game_data.status = MHL_LOSE;
                    game_data.bet_status = 1;
                    print("Draw limits reached.");
                }
                else
                {
                    print("Keep playing and try again..");
                }
            }
        }
        // game_data.hi_lo_bet = 0;
        game_data.bet_status = 1;
        game_data.option_status = 0;
    });
}

ACTION mhlgame::mhlresetbet(int id)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    // check(idx <= 13, "Index should be below 14.");
    check(mjhilo.game_data.status == MHL_ONGOING, "Game already ended.");
    check(mjhilo.game_data.hi_lo_result == MHL_LOSE, "Game already ended.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo) {
        mhlgamedata &game_data = modified_mjhilo.game_data;
        game_data.hi_lo_bet = 0.0000;
    });
}

ACTION mhlgame::mhldscrdtile(int id, int idx)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    // check(idx <= 13, "Index should be below 14.");
    check(mjhilo.game_data.status == MHL_ONGOING, "Game already ended.");
    check(mjhilo.game_data.draw_count < 34, "Your hand is for declaration(win/lose).");
    check(mjhilo.game_data.hand_player.size() == (14 + mjhilo.game_data.kong_count - mjhilo.game_data.reveal_kong.size()), "Have a complete hand before discarding a tile.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo) {
        mhlgamedata &game_data = modified_mjhilo.game_data;
        game_data.winnable = 0;
        game_data.pung_count = DEFAULT;
        game_data.pair_count = DEFAULT;
        game_data.chow_count = DEFAULT;
        game_data.discarded_tiles.insert(game_data.discarded_tiles.begin(), game_data.hand_player[idx]);
        game_data.hand_player.erase(game_data.hand_player.begin() + idx); // Remove the card from the hand
        sorthand(game_data.hand_player);
    });
}

ACTION mhlgame::mhlstartbet(int id)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    check(mjhilo.game_data.hi_lo_balance >= 1 || mjhilo.game_data.hi_lo_stake != 0, "Not sufficient balance on account..");
    check(mjhilo.game_data.hand_player.size() < (14 + mjhilo.game_data.kong_count - mjhilo.game_data.reveal_kong.size()), "Discard a tile to play Hi-Low..");
    check(mjhilo.game_data.bet_status == 1, "Bet in place.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo) {
        mhlgamedata &game_data = modified_mjhilo.game_data;
        // if(game_data.hi_lo_result==3 && game_data.hi_lo_stake!=0)
        // {
        //     game_data.hi_lo_stake = 0.0000;
        // }
        // else
        // {        
            if(game_data.hi_lo_result==2 && game_data.hi_lo_stake != 0)
                {
                    game_data.hi_lo_bet = game_data.hi_lo_stake;
                }
            else
                {
                    // game_data.hi_lo_bet = 1;
                    game_data.hi_lo_balance -= 1;
                    game_data.hi_lo_bet += 1.0000;
                    game_data.hi_lo_stake = game_data.hi_lo_bet;
                }
        // }
        // if (game_data.hi_lo_stake == 0)
        // {
        //     game_data.hi_lo_balance -= 10000;
        //     game_data.hi_lo_stake += 1.0000;
        // }
        // else
        // {
        //     game_data.hi_lo_stake = game_data.hi_lo_stake;
        // }
        game_data.bet_status = 0;
        game_data.option_status = 1;
        game_data.hi_lo_outcome = DEFAULT;
        game_data.standard_tile = game_data.current_tile;
        const auto hilo_tile = table_deck.at(game_data.standard_tile);
        get_odds(game_data, hilo_tile.value);
    });
}

ACTION mhlgame::mhlwintrnsfr(int id)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    check(mjhilo.game_data.status != MHL_INITIALIZED, "Game haven't started.");
    check(mjhilo.game_data.bet_status == 1, "Bet in place.");
    // check(mjhilo.game_data.hi_lo_result != 3, "Last hi-lo was a lost.");
    check(mjhilo.game_data.hi_lo_stake > 0.0000, "No winnings found.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo) {
        mhlgamedata &game_data = modified_mjhilo.game_data;
        game_data.hi_lo_balance += game_data.hi_lo_stake;
        game_data.hi_lo_stake = 0.0000;
        game_data.hi_lo_bet = 0.0000;
    });
}

ACTION mhlgame::mhldclrkong(int id, vector<int> idx)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    vector<mhltile> kongtile{};
    // check(idx.size() == 4, "Array size should be equal to 4 to declare a kong.")
    check(mjhilo.game_data.status == MHL_ONGOING, "No ongoing game..");
    // check(mjhilo.game_data.hand_player.size() == (14 + mjhilo.game_data.kong_count - mjhilo.game_data.reveal_kong.size()), "Must have a complete hand to declare a kong.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo) {
        mhlgamedata &game_data = modified_mjhilo.game_data;
        for (int i = 0; i < 4; i++)
        {
            kongtile[i] = table_deck.at(game_data.hand_player[idx[i]]);
        }
        // const auto kongtile1 = table_deck.at(game_data.hand_player[idx[0]]);
        // const auto kongtile2 = table_deck.at(game_data.hand_player[idx[1]]);
        // const auto kongtile3 = table_deck.at(game_data.hand_player[idx[2]]);
        // const auto kongtile4 = table_deck.at(game_data.hand_player[idx[3]]);
        if(kongtile[0].value==kongtile[1].value && kongtile[0].suit==kongtile[1].suit)
        {
            if(kongtile[2].value==kongtile[3].value && kongtile[2].suit==kongtile[3].suit)
            {
                if(kongtile[2].value==kongtile[0].value && kongtile[2].suit==kongtile[0].suit)
                {game_data.winnable = 0;
                for (int i = 0; i < 4; i++)
                {
                    game_data.reveal_kong.insert(game_data.reveal_kong.begin(), game_data.hand_player[idx[i]]);
                }
                for (int i = 3; i >= 0; i--)
                {
                    game_data.hand_player.erase(game_data.hand_player.begin() + idx[i]);
                }
                game_data.kong_count += 1;}
            }
            else
            {
                print("Tiles are not of the same suit and value.");
            }
        }
        else
        {
            print("Tiles are not of the same suit and value.");
        }
        // int check1 = pair_check(kongtile[0], kongtile[1]);
        // int check2 = pair_check(kongtile[2], kongtile[3]);
        // check(check1 == 1 && check2 == 1, "Tiles are not of the same suit and value.");
        // if (pair_check(kongtile[0], kongtile[2]) == 1)
        // {
            
        // }
        // else
        // {
        //     print("Tiles are not of the same suit and value.");
        // }
        sorthand(game_data.reveal_kong);
    });
}

ACTION mhlgame::mhldclrwnhnd(int id)
{
    require_auth(_self);
    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    check(mjhilo.game_data.status == MHL_ONGOING, "Game already ended.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo) {
        mhlgamedata &game_data = modified_mjhilo.game_data;
        if (game_data.winnable == 1)
        {
            // transferhand(game_data, game_data.hand_player.size());
            vector<uint8_t> temp_hand = game_data.winning_hand;
            sorteye(temp_hand, game_data.eye_idx);
            // getscore(game_data, temp_hand);
            for (int i = 0; i < game_data.reveal_kong.size(); i++)
            {
                game_data.winning_hand.insert(game_data.winning_hand.end(), game_data.reveal_kong[i]);
            }
            for (int i = 0; i < game_data.reveal_kong.size(); i++)
            {
                game_data.reveal_kong.erase(game_data.reveal_kong.begin());
            }
            game_data.status = MHL_WIN;
            // sorteye(temp_hand, game_data.eye_idx);
            // getscore(game_data, temp_hand);
            print("Well played!");
        }
        else
        {
            print("Your hand didn't win..");
        }
    });
}

ACTION mhlgame::mhlwithdraw(int id)
{
    require_auth(_self);
    auto mjhilo = _mjhilos.find(id);
    check(mjhilo->game_data.status != MHL_INITIALIZED, "Game has ended and prize is already transferred or you are on trial.");
    check(mjhilo->game_data.hi_lo_balance > 0, "You have no prize money.");
    check(mjhilo->game_data.hi_lo_stake == 0, "Transfer your winnings to withdraw.");
    // std::string feedback = "MHL Withdraw: " + name{id}.to_string() + " received " + std::to_string(mjhilo->game_data.hi_lo_balance); // transfer funds to mjhilo
    // action{
    //     permission_level{_self, "active"_n},
    //     eosio_token,
    //     "transfer"_n,
    //     std::make_tuple(_self, id, mjhilo->game_data.hi_lo_balance, feedback)}
    //     .send();

    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo) {
        mhlgamedata &game_data = modified_mjhilo.game_data;
        game_data.hi_lo_balance = DEFAULT;
        game_data.hi_lo_stake = DEFAULT;
        game_data.hi_lo_bet = DEFAULT;
    });
}

ACTION mhlgame::mhlend(int id)
{
    require_auth(_self);
    // check(has_auth(_self) || has_auth(id), "Unauthorized user");
    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    check(mjhilo.game_data.hi_lo_balance == 0.0000 && mjhilo.game_data.hi_lo_stake == 0.0000, "Withdraw your balance before you can end.");
    _mjhilos.erase(mjhilo);
}

ACTION mhlgame::mhlresetacc(int id)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    // check(mjhilo.game_data.hi_lo_result != 3, "Last hi-lo was a lost.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo) {
        mhlgamedata &game_data = modified_mjhilo.game_data;
        game_data.status = 2;
    });
}