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
    depositbet(from, quantity);
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

double mahjonghilo::roundoff(double var)
{
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67
    float value = (int)(var * 10 + .5);
    return (float)value / 10;
}

void mahjonghilo::depositbet(name username, asset quantity)
{
    require_auth(username);
    action(
        permission_level{_self, "active"_n},
        _self,
        "acceptbet"_n,
        std::make_tuple(username, quantity))
        .send();
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
    uint8_t deck_tile_idx = rng(gamedata.deck_player.size()); // Pick a random tile from the deck
    // uint8_t deck_tile_idx = 133; // Pick a random tile from the deck

    gamedata.hand_player.insert(gamedata.hand_player.begin(), gamedata.deck_player[deck_tile_idx]); // Assign the tile to the first empty slot in the hand
    gamedata.current_tile = gamedata.deck_player[deck_tile_idx];
    gamedata.deck_player.erase(gamedata.deck_player.begin() + deck_tile_idx); // Remove the tile from the deck
    sorthand(gamedata.hand_player);
    tile num = table_deck.at(gamedata.current_tile);
    gamedata.sumofvalue[num.value - 1] -= 1;
    gamedata.draw_count += 1;
    gamedata.drawn_tiles.insert(gamedata.drawn_tiles.end(), gamedata.current_tile);
    // return gamedata.current_tile;
}

void mahjonghilo::get_odds(game &gamedata, int value)
{
    // gamedata.sumofvalue[value - 1] -= 1;
    double sum, sum1, sum2, num1, num2, num3;
    for (int i = 0; i < gamedata.sumofvalue.size(); i++)
    {
        sum += gamedata.sumofvalue[i];
    }
    num2 = (sum / gamedata.sumofvalue[value - 1]) * 0.9;
    if (value == 1)
    {
        num1 = 0;
        for (int i = value; i < gamedata.sumofvalue.size(); i++)
        {
            sum2 += gamedata.sumofvalue[i];
        }
        num3 = (sum / sum2) * 0.9;
        // for (int i = value; i < gamedata.sumofvalue.size(); i++)
        // {
        //     sum2 += gamedata.sumofvalue[i];
        // }
    }
    else if (value == 11)
    {
        num3 = 0;
        for (int i = 0; i < gamedata.sumofvalue.size(); i++)
        {
            sum1 += gamedata.sumofvalue[i];
        }
        num1 = (sum / sum1) * 0.9;
    }
    else
    {
        for (int i = 0; i < value - 1; i++)
        {
            sum1 += gamedata.sumofvalue[i];
        }
        num1 = (sum / sum1) * 0.9;
        sum2 = sum - sum1 - gamedata.sumofvalue[value - 1];
        // for (int j = value; j < 11; j++)
        // {
        //     sum2 += gamedata.sumofvalue[j];
        // }
        // print(sum);
        // print("hahahaha");
        // print(sum2);
        num3 = (sum / sum2) * 0.9;
    }
    if (num1 > 1.0000)
    {
        gamedata.low_odds = num1;
    }
    else
    {
        if (value != 1)
        {
            gamedata.low_odds = 1.0000;
        }
        else
        {
            gamedata.low_odds = 0.0000;
        }
    }
    if (num2 > 1.0000)
    {
        gamedata.draw_odds = num2;
    }
    else
    {
        gamedata.draw_odds = 1.0000;
    }

    if (num3 > 1.0000)
    {

        gamedata.high_odds = num3;
    }
    else
    {
        if (value != 11)
        {
            gamedata.high_odds = 1.0000;
        }
        else
        {
            gamedata.high_odds = 0.0000;
        }
    }
}

float mahjonghilo::hilo_step(game gamedata, int prev_tile, int current_tile, int option)
{
    if (prev_tile > current_tile && option == 1)
    {
        return gamedata.low_odds;
    }
    else if (prev_tile == current_tile && option == 2)
    {
        return gamedata.draw_odds;
    }
    else if (prev_tile < current_tile && option == 3)
    {
        return gamedata.high_odds;
    }
    else
    {
        return 0;
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

void mahjonghilo::sorteye(vector<uint8_t> &hand, int idx)
{
    int temp = hand[idx];
    hand.erase(hand.begin() + idx);
    hand.erase(hand.begin() + idx);
    hand.insert(hand.end(), temp);
    hand.insert(hand.end(), temp += 1);
}

void mahjonghilo::sumscore(game &gamedata)
{
    int num = 0;
    sorthand(gamedata.score_check);
    while (gamedata.score_check[0] == 0)
    {
        gamedata.score_check.erase(gamedata.score_check.begin());
    }
    for (int i = 0; i < gamedata.score_check.size(); i++)
    {
        const auto tempscore = score_deck.at(gamedata.score_check[i]);
        if (tempscore.score_name != "No Score")
        {
            gamedata.score_type.insert(gamedata.score_type.begin(), tempscore);
        }
        num += tempscore.value;
    }
    gamedata.final_score = num;
}

// void mahjonghilo::four_pungs(game &gamedata, vector<tile> tiles)
// {
//     for (int i = 0; i < tiles.size(); i++)
//     {
//         print(tiles[i].suit);
//     }
// }

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
    else if (remtiles.size() == 11)
    {
        eleven_rem(gamedata, remtiles);
    }
    else if (remtiles.size() == 14)
    {
        fourteen_rem(gamedata, remtiles);
    }
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
}

void mahjonghilo::pung_chow(game &gamedata, int check)
{
    if (check == 2)
    {
        gamedata.pung_count += 1;
    }
    else if (check == 3)
    {
        gamedata.chow_count += 1;
    }
    else if (check == 4)
    {
        gamedata.pung_count += 2;
    }
    else if (check == 5 || check == 6)
    {
        gamedata.chow_count += 1;
        gamedata.pung_count += 1;
    }
    else if (check == 7 || check == 8)
    {
        gamedata.chow_count += 2;
    }
    else
    {
        print("Pung or chow check is invalid.");
    }
}

int mahjonghilo::pair_pung_chow(tile tile1, tile tile2, tile tile3)
{
    if (tile1.suit == tile2.suit && tile1.value == tile2.value)
    {
        // if (tile2.suit == tile3.suit && tile2.value == tile3.value)
        // {
        //     return 2;
        // }
        // else
        // {
        // if (tile2.value < 10 && tile3.value < 10)
        // {
        if (tile2.value == tile3.value && tile2.suit == tile3.suit)
        {
            return 2;
        }
        else
        {
            return 1;
        }
        // }
        // else
        // {
        //     if (tile2.value == tile3.value)
        //     {
        //         if (tile2.suit == tile3.suit)
        //         {
        //             return 2;
        //         }
        //         else
        //         {
        //             return 1;
        //         }
        //     }
        //     else
        //     {
        //         return 1;
        //     }
        // }
        // }
    }
    else if (tile1.suit == tile2.suit && tile2.suit == tile3.suit)
    {
        if ((tile1.value + 1) == tile2.value && (tile1.value + 2) == tile3.value)
        {
            return 3;
        }
        else if ((tile1.value + 3) == tile2.value && (tile2.value + 3) == tile3.value)
        {
            return 4;
        }
        else
        {
            return 5;
        }
    }
    else
        return 0;
}

int mahjonghilo::pair_check(tile tile1, tile tile2) // !!!
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
        if (tile1.value == tile2.value)
        {
            return 1;
        }
        else if ((tile1.value + 1) == tile2.value)
        {
            return 2;
        }
        else if (tile1.value == 1 && tile2.value == 9)
        {
            return 3;
        }
        else if ((tile1.value + 6) == tile2.value)
        {
            return 4;
        }
        else
        {
            return 5;
        }
    }
    else
    {
        return 0;
    }
}

