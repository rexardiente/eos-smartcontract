// #include "mahjonghilo.h"
#include <string>

int coinicagames::pung_check(mhltile tile1, mhltile tile2, mhltile tile3)
{
    if (tile3.value == 11) // --- --- DDD
    {
        if (tile2.value == 11) // --- DDD DDD
        {
            if (tile1.value == 11) // DDD DDD DDD
            {
                return 11;
            }
            else if (tile1.value == 10) // www DDD DDD
            {
                return 13;
            }
            else if (tile1.value == 1 || tile1.value == 9) // 1/9 DDD DDD
            {
                return 14;
            }
            else // 2-8 DDD DDD
            {
                return 15;
            }
        }
        else if (tile2.value == 10) // --- www DDD
        {
            if (tile1.value == 10) // www www DDD
            {
                return 16;
            }
            else if (tile1.value == 1 || tile1.value == 9) // 1/9 www DDD
            {
                return 21;
            }
            else // 2-8 www DDD
            {
                return 31;
            }
        }
        else if (tile2.value == 1 || tile2.value == 9) // --- 1/9 DDD
        {
            if (tile1.value == 1 || tile1.value == 9) // 1/9 1/9 DDD
            {
                return 22;
            }
            else // 2-8 999 DDD
            {
                return 32;
            }
        }
        else
        {
            if (tile1.value == 1) // 111 2-8 DDD
            {
                return 37;
            }
            else // 2-8 2-8 DDD
            {
                return 33;
            }
        }
    }
    else if (tile3.value == 10) // --- --- www
    {
        if (tile2.value == 10) // --- www www
        {
            if (tile1.value == 10) // www www www
            {
                return 12;
            }
            else if (tile1.value == 1 || tile1.value == 9) // 1/9 www www
            {
                return 23;
            }
            else // 2-8 www www
            {
                return 34;
            }
        }
        else if (tile2.value == 1 || tile2.value == 9) // --- 1/9 www
        {
            if (tile1.value == 1 || tile1.value == 9) // 1/9 1/9 www
            {
                return 24;
            }
            else // 2-8 1-9 www
            {
                return 35;
            }
        }
        else
        {
            if (tile1.value == 1) // 1/9 2-8 www
            {
                return 38;
            }
            else // 2-8 2-8 www
            {
                return 36;
            }
        }
    }
    else if (tile3.value == 1 || tile3.value == 9) // --- --- 1/9
    {
        if (tile2.value == 1 || tile2.value == 9) // --- 1/9 1/9
        {
            if (tile1.value == 1 || tile1.value == 9) // 1/9 1/9 1/9
            {
                return 25;
            }
            else // 2-8 999 999
            {
                return 41;
            }
        }
        else
        {
            if (tile1.value == 1) // 111 2-8 999
            {
                return 43;
            }
            else // 2-8, 2-8, 999
            {
                return 42;
            }
        }
    }
    else
    {
        if (tile2.value > 1 && tile2.value < 9)
        {
            if (tile1.value > 1 && tile1.value < 9) // 2-8 2-8 2-8
            {
                return 48;
            }
            else // 111 2-8 2-8
            {
                return 44;
            }
        }
        else
        {
            return 45; // 111 111 2-8
        }
    }
}

int coinicagames::wind_check(mhlgamedata gamedata, mhltile tile1, int check1)
{
    if (tile1.value == 10)
    {
        if (tile1.suit != gamedata.prevalent_wind && tile1.suit != gamedata.seat_wind)
        {
            if (check1 == 0)
            {
                return 1;
            }
            else
            {
                return 5;
            }
        }
        else
        {
            if (tile1.suit == gamedata.prevalent_wind)
            {
                if (tile1.suit == gamedata.seat_wind)
                {
                    return 2;
                }
                else
                {
                    return 3;
                }
            }
            else
            {
                if (tile1.suit == gamedata.seat_wind)
                {
                    return 4;
                }
                else
                {
                    return 5;
                }
            }
        }
    }
    else
    {
        return 5;
    }
}

// void coinicagames::getscore(mhlgamedata &gamedata, vector<uint8_t> & hand)
// {
//     print("Process ongoing.");
// }

