#include <string>
// #include "mhl.hpp"
#include <eosio/transaction.hpp>


int mhlgame::meld_check(mhltile tile1, mhltile tile2, mhltile tile3)
{
    if (tile1.suit == tile2.suit && tile1.value == tile2.value)
    {
        if (tile2.value == tile3.value && tile2.suit == tile3.suit) // 111
        {
            return 2;
        }
        else
        {
            return 1; // 11
        }
    }
    else if (tile1.suit == tile2.suit && tile2.suit == tile3.suit)
    {
        if ((tile1.value + 1) == tile2.value && (tile1.value + 2) == tile3.value) // 123
        {
            return 3;
        }
        else if ((tile1.value + 1) == tile2.value && (tile2.value + 1) != tile3.value) // 124
        {
            return 5;
        }
        else if ((tile1.value + 3) == tile2.value && (tile2.value + 3) == tile3.value) // 147
        {
            return 4;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (pair_check(tile2, tile3) == 1) // 1 44
        {
            return 6;
        }
        else if (pair_check(tile2, tile3) == 1) // 1 34
        {
            return 7;
        }
        else
        {
            return 0;
        }
    }
}

int mhlgame::pair_check(mhltile tile1, mhltile tile2) // !!!
{
    // if (tile1.suit == tile2.suit && tile1.value == tile2.value)
    // {
    //     return 1;
    // }
    // else if (tile1.suit == tile2.suit && (tile1.value + 1) == tile2.value)
    // {
    //     return 2;
    // }
    // else if(tile1.value==1 && tile2.value==9)
    // {
    //     if(tile1.suit==tiles2.suit)

    // }
    if (tile1.suit == tile2.suit)
    {
        if (tile1.value == tile2.value) // 11
        {
            return 1;
        }
        else if ((tile1.value + 1) == tile2.value) // 12
        {
            return 2;
        }
        else if ((tile1.value + 2) == tile2.value) // 13
        {
            return 3;
        }
        else if (tile1.value == 1 && tile2.value == 9) // 19
        {
            return 4;
        }
        else if ((tile1.value + 6) == tile2.value) // 39
        {
            return 5;
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

int mhlgame::five_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5) // !!!!!
{
    if ((meld_check(tile2, tile3, tile4)) == 2)
    {
        if ((meld_check(tile1, tile3, tile5)) == 3)
        {
            return 1;
        }
        else if ((pair_check(tile4, tile5)) == 1 && (pair_check(tile1, tile2)) == 2)
        {
            return 3;
        }
        else if ((pair_check(tile1, tile4)) == 1 && (pair_check(tile4, tile5)) == 2)
        {
            return 3;
        }
        else
        {
            return 0;
        }
    }
    else if ((meld_check(tile3, tile4, tile5)) == 2)
    {
        if ((meld_check(tile1, tile2, tile3)) == 3)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if ((meld_check(tile1, tile2, tile4)) == 3)
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

void mhlgame::tile_insert(mhlgamedata &gamedata, vector<mhltile> tiles, uint8_t idx)
{
    // vector<mhltile> temptiles = remtiles;
    // temptiles.erase(temptiles.begin() + i);
    // gamedata.riichi_status = 1;
    winnables tempwin;
    tempwin.index = idx;
    tempwin.tileswin = tiles;
    gamedata.wintiles.insert(gamedata.wintiles.end(), tempwin);
    gamedata.riichi_status = 1;
}

void mhlgame::thirteen_check(mhlgamedata &gamedata, vector<mhltile> tiles, uint8_t idx)
{
    int sample = handcheck()
}

// void mhlgame::thirteen_check(mhlgamedata &gamedata, vector<mhltile> tiles, uint8_t idx)
// {
//     vector<mhltile> wintile = {};
//     int check1 = meld_check(tiles[0], tiles[1], tiles[2]); // 123meld
//     if (check1 == 2 || check1 == 3)
//     {
//         int check2 = meld_check(tiles[3], tiles[4], tiles[5]); // 1-6meld
//         if (check2 == 2 || check1 == 3)
//         {
//             int check3 = meld_check(tiles[6], tiles[7], tiles[8]); // 1-9meld
//             if (check3 == 2 || check1 == 3)
//             {
//                 int check4 = pair_check(tiles[9], tiles[10]); // 1-9meld 11
//                 if (check4 == 1)
//                 {
//                     if (pair_check(tiles[11], tiles[12]) == 1) // 1-9meld 11 22
//                     {
//                         wintile.insert(wintile.begin(), tiles[9]);
//                         wintile.insert(wintile.begin(), tiles[11]);
//                         tile_insert(gamedata, wintile, idx);
//                     }
//                     else if (pair_check(tiles[11], tiles[12]) == 2) // 1-9meld 11 23
//                     {
//                         if (tiles[10].value == tiles[11].value && tiles[10].suit == tiles[11].suit)
//                         {
//                             mhltile tile1 = tiles[10];
//                             mhltile tile2 = tiles[12];
//                             mhltile tile3 = tiles[12];
//                             tile1.value -= 1;
//                             tile2.value += 1;
//                             if (tile1.value >= 1)
//                             {
//                                 wintile.insert(wintile.begin(), tile1);
//                             }
//                             if (tile2.value <= 9)
//                             {
//                                 wintile.insert(wintile.begin(), tile2);
//                             }
//                             if (tile3.value <= 9)
//                             {
//                                 wintile.insert(wintile.begin(), tile3);
//                             }
//                             tile_insert(gamedata, wintile, idx);
//                         }
//                         else
//                         {
//                             mhltile tile1 = tiles[11];
//                             mhltile tile2 = tiles[12];
//                             tile1.value -= 1;
//                             tile2.value += 1;
//                             if (tile1.value >= 1)
//                             {
//                                 wintile.insert(wintile.begin(), tile1);
//                             }
//                             if (tile2.value <= 9)
//                             {
//                                 wintile.insert(wintile.begin(), tile2);
//                             }
//                             tile_insert(gamedata, wintile, idx);
//                         }
//                     }
//                     else if (pair_check(tiles[11], tiles[12]) == 3)
//                     {
//                         mhltile tile1 = tiles[11];
//                         tile1.value += 1;
//                         wintile.insert(wintile.begin(), tile1);
//                         tile_insert(gamedata, wintile, idx);
//                     }
//                     else
//                     {
//                         print("Hand not reach.. (1)");
//                     }
//                 }
//                 else if (check4 == 2) // 1-9 meld 12
//                 {
//                     if (pair_check(tiles[11], tiles[12]) == 1)
//                     {
//                         mhltile tile1 = tiles[9];
//                         mhltile tile2 = tiles[10];
//                         tile1.value -= 1;
//                         tile2.value += 1;
//                         if (tile1.value >= 1)
//                         {
//                             wintile.insert(wintile.begin(), tile1);
//                         }
//                         if (tile2.value <= 9)
//                         {
//                             wintile.insert(wintile.begin(), tile2);
//                         }
//                         tile_insert(gamedata, wintile, idx);
//                     }
//                 }
//                 else
//                 {
//                     print("Hand not reach.. (1)");
//                 }
//             }
//             else
//             {
//                 print("Hand not reach.. (1)");
//             }
//         }
//         else
//         {
//             print("Hand not reach.. (1)");
//         }
//     }
//     else
//     {
//         print("Hand not reach.. (1)");
//     }
//     // gamedata.riichi_status = 1;
//     // gamedata.wintiles.insert(gamedata.wintiles.end(), pair<uint8_t, vector<mhltile>>(idx, temptiles));
// }

// void mhlgame::transferhand(mhlgamedata &gamedata, int size)
// {
//     for (int i = 0; i < size; i++)
//     {
//         gamedata.winning_hand.insert(gamedata.winning_hand.begin(), gamedata.hand_player[i]);
//     }
//     for (int i = 0; i < size; i++)
//     {
//         gamedata.hand_player.erase(gamedata.hand_player.begin());
//     }
//     sorthand(gamedata.winning_hand);
// }

// int mhlgame::wind_check(mhlgamedata gamedata, mhltile tile1, int check1)
// {
//     if (tile1.value == 10)
//     {
//         if (tile1.suit != gamedata.prevalent_wind && tile1.suit != gamedata.seat_wind)
//         {
//             if (check1 == 0)
//             {
//                 return 1;
//             }
//             else
//             {
//                 return 5;
//             }
//         }
//         else
//         {
//             if (tile1.suit == gamedata.prevalent_wind)
//             {
//                 if (tile1.suit == gamedata.seat_wind)
//                 {
//                     return 2;
//                 }
//                 else
//                 {
//                     return 3;
//                 }
//             }
//             else
//             {
//                 if (tile1.suit == gamedata.seat_wind)
//                 {
//                     return 4;
//                 }
//                 else
//                 {
//                     return 5;
//                 }
//             }
//         }
//     }
//     else
//     {
//         return 5;
//     }
// }

// void mhlgame::pung_chow(mhlgamedata &gamedata, int check)
// {
//     if (check == 2)
//     {
//         gamedata.pung_count += 1;
//     }
//     else if (check == 3)
//     {
//         gamedata.chow_count += 1;
//     }
//     else if (check == 4)
//     {
//         gamedata.pung_count += 2;
//     }
//     else if (check == 5 || check == 6)
//     {
//         gamedata.chow_count += 1;
//         gamedata.pung_count += 1;
//     }
//     else if (check == 7 || check == 8)
//     {
//         gamedata.chow_count += 2;
//     }
//     else
//     {
//         print("Pung or chow check is invalid.");
//     }
// }

// int mhlgame::pung_check(mhltile tile1, mhltile tile2, mhltile tile3)
// {
//     if (tile3.value == 11) // --- --- DDD
//     {
//         if (tile2.value == 11) // --- DDD DDD
//         {
//             if (tile1.value == 11) // DDD DDD DDD
//             {
//                 return 11;
//             }
//             else if (tile1.value == 10) // www DDD DDD
//             {
//                 return 13;
//             }
//             else if (tile1.value == 1 || tile1.value == 9) // 1/9 DDD DDD
//             {
//                 return 14;
//             }
//             else // 2-8 DDD DDD
//             {
//                 return 15;
//             }
//         }
//         else if (tile2.value == 10) // --- www DDD
//         {
//             if (tile1.value == 10) // www www DDD
//             {
//                 return 16;
//             }
//             else if (tile1.value == 1 || tile1.value == 9) // 1/9 www DDD
//             {
//                 return 21;
//             }
//             else // 2-8 www DDD
//             {
//                 return 31;
//             }
//         }
//         else if (tile2.value == 1 || tile2.value == 9) // --- 1/9 DDD
//         {
//             if (tile1.value == 1 || tile1.value == 9) // 1/9 1/9 DDD
//             {
//                 return 22;
//             }
//             else // 2-8 999 DDD
//             {
//                 return 32;
//             }
//         }
//         else
//         {
//             if (tile1.value == 1) // 111 2-8 DDD
//             {
//                 return 37;
//             }
//             else // 2-8 2-8 DDD
//             {
//                 return 33;
//             }
//         }
//     }
//     else if (tile3.value == 10) // --- --- www
//     {
//         if (tile2.value == 10) // --- www www
//         {
//             if (tile1.value == 10) // www www www
//             {
//                 return 12;
//             }
//             else if (tile1.value == 1 || tile1.value == 9) // 1/9 www www
//             {
//                 return 23;
//             }
//             else // 2-8 www www
//             {
//                 return 34;
//             }
//         }
//         else if (tile2.value == 1 || tile2.value == 9) // --- 1/9 www
//         {
//             if (tile1.value == 1 || tile1.value == 9) // 1/9 1/9 www
//             {
//                 return 24;
//             }
//             else // 2-8 1-9 www
//             {
//                 return 35;
//             }
//         }
//         else
//         {
//             if (tile1.value == 1) // 1/9 2-8 www
//             {
//                 return 38;
//             }
//             else // 2-8 2-8 www
//             {
//                 return 36;
//             }
//         }
//     }
//     else if (tile3.value == 1 || tile3.value == 9) // --- --- 1/9
//     {
//         if (tile2.value == 1 || tile2.value == 9) // --- 1/9 1/9
//         {
//             if (tile1.value == 1 || tile1.value == 9) // 1/9 1/9 1/9
//             {
//                 return 25;
//             }
//             else // 2-8 999 999
//             {
//                 return 41;
//             }
//         }
//         else
//         {
//             if (tile1.value == 1) // 111 2-8 999
//             {
//                 return 43;
//             }
//             else // 2-8, 2-8, 999
//             {
//                 return 42;
//             }
//         }
//     }
//     else
//     {
//         if (tile2.value > 1 && tile2.value < 9)
//         {
//             if (tile1.value > 1 && tile1.value < 9) // 2-8 2-8 2-8
//             {
//                 return 48;
//             }
//             else // 111 2-8 2-8
//             {
//                 return 44;
//             }
//         }
//         else
//         {
//             return 45; // 111 111 2-8
//         }
//     }
// }

// int mhlgame::pair_check(mhltile tile1, mhltile tile2) // !!!
// {
//     // if (tile1.suit == tile2.suit && tile1.value == tile2.value)
//     // {
//     //     return 1;
//     // }
//     // else if (tile1.suit == tile2.suit && (tile1.value + 1) == tile2.value)
//     // {
//     //     return 2;
//     // }
//     // else if(tile1.value==1 && tile2.value==9)
//     // {
//     //     if(tile1.suit==tiles2.suit)

//     // }
//     if (tile1.suit == tile2.suit)
//     {
//         if (tile1.value == tile2.value)
//         {
//             return 1;
//         }
//         else if ((tile1.value + 1) == tile2.value)
//         {
//             return 2;
//         }
//         else if (tile1.value == 1 && tile2.value == 9)
//         {
//             return 3;
//         }
//         else if ((tile1.value + 6) == tile2.value)
//         {
//             return 4;
//         }
//         else
//         {
//             return 5;
//         }
//     }
//     else
//     {
//         return 0;
//     }
// }

// int mhlgame::honors_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5, mhltile tile6, mhltile tile7) // !!!
// {
//     if (tile1.value == 10 && tile1.suit == 4)
//     {
//         if (tile2.value == 10 && tile2.suit == 5)
//         {
//             if (tile3.value == 10 && tile3.suit == 6)
//             {
//                 if (tile4.value == 10 && tile4.suit == 7)
//                 {
//                     if (tile5.value == 11 && tile5.suit == 8)
//                     {
//                         if (tile6.value == 11 && tile6.suit == 9)
//                         {
//                             if (tile7.value == 11 && tile7.suit == 10)
//                             {
//                                 return 1;
//                             }
//                             else
//                             {
//                                 return 0;
//                             }
//                         }
//                         else
//                         {
//                             return 0;
//                         }
//                     }
//                     else
//                     {
//                         return 0;
//                     }
//                 }
//                 else
//                 {
//                     return 0;
//                 }
//             }
//             else
//             {
//                 return 0;
//             }
//         }
//         else
//         {
//             return 0;
//         }
//     }
//     else if (tile1.value < 10)
//     {
//         if (tile2.value >= 10 && tile2.suit < tile3.suit) // 9 10[4]
//         {
//             if (tile3.suit < tile4.suit && tile4.suit < tile5.suit) // 10[5] 10[6]
//             {
//                 if (tile5.suit < tile6.suit && tile6.suit < tile7.suit)
//                 {
//                     return 2;
//                 }
//                 else
//                 {
//                     return 0;
//                 }
//             }
//             else
//             {
//                 return 0;
//             }
//         }
//         else if (tile2.value < 10 && tile3.value >= 10)
//         {
//             if (tile3.suit < tile4.suit && tile4.suit < tile5.suit) // 10[5] 10[6]
//             {
//                 if (tile5.suit < tile6.suit && tile6.suit < tile7.suit)
//                 {
//                     return 3;
//                 }
//                 else
//                 {
//                     return 0;
//                 }
//             }
//             else
//             {
//                 return 0;
//             }
//         }
//         else
//         {
//             return 0;
//         }
//     }
//     else
//     {
//         return 0;
//     }
// }