int mahjonghilo::honors_check(tile tile1, tile tile2, tile tile3, tile tile4, tile tile5, tile tile6, tile tile7) // !!!
{
    if (tile1.value == 10 && tile1.suit == 4)
    {
        if (tile2.value == 10 && tile2.suit == 5)
        {
            if (tile3.value == 10 && tile3.suit == 6)
            {
                if (tile4.value == 10 && tile4.suit == 7)
                {
                    if (tile5.value == 11 && tile5.suit == 8)
                    {
                        if (tile6.value == 11 && tile6.suit == 9)
                        {
                            if (tile7.value == 11 && tile7.suit == 10)
                            {
                                return 1;
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
    else if (tile1.value < 10)
    {
        if (tile2.value >= 10 && tile2.suit < tile3.suit) // 9 10[4]
        {
            if (tile3.suit < tile4.suit && tile4.suit < tile5.suit) // 10[5] 10[6]
            {
                if (tile5.suit < tile6.suit && tile6.suit < tile7.suit)
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
        else if (tile2.value < 10 && tile3.value >= 10)
        {
            if (tile3.suit < tile4.suit && tile4.suit < tile5.suit) // 10[5] 10[6]
            {
                if (tile5.suit < tile6.suit && tile6.suit < tile7.suit)
                {
                    return 3;
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

int mahjonghilo::five_tile_check(tile tile1, tile tile2, tile tile3, tile tile4, tile tile5) // !!!!!
{
    if ((pair_pung_chow(tile2, tile3, tile4)) == 2)
    {
        if ((pair_pung_chow(tile1, tile3, tile5)) == 3)
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

int mahjonghilo::six_tile_check(tile tile1, tile tile2, tile tile3, tile tile4, tile tile5, tile tile6) // !!!!!!!
{
    if ((pair_pung_chow(tile1, tile2, tile3)) == 2)
    {
        if ((pair_pung_chow(tile4, tile5, tile6)) == 2)
        {
            return 4;
        }
        else if ((pair_pung_chow(tile4, tile5, tile6)) == 3)
        {
            return 5;
        }
        else
        {
            return 0;
        }
    }
    else if ((pair_pung_chow(tile1, tile2, tile3)) == 3)
    {
        if ((pair_pung_chow(tile4, tile5, tile6)) == 2)
        {
            return 6;
        }
        else if ((pair_pung_chow(tile4, tile5, tile6)) == 3)
        {
            return 7;
        }
        else
        {
            return 0;
        }
    }
    else if ((pair_pung_chow(tile1, tile2, tile3)) == 1)
    {
        if ((pair_check(tile3, tile4)) == 1 && (pair_check(tile5, tile6)) == 1)
        {
            if ((pair_pung_chow(tile1, tile3, tile5)) == 3)
            {
                return 8;
            }
            else if (pair_check(tile3, tile5) == 2)
            {
                return 9;
            }
            else
            {
                return 10;
            }
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

        gamedata.pair_count += 1;
        gamedata.winnable = 1; // transferhand(gamedata, 2);
        // print(tiles[0].value);
        // print("Congratulations, you won!");
        // print(tiles[1].value);
    }
    else
    {
        print("1 - Hand combination not correct!");
    }
}

void mahjonghilo::five_rem(game &gamedata, vector<tile> tiles) // AAAKK
{
    int check1 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
    if (check1 == 2)
    {
        int check2 = pair_check(tiles[3], tiles[4]);
        if (check2 == 1)
        {
            gamedata.pair_count += 1;
            gamedata.eye_idx = 3;
            pung_chow(gamedata, check1);
            gamedata.winnable = 1; // transferhand(gamedata, 5);
        }
        else if (check2 == 2 && pair_pung_chow(tiles[2], tiles[3], tiles[4]) == 3)
        {
            gamedata.chow_count += 1;
            gamedata.pair_count += 1;
            gamedata.eye_idx = 0;
            gamedata.winnable = 1; // transferhand(gamedata, 5);
        }
        else
        {
            print("4 - Hand combination not correct!");
        }
    }
    else if (check1 == 3)
    {
        if ((pair_check(tiles[3], tiles[4])) == 1)
        {
            gamedata.chow_count += 1;
            gamedata.pair_count += 1;
            gamedata.eye_idx = 3;
            gamedata.winnable = 1; // transferhand(gamedata, 5);
        }
        else
        {
            print("3 - Hand combination not correct!");
        }
    }
    else if (check1 == 1) //
    {
        int check2 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
        if (check2 == 2 || check2 == 3)
        {
            gamedata.pair_count += 1;
            gamedata.eye_idx = 0;
            pung_chow(gamedata, check2);
            gamedata.winnable = 1; // transferhand(gamedata, 5);
        }
        else
        {
            print("2 - Hand combination not correct!");
        }
    }
    else // A33334
    {
        if ((five_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4])) == 1)
        {
            gamedata.chow_count += 1;
            gamedata.pair_count += 1;
            gamedata.eye_idx = 3;
            gamedata.winnable = 1; // transferhand(gamedata, 5);
        }
        else
        {
            print("5 - Hand combination not correct!");
        }
    }
}

void mahjonghilo::eight_rem(game &gamedata, vector<tile> tiles)
{
    int check1 = six_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4], tiles[5]);
    if (check1 > 3 && check1 < 9)
    {
        int check2 = pair_check(tiles[6], tiles[7]);
        if (check2 == 1)
        {
            gamedata.pair_count += 1;
            gamedata.eye_idx = 6;
            pung_chow(gamedata, check1);
            gamedata.winnable = 1; // transferhand(gamedata, 8);
        }
        else if (check2 == 2 && pair_pung_chow(tiles[3], tiles[4], tiles[5]) == 2)
        {
            if (pair_pung_chow(tiles[5], tiles[6], tiles[7]) == 3)
            {
                gamedata.pair_count += 1;
                gamedata.chow_count += 1;
                pung_chow(gamedata, check1);
                gamedata.eye_idx = 3;
                gamedata.pung_count -= 1;
                gamedata.winnable = 1; // transferhand(gamedata, 8);
            }
            else
            {
                print("6 - Hand combination not correct!");
            }
        }
        else
        {
            print("7 - Hand combination not correct!");
        }
    }
    else if (check1 == 9 && pair_check(tiles[6], tiles[7]) == 1)
    {
        if (pair_pung_chow(tiles[2], tiles[4], tiles[6]) == 3)
        {
            gamedata.pair_count += 1;
            gamedata.eye_idx = 0;
            gamedata.chow_count += 2;
            gamedata.winnable = 1; // transferhand(gamedata, 8);
        }
        else
        {
            print("8 - Hand combination not correct!");
        }
    }
    else
    {
        int check2 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
        if (check2 == 2 || check2 == 3)
        {
            int check3 = five_tile_check(tiles[3], tiles[4], tiles[5], tiles[6], tiles[7]);
            if (check3 == 1)
            {
                gamedata.pair_count += 1;
                gamedata.eye_idx = 5;
                gamedata.chow_count += 1;
                pung_chow(gamedata, check2);
                gamedata.winnable = 1; // transferhand(gamedata, 8);
            }
            else if (check3 == 2 && check2 == 2) // 11112233
            {
                if (tiles[2].value == tiles[3].value)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 0;
                    gamedata.chow_count += 2;
                    gamedata.winnable = 1; // transferhand(gamedata, 8);
                }
                else
                {
                    print("15 - Hand combination not correct!");
                }
            }
            else if (check3 == 3 && check2 == 2)
            {
                if (pair_pung_chow(tiles[2], tiles[3], tiles[7]) == 3)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 0;
                    gamedata.chow_count += 1;
                    gamedata.pung_count += 1;
                    gamedata.winnable = 1; // transferhand(gamedata, 8);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                }
                else
                {
                    print("9 - Hand combination not correct!");
                }
            }
        }
        else if (check2 == 1)
        {
            int check3 = six_tile_check(tiles[2], tiles[3], tiles[4], tiles[5], tiles[6], tiles[7]);
            if (check3 > 3 && check3 < 9)
            {
                gamedata.pair_count += 1;
                gamedata.eye_idx = 0;
                pung_chow(gamedata, check3);
                gamedata.winnable = 1; // transferhand(gamedata, 14);
            }
            else if (check3 == 10 && pair_pung_chow(tiles[0], tiles[2], tiles[4]) == 3)
            {
                gamedata.pair_count += 1;
                gamedata.eye_idx = 6;
                gamedata.chow_count += 2;
                gamedata.winnable = 1; // transferhand(gamedata, 8);
            }
            else
            {
                print("10 - Hand combination not correct!");
            }
        }
        else
        {
            int check3 = five_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4]);
            if (check3 == 1)
            {
                int check4 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
                if (check4 == 2 || check4 == 3)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 2;
                    gamedata.chow_count += 1;
                    pung_chow(gamedata, check4);
                    gamedata.winnable = 1; // transferhand(gamedata, 8);
                }
                else
                {
                    print("11 - Hand combination not correct!");
                }
            }
            else if (check3 == 2 && pair_pung_chow(tiles[2], tiles[4], tiles[5]) == 3)
            {
                int check10 = pair_check(tiles[6], tiles[7]);
                if (check10 == 1)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 6;
                    gamedata.chow_count += 2;
                    gamedata.winnable = 1; // transferhand(gamedata, 8);
                }
                else
                {
                    print("12 - Hand combination not correct!");
                }
            }
            else if (check3 == 3)
            {
                if (pair_pung_chow(tiles[0], tiles[4], tiles[5]) == 3) // 12222334
                {
                    int check10 = pair_check(tiles[6], tiles[7]);
                    if (check10 == 1)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 6;
                        gamedata.chow_count += 1;
                        gamedata.pung_count += 1;
                        gamedata.winnable = 1; // transferhand(gamedata, 8);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                    }
                    else if (check10 == 2 && pair_pung_chow(tiles[3], tiles[6], tiles[7]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 1;
                        gamedata.chow_count += 2;
                        gamedata.winnable = 1; // transferhand(gamedata, 8);
                    }
                    else
                    {
                        print("13 - Hand combination not correct!");
                    }
                }
                else
                {
                    print("14 - Hand combination not correct!");
                }
            }
            else
            {
                print("15 - Hand combination not correct!");
            }
        }
    }
}

void mahjonghilo::eleven_rem(game &gamedata, vector<tile> tiles) // 11Rem2nd
{
    int check1 = six_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4], tiles[5]);
    if (check1 > 3 && check1 < 9)
    {
        int check2 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
        int check10 = pair_check(tiles[9], tiles[10]);
        if (check2 == 3 && check10 == 1)
        {
            gamedata.pair_count += 1;
            gamedata.eye_idx = 9;
            gamedata.chow_count += 1;
            pung_chow(gamedata, check1);
            gamedata.winnable = 1; // transferhand(gamedata, 11);
        }
        else if (check2 == 2)
        {
            if (check10 == 1)
            {
                gamedata.pair_count += 1;
                gamedata.pung_count += 1;
                gamedata.eye_idx = 9;
                pung_chow(gamedata, check1);
                gamedata.winnable = 1; // transferhand(gamedata, 11);
            }
            else if (check10 == 2 && pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
            {
                gamedata.pair_count += 1;
                gamedata.eye_idx = 6;
                gamedata.chow_count += 1;
                pung_chow(gamedata, check1);
                gamedata.winnable = 1; // transferhand(gamedata, 11);
            }
            else
            {
                print("16 - Hand combination not correct!");
            }
        }
        else if (check2 == 1)
        {
            if (pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 2)
            {
                gamedata.pair_count += 1;
                gamedata.eye_idx = 6;
                gamedata.pung_count += 1;
                pung_chow(gamedata, check1);
                gamedata.winnable = 1; // transferhand(gamedata, 11);
            }
            else if (pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
            {
                gamedata.pair_count += 1;
                gamedata.eye_idx = 6;
                gamedata.chow_count += 1;
                pung_chow(gamedata, check1);
                gamedata.winnable = 1; // transferhand(gamedata, 11);
            }
            else
            {
                print("17 - Hand combination not correct!");
            }
        }
        else
        {
            int check3 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
            if (check3 == 1)
            {
                gamedata.pair_count += 1;
                gamedata.eye_idx = 8;
                gamedata.chow_count += 1;
                pung_chow(gamedata, check1);
                gamedata.winnable = 1; // transferhand(gamedata, 11);
            }
            else if (check3 == 3 && pair_pung_chow(tiles[3], tiles[4], tiles[5]) == 2) //111222 33334/34444
            {
                if (pair_pung_chow(tiles[5], tiles[6], tiles[10]) == 3)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 3;
                    gamedata.chow_count += 1;
                    pung_chow(gamedata, check1);
                    gamedata.winnable = 1; // transferhand(gamedata, 11);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                }
                else
                {
                    print("18 - Hand combination not correct!");
                }
            }
            else
            {
                print("19 - Hand combination not correct!");
            }
        }
    }
    else if (check1 == 9 && pair_check(tiles[6], tiles[7]) == 1)
    {
        if (pair_pung_chow(tiles[2], tiles[4], tiles[6]) == 3)
        {
            int check2 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
            if (check2 == 2 || check2 == 3)
            {
                gamedata.pair_count += 1;
                gamedata.eye_idx = 0;
                gamedata.chow_count += 2;
                pung_chow(gamedata, check2);
                gamedata.winnable = 1; // transferhand(gamedata, 11);
            }
            else
            {
                print("20 - Hand combination not correct!");
            }
        }
        else
        {
            print("21 - Hand combination not correct!");
        }
    }
    else
    {
        int check2 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
        if (check2 == 2 || check2 == 3)
        {
            int check3 = five_tile_check(tiles[3], tiles[4], tiles[5], tiles[6], tiles[7]);
            if (check3 == 1)
            {
                int check4 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
                if (check4 == 2 || check4 == 3)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 5;
                    gamedata.chow_count += 1;
                    pung_chow(gamedata, check2);
                    pung_chow(gamedata, check4);
                    gamedata.winnable = 1; // transferhand(gamedata, 11);
                }
                else
                {
                    print("22 - Hand combination not correct!");
                }
            }
            else if (check3 == 2 && pair_pung_chow(tiles[5], tiles[7], tiles[8]) == 3) // 111 233445
            {
                int check4 = pair_check(tiles[9], tiles[10]);
                if (check4)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 9;
                    gamedata.chow_count += 2;
                    pung_chow(gamedata, check2);
                    gamedata.winnable = 1; // transferhand(gamedata, 11);
                }
                else if (check4 == 2 && check2 == 2)
                {
                    if (pair_check(tiles[2], tiles[3]) == 1 && pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 0;
                        gamedata.chow_count += 3;
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else
                    {
                        print("23 - Hand combination not correct!");
                    }
                }
                else
                {
                    print("23.5 - Hand combination not correct!");
                }
            }
            else if (check3 == 3 && pair_pung_chow(tiles[3], tiles[7], tiles[8]) == 3) // 333 45555 6
            {
                int check4 = pair_check(tiles[9], tiles[10]);
                if (check4 == 1)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 9;
                    gamedata.chow_count += 1;
                    gamedata.pung_count += 1;
                    pung_chow(gamedata, check2);
                    gamedata.winnable = 1; // transferhand(gamedata, 11);
                    gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                }
                else if (check4 == 2 && pair_pung_chow(tiles[6], tiles[9], tiles[10]) == 3)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 4;
                    gamedata.chow_count += 2;
                    pung_chow(gamedata, check2);
                    gamedata.winnable = 1; // transferhand(gamedata, 11);
                }
                else if (check2 == 2 && pair_pung_chow(tiles[2], tiles[3], tiles[7]) == 3) // 222
                {
                    int check5 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
                    if (check5 == 2 || check5 == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 0;
                        gamedata.chow_count += 2;
                        pung_chow(gamedata, check5);
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else
                    {
                        print("24 - Hand combination not correct!");
                    }
                }
                else
                {
                    print("25 - Hand combination not correct!");
                }
            }
            else
            {
                print("25.5 - Hand combination not correct!");
            }
        }
        else if (check2 == 1)
        {
            int check3 = six_tile_check(tiles[2], tiles[3], tiles[4], tiles[5], tiles[6], tiles[7]);
            if (check3 > 3 && check3 < 9)
            {
                int check4 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
                if (check4 == 2 || check4 == 3)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 0;
                    pung_chow(gamedata, check3);
                    pung_chow(gamedata, check4);
                    gamedata.winnable = 1; // transferhand(gamedata, 11);
                }
                else
                {
                    print("26 - Hand combination not correct!");
                }
            }
            else
            {
                int check4 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
                if (check4 == 2 || check4 == 3)
                {
                    int check5 = five_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9]);
                    if (check5 == 2 && pair_pung_chow(tiles[7], tiles[9], tiles[10]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 0;
                        gamedata.chow_count += 2;
                        pung_chow(gamedata, check4);
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else if (check5 == 3 && pair_pung_chow(tiles[5], tiles[9], tiles[10]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 0;
                        gamedata.chow_count += 1;
                        gamedata.pung_count += 1;
                        pung_chow(gamedata, check4);
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                    }
                    else
                    {
                        print("27 - Hand combination not correct!");
                    }
                }
                else
                {
                    int check5 = five_tile_check(tiles[2], tiles[3], tiles[4], tiles[5], tiles[6]); // 11 23334
                    if (check5 == 1)
                    {
                        if (pair_check(tiles[7], tiles[8]) == 1 && pair_check(tiles[9], tiles[10]) == 1)
                        {
                            if (pair_pung_chow(tiles[5], tiles[7], tiles[9]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 0;
                                gamedata.chow_count += 3;
                                gamedata.winnable = 1; // transferhand(gamedata, 11);
                            }
                            else if (pair_pung_chow(tiles[1], tiles[5], tiles[7]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 9;
                                gamedata.chow_count += 3;
                                gamedata.winnable = 1; // transferhand(gamedata, 11);
                            }

                            else
                            {
                                print("28 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            print("29 - Hand combination not correct!");
                        }
                    }
                    else if (check5 == 2 && pair_pung_chow(tiles[4], tiles[6], tiles[7]) == 3)
                    {
                        int check6 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
                        if (check6 == 2 || check6 == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 0;
                            gamedata.chow_count += 2;
                            pung_chow(gamedata, check6);
                            gamedata.winnable = 1; // transferhand(gamedata, 11);
                        }
                        else
                        {
                            print("30 - Hand combination not correct!");
                        }
                    }
                    else if (check5 == 3) // 11 22223 4 567
                    {
                        int check6 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
                        if (pair_pung_chow(tiles[2], tiles[6], tiles[7]) == 3)
                        {
                            if (check6 == 2 || check6 == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 0;
                                gamedata.chow_count += 1;
                                gamedata.pung_count += 1;
                                pung_chow(gamedata, check6);
                                gamedata.winnable = 1; // transferhand(gamedata, 11);
                                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                            }
                            else
                            {
                                print("31 - Hand combination not correct!");
                            }
                        }
                        else if (pair_check(tiles[6], tiles[7]) == 1 && pair_pung_chow(tiles[1], tiles[3], tiles[7]) == 3)
                        {
                            if (check6 == 2 || check6 == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 4;
                                gamedata.chow_count += 1;
                                gamedata.pung_count += 1;
                                pung_chow(gamedata, check6);
                                gamedata.winnable = 1; // transferhand(gamedata, 11);
                            }
                            else
                            {
                                print("32 - Hand combination not correct!");
                            }
                        }
                    }
                    else
                    {
                        print("33 - Hand combination not correct!");
                    }
                }
            }
        }
        else
        {
            int check3 = five_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4]);
            if (check3 == 1)
            {
                int check4 = six_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
                if (check4 > 3 && check4 < 9)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 3;
                    gamedata.chow_count += 1;
                    pung_chow(gamedata, check4);
                    gamedata.winnable = 1; // transferhand(gamedata, 11);
                }
                else if (check4 == 10) //  12223 3344 66
                {
                    if (pair_pung_chow(tiles[3], tiles[5], tiles[7]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 9;
                        gamedata.chow_count += 3;
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else
                    {
                        print("34 - Hand combination not correct!");
                    }
                }
                else
                {
                    int check5 = five_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9]);
                    if (check5 == 2 && pair_pung_chow(tiles[7], tiles[9], tiles[10]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 3;
                        gamedata.chow_count += 3;
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else if (check5 == 3 && pair_pung_chow(tiles[0], tiles[4], tiles[5]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 3;
                        gamedata.chow_count += 2;
                        gamedata.pung_count += 1;
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else
                    {
                        print("35 - Hand combination not correct!");
                    }
                }
            }
            else if (check3 == 2 && pair_pung_chow(tiles[2], tiles[4], tiles[5]) == 3)
            {
                int check4 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
                int check10 = pair_check(tiles[9], tiles[10]);
                if (check4 == 3 && check10 == 1)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 9;
                    gamedata.chow_count += 3;
                    gamedata.winnable = 1; // transferhand(gamedata, 11);
                }
                else if (check4 == 2)
                {
                    if (check10 == 1)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 9;
                        gamedata.chow_count += 2;
                        gamedata.pung_count += 1;
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else if (check10 == 2 && pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 6;
                        gamedata.chow_count += 3;
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else
                    {
                        print("36 - Hand combination not correct!");
                    }
                }
                else if (check4 == 1)
                {
                    int check5 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
                    if (check5 == 2 || check5 == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 6;
                        gamedata.chow_count += 2;
                        pung_chow(gamedata, check5);
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else
                    {
                        print("37 - Hand combination not correct!");
                    }
                }
                else
                {
                    int check5 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
                    if (check5 == 1)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 8;
                        gamedata.chow_count += 3;
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else
                    {
                        print("38 - Hand combination not correct!");
                    }
                }
            }
            else if (check3 == 3 && pair_pung_chow(tiles[0], tiles[4], tiles[5]) == 3)
            {
                int check4 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
                int check10 = pair_check(tiles[9], tiles[10]);
                if (check4 == 3 && check10 == 1)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 9;
                    gamedata.chow_count += 3;
                    gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                    gamedata.winnable = 1; // transferhand(gamedata, 11);
                }
                else if (check4 == 2)
                {
                    if (check10 == 1)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 9;
                        gamedata.chow_count += 2;
                        gamedata.pung_count += 1;
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                    }
                    else if (check10 == 2 && pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 6;
                        gamedata.chow_count += 3;
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                    }
                    else
                    {
                        print("39 - Hand combination not correct!");
                    }
                }
                else if (check4 == 1) // 1[0] 2[1] 2[2] 2[3] 2[4] 3[5] 3[6] 3[7] 4[8] 4[9] 5[10]
                {
                    int check5 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
                    if (check5 == 2 || check5 == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 6;
                        gamedata.chow_count += 2;
                        pung_chow(gamedata, check5);
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else if (check5 == 1 && pair_pung_chow(tiles[3], tiles[7], tiles[9]) == 3)
                    {
                        if (check10 == 2 && pair_pung_chow(tiles[6], tiles[8], tiles[10]) == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 2;
                            gamedata.chow_count += 3;
                            gamedata.winnable = 1; // transferhand(gamedata, 11);
                        }
                        else
                        {
                            print("40 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        print("41 - Hand combination not correct!");
                    }
                }
                else // 12222 3 34 666 / 789
                {
                    int check5 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
                    if (check5 == 1)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 8;
                        gamedata.chow_count += 3;
                        gamedata.winnable = 1; // transferhand(gamedata, 11);
                    }
                    else
                    {
                        int check6 = pair_check(tiles[6], tiles[7]);
                        if (check6 == 2 && pair_pung_chow(tiles[3], tiles[6], tiles[7]) == 3)
                        {
                            int check7 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
                            if (check7 == 2 || check7 == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 1;
                                gamedata.chow_count += 2;
                                pung_chow(gamedata, check7);
                                gamedata.winnable = 1; // transferhand(gamedata, 11);
                            }
                            else
                            {
                                print("42 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            print("43 - Hand combination not correct!");
                        }
                    }
                }
            }
            else
            {
                print("44 - Hand combination not correct!");
            }
        }
    }
}

void mahjonghilo::fourteen_rem(game &gamedata, vector<tile> tiles) // 14Rem
{
    int check1 = six_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4], tiles[5]);
    if (check1 == 9 || check1 == 10)
    {
        int check2 = six_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10], tiles[11]);
        if (check2 > 7 && (pair_check(tiles[12], tiles[13])) == 1)
        {
            gamedata.pair_count += 7;
            gamedata.winnable = 1; // transferhand(gamedata, 14);
        }
        else
        {
            if ((pair_check(tiles[6], tiles[7])) == 1 && (pair_pung_chow(tiles[2], tiles[4], tiles[6])) == 3)
            {
                int check3 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                if (check3 > 3 && check3 < 9)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 0;
                    gamedata.chow_count += 2;
                    pung_chow(gamedata, check3);
                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                }
                else
                {
                    int check4 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                    if (check4 == 3 && (pair_pung_chow(tiles[8], tiles[12], tiles[13])) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 0;
                        gamedata.chow_count += 3;
                        gamedata.pung_count += 1;
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                    }
                    else if (check4 == 2 && (pair_pung_chow(tiles[10], tiles[12], tiles[13])) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 0;
                        gamedata.chow_count += 4;
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("56 - Hand combination not correct!");
                    }
                }
            }
            else
            {
                print("57 - Hand combination not correct!");
            }
        }
    }
    else if (check1 == 8)
    {
        int check2 = six_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10], tiles[11]);
        if (check2 > 3 && check2 < 8)
        {
            if ((pair_check(tiles[12], tiles[13])) == 1)
            {
                gamedata.pair_count += 1;
                gamedata.eye_idx = 12;
                gamedata.chow_count += 2;
                pung_chow(gamedata, check2);
                gamedata.winnable = 1; // transferhand(gamedata, 11);
            }
            else if (pair_check(tiles[12], tiles[13]) == 2 && (pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3))
            {
                if (check2 == 4 || check2 == 6)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 9;
                    gamedata.pung_count -= 1;
                    gamedata.chow_count += 3;
                    pung_chow(gamedata, check2);
                    gamedata.winnable = 1; // transferhand(gamedata, 11);
                }
                else
                {
                    print("58 - Hand combination not correct!");
                }
            }
            else
            {
                print("59 - Hand combination not correct!");
            }
        }
        else if (check2 > 7 && check2 < 11)
        {
            if ((pair_check(tiles[12], tiles[13])) == 1)
            {
                gamedata.pair_count += 7;
                gamedata.winnable = 1; // transferhand(gamedata, 14);
            }
            else
            {
                print("60 - Hand combination not correct!");
            }
        }
        else
        {
            int check3 = pair_pung_chow(tiles[6], tiles[7], tiles[8]); // 112233
            if (check3 == 2 || check3 == 3)
            {
                int check4 = pair_check(tiles[9], tiles[10]);
                if (check4 == 1)
                {
                    int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                    if (check5 == 2 || check5 == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 9;
                        gamedata.chow_count += 2;
                        pung_chow(gamedata, check3);
                        pung_chow(gamedata, check5);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else if (check5 == 1 && check3 == 2) // 112233 444 55 66 7
                    {
                        if (pair_pung_chow(tiles[8], tiles[9], tiles[11]) == 3 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 6;
                            gamedata.chow_count += 4;
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            print("61 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        print("62 - Hand combination not correct!");
                    }
                }
                else if (check4 == 2)
                {
                    if (check3 == 2 && (pair_pung_chow(tiles[8], tiles[9], tiles[10])) == 3)
                    {
                        int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                        if (check5 == 2 || check5 == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 6;
                            gamedata.chow_count += 3;
                            pung_chow(gamedata, check5);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            print("63 - Hand combination not correct!");
                        }
                    }
                    else if ((pair_pung_chow(tiles[11], tiles[12], tiles[13])) == 2 && (pair_pung_chow(tiles[9], tiles[10], tiles[11])) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 12;
                        gamedata.chow_count += 3;
                        pung_chow(gamedata, check4);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("64 - Hand combination not correct!");
                    }
                }
                else
                {
                    int check5 = five_tile_check(tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]); //112233 444 56667  112233 456 78999
                    if (check5 == 1)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 11;
                        gamedata.chow_count += 3;
                        pung_chow(gamedata, check3);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else if (check5 == 3 && check3 == 2) // 112233 555 66667 67777
                    {
                        if (pair_pung_chow(tiles[8], tiles[9], tiles[13]) == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 6;
                            gamedata.chow_count += 3;
                            gamedata.pung_count += 1;
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                            gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                        }
                        else
                        {
                            print("65 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        print("66 - Hand combination not correct!");
                    }
                }
            }
            else if (check3 == 1) // 11223344
            {
                int check4 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                if (check4 > 3 && check4 < 8)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 6;
                    gamedata.chow_count += 2;
                    pung_chow(gamedata, check4);
                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                }
                else
                {
                    int check5 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                    if (check5 == 2 && pair_pung_chow(tiles[9], tiles[11], tiles[13]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 6;
                        gamedata.chow_count += 4;
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else if (check5 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 6;
                        gamedata.pung_count += 1;
                        gamedata.chow_count += 3;
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                    }
                    else
                    {
                        print("67 - Hand combination not correct!");
                    }
                }
            }
            else
            {
                int check4 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]); // 112233 56667/56777
                int check10 = pair_check(tiles[12], tiles[13]);
                if (check4 == 1)
                {
                    int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                    if (check5 == 2 || check5 == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 8;
                        gamedata.chow_count += 3;
                        pung_chow(gamedata, check5);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("68 - Hand combination not correct!");
                    }
                }
                else if (check4 == 2 && pair_pung_chow(tiles[8], tiles[10], tiles[11]) == 3)
                {
                    if (check10 == 1)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 12;
                        gamedata.chow_count += 4;
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("69 - Hand combination not correct!");
                    }
                }
                else if (check4 == 3 && pair_pung_chow(tiles[6], tiles[10], tiles[11]) == 3) // 112233 11112 3 34/44 /12222 3 34/44
                {
                    if (check10 == 1)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 12;
                        gamedata.chow_count += 3;
                        gamedata.pung_count += 1;
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                    }
                    else if (check10 == 2 && pair_pung_chow(tiles[9], tiles[12], tiles[13]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 7;
                        gamedata.chow_count += 4;
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("70 - Hand combination not correct!");
                    }
                }
                else
                {
                    print("71 - Hand combination not correct!");
                }
            }
        }
    }
    else if (check1 > 3 && check1 < 8)
    {
        int check2 = six_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10], tiles[11]);
        if (check2 > 3 && check2 < 9)
        {
            int check3 = pair_check(tiles[12], tiles[13]);
            if (check3 == 1)
            {
                gamedata.pair_count += 1;
                gamedata.eye_idx = 12;
                pung_chow(gamedata, check1);
                pung_chow(gamedata, check2);
                gamedata.winnable = 1; // transferhand(gamedata, 14);
            }
            else if (check3 == 2)
            {
                if (pair_pung_chow(tiles[9], tiles[10], tiles[11]) == 2 && pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 9;
                    pung_chow(gamedata, check1);
                    pung_chow(gamedata, check2);
                    gamedata.pung_count -= 1;
                    gamedata.chow_count += 1;
                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                }
                else
                {
                    print("72 - Hand combination not correct!");
                }
            }
            else
            {
                print("73 - Hand combination not correct!");
            }
        }
        else
        {
            int check3 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
            if (check3 == 2 || check3 == 3)
            {
                int check4 = pair_check(tiles[9], tiles[10]);
                if (check4 == 1)
                {
                    int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                    if (check5 == 2 || check5 == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 9;
                        pung_chow(gamedata, check1);
                        pung_chow(gamedata, check3);
                        pung_chow(gamedata, check5);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("74 - Hand combination not correct!");
                    }
                }
                else if (check4 == 2)
                {
                    if (pair_pung_chow(tiles[6], tiles[7], tiles[8]) == 2 && pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
                    {
                        int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                        if (check5 == 2 || check5 == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 6;
                            gamedata.chow_count += 1;
                            pung_chow(gamedata, check1);
                            pung_chow(gamedata, check3);
                            pung_chow(gamedata, check5);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            print("75 - Hand combination not correct!");
                        }
                    }
                    else if (pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 2 && pair_pung_chow(tiles[9], tiles[10], tiles[11]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 12;
                        gamedata.chow_count += 1;
                        pung_chow(gamedata, check1);
                        pung_chow(gamedata, check3);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("76 - Hand combination not correct!");
                    }
                }
                else
                {
                    print("77 - Hand combination not correct!");
                }
            }
            else if (check3 == 1)
            {
                int check4 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                if (check4 > 3 && check4 < 9)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 6;
                    pung_chow(gamedata, check1);
                    pung_chow(gamedata, check4);
                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                }
                else
                {
                    int check5 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                    if (check5 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 6;
                        gamedata.chow_count += 2;
                        pung_chow(gamedata, check1);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else if (check5 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 6;
                        gamedata.chow_count += 1;
                        gamedata.pung_count += 1;
                        pung_chow(gamedata, check1);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                    }
                    else
                    {
                        print("78 - Hand combination not correct!");
                    }
                }
            }
            else
            {
                int check4 = pair_check(tiles[6], tiles[7]);
                if (check4 == 2)
                {
                    if (pair_pung_chow(tiles[3], tiles[4], tiles[5]) == 2 && pair_pung_chow(tiles[5], tiles[6], tiles[7]) == 3) // 12344456
                    {
                        int check5 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                        if (check5 > 3 && check5 < 9)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 3;
                            gamedata.chow_count += 1;
                            pung_chow(gamedata, check1);
                            pung_chow(gamedata, check5);
                            gamedata.pung_count -= 1;
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            int check6 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                            if (check6 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 3;
                                gamedata.chow_count += 2;
                                pung_chow(gamedata, check1);
                                gamedata.pung_count -= 1;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else if (check6 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 3;
                                gamedata.chow_count += 1;
                                gamedata.pung_count += 1;
                                pung_chow(gamedata, check1);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                            }
                            else
                            {
                                print("79 - Hand combination not correct!");
                            }
                        }
                    }
                    else if (pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 2 && pair_pung_chow(tiles[6], tiles[7], tiles[8]) == 3) // 11223345666
                    {
                        int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                        if (check5 == 2 || check5 == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 9;
                            gamedata.chow_count += 1;
                            pung_chow(gamedata, check1);
                            pung_chow(gamedata, check5);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            print("80 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        print("81 - Hand combination not correct!");
                    }
                }
                else
                {
                    print("82 - Hand combination not correct!");
                }
            }
        }
    }
    else
    {
        int check2 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
        if (check2 == 2 || check2 == 3)
        {
            int check3 = pair_check(tiles[3], tiles[4]);
            if (check3 == 1)
            {
                int check4 = six_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
                if (check4 > 3 && check4 < 9)
                {
                    int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                    if (check5 == 2 || check5 == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 3;
                        pung_chow(gamedata, check2);
                        pung_chow(gamedata, check4);
                        pung_chow(gamedata, check5);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("83 - Hand combination not correct!");
                    }
                }
                else if (check4 == 10 && pair_pung_chow(tiles[3], tiles[5], tiles[7]) == 3)
                {
                    int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                    if (check5 == 2 || check5 == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 9;
                        gamedata.chow_count += 2;
                        pung_chow(gamedata, check2);
                        pung_chow(gamedata, check5);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("84 - Hand combination not correct!");
                    }
                }
                else
                {
                    int check5 = pair_pung_chow(tiles[5], tiles[6], tiles[7]); // 1112256  1235566
                    if (check5 == 2 || check5 == 3)
                    {
                        int check6 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                        if (check6 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 3;
                            gamedata.chow_count += 2;
                            pung_chow(gamedata, check2);
                            pung_chow(gamedata, check5);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else if (check6 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 3;
                            gamedata.chow_count += 1;
                            gamedata.pung_count += 1;
                            pung_chow(gamedata, check2);
                            pung_chow(gamedata, check5);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                            gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                        }
                        else
                        {
                            print("85 - Hand combination not correct!");
                        }
                    }
                    else if (check5 == 1)
                    {
                        int check6 = pair_check(tiles[7], tiles[8]); // 111334455
                        if (check6 == 1 && pair_pung_chow(tiles[3], tiles[5], tiles[7]) == 3)
                        {
                            int check7 = pair_pung_chow(tiles[9], tiles[10], tiles[11]);
                            if (check7 == 3 && pair_check(tiles[12], tiles[13]) == 1)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 12;
                                gamedata.chow_count += 3;
                                pung_chow(gamedata, check2);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else if (check7 == 2)
                            {
                                if (pair_check(tiles[12], tiles[13]) == 1)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 12;
                                    gamedata.pung_count += 1;
                                    gamedata.chow_count += 2;
                                    pung_chow(gamedata, check2);
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else if (pair_check(tiles[12], tiles[13]) == 2 && pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 9;
                                    gamedata.chow_count += 3;
                                    pung_chow(gamedata, check2);
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("86 - Hand combination not correct!");
                                }
                            }
                            else if (check7 == 1)
                            {
                                int check8 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                                if (check8 == 2 || check8 == 3)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 9;
                                    gamedata.chow_count += 2;
                                    pung_chow(gamedata, check2);
                                    pung_chow(gamedata, check8);
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("87 - Hand combination not correct!");
                                }
                            }
                            else
                            {
                                int check8 = five_tile_check(tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                                if (check8 == 1)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 11;
                                    gamedata.chow_count += 3;
                                    pung_chow(gamedata, check2);
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("88 - Hand combination not correct!");
                                }
                            }
                        }
                        else
                        {
                            print("89 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        print("90 - Hand combination not correct!");
                    }
                }
            }
            else if (check3 == 2 && check2 == 2) // 11123
            {
                if (pair_pung_chow(tiles[2], tiles[3], tiles[4]) == 3)
                {
                    int check4 = six_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
                    if (check4 > 3 && check4 < 9)
                    {
                        int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                        if (check5 == 2 || check5 == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 0;
                            gamedata.chow_count += 1;
                            pung_chow(gamedata, check4);
                            pung_chow(gamedata, check5);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            print("91 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        int check5 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
                        if (check5 == 2 || check5 == 3)
                        {
                            int check6 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                            if (check6 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 0;
                                gamedata.chow_count += 3;
                                pung_chow(gamedata, check5);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else if (check6 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.pung_count += 1;
                                gamedata.chow_count += 2;
                                pung_chow(gamedata, check5);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                            }
                            else
                            {
                                print("92 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            int check6 = five_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9]);
                            int check7 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                            if (check6 == 2 && pair_pung_chow(tiles[7], tiles[9], tiles[10]) == 3)
                            {
                                if (check7 == 2 || check7 == 3)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 0;
                                    gamedata.chow_count += 3;
                                    pung_chow(gamedata, check7);
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("93 - Hand combination not correct!");
                                }
                            }
                            else if (check6 == 3 && pair_pung_chow(tiles[5], tiles[9], tiles[10]) == 3)
                            {
                                if (check7 == 2 || check7 == 3)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 0;
                                    gamedata.pung_count += 1;
                                    gamedata.chow_count += 2;
                                    pung_chow(gamedata, check7);
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                    gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                                }
                                else
                                {
                                    print("94 - Hand combination not correct!");
                                }
                            }
                            else
                            {
                                print("95 - Hand combination not correct!");
                            }
                        }
                    }
                }
                else
                {
                    print("96 - Hand combination not correct!");
                }
            }
            else
            {
                print("97 - Hand combination not correct!");
            }
        }
        else if (check2 == 1) // 11
        {
            int check3 = six_tile_check(tiles[2], tiles[3], tiles[4], tiles[5], tiles[6], tiles[7]);
            if (check3 > 3 && check3 < 9)
            {
                int check4 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                if (check4 > 3 && check4 < 9)
                {
                    gamedata.pair_count += 1;
                    gamedata.eye_idx = 0;
                    pung_chow(gamedata, check3);
                    pung_chow(gamedata, check4);
                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                }
                else
                {
                    int check5 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                    if (check5 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 0;
                        gamedata.chow_count += 2;
                        pung_chow(gamedata, check3);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else if (check5 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 0;
                        gamedata.chow_count += 1;
                        gamedata.pung_count += 1;
                        pung_chow(gamedata, check3);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                        gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                    }
                    else
                    {
                        print("98 - Hand combination not correct!");
                    }
                }
            }
            else
            {
                int check4 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
                if (check4 == 2 || check4 == 3)
                {
                    int check5 = five_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9]);
                    if (check5 == 2 && pair_pung_chow(tiles[7], tiles[9], tiles[10]) == 3)
                    {
                        int check6 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                        if (check6 == 2 || check6 == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 0;
                            gamedata.chow_count += 2;
                            pung_chow(gamedata, check4);
                            pung_chow(gamedata, check6);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            print("99 - Hand combination not correct!");
                        }
                    }
                    else if (check5 == 3 && pair_pung_chow(tiles[5], tiles[9], tiles[10]) == 3)
                    {
                        int check6 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                        if (check6 == 2 || check6 == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 0;
                            gamedata.chow_count += 1;
                            gamedata.pung_count += 1;
                            pung_chow(gamedata, check4);
                            pung_chow(gamedata, check6);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                            gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                        }
                        else
                        {
                            print("100 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        print("101 - Hand combination not correct!");
                    }
                }
                else
                {
                    int check5 = five_tile_check(tiles[2], tiles[3], tiles[4], tiles[5], tiles[6]);
                    if (check5 == 1 && pair_pung_chow(tiles[6], tiles[8], tiles[10]) == 3)
                    {
                        if (pair_check(tiles[7], tiles[8]) == 1 && pair_check(tiles[9], tiles[10]) == 1)
                        {
                            int check6 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                            if (check6 == 2 || check6 == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 0;
                                gamedata.chow_count += 3;
                                pung_chow(gamedata, check6);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else
                            {
                                print("102 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            print("103 - Hand combination not correct!");
                        }
                    }
                    else if (check5 == 2 && pair_pung_chow(tiles[4], tiles[6], tiles[7]) == 3)
                    {
                        int check6 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                        if (check6 > 3 || check6 < 9)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 0;
                            gamedata.chow_count += 2;
                            pung_chow(gamedata, check6);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            int check7 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                            if (check7 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 0;
                                gamedata.chow_count += 4;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else if (check7 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 0;
                                gamedata.chow_count += 3;
                                gamedata.pung_count += 1;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else
                            {
                                print("104 - Hand combination not correct!");
                            }
                        }
                    }
                    else if (check5 == 3 && pair_pung_chow(tiles[2], tiles[6], tiles[7]) == 3)
                    {
                        int check6 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                        if (check6 > 3 || check6 < 9)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 0;
                            gamedata.chow_count += 1;
                            gamedata.pair_count += 1;
                            pung_chow(gamedata, check6);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                            gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                        }
                        else
                        {
                            int check7 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                            if (check7 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 0;
                                gamedata.pair_count += 1;
                                gamedata.chow_count += 3;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                            }
                            else if (check7 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 0;
                                gamedata.chow_count += 2;
                                gamedata.pung_count += 2;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                            }
                            else
                            {
                                print("105 - Hand combination not correct!");
                            }
                        }
                    }
                    else if (tiles[2].value == 9 && tiles[1].suit == tiles[2].suit) // 119
                    {
                        int check3 = pair_check(tiles[3], tiles[4]); // 11919
                        int check4 = pair_check(tiles[5], tiles[6]); // 1191919
                        if (check3 == 3 && check4 == 3)              // -----
                        {
                            int check5 = honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                            if (check5 == 1)
                            {

                                gamedata.eye_idx = 0;
                                gamedata.pair_count += 1;
                                gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else
                            {
                                print("106.1 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            print("106.2 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        print("106.3 - Hand combination not correct!");
                    }
                }
            }
        }
        else
        {
            int check3 = five_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4]);
            if (check3 == 1)
            {
                int check4 = six_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
                if (check4 > 3 && check4 < 9)
                {
                    int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                    if (check5 == 2 || check5 == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 2;
                        gamedata.chow_count += 1;
                        pung_chow(gamedata, check4);
                        pung_chow(gamedata, check5);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("107 - Hand combination not correct!");
                    }
                }
                else
                {
                    int check5 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
                    if (check5 == 2 || check5 == 3)
                    {
                        int check6 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                        if (check6 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 2;
                            gamedata.chow_count += 3;
                            pung_chow(gamedata, check5);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else if (check6 == 3 && pair_pung_chow(tiles[8], tiles[10], tiles[13]) == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 2;
                            gamedata.chow_count += 2;
                            gamedata.pung_count += 1;
                            pung_chow(gamedata, check5);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                            gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                        }
                        else
                        {
                            print("108 - Hand combination not correct!");
                        }
                    }
                    else if (check5 == 1 && pair_check(tiles[7], tiles[8]) == 1) // 12223 33 44
                    {
                        if (pair_pung_chow(tiles[3], tiles[5], tiles[7]) == 3)
                        {
                            int check6 = pair_pung_chow(tiles[9], tiles[10], tiles[11]);
                            int check7 = pair_check(tiles[12], tiles[13]);
                            if (check6 == 3 && check7 == 1)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 12;
                                gamedata.chow_count += 4;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else if (check6 == 2)
                            {
                                if (check7 == 1)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 12;
                                    gamedata.chow_count += 3;
                                    gamedata.pung_count += 1;
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else if (check7 == 2 && pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 9;
                                    gamedata.chow_count += 4;
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("109 - Hand combination not correct!");
                                }
                            }
                            else if (check6 == 1)
                            {
                                int check8 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                                if (check8 == 2 || check8 == 3)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 9;
                                    gamedata.chow_count += 3;
                                    pung_chow(gamedata, check8);
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("110 - Hand combination not correct!");
                                }
                            }
                            else
                            {
                                int check8 = five_tile_check(tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                                if (check8 == 1)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 11;
                                    gamedata.chow_count += 4;
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("111 -Hand combination not correct!");
                                }
                            }
                        }
                        else
                        {
                            print("112 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        print("113 - Hand combination not correct!");
                    }
                }
            }
            else if (check3 == 2 && pair_pung_chow(tiles[2], tiles[4], tiles[5]) == 3) // 12233
            {
                int check4 = six_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10], tiles[11]);
                if (check4 > 3 && check4 < 9)
                {
                    int check5 = pair_check(tiles[12], tiles[13]);
                    if (check5 == 1)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 12;
                        gamedata.chow_count += 2;
                        pung_chow(gamedata, check4);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else if (check5 == 2 && pair_pung_chow(tiles[9], tiles[10], tiles[11]) == 2)
                    {
                        if (pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 9;
                            gamedata.chow_count += 3;
                            pung_chow(gamedata, check4);
                            gamedata.pung_count -= 1;
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            print("114 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        print("115 - Hand combination not correct!");
                    }
                }
                else if (check4 == 9 && pair_check(tiles[12], tiles[13]) == 1)
                {
                    if (pair_pung_chow(tiles[8], tiles[10], tiles[12]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 6;
                        gamedata.chow_count += 4;
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("116 - Hand combination not correct!");
                    }
                }
                else
                {
                    int check5 = pair_pung_chow(tiles[6], tiles[7], tiles[8]); // 122334 555 66 778
                    if (check5 == 2 || check5 == 3)
                    {
                        int check6 = pair_check(tiles[9], tiles[10]);
                        if (check6 == 1)
                        {
                            int check7 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                            if (check7 == 2 || check3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 9;
                                gamedata.chow_count += 2;
                                pung_chow(gamedata, check5);
                                pung_chow(gamedata, check7);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else if (check7 == 1 && check5 == 2)
                            {
                                if (pair_pung_chow(tiles[8], tiles[9], tiles[11]) == 3 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 6;
                                    gamedata.chow_count += 4;
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("117 - Hand combination not correct!");
                                }
                            }
                            else
                            {
                                print("117 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            int check7 = five_tile_check(tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                            if (check7 == 1)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 11;
                                gamedata.chow_count += 3;
                                pung_chow(gamedata, check5);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else
                            {
                                print("118 - Hand combination not correct!");
                            }
                        }
                    }
                    else if (check5 == 1)
                    {
                        int check6 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                        if (check6 > 3 && check6 < 9)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 6;
                            gamedata.chow_count += 2;
                            pung_chow(gamedata, check6);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else if (check6 == 10 && pair_pung_chow(tiles[6], tiles[8], tiles[10]) == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 12;
                            gamedata.chow_count += 4;
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            int check7 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                            if (check7 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 6;
                                gamedata.chow_count += 4;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else if (check7 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 6;
                                gamedata.chow_count += 3;
                                gamedata.pung_count += 1;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                            }
                            else
                            {
                                print("119 - Hand combination not correct!");
                            }
                        }
                    }
                    else
                    {
                        int check6 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
                        if (check6 == 1)
                        {
                            int check7 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                            if (check7 == 2 || check7 == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 8;
                                gamedata.chow_count += 3;
                                pung_chow(gamedata, check7);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else
                            {
                                print("120 - Hand combination not correct!");
                            }
                        }
                        else if (check6 == 2 && pair_pung_chow(tiles[8], tiles[10], tiles[11]) == 3)
                        {
                            if (pair_check(tiles[12], tiles[13]) == 1)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 12;
                                gamedata.chow_count += 4;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else
                            {
                                print("121 - Hand combination not correct!");
                            }
                        }
                        else if (check6 == 3 && pair_pung_chow(tiles[6], tiles[10], tiles[11]) == 3)
                        {
                            int check7 = pair_check(tiles[12], tiles[13]);
                            if (check7 == 1)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 12;
                                gamedata.chow_count += 3;
                                gamedata.pung_count += 1;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                            }
                            else if (check7 == 2 && pair_pung_chow(tiles[9], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 8;
                                gamedata.chow_count += 4;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                            }
                            else
                            {
                                print("122 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            print("123 - Hand combination not correct!");
                        }
                    }
                }
            }
            else if (check3 == 3 && pair_pung_chow(tiles[0], tiles[4], tiles[5]) == 3) // 111123     / 122223
            {
                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                int check4 = six_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10], tiles[11]);
                if (check4 > 3 && check4 < 9)
                {
                    int check5 = pair_check(tiles[12], tiles[13]);
                    if (check5 == 1)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 12;
                        gamedata.chow_count += 1;
                        gamedata.pung_count += 1;
                        pung_chow(gamedata, check4);
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else if (check5 == 2 && pair_pung_chow(tiles[9], tiles[10], tiles[11]) == 2)
                    {
                        if (pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 9;
                            gamedata.chow_count += 2;
                            pung_chow(gamedata, check4);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            print("124 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        print("125 - Hand combination not correct!");
                    }
                }
                else if (check4 == 9 && pair_check(tiles[12], tiles[13]) == 1) /// 12222 55778899
                {
                    if (pair_pung_chow(tiles[8], tiles[10], tiles[12]) == 3)
                    {
                        gamedata.pair_count += 1;
                        gamedata.eye_idx = 6;
                        gamedata.chow_count += 3;
                        gamedata.pung_count += 1;
                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                    }
                    else
                    {
                        print("126 - Hand combination not correct!");
                    }
                }
                else
                {
                    int check5 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
                    if (check5 == 2 || check5 == 3)
                    {
                        int check6 = pair_check(tiles[9], tiles[10]);
                        if (check6 == 1)
                        {
                            int check7 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                            if (check7 == 2 || check3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 9;
                                gamedata.chow_count += 1;
                                gamedata.pung_count += 1;
                                pung_chow(gamedata, check5);
                                pung_chow(gamedata, check7);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else if (check7 == 1 && check5 == 2)
                            {
                                if (pair_pung_chow(tiles[8], tiles[9], tiles[11]) == 3 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 6;
                                    gamedata.chow_count += 3;
                                    gamedata.pung_count += 1;
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("127 - Hand combination not correct!");
                                }
                            }
                            else
                            {
                                print("128 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            int check7 = five_tile_check(tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                            if (check7 == 1)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 11;
                                gamedata.chow_count += 2;
                                gamedata.pung_count += 1;
                                pung_chow(gamedata, check5);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else
                            {
                                print("129 - Hand combination not correct!");
                            }
                        }
                    }
                    else if (check5 == 1)
                    {
                        int check6 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                        if (check6 > 3 && check6 < 9)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 6;
                            gamedata.chow_count += 1;
                            gamedata.pung_count += 1;
                            pung_chow(gamedata, check6);
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else if (check6 == 10 && pair_pung_chow(tiles[6], tiles[8], tiles[10]) == 3)
                        {
                            gamedata.pair_count += 1;
                            gamedata.eye_idx = 12;
                            gamedata.chow_count += 3;
                            gamedata.pung_count += 1;
                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                        }
                        else
                        {
                            int check7 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                            if (check7 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 6;
                                gamedata.chow_count += 3;
                                gamedata.pung_count += 1;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else if (check7 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 6;
                                gamedata.chow_count += 2;
                                gamedata.pung_count += 2;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                            }
                            else
                            {
                                print("130 - Hand combination not correct!");
                            }
                        }
                    }
                    else
                    {
                        if (pair_check(tiles[6], tiles[7]) == 2 && pair_pung_chow(tiles[4], tiles[6], tiles[7]) == 3) // 122223 34
                        {
                            int check6 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                            if (check6 > 3 && check6 < 9)
                            {
                                gamedata.pair_count += 1;
                                gamedata.eye_idx = 3;
                                gamedata.chow_count += 2;
                                pung_chow(gamedata, check6);
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                            }
                            else
                            {
                                int check7 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
                                if (check7 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 3;
                                    gamedata.chow_count += 4;
                                    pung_chow(gamedata, check6);
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else if (check7 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
                                {

                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 3;
                                    gamedata.chow_count += 3;
                                    gamedata.pung_count += 1;
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                    gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                                }
                                else
                                {
                                    print("131 - Hand combination not correct!");
                                }
                            }
                        }
                        else
                        {
                            int check6 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
                            if (check6 == 1)
                            {
                                int check7 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
                                if (check7 == 2 || check7 == 3)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 8;
                                    gamedata.chow_count += 2;
                                    gamedata.pung_count += 1;
                                    pung_chow(gamedata, check7);
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("132 - Hand combination not correct!");
                                }
                            }
                            else if (check6 == 2 && pair_pung_chow(tiles[8], tiles[10], tiles[11]) == 3)
                            {
                                if (pair_check(tiles[12], tiles[13]) == 1)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 12;
                                    gamedata.chow_count += 3;
                                    gamedata.pung_count += 1;
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("133 - Hand combination not correct!");
                                }
                            }
                            else if (check6 == 3 && pair_pung_chow(tiles[6], tiles[10], tiles[11]) == 3)
                            {
                                gamedata.score_check.insert(gamedata.score_check.begin(), 68);
                                int check7 = pair_check(tiles[12], tiles[13]);
                                if (check7 == 1)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 12;
                                    gamedata.chow_count += 2;
                                    gamedata.pung_count += 2;
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else if (check7 == 2 && pair_pung_chow(tiles[9], tiles[12], tiles[13]) == 3)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.eye_idx = 8;
                                    gamedata.chow_count += 3;
                                    gamedata.pung_count += 1;
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                }
                                else
                                {
                                    print("134 - Hand combination not correct!");
                                }
                            }
                            else
                            {
                                print("135 - Hand combination not correct!");
                            }
                        }
                    }
                }
            }
            else
            {
                int check10 = pair_check(tiles[0], tiles[1]);
                if (check10 == 3) // 19
                {
                    if (pair_check(tiles[1], tiles[2]) == 1) // 199
                    {
                        if (pair_check(tiles[3], tiles[4]) == 3 && pair_check(tiles[5], tiles[6]) == 3)
                        {
                            if (honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1)
                            {
                                gamedata.pair_count += 1;
                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                            }
                            else
                            {
                                print("136.1 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            print("136.2 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        int check11 = pair_check(tiles[2], tiles[3]);
                        if (check11 == 1 && tiles[2].value == 1) // 1911
                        {
                            if (tiles[4].value == 9 && tiles[3].suit == tiles[4].suit) //
                            {
                                if (pair_check(tiles[5], tiles[6]) == 3 && honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                    gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                }
                                else
                                {
                                    print("136.3 - Hand combination not correct!");
                                }
                            }
                            else
                            {
                                print("136.4 - Hand combination not correct!");
                            }
                        }
                        else if (check11 == 3) // 1919
                        {
                            if (pair_check(tiles[3], tiles[4]) == 1) // 19199
                            {
                                if (pair_check(tiles[5], tiles[6]) == 3 && honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1)
                                {
                                    gamedata.pair_count += 1;
                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                    gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                }
                                else
                                {
                                    print("136.3 - Hand combination not correct!");
                                }
                            }
                            else
                            {
                                int check12 = pair_check(tiles[4], tiles[5]);
                                if (check12 == 1 && tiles[4].value == 1) //191911
                                {
                                    if (tiles[6].value == 9 && tiles[5].suit == tiles[6].suit)
                                    {
                                        if (honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1)
                                        {
                                            gamedata.pair_count += 1;
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                        }
                                        else
                                        {
                                            print("136.55 - Hand combination not correct!");
                                        }
                                    }
                                    else
                                    {
                                        print("136.6 - Hand combination not correct!");
                                    }
                                }
                                else if (check12 == 3) // 191919
                                {
                                    if (pair_check(tiles[5], tiles[6]) == 1)
                                    {
                                        if (honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1) // 1919199ESWNRWG
                                        {
                                            gamedata.pair_count += 1;
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                        }
                                        else
                                        {
                                            print("136.7 - Hand combination not correct!");
                                        }
                                    }
                                    else
                                    {
                                        if (tiles[6].value == 10 && tiles[6].suit == 4)
                                        {
                                            if (honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1) // 191919EESWNRWG
                                            {
                                                gamedata.pair_count += 1;
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                            }
                                            else if (tiles[7].value == 10 && tiles[7].suit == 5)
                                            {
                                                if (pair_check(tiles[7], tiles[8]) == 1 && (tiles[8].suit + 1) == tiles[9].suit) // 191919ESSWNRWG
                                                {
                                                    if ((tiles[9].suit + 1) == tiles[10].suit && (tiles[10].suit + 1) == tiles[11].suit)
                                                    {
                                                        if ((tiles[11].suit + 1) == tiles[12].suit && (tiles[12].suit + 1) == tiles[13].suit)
                                                        {
                                                            gamedata.pair_count += 1;
                                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                            gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                                        }
                                                        else
                                                        {
                                                            print("136.8 - Hand combination not correct!");
                                                        }
                                                    }
                                                    else
                                                    {
                                                        print("136.9 - Hand combination not correct!");
                                                    }
                                                }
                                                else if ((tiles[7].suit + 1) == tiles[8].suit)
                                                {
                                                    if (pair_check(tiles[8], tiles[9]) == 1 && (tiles[9].suit + 1) == tiles[10].suit) // 191919ESWWNRWG
                                                    {
                                                        if ((tiles[10].suit + 1) == tiles[11].suit && (tiles[11].suit + 1) == tiles[12].suit)
                                                        {
                                                            if ((tiles[12].suit + 1) == tiles[13].suit)
                                                            {
                                                                gamedata.pair_count += 1;
                                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                                gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                                            }
                                                            else
                                                            {
                                                                print("136.10 - Hand combination not correct!");
                                                            }
                                                        }
                                                        else
                                                        {
                                                            print("136.11 - Hand combination not correct!");
                                                        }
                                                    }
                                                    else if ((tiles[8].suit + 1) == tiles[9].suit)
                                                    {
                                                        if (pair_check(tiles[9], tiles[10]) == 1 && (tiles[10].suit + 1) == tiles[11].suit) // 191919ESWNNRWG
                                                        {
                                                            if ((tiles[11].suit + 1) == tiles[12].suit && (tiles[12].suit + 1) == tiles[13].suit)
                                                            {
                                                                gamedata.pair_count += 1;
                                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                                gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                                            }
                                                            else
                                                            {
                                                                print("136.12 - Hand combination not correct!");
                                                            }
                                                        }
                                                        else if ((tiles[9].suit + 1) == tiles[10].suit)
                                                        {
                                                            if (pair_check(tiles[10], tiles[11]) == 1 && (tiles[11].suit + 1) == tiles[12].suit) // 191919ESWNRRWG
                                                            {
                                                                if ((tiles[12].suit + 1) == tiles[13].suit)
                                                                {
                                                                    gamedata.pair_count += 1;
                                                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                                    gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                                                }
                                                                else
                                                                {
                                                                    print("136.13 - Hand combination not correct!");
                                                                }
                                                            }
                                                            else if ((tiles[10].suit + 1) == tiles[11].suit)
                                                            {
                                                                if (pair_check(tiles[11], tiles[12]) == 1 && tiles[12].suit + 1 == tiles[13].suit) // 191919ESWNRWWG
                                                                {
                                                                    gamedata.pair_count += 1;
                                                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                                    gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                                                }
                                                                else if ((tiles[11].suit + 1) == tiles[12].suit && pair_check(tiles[12], tiles[13]) == 1) // 191919ESWNRWGG
                                                                {
                                                                    gamedata.pair_count += 1;
                                                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                                    gamedata.score_check.insert(gamedata.score_check.begin(), 7);
                                                                }
                                                                else
                                                                {
                                                                    print("136.14 - Hand combination not correct!");
                                                                }
                                                            }
                                                            else
                                                            {
                                                                print("136.15 - Hand combination not correct!");
                                                            }
                                                        }
                                                        else
                                                        {
                                                            print("136.16 - Hand combination not correct!");
                                                        }
                                                    }
                                                    else
                                                    {
                                                        print("136.17 - Hand combination not correct!");
                                                    }
                                                }
                                                else
                                                {
                                                    print("136.18 - Hand combination not correct!");
                                                }
                                            }
                                            else
                                            {
                                                print("136.19 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.20 - Hand combination not correct!");
                                        }
                                    }
                                }
                                else
                                {
                                    print("136.21 - Hand combination not correct!");
                                }
                            }
                        }
                        else
                        {
                            print("136.22 - Hand combination not correct!");
                        }
                    }
                }
                else
                {
                    int check15 = honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
                    if (check15 == 1)
                    {
                        int check16 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
                        if (check16 == 4)
                        {
                            int check17 = pair_check(tiles[3], tiles[4]);
                            int check18 = pair_check(tiles[5], tiles[6]);
                            if (tiles[0].value == 1)
                            {
                                if (check17 == 4 && check18 == 4)
                                {
                                    if (tiles[3].value == 2 || tiles[3].value == 5)
                                    {
                                        if (tiles[5].value == 3 || tiles[5].value == 6)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                        }
                                        else
                                        {
                                            print("136.30 - Hand combination not correct!");
                                        }
                                    }
                                    else if (tiles[3].value == 3 || tiles[3].value == 6)
                                    {
                                        if (tiles[5].value == 2 || tiles[5].value == 5)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                        }
                                        else
                                        {
                                            print("136.31 - Hand combination not correct!");
                                        }
                                    }
                                    else
                                    {
                                        print("136.32 - Hand combination not correct!");
                                    }
                                }
                            }
                            else if (tiles[0].value == 2)
                            {
                                if (check17 == 4 && check18 == 4)
                                {
                                    if (tiles[3].value == 1 || tiles[3].value == 4)
                                    {
                                        if (tiles[5].value == 3 || tiles[5].value == 6)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                        }
                                        else
                                        {
                                            print("136.33 - Hand combination not correct!");
                                        }
                                    }
                                    else if (tiles[3].value == 3 || tiles[3].value == 6)
                                    {
                                        if (tiles[5].value == 1 || tiles[5].value == 4)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                        }
                                        else
                                        {
                                            print("136.34 - Hand combination not correct!");
                                        }
                                    }
                                    else
                                    {
                                        print("136.35 - Hand combination not correct!");
                                    }
                                }
                            }
                            else if (tiles[0].value == 3)
                            {
                                if (check17 == 4 && check18 == 4)
                                {
                                    if (tiles[3].value == 1 || tiles[3].value == 4)
                                    {
                                        if (tiles[5].value == 2 || tiles[5].value == 5)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                        }
                                        else
                                        {
                                            print("136.36 - Hand combination not correct!");
                                        }
                                    }
                                    else if (tiles[3].value == 2 || tiles[3].value == 5)
                                    {
                                        if (tiles[5].value == 1 || tiles[5].value == 4)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                        }
                                        else
                                        {
                                            print("136.37 - Hand combination not correct!");
                                        }
                                    }
                                    else
                                    {
                                        print("136.38 - Hand combination not correct!");
                                    }
                                }
                            }
                            else
                            {
                                print("136.39 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            int check17 = pair_check(tiles[0], tiles[1]);
                            if (check17 == 4)
                            {
                                if (tiles[0].value == 1 || tiles[0].value == 4)
                                {
                                    int check18 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
                                    if (check18 == 4)
                                    {
                                        if (tiles[2].value == 2)
                                        {
                                            int check19 = pair_check(tiles[5], tiles[6]);
                                            if (check19 == 4)
                                            {
                                                if (tiles[5].value == 3 || tiles[5].value == 6)
                                                {
                                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                    gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                }
                                                else
                                                {
                                                    print("136.40 - Hand combination not correct!");
                                                }
                                            }
                                            else
                                            {
                                                print("136.41 - Hand combination not correct!");
                                            }
                                        }
                                        else if (tiles[2].value == 3)
                                        {
                                            int check19 = pair_check(tiles[5], tiles[6]);
                                            if (check19 == 4)
                                            {
                                                if (tiles[5].value == 2 || tiles[5].value == 5)
                                                {
                                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                    gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                }
                                                else
                                                {
                                                    print("136.42 - Hand combination not correct!");
                                                }
                                            }
                                            else
                                            {
                                                print("136.43 - Hand combination not correct!");
                                            }
                                        }
                                    }
                                    else
                                    {
                                        int check19 = pair_check(tiles[2], tiles[3]);
                                        if (check19 == 4)
                                        {
                                            if (tiles[2].value == 2 || tiles[2].value == 5)
                                            {
                                                int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
                                                if (check20 == 4)
                                                {
                                                    if (tiles[4].value == 3)
                                                    {
                                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                        gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                    }
                                                    else
                                                    {
                                                        print("136.44 - Hand combination not correct!");
                                                    }
                                                }
                                                else
                                                {
                                                    print("136.45 - Hand combination not correct!");
                                                }
                                            }
                                            else if (tiles[2].value == 3 || tiles[2].value == 6)
                                            {
                                                int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
                                                if (check20 == 4)
                                                {
                                                    if (tiles[4].value == 2)
                                                    {
                                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                        gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                    }
                                                    else
                                                    {
                                                        print("136.46 - Hand combination not correct!");
                                                    }
                                                }
                                                else
                                                {
                                                    print("136.47 - Hand combination not correct!");
                                                }
                                            }
                                            else
                                            {
                                                print("136.48 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.49 - Hand combination not correct!");
                                        }
                                    }
                                }
                                else if (tiles[0].value == 2 || tiles[0].value == 5)
                                {
                                    int check18 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
                                    if (check18 == 4)
                                    {
                                        if (tiles[2].value == 1)
                                        {
                                            int check19 = pair_check(tiles[5], tiles[6]);
                                            if (check19 == 4)
                                            {
                                                if (tiles[5].value == 3 || tiles[5].value == 6)
                                                {
                                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                    gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                }
                                                else
                                                {
                                                    print("136.50 - Hand combination not correct!");
                                                }
                                            }
                                            else
                                            {
                                                print("136.51 - Hand combination not correct!");
                                            }
                                        }
                                        else if (tiles[2].value == 3)
                                        {
                                            int check19 = pair_check(tiles[5], tiles[6]);
                                            if (check19 == 4)
                                            {
                                                if (tiles[5].value == 1 || tiles[5].value == 4)
                                                {
                                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                    gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                }
                                                else
                                                {
                                                    print("136.52 - Hand combination not correct!");
                                                }
                                            }
                                            else
                                            {
                                                print("136.53 - Hand combination not correct!");
                                            }
                                        }
                                    }
                                    else
                                    {
                                        int check19 = pair_check(tiles[2], tiles[3]);
                                        if (check19 == 4)
                                        {
                                            if (tiles[2].value == 1 || tiles[2].value == 4)
                                            {
                                                int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
                                                if (check20 == 4)
                                                {
                                                    if (tiles[4].value == 3)
                                                    {
                                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                        gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                    }
                                                    else
                                                    {
                                                        print("136.54 - Hand combination not correct!");
                                                    }
                                                }
                                                else
                                                {
                                                    print("136.55 - Hand combination not correct!");
                                                }
                                            }
                                            else if (tiles[2].value == 3 || tiles[2].value == 6)
                                            {
                                                int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
                                                if (check20 == 4)
                                                {
                                                    if (tiles[4].value == 1)
                                                    {
                                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                        gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                    }
                                                    else
                                                    {
                                                        print("136.56 - Hand combination not correct!");
                                                    }
                                                }
                                                else
                                                {
                                                    print("136.57 - Hand combination not correct!");
                                                }
                                            }
                                            else
                                            {
                                                print("136.58 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.59 - Hand combination not correct!");
                                        }
                                    }
                                }
                                else if (tiles[0].value == 3 || tiles[0].value == 6)
                                {
                                    int check18 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
                                    if (check18 == 4)
                                    {
                                        if (tiles[2].value == 1)
                                        {
                                            int check19 = pair_check(tiles[5], tiles[6]);
                                            if (check19 == 4)
                                            {
                                                if (tiles[5].value == 2 || tiles[5].value == 5)
                                                {
                                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                    gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                }
                                                else
                                                {
                                                    print("136.60 - Hand combination not correct!");
                                                }
                                            }
                                            else
                                            {
                                                print("136.61 - Hand combination not correct!");
                                            }
                                        }
                                        else if (tiles[2].value == 2)
                                        {
                                            int check19 = pair_check(tiles[5], tiles[6]);
                                            if (check19 == 4)
                                            {
                                                if (tiles[5].value == 1 || tiles[5].value == 4)
                                                {
                                                    gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                    gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                }
                                                else
                                                {
                                                    print("136.62 - Hand combination not correct!");
                                                }
                                            }
                                            else
                                            {
                                                print("136.63 - Hand combination not correct!");
                                            }
                                        }
                                    }
                                    else
                                    {
                                        int check19 = pair_check(tiles[2], tiles[3]);
                                        if (check19 == 4)
                                        {
                                            if (tiles[2].value == 1 || tiles[2].value == 4)
                                            {
                                                int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
                                                if (check20 == 4)
                                                {
                                                    if (tiles[4].value == 2)
                                                    {
                                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                        gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                    }
                                                    else
                                                    {
                                                        print("136.64 - Hand combination not correct!");
                                                    }
                                                }
                                                else
                                                {
                                                    print("136.65 - Hand combination not correct!");
                                                }
                                            }
                                            else if (tiles[2].value == 2 || tiles[2].value == 5)
                                            {
                                                int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
                                                if (check20 == 4)
                                                {
                                                    if (tiles[4].value == 1)
                                                    {
                                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                        gamedata.score_check.insert(gamedata.score_check.begin(), 27);
                                                    }
                                                    else
                                                    {
                                                        print("136.66 - Hand combination not correct!");
                                                    }
                                                }
                                                else
                                                {
                                                    print("136.67 - Hand combination not correct!");
                                                }
                                            }
                                            else
                                            {
                                                print("136.68 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.69 - Hand combination not correct!");
                                        }
                                    }
                                }
                                else
                                {
                                    print("136.70 - Hand combination not correct!");
                                }
                            }
                            else
                            {
                                print("136.71 - Hand combination not correct!");
                            }
                        }
                    }
                    else if (check15 == 2)
                    {
                        int check16 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
                        if (check16 == 4)
                        {
                            if (tiles[0].value == 1)
                            {
                                int check16 = pair_pung_chow(tiles[3], tiles[4], tiles[5]);
                                if (check16 == 4)
                                {
                                    if (tiles[3].value == 2)
                                    {
                                        int check17 = pair_check(tiles[6], tiles[7]);
                                        if (check17 == 4)
                                        {
                                            if (tiles[6].value == 3 || tiles[6].value == 6)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.71 - Hand combination not correct!");
                                        }
                                    }
                                    else if (tiles[3].value == 3)
                                    {
                                        int check17 = pair_check(tiles[6], tiles[7]);
                                        if (check17 == 4)
                                        {
                                            if (tiles[6].value == 3 || tiles[6].value == 6)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.71 - Hand combination not correct!");
                                        }
                                    }
                                    else
                                    {
                                        print("136.71 - Hand combination not correct!");
                                    }
                                }
                                else
                                {
                                    int check17 = pair_check(tiles[3], tiles[4]);
                                    if (check17 == 4)
                                    {
                                        if (tiles[3].value == 2 || tiles[3].value == 5)
                                        {
                                            int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
                                            if (check18 == 4 && tiles[5].value == 3)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else if (tiles[3].value == 3 || tiles[3].value == 6)
                                        {
                                            int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
                                            if (check18 == 4 && tiles[5].value == 2)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.71 - Hand combination not correct!");
                                        }
                                    }
                                    else
                                    {
                                        print("136.71 - Hand combination not correct!");
                                    }
                                }
                            }
                            else if (tiles[0].value == 2) // ------
                            {
                                int check16 = pair_pung_chow(tiles[3], tiles[4], tiles[5]);
                                if (check16 == 4)
                                {
                                    if (tiles[3].value == 1)
                                    {
                                        int check17 = pair_check(tiles[6], tiles[7]);
                                        if (check17 == 4)
                                        {
                                            if (tiles[6].value == 3 || tiles[6].value == 6)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.71 - Hand combination not correct!");
                                        }
                                    }
                                    else if (tiles[3].value == 3)
                                    {
                                        int check17 = pair_check(tiles[6], tiles[7]);
                                        if (check17 == 4)
                                        {
                                            if (tiles[6].value == 1 || tiles[6].value == 4)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.71 - Hand combination not correct!");
                                        }
                                    }
                                    else
                                    {
                                        print("136.71 - Hand combination not correct!");
                                    }
                                }
                                else
                                {
                                    int check17 = pair_check(tiles[3], tiles[4]);
                                    if (check17 == 4)
                                    {
                                        if (tiles[3].value == 1 || tiles[3].value == 4)
                                        {
                                            int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
                                            if (check18 == 4 && tiles[5].value == 3)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else if (tiles[3].value == 3 || tiles[3].value == 6)
                                        {
                                            int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
                                            if (check18 == 4 && tiles[5].value == 1)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.71 - Hand combination not correct!");
                                        }
                                    }
                                    else
                                    {
                                        print("136.71 - Hand combination not correct!");
                                    }
                                }
                            }
                            else if (tiles[0].value == 3)
                            {
                                int check16 = pair_pung_chow(tiles[3], tiles[4], tiles[5]);
                                if (check16 == 4)
                                {
                                    if (tiles[3].value == 2)
                                    {
                                        int check17 = pair_check(tiles[6], tiles[7]);
                                        if (check17 == 4)
                                        {
                                            if (tiles[6].value == 1 || tiles[6].value == 4)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.71 - Hand combination not correct!");
                                        }
                                    }
                                    else if (tiles[3].value == 1)
                                    {
                                        int check17 = pair_check(tiles[6], tiles[7]);
                                        if (check17 == 4)
                                        {
                                            if (tiles[6].value == 5 || tiles[6].value == 8)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.71 - Hand combination not correct!");
                                        }
                                    }
                                    else
                                    {
                                        print("136.71 - Hand combination not correct!");
                                    }
                                }
                                else
                                {
                                    int check17 = pair_check(tiles[3], tiles[4]);
                                    if (check17 == 4)
                                    {
                                        if (tiles[3].value == 1 || tiles[3].value == 4)
                                        {
                                            int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
                                            if (check18 == 4 && tiles[5].value == 2)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else if (tiles[3].value == 2 || tiles[3].value == 5)
                                        {
                                            int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
                                            if (check18 == 4 && tiles[5].value == 1)
                                            {
                                                gamedata.winnable = 1; // transferhand(gamedata, 14);
                                                gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                            }
                                            else
                                            {
                                                print("136.71 - Hand combination not correct!");
                                            }
                                        }
                                        else
                                        {
                                            print("136.71 - Hand combination not correct!");
                                        }
                                    }
                                    else
                                    {
                                        print("136.71 - Hand combination not correct!");
                                    }
                                }
                            }
                            else
                            {
                                print("136.71 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            int check17 = pair_check(tiles[0], tiles[1]);
                            if (check17 == 4)
                            {
                                int check18 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
                                int check19 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
                                if (check18 == 4 && check19 == 4)
                                {
                                    if (tiles[0].value == 1 || tiles[0].value == 4)
                                    {
                                        if (tiles[2].value == 2 && tiles[5].value == 3)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                        }
                                        else if (tiles[2].value == 3 && tiles[5].value == 2)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                        }
                                    }
                                    else if (tiles[0].value == 2 || tiles[0].value == 5)
                                    {
                                        if (tiles[2].value == 1 && tiles[5].value == 3)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                        }
                                        else if (tiles[2].value == 3 && tiles[5].value == 1)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                        }
                                    }
                                    else if (tiles[0].value == 3 || tiles[0].value == 6)
                                    {
                                        if (tiles[2].value == 1 && tiles[5].value == 2)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                        }
                                        else if (tiles[2].value == 2 && tiles[5].value == 1)
                                        {
                                            gamedata.winnable = 1; // transferhand(gamedata, 14);
                                            gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                        }
                                    }
                                    else
                                    {
                                        print("136.71 - Hand combination not correct!");
                                    }
                                }
                                else
                                {
                                    print("136.71 - Hand combination not correct!");
                                }
                            }
                            else
                            {
                                print("136.71 - Hand combination not correct!");
                            }
                        }
                    }
                    else if (check15 == 3)
                    {
                        int check16 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
                        int check17 = pair_pung_chow(tiles[3], tiles[4], tiles[5]);
                        int check18 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
                        if (check16 == 4 && check17 == 4)
                        {
                            if (check18 == 4)
                            {
                                if (tiles[0].value == 1)
                                {
                                    if (tiles[3].value == 2 && tiles[6].value == 3)
                                    {
                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                        // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
                                        gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                    }
                                    else if (tiles[3].value == 3 && tiles[6].value == 2)
                                    {
                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                        // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
                                        gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                    }
                                    else
                                    {
                                        print("136.72 - Hand combination not correct!");
                                    }
                                }
                                else if (tiles[0].value == 2)
                                {
                                    if (tiles[3].value == 1 && tiles[6].value == 3)
                                    {
                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                        // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
                                        gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                    }
                                    else if (tiles[3].value == 3 && tiles[6].value == 1)
                                    {
                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                        // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
                                        gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                    }
                                    else
                                    {
                                        print("136.23 - Hand combination not correct!");
                                    }
                                }
                                else if (tiles[0].value == 3)
                                {
                                    if (tiles[3].value == 1 && tiles[6].value == 2)
                                    {
                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                        // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
                                        gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                    }
                                    else if (tiles[3].value == 2 && tiles[6].value == 1)
                                    {
                                        gamedata.winnable = 1; // transferhand(gamedata, 14);
                                        // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
                                        gamedata.score_check.insert(gamedata.score_check.begin(), 38);
                                    }
                                    else
                                    {
                                        print("136.24 - Hand combination not correct!");
                                    }
                                }
                                else
                                {
                                    print("136.25 - Hand combination not correct!");
                                }
                            }
                            else
                            {
                                print("136.25 - Hand combination not correct!");
                            }
                        }
                        else
                        {
                            print("136.26 - Hand combination not correct!");
                        }
                    }
                    else
                    {
                        print("150 - Hand combination not correct!");
                    }
                }
            }
        }
    }
}

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