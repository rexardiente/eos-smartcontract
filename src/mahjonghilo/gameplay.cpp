#include "mahjonghilo.hpp"
#include <string>

void mahjonghilo::ondeposit(name from,
                            name to,
                            asset quantity,
                            string memo)
{
    if (from == _self)
    {
        if (memo.find(HAS_ON_SETTLE_PAY) != std::string::npos)
        {
            onsettledpay(to, quantity, memo);
        }

        // we're sending money, do nothing additional
        return;
    }
    check(to == _self, "Not to our contract");
    check(quantity.symbol.is_valid(), "Invalid quantity");
    check(quantity.amount > 0, "Only positive quantity allowed");
    check(quantity.symbol == mahjonghilo_symbol, "Invalid EOS Token");

    // std::string str = memo.substr(9);
    // if (memo.find("ADD_LIFE") != std::string::npos)
    // {
    //     set_add_life(from, quantity, str);
    // }
    // else
    // {
    //     int limit = stoi(str);
    //     summon_ready(from, quantity, limit);
    // }
}

void mahjonghilo::onsettledpay(name username, asset quantity, string memo)
{
    require_auth(_self);
    action(
        permission_level{_self, "active"_n},
        _self,
        "settledpay"_n,
        std::make_tuple(username, quantity, memo))
        .send();
}

void mahjonghilo::gettile(game &gamedata)
{
    // int deck_tile_idx = rng(gamedata.deck_player.size()); // Pick a random tile from the deck
    int deck_tile_idx = 4; // Pick a random tile from the deck

    gamedata.hand_player.insert(gamedata.hand_player.begin(), gamedata.deck_player[deck_tile_idx]); // Assign the tile to the first empty slot in the hand
    gamedata.current_tile = gamedata.deck_player[deck_tile_idx];
    gamedata.deck_player.erase(gamedata.deck_player.begin() + deck_tile_idx); // Remove the tile from the deck
    sorthand(gamedata.hand_player);
}

void mahjonghilo::get_odds(game &gamedata, int value)
{
    float odds;
    if (value == 1)
    {
        gamedata.low_odds = 0;
    }
    else
    {
        gamedata.low_odds = (135 / ((value - 1) * 12)) * 0.9;
        if (gamedata.low_odds < 1)
        {
            gamedata.low_odds = 1;
        }
    }
    if (value == 10)
    {
        gamedata.draw_odds = (135 / 15) * 0.9;
    }
    else
    {
        gamedata.draw_odds = (135 / 11) * 0.9;
        if (gamedata.draw_odds < 1)
        {
            gamedata.draw_odds = 1;
        }
    }
    if (value == 11)
    {
        gamedata.high_odds = 0;
    }
    else
    {
        gamedata.high_odds = (135 / ((11 - value) * 12)) * 0.9;
        if (gamedata.high_odds < 1)
        {
            gamedata.high_odds = 1;
        }
    }
}

void mahjonghilo::sorthand(vector<uint8_t> &hand)
{
    int n = hand.size(), i, j, temp;
    for (i = 0; i < (n - 1); i++)
    {
        for (j = 0; j < (n - i - 1); j++)
        {
            if (hand[j] > hand[j + 1])
            {
                temp = hand[j];
                hand[j] = hand[j + 1];
                hand[j + 1] = temp;
            }
        }
    }
}

void mahjonghilo::hilo_step(game &gamedata, int prev_tile, int current_tile, int option)
{
    int result;
    if (prev_tile > current_tile)
    {
        result = 1;
    }
    else if (prev_tile == current_tile)
    {
        result = 2;
    }
    else
    {
        result = 3;
    }
    if (result == option)
    {
        if (result == 1)
        {
            gamedata.hi_lo_prize.amount *= gamedata.low_odds;
        }
        else if (result == 2)
        {
            gamedata.hi_lo_prize.amount *= gamedata.draw_odds;
        }
        else
        {
            gamedata.hi_lo_prize.amount *= gamedata.high_odds;
        }
    }
    else
    {
        gamedata.hi_lo_prize.amount = 0;
    }
}