void coinicagames::getscore(mhlgamedata &gamedata, vector<uint8_t> & hand)
{
    vector<mhltile> tiles = {};
    for (int i = 0; i < hand.size(); i++)
    {
        tiles.insert(tiles.end(), table_deck.at(hand[i]));
    }
    if (gamedata.kong_count == 0)
    {
        if (gamedata.pung_count == 4)
        {
            gamedata.score_check.insert(gamedata.score_check.begin(), 12);
            vector<mhltile> tiles2 = {};
            tiles2.insert(tiles2.begin(), tiles[0]);
            tiles2.insert(tiles2.begin(), tiles[3]);
            tiles2.insert(tiles2.begin(), tiles[6]);
            tiles2.insert(tiles2.begin(), tiles[9]);
            int n = tiles2.size(), i, j;
            mhltile temp;
            for (i = 0; i < (n - 1); i++)
            {
                for (j = 0; j < (n - i - 1); j++)
                {
                    if (tiles2[j].value > tiles2[j + 1].value)
                    {
                        temp = tiles2[j];
                        tiles2[j] = tiles2[j + 1];
                        tiles2[j + 1] = temp;
                    }
                }
            }
            int checkfirst = tiles2[0].value;
            int checkwind = 0;
            int check1 = pung_check(tiles2[1], tiles2[2], tiles2[3]);
            if (checkfirst == 10) // www --- --- ---
            {
                if (check1 == 11) // www DDD DDD DDD --
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 2);
                    if (tiles[12].value == 10) // www DDD DDD DDD ww
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 10);
                        checkwind = 1;
                    }
                    else // www DDD DDD DDD 1/9 or www DDD DDD DDD 2-8
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        if (tiles[12].value == 1 || tiles[12].value == 9)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                            checkwind = 1;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                }
                else if (check1 == 12) // www www www www --
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 1);
                    checkwind = 1;
                    if (tiles[12].value == 11)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 10);
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        if (tiles[12].value == 1 || tiles[12].value == 9)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                }
                else if (check1 == 13) // www www DDD DDD --
                {
                    if (tiles[12].value > 9) // www www DDD DDD DD or www www DDD DDD ww
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 10);
                        if (tiles[12].value == 11)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 9);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 49);
                        }
                        checkwind = 1;
                    }
                    else // www www DDD DDD 1/9 or www www DDD DDD 2-8
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 49);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        if (tiles[12].value == 1 || tiles[12].value == 9)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                            checkwind = 1;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                }
                else // www www www DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 59);
                    if (tiles[12].value > 9) // www www www DDD DD or www www www DDD ww
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 10);
                        if (tiles[12].value == 11)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 34);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 8);
                        }
                        checkwind = 1;
                    }
                    else // www www www DDD 1/9 or www www www DDD 2-8
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 34);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        if (tiles[12].value == 1 || tiles[12].value == 9)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                            checkwind = 1;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                }
            }
            else if (checkfirst == 9) // 999 --- --- ---
            {
                if (check1 == 11) // 999 DDD DDD DDD --
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 2);
                    if (tiles[12].value == 10) // 999 DDD DDD DDD ww
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                        checkwind = 1;
                    }
                    else // 999 DDD DDD DDD 1/9 or 999 DDD DDD DDD 2-8
                    {
                        if (tiles[12].value == 1 || tiles[12].value == 9)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                            checkwind = 1;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                        if (tiles[0].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                    }
                }
                else if (check1 == 12) // 999 www www www --
                {
                    if (tiles[12].value > 9)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                        checkwind = 1;
                        if (tiles[12].value == 11)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 34);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 8);
                        }
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 34);
                        if (tiles[12].value == 1 || tiles[12].value == 9)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                        if (tiles[0].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                    }
                }
                else if (check1 == 13) // 999 www DDD DDD --
                {
                    if (tiles[12].value > 9)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        if (tiles[12].value == 11)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 9);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 49);
                        }
                    }
                    else
                    {
                        if (tiles[12].value == 1 || tiles[12].value == 9)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                            checkwind = 1;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                        if (tiles[0].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                    }
                }
                else if (check1 == 14) // 999 999 DDD DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                    if (tiles[12].value > 9)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        if (tiles[12].value == 11)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 9);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 49);
                        }
                    }
                    else
                    {
                        if (tiles[12].value == 1 || tiles[12].value == 9)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                            checkwind = 1;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                        if (tiles2[0].suit == tiles[12].suit || tiles2[1].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                }
                else if (check1 == 16) // 999 www www DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 59);
                    if (tiles[12].value > 9)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                        checkwind = 1;
                        gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                    }
                    else
                    {
                        if (tiles[12].value == 1 || tiles[12].value == 9)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                            checkwind = 1;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                        if (tiles[0].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                    }
                }
                else if (check1 == 21) // 999 999 www DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 59);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                    if (tiles[12].value > 9)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                        checkwind = 1;
                        gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                    }
                    else
                    {
                        if (tiles[12].value == 1 || tiles[12].value == 9)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                            checkwind = 1;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                        if (tiles2[0].suit == tiles[12].suit || tiles2[1].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 52);
                        }
                    }
                }
                else if (check1 == 22) // 999 999 999 DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 31);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 59);
                    if (tiles[12].value > 9)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                        checkwind = 1;
                        if (tiles[12].value == 10)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 52);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                    else
                    {
                        if (tiles[12].value == 1)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                            checkwind = 1;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                }
                else if (check1 == 23) // 999 999 www www
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                    if (tiles[12].value > 9)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        checkwind = 1;
                    }
                    else
                    {
                        if (tiles[12].value == 1 || tiles[12].value == 9)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                            checkwind = 1;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                        if (tiles2[0].suit == tiles[12].suit || tiles2[1].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                }
                else //if (check1 == 24) // 999 999 999 www
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 31);
                    if (tiles[12].value > 9)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                        checkwind = 1;
                        if (tiles[12].value == 11)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 52);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                    else
                    {
                        if (tiles[12].value == 1)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 17);
                            checkwind = 1;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                }
            }
            else if (checkfirst > 1 && checkfirst < 9) // 2-8 --- --- ---
            {
                int check1 = pung_check(tiles2[1], tiles2[2], tiles2[3]);
                if (check1 == 11) // 2-8 DDD DDD DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 2);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    if (tiles[12].value == 10) // 2-8 DDD DDD DDD ww
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                    }
                    else // 2-8 DDD DDD DDD 1/9 or 2-8 DDD DDD DDD 2-8
                    {
                        if (tiles[0].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                    }
                }
                else if (check1 == 12) // 2-8 www www www
                {
                    if (tiles[12].value > 9) // 2-8 www www www DD or 2-8 www www www ww
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        if (tiles[12].value == 11)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 34);
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 8);
                            checkwind = 1;
                        }
                    }
                    else // 2-8 www www www 1/9 or 2-8 www www www 2-8
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 34);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        if (tiles[0].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                    }
                }
                else if (check1 == 13) // 2-8 www DDD DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    if (tiles[12].value > 9)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        if (tiles[12].value == 11)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 9);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 49);
                        }
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 49);
                        if (tiles[0].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                    }
                }
                else if (check1 == 14 || check1 == 15) // 2-8 2-8/19 DDD DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    if (tiles2[0].value == tiles2[1].value)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    }
                    if (tiles[12].value > 9)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        if (tiles[12].value == 11)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 9);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 49);
                        }
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 49);
                        if (tiles[0].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                    }
                }
                else if (check1 == 16) // 2-8 www www DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 59);
                    if (tiles[12].value > 9)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                    }
                    else
                    {
                        if (tiles[0].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                    }
                }
                else if (check1 == 21 || check1 == 31) // 2-8 2-8/1/9 www DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 59);
                    if (tiles2[0].value == tiles2[1].value)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    }
                    if (tiles[12].value > 9)
                    {
                        if (tiles[0].suit == tiles[3].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                    }
                    else
                    {
                        if (tiles2[0].suit != tiles2[1].suit)
                        {
                            if (tiles2[0].suit == tiles[12].suit || tiles2[1].suit == tiles[12].suit)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                            }
                            else
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 52);
                            }
                        }
                        else
                        {
                            if (tiles2[0].suit == tiles[12].suit)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                            }
                            else
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                            }
                        }
                    }
                }
                else if (check1 == 22) // 2-8 999 999 DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 59);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                    if (tiles[12].value == 10)
                    {
                        if (tiles[0].suit == tiles2[1].suit || tiles[0].suit == tiles[2].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 52);
                        }
                    }
                    else
                    {
                        if (tiles[0].suit == tiles2[1].suit || tiles[0].suit == tiles[2].suit)
                        {
                            if (tiles[12].suit == tiles2[1].suit || tiles[12].suit == tiles2[2].suit)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                            }
                            else
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                            }
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                }
                else if (check1 == 23 || check1 == 34) // 2-8 2-8/1/9 www www
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    if (tiles2[0].value == tiles2[1].value)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    }
                    if (tiles[12].value > 9)
                    {
                        if (tiles[0].suit == tiles[3].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                    }
                    else
                    {
                        if (tiles2[0].suit == tiles[12].suit || tiles2[1].suit == tiles[12].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 52);
                        }
                    }
                }
                else if (check1 == 24) // 2-8 999 999 www
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                    if (tiles[12].value == 11)
                    {
                        if (tiles[0].suit == tiles2[1].suit || tiles[0].suit == tiles2[2].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    }
                }
                else if (check1 == 25) // 2-8 999 999 999
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 31);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    if (tiles[12].value < 10)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 76);
                    }
                    else
                    {
                        if (tiles[0].value >= 6 && tiles[12].value >= 6)
                        {
                            if (tiles[0].value >= 7 && tiles[12].value >= 7)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 24);
                            }
                            else
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 36);
                            }
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                }
                else if (check1 == 32) // 2-8 2-8 999 DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 59);
                    if (tiles2[0].value == tiles2[1].value)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                    }
                    else if (tiles2[0].value + 1 == tiles2[1].value && tiles2[0].value == 7)
                    {
                        if (tiles2[0].suit != tiles2[1].suit && tiles2[1].suit != tiles2[2].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 41);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    }
                    if (tiles[12].value == 10)
                    {
                        if (tiles[0].suit != tiles2[1].suit)
                        {
                            if (tiles2[1].suit != tiles2[2].suit)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 52);
                            }
                            else
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                            }
                        }
                        else
                        {
                            if (tiles[1].suit == tiles2[2].suit)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                            }
                            else
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                            }
                        }
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    }
                }
                else if (check1 == 33) // 2-8 2-8 2-8 DDD
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 59);
                    int checktype = 0;
                    if (tiles2[0].value + 1 == tiles2[1].value && tiles2[1].value + 1 == tiles2[2].value)
                    {
                        if (tiles2[0].suit == tiles2[1].suit && tiles2[1].suit == tiles2[2].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 20);
                            checktype = 1;
                        }
                        else if (tiles2[0].suit != tiles2[1].suit && tiles2[1].suit != tiles2[2].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 41);
                            checktype = 2;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                    else if (tiles2[0].value == tiles2[1].value)
                    {
                        if (tiles2[1].value == tiles2[2].value)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 31);
                            checktype = 2;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                            if (tiles2[2].suit == tiles2[0].suit || tiles2[2].suit == tiles2[1].suit)
                            {
                                checktype = 3;
                            }
                            else
                            {
                                checktype = 2;
                            }
                        }
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    }
                    if (tiles[12].value > 1 && tiles[12].value < 9)
                    {
                        if (tiles2[3].suit == GREEN)
                        {
                            if (tiles2[0].suit == BAMBOO && tiles2[1].suit == BAMBOO)
                            {
                                if (tiles2[2].suit == BAMBOO && tiles[12].suit == BAMBOO)
                                {
                                    if (tiles2[0].value % 2 == 0 || tiles2[0].value == 3)
                                    {
                                        if (tiles2[1].value % 2 == 0 || tiles2[1].value == 3)
                                        {
                                            if (tiles2[2].value % 2 == 0 || tiles2[2].value == 3)
                                            {
                                                if (tiles[12].value % 2 == 0 || tiles[12].value == 3)
                                                {
                                                    gamedata.score_check.insert(gamedata.score_check.begin(), 5);
                                                }
                                                else
                                                {
                                                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                                                }
                                            }
                                            else
                                            {
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                                            }
                                        }
                                        else
                                        {
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                                        }
                                    }
                                    else
                                    {
                                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                                    }
                                }
                                else
                                {
                                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                                }
                            }
                            else
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                            }
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                    else if (tiles[12].value == 10)
                    {
                        if (checktype == 1)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else if (checktype == 2)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 52);
                        }
                        else if (checktype == 3)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    }
                }
                else if (check1 == 35) // 2-8 2-8 999 www
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    if (tiles2[0].value == tiles2[1].value)
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                    }
                    else if (tiles2[0].value + 1 == tiles2[1].value && tiles2[0].value == 7)
                    {
                        if (tiles2[0].suit != tiles2[1].suit && tiles2[1].suit != tiles2[2].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 41);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    }
                    if (tiles[12].value == 11)
                    {
                        if (tiles[0].suit != tiles2[1].suit)
                        {
                            if (tiles2[1].suit != tiles2[2].suit)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 52);
                            }
                            else
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                            }
                        }
                        else
                        {
                            if (tiles[1].suit == tiles2[2].suit)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                            }
                            else
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                            }
                        }
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    }
                }
                else if (check1 == 36) // 2-8 2-8 2-8 www
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                    int checktype = 0;
                    if (tiles2[0].value + 1 == tiles2[1].value && tiles2[1].value + 1 == tiles2[2].value)
                    {
                        if (tiles2[0].suit == tiles2[1].suit && tiles2[1].suit == tiles2[2].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 20);
                            checktype = 1;
                        }
                        else if (tiles2[0].suit != tiles2[1].suit && tiles2[1].suit != tiles2[2].suit)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 41);
                            checktype = 2;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                    else if (tiles2[0].value == tiles2[1].value)
                    {
                        if (tiles2[1].value == tiles2[2].value)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 31);
                            checktype = 2;
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                            if (tiles2[2].suit == tiles2[0].suit || tiles2[2].suit == tiles2[1].suit)
                            {
                                checktype = 3;
                            }
                            else
                            {
                                checktype = 2;
                            }
                        }
                    }
                    else
                    {
                        gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                    }
                    if (tiles[12].value == 11)
                    {
                        if (checktype == 1)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                        }
                        else if (checktype == 2)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 52);
                        }
                        else if (checktype == 3)
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                    else
                    {
                        if (checktype == 1)
                        {
                            if (tiles[12].suit == tiles[0].suit)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 53);
                            }
                            else
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                            }
                        }
                        else if (checktype == 3)
                        {
                            if (tiles[12].suit == tiles[0].suit || tiles[12].suit == tiles[3].suit)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                            }
                            else if (tiles[12].suit == tiles[6].suit)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                            }
                            else
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                            }
                        }
                        else
                        {
                            gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                        }
                    }
                }
                // else if (check1 == 41) // 2-8 2-8 999 999
                // {
                // gamedata.score_check.insert(gamedata.score_check.begin(), 66);
                // if (tiles2[0].value + 1 == tiles2[1].value && tiles2[1].value + 1 == tiles2[2].value)
                // {
                //     if (tiles2[0].suit == tiles2[1].suit && tiles2[1].suit == tiles2[2].suit)
                //     {
                //         gamedata.score_check.insert(gamedata.score_check.begin(), 20);
                //         checktype = 1;
                //     }
                //     else if (tiles2[0].suit != tiles2[1].suit && tiles2[1].suit != tiles2[2].suit)
                //     {
                //         gamedata.score_check.insert(gamedata.score_check.begin(), 41);
                //         checktype = 2;
                //     }
                //     else
                //     {
                //         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                //         checktype = 3;
                //     }
                // }
                // else
                // {
                //     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                // }
                // if (tiles[12].value > 9)
                // {
                //     if (checktype == 1)
                //     {
                //         gamedata.score_check.insert(gamedata.score_check.begin(), 75);
                //     }
                //     else
                //     {
                //         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                //     }
                // }
                // else
                // {
                //     gamedata.score_check.insert(gamedata.score_check.begin(), 76);
                //     if (tiles[12].value > 5)
                //     {
                //     }
                //     else
                //     {
                //         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
                //     }
                // }
                // }
                else // check1
                {
                    gamedata.score_check.insert(gamedata.score_check.begin(), 56); // trial
                }
            }
            else // checkfirst == 1
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 65); // trial
            }
            int checksamp1 = wind_check(gamedata, tiles2[0], checkwind);
            if (checksamp1 == 1)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 73);
                checkwind = 1;
            }
            else if (checksamp1 == 2)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 60);
                gamedata.score_check.insert(gamedata.score_check.begin(), 61);
            }
            else if (checksamp1 == 3)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 60);
            }
            else if (checksamp1 == 4)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 61);
            }
            else
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 0);
            }
            int checksamp2 = wind_check(gamedata, tiles2[1], checkwind);
            if (checksamp2 == 1)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 73);
                checkwind = 1;
            }
            else if (checksamp2 == 2)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 60);
                gamedata.score_check.insert(gamedata.score_check.begin(), 61);
            }
            else if (checksamp2 == 3)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 60);
            }
            else if (checksamp2 == 4)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 61);
            }
            else
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 0);
            }
            int checksamp3 = wind_check(gamedata, tiles2[2], checkwind);
            if (checksamp3 == 1)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 73);
                checkwind = 1;
            }
            else if (checksamp3 == 2)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 60);
                gamedata.score_check.insert(gamedata.score_check.begin(), 61);
            }
            else if (checksamp3 == 3)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 60);
            }
            else if (checksamp3 == 4)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 61);
            }
            else
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 0);
            }
            int checksamp4 = wind_check(gamedata, tiles2[3], checkwind);
            if (checksamp4 == 1)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 73);
                checkwind = 1;
            }
            else if (checksamp4 == 2)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 60);
                gamedata.score_check.insert(gamedata.score_check.begin(), 61);
            }
            else if (checksamp4 == 3)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 60);
            }
            else if (checksamp4 == 4)
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 61);
            }
            else
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 0);
            }
            // vector<int> checksamp = {};
            // for (int i = 0; i < tiles2.size(); i++)
            // {
            //     if (tiles2[i].value == 10)
            //     {
            //         checksamp[i] = wind_check(gamedata, tiles2[i], checkwind);
            //         if (checksamp[i] == 1)
            //         {
            //             gamedata.score_check.insert(gamedata.score_check.begin(), 73);
            //             checkwind = 1;
            //         }
            //         else if (checksamp[i] == 2)
            //         {
            //             gamedata.score_check.insert(gamedata.score_check.begin(), 60);
            //             gamedata.score_check.insert(gamedata.score_check.begin(), 61);
            //         }
            //         else if (checksamp[i] == 3)
            //         {
            //             gamedata.score_check.insert(gamedata.score_check.begin(), 60);
            //         }
            //         else if (checksamp[i] == 4)
            //         {
            //             gamedata.score_check.insert(gamedata.score_check.begin(), 61);
            //         }
            //         else
            //         {
            //             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
            //         }
            //     }
            //     // else
            //     // {
            //     //     print("No wind tiles..");
            //     // }
            // }
        }
        else if (gamedata.pung_count == 3)
        {
            gamedata.score_check.insert(gamedata.score_check.begin(), 32);
        }
        else if (gamedata.pung_count == 2)
        {
            gamedata.score_check.insert(gamedata.score_check.begin(), 74);
        }
        else if (gamedata.pung_count == 1)
        {
            gamedata.score_check.insert(gamedata.score_check.begin(), 74);
        }
        else // Seven Pairs
        {
            gamedata.score_check.insert(gamedata.score_check.begin(), 74);
        }
    }
    else if (gamedata.kong_count == 1)
    {
        gamedata.score_check.insert(gamedata.score_check.begin(), 74);
    }
    else if (gamedata.kong_count == 2)
    {
        gamedata.score_check.insert(gamedata.score_check.begin(), 74);
    }
    else if (gamedata.kong_count == 3)
    {
        gamedata.score_check.insert(gamedata.score_check.begin(), 18);
    }
    else
    {
        gamedata.score_check.insert(gamedata.score_check.begin(), 3);
        gamedata.score_check.insert(gamedata.score_check.begin(), 12);
    }
    sumscore(gamedata);
}
