// #include "mahjonghilo.h"
#include <string>

int coinicagames::meld_check(mhltile tile1, mhltile tile2, mhltile tile3)
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
        return 0;
}

int coinicagames::pair_check(mhltile tile1, mhltile tile2) // !!!
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

int coinicagames::five_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5) // !!!!!
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

void coinicagames::tile_insert(mhlgamedata &gamedata, vector<mhltile> tiles, uint8_t idx)
{
    gamedata.wintiles.insert(gamedata.wintiles.end(), pair<uint8_t, vector<mhltile> >(idx, tiles));
    gamedata.riichi_status = 1;
}

void coinicagames::thirteen_check(mhlgamedata &gamedata, vector<mhltile> tiles, uint8_t idx)
{
    vector<mhltile> wintile = {};
    int check1 = meld_check(tiles[0], tiles[1], tiles[2]); // 123meld
    if (check1 == 2 || check1 == 3)
    {
        int check2 = meld_check(tiles[3], tiles[4], tiles[5]); // 1-6meld
        if (check2 == 2 || check1 == 3)
        {
            int check3 = meld_check(tiles[6], tiles[7], tiles[8]); // 1-9meld
            if (check3 == 2 || check1 == 3)
            {
                int check4 = pair_check(tiles[9], tiles[10]); // 1-9meld 11
                if (check4 == 1)
                {
                    if (pair_check(tiles[11], tiles[12]) == 1) // 1-9meld 11 22
                    {
                        wintile.insert(wintile.begin(), tiles[9]);
                        wintile.insert(wintile.begin(), tiles[11]);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else if (pair_check(tiles[11], tiles[12]) == 2) // 1-9meld 11 23
                    {
                        if (tiles[10].value == tiles[11].value && tiles[10].suit == tiles[11].suit)
                        {
                            mhltile tile1 = tiles[10];
                            mhltile tile2 = tiles[12];
                            mhltile tile3 = tiles[12];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            if (tile3.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile3);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                        else
                        {
                            mhltile tile1 = tiles[11];
                            mhltile tile2 = tiles[12];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                    }
                    else if (pair_check(tiles[11], tiles[12]) == 3)
                    {
                        mhltile tile1 = tiles[11];
                        tile1.value += 1;
                        wintile.insert(wintile.begin(), tile1);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else
                    {
                        print("Hand not reach.. (1)");
                    }
                }
                else if (check4 == 2) // 1-9 meld 12
                {
                    if (pair_check(tiles[11], tiles[12]) == 1)
                    {
                        mhltile tile1 = tiles[9];
                        mhltile tile2 = tiles[10];
                        tile1.value -= 1;
                        tile2.value += 1;
                        if (tile1.value >= 1)
                        {
                            wintile.insert(wintile.begin(), tile1);
                        }
                        if (tile2.value <= 9)
                        {
                            wintile.insert(wintile.begin(), tile2);
                        }
                        tile_insert(gamedata, wintile, idx);
                    }
                }
                else
                {
                    print("Hand not reach.. (1)");
                }
            }
            else
            {
                int check5 = pair_check(tiles[8], tiles[9]); // 1-9meld 11
                if (check5 == 1)
                {
                    if (pair_check(tiles[10], tiles[11]) == 1) // 1-9meld 11 22
                    {
                        wintile.insert(wintile.begin(), tiles[9]);
                        wintile.insert(wintile.begin(), tiles[11]);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else if (pair_check(tiles[10], tiles[11]) == 2) // 1-9meld 11 23
                    {
                        if (tiles[10].value == tiles[11].value && tiles[10].suit == tiles[11].suit)
                        {
                            mhltile tile1 = tiles[10];
                            mhltile tile2 = tiles[11];
                            mhltile tile3 = tiles[11];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            if (tile3.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile3);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                        else
                        {
                            mhltile tile1 = tiles[11];
                            mhltile tile2 = tiles[12];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                    }
                    else if (pair_check(tiles[11], tiles[12]) == 3)
                    {
                        mhltile tile1 = tiles[11];
                        tile1.value += 1;
                        wintile.insert(wintile.begin(), tile1);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else
                    {
                        print("Hand not reach.. (2)");
                    }
                }
                else if (check5 == 2) // 1-9 meld 12
                {
                    if (pair_check(tiles[11], tiles[12]) == 1)
                    {
                        mhltile tile1 = tiles[9];
                        mhltile tile2 = tiles[10];
                        tile1.value -= 1;
                        tile2.value += 1;
                        if (tile1.value >= 1)
                        {
                            wintile.insert(wintile.begin(), tile1);
                        }
                        if (tile2.value <= 9)
                        {
                            wintile.insert(wintile.begin(), tile2);
                        }
                        tile_insert(gamedata, wintile, idx);
                    }
                }
                else
                {
                    print("Hand not reach.. (3)");
                }
            }
        }
        else
        {
            print("Hand not reach.. (1)");
        }
    }
    else if (check1 == 1)
    {
        int check2 = meld_check(tiles[3], tiles[4], tiles[5]); // 1-6meld
        if (check2 == 2 || check1 == 3)
        {
            int check3 = meld_check(tiles[6], tiles[7], tiles[8]); // 1-9meld
            if (check3 == 2 || check1 == 3)
            {
                int check4 = pair_check(tiles[9], tiles[10]); // 1-9meld 11
                if (check4 == 1)
                {
                    if (pair_check(tiles[11], tiles[12]) == 1) // 1-9meld 11 22
                    {
                        wintile.insert(wintile.begin(), tiles[9]);
                        wintile.insert(wintile.begin(), tiles[11]);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else if (pair_check(tiles[11], tiles[12]) == 2) // 1-9meld 11 23
                    {
                        if (tiles[10].value == tiles[11].value && tiles[10].suit == tiles[11].suit)
                        {
                            mhltile tile1 = tiles[10];
                            mhltile tile2 = tiles[12];
                            mhltile tile3 = tiles[12];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            if (tile3.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile3);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                        else
                        {
                            mhltile tile1 = tiles[11];
                            mhltile tile2 = tiles[12];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                    }
                    else if (pair_check(tiles[11], tiles[12]) == 3)
                    {
                        mhltile tile1 = tiles[11];
                        tile1.value += 1;
                        wintile.insert(wintile.begin(), tile1);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else
                    {
                        print("Hand not reach.. (1)");
                    }
                }
                else if (check4 == 2) // 1-9 meld 12
                {
                    if (pair_check(tiles[11], tiles[12]) == 1)
                    {
                        mhltile tile1 = tiles[9];
                        mhltile tile2 = tiles[10];
                        tile1.value -= 1;
                        tile2.value += 1;
                        if (tile1.value >= 1)
                        {
                            wintile.insert(wintile.begin(), tile1);
                        }
                        if (tile2.value <= 9)
                        {
                            wintile.insert(wintile.begin(), tile2);
                        }
                        tile_insert(gamedata, wintile, idx);
                    }
                }
                else
                {
                    print("Hand not reach.. (1)");
                }
            }
            else
            {
                int check5 = pair_check(tiles[8], tiles[9]); // 1-9meld 11
                if (check5 == 1)
                {
                    if (pair_check(tiles[10], tiles[11]) == 1) // 1-9meld 11 22
                    {
                        wintile.insert(wintile.begin(), tiles[9]);
                        wintile.insert(wintile.begin(), tiles[11]);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else if (pair_check(tiles[10], tiles[11]) == 2) // 1-9meld 11 23
                    {
                        if (tiles[10].value == tiles[11].value && tiles[10].suit == tiles[11].suit)
                        {
                            mhltile tile1 = tiles[10];
                            mhltile tile2 = tiles[11];
                            mhltile tile3 = tiles[11];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            if (tile3.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile3);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                        else
                        {
                            mhltile tile1 = tiles[11];
                            mhltile tile2 = tiles[12];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                    }
                    else if (pair_check(tiles[11], tiles[12]) == 3)
                    {
                        mhltile tile1 = tiles[11];
                        tile1.value += 1;
                        wintile.insert(wintile.begin(), tile1);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else
                    {
                        print("Hand not reach.. (2)");
                    }
                }
                else if (check5 == 2) // 1-9 meld 12
                {
                    if (pair_check(tiles[11], tiles[12]) == 1)
                    {
                        mhltile tile1 = tiles[9];
                        mhltile tile2 = tiles[10];
                        tile1.value -= 1;
                        tile2.value += 1;
                        if (tile1.value >= 1)
                        {
                            wintile.insert(wintile.begin(), tile1);
                        }
                        if (tile2.value <= 9)
                        {
                            wintile.insert(wintile.begin(), tile2);
                        }
                        tile_insert(gamedata, wintile, idx);
                    }
                }
                else
                {
                    print("Hand not reach.. (3)");
                }
            }
        }
    }
    else if (check1 == 4)
    {
        int check5 = pair_check(tiles[8], tiles[9]); // 1-9meld 11
        if (check5 == 1)
        {
            if (pair_check(tiles[10], tiles[11]) == 1) // 1-9meld 11 22
            {
                wintile.insert(wintile.begin(), tiles[9]);
                wintile.insert(wintile.begin(), tiles[11]);
                tile_insert(gamedata, wintile, idx);
            }
            else if (pair_check(tiles[10], tiles[11]) == 2) // 1-9meld 11 23
            {
                if (tiles[10].value == tiles[11].value && tiles[10].suit == tiles[11].suit)
                {
                    mhltile tile1 = tiles[10];
                    mhltile tile2 = tiles[11];
                    mhltile tile3 = tiles[11];
                    tile1.value -= 1;
                    tile2.value += 1;
                    if (tile1.value >= 1)
                    {
                        wintile.insert(wintile.begin(), tile1);
                    }
                    if (tile2.value <= 9)
                    {
                        wintile.insert(wintile.begin(), tile2);
                    }
                    if (tile3.value <= 9)
                    {
                        wintile.insert(wintile.begin(), tile3);
                    }
                    tile_insert(gamedata, wintile, idx);
                }
                else
                {
                    mhltile tile1 = tiles[11];
                    mhltile tile2 = tiles[12];
                    tile1.value -= 1;
                    tile2.value += 1;
                    if (tile1.value >= 1)
                    {
                        wintile.insert(wintile.begin(), tile1);
                    }
                    if (tile2.value <= 9)
                    {
                        wintile.insert(wintile.begin(), tile2);
                    }
                    tile_insert(gamedata, wintile, idx);
                }
            }
            else if (pair_check(tiles[11], tiles[12]) == 3)
            {
                mhltile tile1 = tiles[11];
                tile1.value += 1;
                wintile.insert(wintile.begin(), tile1);
                tile_insert(gamedata, wintile, idx);
            }
            else
            {
                print("Hand not reach.. (2)");
            }
        }
        else if (check5 == 2) // 1-9 meld 12
        {
            if (pair_check(tiles[11], tiles[12]) == 1)
            {
                mhltile tile1 = tiles[9];
                mhltile tile2 = tiles[10];
                tile1.value -= 1;
                tile2.value += 1;
                if (tile1.value >= 1)
                {
                    wintile.insert(wintile.begin(), tile1);
                }
                if (tile2.value <= 9)
                {
                    wintile.insert(wintile.begin(), tile2);
                }
                tile_insert(gamedata, wintile, idx);
            }
        }
        else
        {
            if (pair_check(tiles[10], tiles[11]) == 1) // 1-9meld 11 22
            {
                wintile.insert(wintile.begin(), tiles[9]);
                wintile.insert(wintile.begin(), tiles[11]);
                tile_insert(gamedata, wintile, idx);
            }
            else if (pair_check(tiles[10], tiles[11]) == 2) // 1-9meld 11 23
            {
                if (tiles[10].value == tiles[11].value && tiles[10].suit == tiles[11].suit)
                {
                    mhltile tile1 = tiles[10];
                    mhltile tile2 = tiles[11];
                    mhltile tile3 = tiles[11];
                    tile1.value -= 1;
                    tile2.value += 1;
                    if (tile1.value >= 1)
                    {
                        wintile.insert(wintile.begin(), tile1);
                    }
                    if (tile2.value <= 9)
                    {
                        wintile.insert(wintile.begin(), tile2);
                    }
                    if (tile3.value <= 9)
                    {
                        wintile.insert(wintile.begin(), tile3);
                    }
                    tile_insert(gamedata, wintile, idx);
                }
                else
                {
                    mhltile tile1 = tiles[11];
                    mhltile tile2 = tiles[12];
                    tile1.value -= 1;
                    tile2.value += 1;
                    if (tile1.value >= 1)
                    {
                        wintile.insert(wintile.begin(), tile1);
                    }
                    if (tile2.value <= 9)
                    {
                        wintile.insert(wintile.begin(), tile2);
                    }
                    tile_insert(gamedata, wintile, idx);
                }
            }
            print("Hand not reach.. (3)");
        }
    }
    else
    {
        int check7 = pair_check(tiles[0], tiles[1]);
        if (check7 == 1)
        {
            int check8 = meld_check(tiles[6], tiles[7], tiles[8]); // 1-9meld
            if (check8 == 2 || check7 == 3)
            {
                int check9 = pair_check(tiles[9], tiles[10]); // 1-9meld 11
                if (check9 == 1)
                {
                    if (pair_check(tiles[11], tiles[12]) == 1) // 1-9meld 11 22
                    {
                        wintile.insert(wintile.begin(), tiles[9]);
                        wintile.insert(wintile.begin(), tiles[11]);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else if (pair_check(tiles[11], tiles[12]) == 2) // 1-9meld 11 23
                    {
                        if (tiles[10].value == tiles[11].value && tiles[10].suit == tiles[11].suit)
                        {
                            mhltile tile1 = tiles[10];
                            mhltile tile2 = tiles[12];
                            mhltile tile3 = tiles[12];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            if (tile3.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile3);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                        else
                        {
                            mhltile tile1 = tiles[11];
                            mhltile tile2 = tiles[12];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                    }
                    else if (pair_check(tiles[11], tiles[12]) == 3)
                    {
                        mhltile tile1 = tiles[11];
                        tile1.value += 1;
                        wintile.insert(wintile.begin(), tile1);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else
                    {
                        print("Hand not reach.. (1)");
                    }
                }
                else if (check9 == 2) // 1-9 meld 12
                {
                    if (pair_check(tiles[11], tiles[12]) == 1)
                    {
                        mhltile tile1 = tiles[9];
                        mhltile tile2 = tiles[10];
                        tile1.value -= 1;
                        tile2.value += 1;
                        if (tile1.value >= 1)
                        {
                            wintile.insert(wintile.begin(), tile1);
                        }
                        if (tile2.value <= 9)
                        {
                            wintile.insert(wintile.begin(), tile2);
                        }
                        tile_insert(gamedata, wintile, idx);
                    }
                }
                else
                {
                    print("Hand not reach.. (1)");
                }
            }
            else
            {
                int check10 = pair_check(tiles[8], tiles[9]); // 1-9meld 11
                if (check10 == 1)
                {
                    if (pair_check(tiles[10], tiles[11]) == 1) // 1-9meld 11 22
                    {
                        wintile.insert(wintile.begin(), tiles[9]);
                        wintile.insert(wintile.begin(), tiles[11]);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else if (pair_check(tiles[10], tiles[11]) == 2) // 1-9meld 11 23
                    {
                        if (tiles[10].value == tiles[11].value && tiles[10].suit == tiles[11].suit)
                        {
                            mhltile tile1 = tiles[10];
                            mhltile tile2 = tiles[11];
                            mhltile tile3 = tiles[11];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            if (tile3.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile3);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                        else
                        {
                            mhltile tile1 = tiles[11];
                            mhltile tile2 = tiles[12];
                            tile1.value -= 1;
                            tile2.value += 1;
                            if (tile1.value >= 1)
                            {
                                wintile.insert(wintile.begin(), tile1);
                            }
                            if (tile2.value <= 9)
                            {
                                wintile.insert(wintile.begin(), tile2);
                            }
                            tile_insert(gamedata, wintile, idx);
                        }
                    }
                    else if (pair_check(tiles[11], tiles[12]) == 3)
                    {
                        mhltile tile1 = tiles[11];
                        tile1.value += 1;
                        wintile.insert(wintile.begin(), tile1);
                        tile_insert(gamedata, wintile, idx);
                    }
                    else
                    {
                        print("Hand not reach.. (2)");
                    }
                }
                else if (check10 == 2) // 1-9 meld 12
                {
                    if (pair_check(tiles[11], tiles[12]) == 1)
                    {
                        mhltile tile1 = tiles[9];
                        mhltile tile2 = tiles[10];
                        tile1.value -= 1;
                        tile2.value += 1;
                        if (tile1.value >= 1)
                        {
                            wintile.insert(wintile.begin(), tile1);
                        }
                        if (tile2.value <= 9)
                        {
                            wintile.insert(wintile.begin(), tile2);
                        }
                        tile_insert(gamedata, wintile, idx);
                    }
                }
                else
                {
                    print("Hand not reach.. (3)");
                }
            }
        }
        print("Hand not reach.. (1)");
    }
    // gamedata.riichi_status = 1;
    // gamedata.wintiles.insert(gamedata.wintiles.end(), pair<uint8_t, vector<mhltile>>(idx, temptiles));
}