void mahjonghilo::winhand_check(game &gamedata, vector<uint8_t> &hand)
{
    vector<tile> remtiles = {};
    for (int i = 0; i < gamedata.hand_player.size(); i++)
    {
        remtiles.insert(remtiles.end(), table_deck.at(gamedata.hand_player[i]));
    }
    if (remtiles.size() == 2)
    {
        two_rem(gamedata, remtiles);
    }
    else if (remtiles.size() == 5)
    {
        five_rem(gamedata, remtiles);
    }
    else if (remtiles.size() == 8)
    {
        eight_rem(gamedata, remtiles);
    }
    // else if (remtiles.size() == 11)
    // {
    //     eleven_rem(gamedata, remtiles);
    // }
    // else if (remtiles.size() == 14)
    // {
    //     fourteen_rem(gamedata, remtiles);
    // }
    else
    {
        print("Tiles in your hand can not be formed into a winning hand.");
    }
}

void mahjonghilo::transferhand(game &gamedata, int size)
{
    for (int i = 0; i < size; i++)
    {
        gamedata.winning_hand.insert(gamedata.winning_hand.begin(), gamedata.hand_player[i]);
    }
    for (int i = 0; i < size; i++)
    {
        gamedata.hand_player.erase(gamedata.hand_player.begin());
    }
    sorthand(gamedata.winning_hand);
    print("Congratulations, you won!");
}

void mahjonghilo::pung_chow(game &gamedata, int check)
{

    if (check == 2)
    {
        gamedata.pung_count += 1;
    }
    else
    {
        gamedata.chow_count += 1;
    }
}

int mahjonghilo::pair_pung_chow(tile tile1, tile tile2, tile tile3)
{
    if (tile1.suit == tile2.suit && tile1.value == tile2.value)
    {
        if (tile2.suit == tile3.suit && tile2.value == tile3.value)
        {
            return 2;
        }
        else if (tile2.value != tile3.value)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (tile1.suit == tile2.suit && tile2.suit == tile3.suit)
    {
        if ((tile1.value + 1) == tile2.value && (tile1.value + 2) == tile3.value)
        {
            return 3;
        }
        else
        {
            return 0;
        }
    }
    else
        return 0;
}

int mahjonghilo::pair_check(tile tile1, tile tile2)
{
    if (tile1.suit == tile2.suit && tile1.value == tile2.value)
    {
        return 1;
    }
    else if (tile1.suit == tile2.suit && (tile1.value + 1) == tile2.value)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

int mahjonghilo::five_tile_check(tile tile1, tile tile2, tile tile3, tile tile4, tile tile5)
{
    if ((pair_pung_chow(tile2, tile3, tile4)) == 2)
    {
        if ((pair_pung_chow(tile1, tile3, tile5)) == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if ((pair_pung_chow(tile3, tile4, tile5)) == 2)
    {
        if ((pair_pung_chow(tile1, tile2, tile3)) == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if ((pair_pung_chow(tile1, tile2, tile4)) == 3)
    {
        if ((pair_check(tile3, tile5)) == 2)
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

void mahjonghilo::two_rem(game &gamedata, vector<tile> tiles)
{
    if ((pair_check(tiles[0], tiles[1])) == 1)
    {
        transferhand(gamedata, 2);
        gamedata.pair_count += 1;
        print(tiles[0].value);
        print("Congratulations, you won!");
        print(tiles[1].value);
    }
    else
    {
        print("1 - Hand combination not correct!");
    }
}

void mahjonghilo::five_rem(game &gamedata, vector<tile> tiles)
{
    int check1 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
    if (check1 == 1)
    {
        int check2 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
        if (check2 == 2 || check2 == 3)
        {
            transferhand(gamedata, 5);
            pung_chow(gamedata, check2);
            gamedata.pair_count += 1;
        }
        else
        {
            print("2 - Hand combination not correct!");
        }
    }
    else if (check1 == 3)
    {
        if ((pair_check(tiles[3], tiles[4])) == 1)
        {
            gamedata.chow_count += 1;
            gamedata.pair_count += 1;
            transferhand(gamedata, 5);
        }
        else
        {
            print("3 - Hand combination not correct!");
        }
    }
    else if (check1 == 2)
    {
        if ((pair_check(tiles[3], tiles[4])) == 1)
        {
            gamedata.pair_count += 1;
            pung_chow(gamedata, check1);
            transferhand(gamedata, 5);
        }
        else
        {
            int check2 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
            if (check2 == 3)
            {
                gamedata.chow_count += 1;
                gamedata.pair_count += 1;
                transferhand(gamedata, 5);
            }
            else
            {
                print("4 - Hand combination not correct!");
            }
        }
    }
    else
    {
        if ((five_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4])) == 1)
        {
            transferhand(gamedata, 5);
            gamedata.chow_count += 1;
            gamedata.pair_count += 1;
        }
        else
        {
            print("5 - Hand combination not correct!");
        }
    }
}

void mahjonghilo::eight_rem(game &gamedata, vector<tile> tiles)
{
    int check1 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
    if (check1 == 1)
    {
        int check2 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
        if (check2 == 2 || check2 == 3)
        {
            pung_chow(gamedata, check2);
            int check3 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
            if (check3 == 2 || check3 == 3)
            {
                transferhand(gamedata, 8);
                pung_chow(gamedata, check3);
                gamedata.pair_count += 1;
            }
            else
            {
                print("6 - Hand combination not correct!");
            }
        }
        else if (check2 == 1)
        {
            int check3 = pair_check(tiles[4], tiles[5]);
            int check4 = pair_check(tiles[6], tiles[7]);
            if (check3 == 1 && check4 == 1)
            {
                if ((pair_pung_chow(tiles[0], tiles[2], tiles[4])) == 3 || (pair_pung_chow(tiles[2], tiles[4], tiles[6])) == 3)
                {
                    gamedata.chow_count += 2;
                    gamedata.pair_count += 1;
                    transferhand(gamedata, 8);
                }
                else
                {
                    print("7 - Hand combination not correct!");
                }
            }
            else
            {
                print("8 - Hand combination not correct!");
            }
        }
        else
        {
            if ((five_tile_check(tiles[2], tiles[3], tiles[4], tiles[5], tiles[6])) == 2 && (pair_check(tiles[6], tiles[7])) == 2)
            {
                gamedata.chow_count += 2;
                gamedata.pair_count += 1;
                transferhand(gamedata, 8);
            }
            else
            {
                print("8 - Hand combination not correct!");
            }
        }
    }
    else if (check1 == 2)
    {
        /* code */
    }
    else
    {
        /* code */
    }
}

// void mahjonghilo::eleven_rem(game &gamedata, vector<tile> tiles)
// {
//     for (int i = 0; i < tiles.size(); i++)
//     {
//         print(tiles[i].tile_value);
//     }
//     int check1 = pair_pung_chow(tiles[0].tile_value, tiles[0].suit, tiles[1].tile_value, tiles[1].suit, tiles[2].tile_value, tiles[2].suit);
//     if (check1 == 1)
//     {
//         int check2 = pair_pung_chow(tiles[2].tile_value, tiles[2].suit, tiles[3].tile_value, tiles[3].suit, tiles[4].tile_value, tiles[4].suit);
//         if (check2 == 2 || check2 == 3)
//         {
//             pung_chow(gamedata, check2);
//             int check3 = pair_pung_chow(tiles[5].tile_value, tiles[5].suit, tiles[6].tile_value, tiles[6].suit, tiles[7].tile_value, tiles[7].suit);
//             if (check3 == 2 || check3 == 3)
//             {
//                 pung_chow(gamedata, check3);
//                 int check4 = pair_pung_chow(tiles[8].tile_value, tiles[8].suit, tiles[9].tile_value, tiles[9].suit, tiles[10].tile_value, tiles[10].suit);
//                 if (check4 == 2 || check4 == 3)
//                 {
//                     transferhand(gamedata, 11);
//                     pung_chow(gamedata, check4);
//                     gamedata.pair_count += 1;
//                 }
//                 else
//                 {
//                     print("21 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 print("22 - Hand combination not correct!");
//             }
//         }
//         else if (check2 == 1)
//         {
//             int check3 = pair_pung_chow(tiles[4].tile_value, tiles[4].suit, tiles[5].tile_value, tiles[5].suit, tiles[6].tile_value, tiles[6].suit);
//             if (check3 == 1)
//             {
//                 if (tiles[0].suit == tiles[2].suit && (tiles[0].tile_value + 1) == tiles[2].tile_value)
//                 {
//                     if (tiles[3].suit == tiles[5].suit && (tiles[3].tile_value + 1) == tiles[5].tile_value)
//                     {
//                         gamedata.chow_count += 2;
//                         int check4 = pair_pung_chow(tiles[6].tile_value, tiles[6].suit, tiles[7].tile_value, tiles[7].suit, tiles[8].tile_value, tiles[8].suit);
//                         if (check4 == 2 || check4 == 3)
//                         {
//                             pung_chow(gamedata, check4);
//                             if (tiles[9].suit == tiles[9].suit && tiles[10].tile_value == tiles[10].tile_value)
//                             {
//                                 transferhand(gamedata, 11);
//                                 gamedata.pair_count += 1;
//                             }
//                             else
//                             {
//                                 print("25 - Hand combination not correct!");
//                             }
//                         }
//                         else if (check4 == 1)
//                         {
//                             gamedata.pair_count += 1;
//                             int check5 = pair_pung_chow(tiles[8].tile_value, tiles[8].suit, tiles[9].tile_value, tiles[9].suit, tiles[10].tile_value, tiles[10].suit);
//                             if (check5 == 2 || check5 == 3)
//                             {
//                                 transferhand(gamedata, 11);
//                                 pung_chow(gamedata, check5);
//                             }
//                             else
//                             {
//                                 print("23 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             int check6 = pair_pung_chow(tiles[7].tile_value, tiles[7].suit, tiles[8].tile_value, tiles[8].suit, tiles[9].tile_value, tiles[9].suit);
//                             if (check6 == 2)
//                             {
//                                 if (tiles[6].suit == tiles[7].suit && tiles[9].suit == tiles[10].suit)
//                                 {
//                                     if ((tiles[6].tile_value + 1) == tiles[7].tile_value && (tiles[9].tile_value + 1) == tiles[10].tile_value)
//                                     {
//                                         transferhand(gamedata, 11);
//                                         gamedata.pair_count += 1;
//                                         gamedata.chow_count += 1;
//                                     }
//                                     else
//                                     {
//                                         print("24 - Hand combination not correct!");
//                                     }
//                                 }
//                                 else
//                                 {
//                                     print("31 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 print("32 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                 }
//                 else if (tiles[2].suit == tiles[4].suit && (tiles[2].tile_value + 1) == tiles[4].tile_value)
//                 {
//                     int check6 = pair_pung_chow(tiles[6].tile_value, tiles[6].suit, tiles[7].tile_value, tiles[7].suit, tiles[8].tile_value, tiles[8].suit);
//                     if (check6 == 1)
//                     {
//                         if (tiles[2].suit == tiles[4].suit && (tiles[2].tile_value + 1) == tiles[4].tile_value)
//                         {
//                             if (tiles[4].suit == tiles[6].suit && (tiles[4].tile_value + 1) == tiles[6].tile_value)
//                             {
//                                 gamedata.chow_count += 2;
//                                 gamedata.pair_count += 1;
//                                 int check8 = pair_pung_chow(tiles[8].tile_value, tiles[8].suit, tiles[9].tile_value, tiles[9].suit, tiles[10].tile_value, tiles[10].suit);
//                                 if (check8 == 2 || check8 == 3)
//                                 {
//                                     transferhand(gamedata, 11);
//                                     pung_chow(gamedata, check8);
//                                 }
//                                 else
//                                 {
//                                     print("26 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 print("26 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             print("33 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("34 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     print("35 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 print("36 - Hand combination not correct!");
//             }
//         }
//         else
//         {
//             print("37 - Hand combination not correct!");
//         }
//     }
//     else if (check1 == 2 || check1 == 3)
//     {
//         pung_chow(gamedata, check1);
//         int check2 = pair_pung_chow(tiles[3].tile_value, tiles[3].suit, tiles[4].tile_value, tiles[4].suit, tiles[5].tile_value, tiles[5].suit);
//         if (check2 == 2 || check2 == 3)
//         {
//             pung_chow(gamedata, check2);
//             int check3 = pair_pung_chow(tiles[6].tile_value, tiles[6].suit, tiles[7].tile_value, tiles[7].suit, tiles[8].tile_value, tiles[8].suit);
//             if (check3 == 2 || check3 == 3)
//             {
//                 pung_chow(gamedata, check3);
//                 if (tiles[9].suit == tiles[10].suit && tiles[9].tile_value == tiles[10].tile_value)
//                 {
//                     transferhand(gamedata, 11);
//                     gamedata.pair_count += 1;
//                 }
//                 else
//                 {
//                     print("26 - Hand combination not correct!");
//                 }
//             }
//             else if (check3 == 1)
//             {
//                 int check4 = pair_pung_chow(tiles[8].tile_value, tiles[8].suit, tiles[9].tile_value, tiles[9].suit, tiles[10].tile_value, tiles[10].suit);
//                 if (check4 == 2 || check4 == 3)
//                 {
//                     transferhand(gamedata, 11);
//                     pung_chow(gamedata, check4);
//                     gamedata.pair_count += 1;
//                 }
//                 else
//                 {
//                     print("27 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 int check5 = pair_pung_chow(tiles[7].tile_value, tiles[7].suit, tiles[8].tile_value, tiles[8].suit, tiles[9].tile_value, tiles[9].suit);
//                 if (check5 == 2)
//                 {
//                     if (tiles[6].suit == tiles[7].suit && tiles[9].suit == tiles[10].suit)
//                     {
//                         if ((tiles[6].tile_value + 1) == tiles[7].tile_value && (tiles[9].tile_value + 1) == tiles[10].tile_value)
//                         {
//                             transferhand(gamedata, 11);
//                             gamedata.pair_count += 1;
//                             gamedata.chow_count += 1;
//                         }
//                         else
//                         {
//                             print("28 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("29 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     print("30 - Hand combination not correct!");
//                 }
//             }
//         }
//         else if (check2 == 1)
//         {
//             int check3 = pair_pung_chow(tiles[5].tile_value, tiles[5].suit, tiles[6].tile_value, tiles[6].suit, tiles[7].tile_value, tiles[7].suit);
//             if (check3 == 2 || check3 == 3)
//             {
//                 pung_chow(gamedata, check3);
//                 int check4 = pair_pung_chow(tiles[8].tile_value, tiles[8].suit, tiles[9].tile_value, tiles[9].suit, tiles[10].tile_value, tiles[10].suit);
//                 if (check4 == 2 || check4 == 3)
//                 {
//                     transferhand(gamedata, 11);
//                     pung_chow(gamedata, check4);
//                 }
//                 else
//                 {
//                     print("38 - Hand combination not correct!");
//                 }
//             }
//             else if (check3 == 1)
//             {
//                 int check4 = pair_pung_chow(tiles[7].tile_value, tiles[7].suit, tiles[8].tile_value, tiles[8].suit, tiles[9].tile_value, tiles[9].suit);
//                 if (check4 == 1)
//                 {
//                     if (tiles[3].suit == tiles[5].suit && tiles[5].suit == tiles[7].suit)
//                     {
//                         if ((tiles[3].tile_value + 1) == tiles[5].tile_value && (tiles[5].tile_value + 1) == tiles[7].tile_value)
//                         {
//                             gamedata.chow_count += 2;
//                             if (tiles[9].suit == tiles[10].suit && tiles[9].tile_value == tiles[10].tile_value)
//                             {
//                                 transferhand(gamedata, 11);
//                                 gamedata.pair_count += 1;
//                             }
//                             else
//                             {
//                                 print("39 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             if (tiles[9].suit == tiles[10].suit && tiles[9].tile_value == tiles[10].tile_value)
//                             {
//                                 if (tiles[5].suit == tiles[7].suit && tiles[7].suit == tiles[9].suit)
//                                 {
//                                     if ((tiles[5].tile_value + 1) == tiles[7].tile_value && (tiles[7].tile_value + 1) == tiles[9].tile_value)
//                                     {
//                                         transferhand(gamedata, 11);
//                                         gamedata.pair_count += 1;
//                                         gamedata.chow_count += 2;
//                                     }
//                                     else
//                                     {
//                                         print("40 - Hand combination not correct!");
//                                     }
//                                 }
//                                 else
//                                 {
//                                     print("41 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 print("42 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                     else if (tiles[9].suit == tiles[10].suit && tiles[9].tile_value == tiles[10].tile_value)
//                     {
//                         if (tiles[5].suit == tiles[7].suit && tiles[7].suit == tiles[9].suit)
//                         {
//                             if ((tiles[5].tile_value + 1) == tiles[7].tile_value && (tiles[8].tile_value + 1) == tiles[9].tile_value)
//                             {
//                                 transferhand(gamedata, 11);
//                                 gamedata.pair_count += 1;
//                                 gamedata.chow_count += 2;
//                             }
//                             else
//                             {
//                                 print("43 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             print("44 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("45 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     print("46 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 print("47 - Hand combination not correct!");
//             }
//         }
//         else
//         {
//             int check3 = pair_pung_chow(tiles[4].tile_value, tiles[4].suit, tiles[5].tile_value, tiles[5].suit, tiles[6].tile_value, tiles[6].suit);
//             if (check3 == 2)
//             {
//                 if (tiles[3].suit == tiles[4].suit && tiles[6].suit == tiles[7].suit)
//                 {
//                     if ((tiles[3].tile_value + 1) == tiles[4].tile_value && (tiles[6].tile_value + 1) == tiles[7].tile_value)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.chow_count += 1;
//                         int check4 = pair_pung_chow(tiles[8].tile_value, tiles[8].suit, tiles[9].tile_value, tiles[9].suit, tiles[10].tile_value, tiles[10].suit);
//                         if (check4 == 2 || check4 == 3)
//                         {
//                             pung_chow(gamedata, check4);
//                             transferhand(gamedata, 11);
//                         }
//                         else
//                         {
//                             print("48 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("49 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     print("50 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 print("51 - Hand combination not correct!");
//             }
//         }
//     }
//     else
//     {
//         int check2 = pair_pung_chow(tiles[1].tile_value, tiles[1].suit, tiles[2].tile_value, tiles[2].suit, tiles[3].tile_value, tiles[3].suit);
//         if (check2 == 2)
//         {
//             if (tiles[0].suit == tiles[1].suit && tiles[3].suit == tiles[4].suit)
//             {
//                 if ((tiles[0].tile_value + 1) == tiles[1].tile_value && (tiles[3].tile_value + 1) == tiles[4].tile_value)
//                 {
//                     gamedata.chow_count += 1;
//                     gamedata.pair_count += 1;
//                     int check3 = pair_pung_chow(tiles[5].tile_value, tiles[5].suit, tiles[6].tile_value, tiles[6].suit, tiles[7].tile_value, tiles[7].suit);
//                     if (check3 == 2 || check3 == 3)
//                     {
//                         pung_chow(gamedata, check3);
//                         int check4 = pair_pung_chow(tiles[8].tile_value, tiles[8].suit, tiles[9].tile_value, tiles[9].suit, tiles[10].tile_value, tiles[10].suit);
//                         if (check4 == 2 || check4 == 3)
//                         {
//                             pung_chow(gamedata, check4);
//                             transferhand(gamedata, 11);
//                         }
//                         else
//                         {
//                             print("52 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("53 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     print("54 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 print("55 - Hand combination not correct!");
//             }
//         }
//         else
//         {
//             print("56 - Hand combination not correct!");
//         }
//     }
// }

// void mahjonghilo::fourteen_rem(game &gamedata, vector<tile> tiles)
// {
//     print("Hello! - 2");
// }

int mahjonghilo::rng(const int &range)
{
    // Find the existing seed
    auto seed_iterator = _seeds.begin();

    // Initialize the seed with default value if it is not found
    if (seed_iterator == _seeds.end())
    {
        seed_iterator = _seeds.emplace(_self, [&](auto &seed) {});
    }

    // Generate new seed value using the existing seed value
    int prime = 65537;
    auto new_seed_value = (seed_iterator->value + current_time_point().elapsed.count()) % prime;

    // Store the updated seed value in the table
    _seeds.modify(seed_iterator, _self, [&](auto &s) {
        s.value = new_seed_value;
    });

    // Get the random result in desired range
    int random_result = new_seed_value % range;
    return random_result;
}

string mahjonghilo::checksum256_to_string(std::array<uint8_t, 32UL> arr, size_t size)
{
    std::string r;
    const char *to_hex = "0123456789abcdef";
    for (uint32_t i = 0; i < arr.size(); ++i)
    {
        (r += to_hex[(arr[i] >> 4)]) += to_hex[(arr[i] & 0x0f)];
    }
    return r;
}