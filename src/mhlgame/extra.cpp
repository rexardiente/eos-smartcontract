


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

// int mhlgame::pair_pung_chow(mhltile tile1, mhltile tile2, mhltile tile3)
// {
//     if (tile1.suit == tile2.suit && tile1.value == tile2.value)
//     {
//         // if (tile2.suit == tile3.suit && tile2.value == tile3.value)
//         // {
//         //     return 2;
//         // }
//         // else
//         // {
//         // if (tile2.value < 10 && tile3.value < 10)
//         // {
//         if (tile2.value == tile3.value && tile2.suit == tile3.suit)
//         {
//             return 2;
//         }
//         else
//         {
//             return 1;
//         }
//         // }
//         // else
//         // {
//         //     if (tile2.value == tile3.value)
//         //     {
//         //         if (tile2.suit == tile3.suit)
//         //         {
//         //             return 2;
//         //         }
//         //         else
//         //         {
//         //             return 1;
//         //         }
//         //     }
//         //     else
//         //     {
//         //         return 1;
//         //     }
//         // }
//         // }
//     }
//     else if (tile1.suit == tile2.suit && tile2.suit == tile3.suit)
//     {
//         if ((tile1.value + 1) == tile2.value && (tile1.value + 2) == tile3.value)
//         {
//             return 3;
//         }
//         else if ((tile1.value + 3) == tile2.value && (tile2.value + 3) == tile3.value)
//         {
//             return 4;
//         }
//         else
//         {
//             return 5;
//         }
//     }
//     else
//         return 0;
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

// int mhlgame::five_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5) // !!!!!
// {
//     if ((pair_pung_chow(tile2, tile3, tile4)) == 2)
//     {
//         if ((pair_pung_chow(tile1, tile3, tile5)) == 3)
//         {
//             return 1;
//         }
//         else if ((pair_check(tile4, tile5)) == 1 && (pair_check(tile1, tile2)) == 2)
//         {
//             return 3;
//         }
//         else if ((pair_check(tile1, tile4)) == 1 && (pair_check(tile4, tile5)) == 2)
//         {
//             return 3;
//         }
//         else
//         {
//             return 0;
//         }
//     }
//     else if ((pair_pung_chow(tile3, tile4, tile5)) == 2)
//     {
//         if ((pair_pung_chow(tile1, tile2, tile3)) == 3)
//         {
//             return 1;
//         }
//         else
//         {
//             return 0;
//         }
//     }
//     else if ((pair_pung_chow(tile1, tile2, tile4)) == 3)
//     {
//         if ((pair_check(tile3, tile5)) == 2)
//         {
//             return 2;
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

// int mhlgame::six_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5, mhltile tile6) // !!!!!!!
// {
//     if ((pair_pung_chow(tile1, tile2, tile3)) == 2)
//     {
//         if ((pair_pung_chow(tile4, tile5, tile6)) == 2)
//         {
//             return 4;
//         }
//         else if ((pair_pung_chow(tile4, tile5, tile6)) == 3)
//         {
//             return 5;
//         }
//         else
//         {
//             return 0;
//         }
//     }
//     else if ((pair_pung_chow(tile1, tile2, tile3)) == 3)
//     {
//         if ((pair_pung_chow(tile4, tile5, tile6)) == 2)
//         {
//             return 6;
//         }
//         else if ((pair_pung_chow(tile4, tile5, tile6)) == 3)
//         {
//             return 7;
//         }
//         else
//         {
//             return 0;
//         }
//     }
//     else if ((pair_pung_chow(tile1, tile2, tile3)) == 1)
//     {
//         if ((pair_check(tile3, tile4)) == 1 && (pair_check(tile5, tile6)) == 1)
//         {
//             if ((pair_pung_chow(tile1, tile3, tile5)) == 3)
//             {
//                 return 8;
//             }
//             else if (pair_check(tile3, tile5) == 2)
//             {
//                 return 9;
//             }
//             else
//             {
//                 return 10;
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

// void mhlgame::two_rem(mhlgamedata &gamedata, vector<mhltile> tiles)
// {
//     if ((pair_check(tiles[0], tiles[1])) == 1)
//     {

//         gamedata.pair_count += 1;
//         gamedata.winnable = 1; // transferhand(gamedata, 2);
//         // print(tiles[0].value);
//         // print("Congratulations, you won!");
//         // print(tiles[1].value);
//     }
//     else
//     {
//         print("1 - Hand combination not correct!");
//     }
// }

// void mhlgame::five_rem(mhlgamedata &gamedata, vector<mhltile> tiles) // AAAKK
// {
//     int check1 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
//     if (check1 == 2)
//     {
//         int check2 = pair_check(tiles[3], tiles[4]);
//         if (check2 == 1)
//         {
//             gamedata.pair_count += 1;
//             gamedata.eye_idx = 3;
//             pung_chow(gamedata, check1);
//             gamedata.winnable = 1; // transferhand(gamedata, 5);
//         }
//         else if (check2 == 2 && pair_pung_chow(tiles[2], tiles[3], tiles[4]) == 3)
//         {
//             gamedata.chow_count += 1;
//             gamedata.pair_count += 1;
//             gamedata.eye_idx = 0;
//             gamedata.winnable = 1; // transferhand(gamedata, 5);
//         }
//         else
//         {
//             print("4 - Hand combination not correct!");
//         }
//     }
//     else if (check1 == 3)
//     {
//         if ((pair_check(tiles[3], tiles[4])) == 1)
//         {
//             gamedata.chow_count += 1;
//             gamedata.pair_count += 1;
//             gamedata.eye_idx = 3;
//             gamedata.winnable = 1; // transferhand(gamedata, 5);
//         }
//         else
//         {
//             print("3 - Hand combination not correct!");
//         }
//     }
//     else if (check1 == 1) //
//     {
//         int check2 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
//         if (check2 == 2 || check2 == 3)
//         {
//             gamedata.pair_count += 1;
//             gamedata.eye_idx = 0;
//             pung_chow(gamedata, check2);
//             gamedata.winnable = 1; // transferhand(gamedata, 5);
//         }
//         else
//         {
//             print("2 - Hand combination not correct!");
//         }
//     }
//     else // A33334
//     {
//         if ((five_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4])) == 1)
//         {
//             gamedata.chow_count += 1;
//             gamedata.pair_count += 1;
//             gamedata.eye_idx = 3;
//             gamedata.winnable = 1; // transferhand(gamedata, 5);
//         }
//         else
//         {
//             print("5 - Hand combination not correct!");
//         }
//     }
// }

// void mhlgame::eight_rem(mhlgamedata &gamedata, vector<mhltile> tiles)
// {
//     int check1 = six_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4], tiles[5]);
//     if (check1 > 3 && check1 < 9)
//     {
//         int check2 = pair_check(tiles[6], tiles[7]);
//         if (check2 == 1)
//         {
//             gamedata.pair_count += 1;
//             gamedata.eye_idx = 6;
//             pung_chow(gamedata, check1);
//             gamedata.winnable = 1; // transferhand(gamedata, 8);
//         }
//         else if (check2 == 2 && pair_pung_chow(tiles[3], tiles[4], tiles[5]) == 2)
//         {
//             if (pair_pung_chow(tiles[5], tiles[6], tiles[7]) == 3)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.chow_count += 1;
//                 pung_chow(gamedata, check1);
//                 gamedata.eye_idx = 3;
//                 gamedata.pung_count -= 1;
//                 gamedata.winnable = 1; // transferhand(gamedata, 8);
//             }
//             else
//             {
//                 print("6 - Hand combination not correct!");
//             }
//         }
//         else
//         {
//             print("7 - Hand combination not correct!");
//         }
//     }
//     else if (check1 == 9 && pair_check(tiles[6], tiles[7]) == 1)
//     {
//         if (pair_pung_chow(tiles[2], tiles[4], tiles[6]) == 3)
//         {
//             gamedata.pair_count += 1;
//             gamedata.eye_idx = 0;
//             gamedata.chow_count += 2;
//             gamedata.winnable = 1; // transferhand(gamedata, 8);
//         }
//         else
//         {
//             print("8 - Hand combination not correct!");
//         }
//     }
//     else
//     {
//         int check2 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
//         if (check2 == 2 || check2 == 3)
//         {
//             int check3 = five_tile_check(tiles[3], tiles[4], tiles[5], tiles[6], tiles[7]);
//             if (check3 == 1)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.eye_idx = 5;
//                 gamedata.chow_count += 1;
//                 pung_chow(gamedata, check2);
//                 gamedata.winnable = 1; // transferhand(gamedata, 8);
//             }
//             else if (check3 == 2 && check2 == 2) // 11112233
//             {
//                 if (tiles[2].value == tiles[3].value)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 0;
//                     gamedata.chow_count += 2;
//                     gamedata.winnable = 1; // transferhand(gamedata, 8);
//                 }
//                 else
//                 {
//                     print("15 - Hand combination not correct!");
//                 }
//             }
//             else if (check3 == 3 && check2 == 2)
//             {
//                 if (pair_pung_chow(tiles[2], tiles[3], tiles[7]) == 3)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 0;
//                     gamedata.chow_count += 1;
//                     gamedata.pung_count += 1;
//                     gamedata.winnable = 1; // transferhand(gamedata, 8);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                 }
//                 else
//                 {
//                     print("9 - Hand combination not correct!");
//                 }
//             }
//         }
//         else if (check2 == 1)
//         {
//             int check3 = six_tile_check(tiles[2], tiles[3], tiles[4], tiles[5], tiles[6], tiles[7]);
//             if (check3 > 3 && check3 < 9)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.eye_idx = 0;
//                 pung_chow(gamedata, check3);
//                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//             }
//             else if (check3 == 10 && pair_pung_chow(tiles[0], tiles[2], tiles[4]) == 3)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.eye_idx = 6;
//                 gamedata.chow_count += 2;
//                 gamedata.winnable = 1; // transferhand(gamedata, 8);
//             }
//             else
//             {
//                 print("10 - Hand combination not correct!");
//             }
//         }
//         else
//         {
//             int check3 = five_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4]);
//             if (check3 == 1)
//             {
//                 int check4 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
//                 if (check4 == 2 || check4 == 3)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 2;
//                     gamedata.chow_count += 1;
//                     pung_chow(gamedata, check4);
//                     gamedata.winnable = 1; // transferhand(gamedata, 8);
//                 }
//                 else
//                 {
//                     print("11 - Hand combination not correct!");
//                 }
//             }
//             else if (check3 == 2 && pair_pung_chow(tiles[2], tiles[4], tiles[5]) == 3)
//             {
//                 int check10 = pair_check(tiles[6], tiles[7]);
//                 if (check10 == 1)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 6;
//                     gamedata.chow_count += 2;
//                     gamedata.winnable = 1; // transferhand(gamedata, 8);
//                 }
//                 else
//                 {
//                     print("12 - Hand combination not correct!");
//                 }
//             }
//             else if (check3 == 3)
//             {
//                 if (pair_pung_chow(tiles[0], tiles[4], tiles[5]) == 3) // 12222334
//                 {
//                     int check10 = pair_check(tiles[6], tiles[7]);
//                     if (check10 == 1)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 6;
//                         gamedata.chow_count += 1;
//                         gamedata.pung_count += 1;
//                         gamedata.winnable = 1; // transferhand(gamedata, 8);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                     }
//                     else if (check10 == 2 && pair_pung_chow(tiles[3], tiles[6], tiles[7]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 1;
//                         gamedata.chow_count += 2;
//                         gamedata.winnable = 1; // transferhand(gamedata, 8);
//                     }
//                     else
//                     {
//                         print("13 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     print("14 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 print("15 - Hand combination not correct!");
//             }
//         }
//     }
// }

// void mhlgame::eleven_rem(mhlgamedata &gamedata, vector<mhltile> tiles) // 11Rem2nd
// {
//     int check1 = six_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4], tiles[5]);
//     if (check1 > 3 && check1 < 9)
//     {
//         int check2 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
//         int check10 = pair_check(tiles[9], tiles[10]);
//         if (check2 == 3 && check10 == 1)
//         {
//             gamedata.pair_count += 1;
//             gamedata.eye_idx = 9;
//             gamedata.chow_count += 1;
//             pung_chow(gamedata, check1);
//             gamedata.winnable = 1; // transferhand(gamedata, 11);
//         }
//         else if (check2 == 2)
//         {
//             if (check10 == 1)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.pung_count += 1;
//                 gamedata.eye_idx = 9;
//                 pung_chow(gamedata, check1);
//                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//             }
//             else if (check10 == 2 && pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.eye_idx = 6;
//                 gamedata.chow_count += 1;
//                 pung_chow(gamedata, check1);
//                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//             }
//             else
//             {
//                 print("16 - Hand combination not correct!");
//             }
//         }
//         else if (check2 == 1)
//         {
//             if (pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 2)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.eye_idx = 6;
//                 gamedata.pung_count += 1;
//                 pung_chow(gamedata, check1);
//                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//             }
//             else if (pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.eye_idx = 6;
//                 gamedata.chow_count += 1;
//                 pung_chow(gamedata, check1);
//                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//             }
//             else
//             {
//                 print("17 - Hand combination not correct!");
//             }
//         }
//         else
//         {
//             int check3 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
//             if (check3 == 1)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.eye_idx = 8;
//                 gamedata.chow_count += 1;
//                 pung_chow(gamedata, check1);
//                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//             }
//             else if (check3 == 3 && pair_pung_chow(tiles[3], tiles[4], tiles[5]) == 2) //111222 33334/34444
//             {
//                 if (pair_pung_chow(tiles[5], tiles[6], tiles[10]) == 3)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 3;
//                     gamedata.chow_count += 1;
//                     pung_chow(gamedata, check1);
//                     gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                 }
//                 else
//                 {
//                     print("18 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 print("19 - Hand combination not correct!");
//             }
//         }
//     }
//     else if (check1 == 9 && pair_check(tiles[6], tiles[7]) == 1)
//     {
//         if (pair_pung_chow(tiles[2], tiles[4], tiles[6]) == 3)
//         {
//             int check2 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
//             if (check2 == 2 || check2 == 3)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.eye_idx = 0;
//                 gamedata.chow_count += 2;
//                 pung_chow(gamedata, check2);
//                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//             }
//             else
//             {
//                 print("20 - Hand combination not correct!");
//             }
//         }
//         else
//         {
//             print("21 - Hand combination not correct!");
//         }
//     }
//     else
//     {
//         int check2 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
//         if (check2 == 2 || check2 == 3)
//         {
//             int check3 = five_tile_check(tiles[3], tiles[4], tiles[5], tiles[6], tiles[7]);
//             if (check3 == 1)
//             {
//                 int check4 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
//                 if (check4 == 2 || check4 == 3)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 5;
//                     gamedata.chow_count += 1;
//                     pung_chow(gamedata, check2);
//                     pung_chow(gamedata, check4);
//                     gamedata.winnable = 1; // transferhand(gamedata, 11);
//                 }
//                 else
//                 {
//                     print("22 - Hand combination not correct!");
//                 }
//             }
//             else if (check3 == 2 && pair_pung_chow(tiles[5], tiles[7], tiles[8]) == 3) // 111 233445
//             {
//                 int check4 = pair_check(tiles[9], tiles[10]);
//                 if (check4)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 9;
//                     gamedata.chow_count += 2;
//                     pung_chow(gamedata, check2);
//                     gamedata.winnable = 1; // transferhand(gamedata, 11);
//                 }
//                 else if (check4 == 2 && check2 == 2)
//                 {
//                     if (pair_check(tiles[2], tiles[3]) == 1 && pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 0;
//                         gamedata.chow_count += 3;
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else
//                     {
//                         print("23 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     print("23.5 - Hand combination not correct!");
//                 }
//             }
//             else if (check3 == 3 && pair_pung_chow(tiles[3], tiles[7], tiles[8]) == 3) // 333 45555 6
//             {
//                 int check4 = pair_check(tiles[9], tiles[10]);
//                 if (check4 == 1)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 9;
//                     gamedata.chow_count += 1;
//                     gamedata.pung_count += 1;
//                     pung_chow(gamedata, check2);
//                     gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                 }
//                 else if (check4 == 2 && pair_pung_chow(tiles[6], tiles[9], tiles[10]) == 3)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 4;
//                     gamedata.chow_count += 2;
//                     pung_chow(gamedata, check2);
//                     gamedata.winnable = 1; // transferhand(gamedata, 11);
//                 }
//                 else if (check2 == 2 && pair_pung_chow(tiles[2], tiles[3], tiles[7]) == 3) // 222
//                 {
//                     int check5 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 0;
//                         gamedata.chow_count += 2;
//                         pung_chow(gamedata, check5);
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else
//                     {
//                         print("24 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     print("25 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 print("25.5 - Hand combination not correct!");
//             }
//         }
//         else if (check2 == 1)
//         {
//             int check3 = six_tile_check(tiles[2], tiles[3], tiles[4], tiles[5], tiles[6], tiles[7]);
//             if (check3 > 3 && check3 < 9)
//             {
//                 int check4 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
//                 if (check4 == 2 || check4 == 3)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 0;
//                     pung_chow(gamedata, check3);
//                     pung_chow(gamedata, check4);
//                     gamedata.winnable = 1; // transferhand(gamedata, 11);
//                 }
//                 else
//                 {
//                     print("26 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 int check4 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
//                 if (check4 == 2 || check4 == 3)
//                 {
//                     int check5 = five_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9]);
//                     if (check5 == 2 && pair_pung_chow(tiles[7], tiles[9], tiles[10]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 0;
//                         gamedata.chow_count += 2;
//                         pung_chow(gamedata, check4);
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else if (check5 == 3 && pair_pung_chow(tiles[5], tiles[9], tiles[10]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 0;
//                         gamedata.chow_count += 1;
//                         gamedata.pung_count += 1;
//                         pung_chow(gamedata, check4);
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                     }
//                     else
//                     {
//                         print("27 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     int check5 = five_tile_check(tiles[2], tiles[3], tiles[4], tiles[5], tiles[6]); // 11 23334
//                     if (check5 == 1)
//                     {
//                         if (pair_check(tiles[7], tiles[8]) == 1 && pair_check(tiles[9], tiles[10]) == 1)
//                         {
//                             if (pair_pung_chow(tiles[5], tiles[7], tiles[9]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 0;
//                                 gamedata.chow_count += 3;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//                             }
//                             else if (pair_pung_chow(tiles[1], tiles[5], tiles[7]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 9;
//                                 gamedata.chow_count += 3;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//                             }

//                             else
//                             {
//                                 print("28 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             print("29 - Hand combination not correct!");
//                         }
//                     }
//                     else if (check5 == 2 && pair_pung_chow(tiles[4], tiles[6], tiles[7]) == 3)
//                     {
//                         int check6 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
//                         if (check6 == 2 || check6 == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 0;
//                             gamedata.chow_count += 2;
//                             pung_chow(gamedata, check6);
//                             gamedata.winnable = 1; // transferhand(gamedata, 11);
//                         }
//                         else
//                         {
//                             print("30 - Hand combination not correct!");
//                         }
//                     }
//                     else if (check5 == 3) // 11 22223 4 567
//                     {
//                         int check6 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
//                         if (pair_pung_chow(tiles[2], tiles[6], tiles[7]) == 3)
//                         {
//                             if (check6 == 2 || check6 == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 0;
//                                 gamedata.chow_count += 1;
//                                 gamedata.pung_count += 1;
//                                 pung_chow(gamedata, check6);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                             }
//                             else
//                             {
//                                 print("31 - Hand combination not correct!");
//                             }
//                         }
//                         else if (pair_check(tiles[6], tiles[7]) == 1 && pair_pung_chow(tiles[1], tiles[3], tiles[7]) == 3)
//                         {
//                             if (check6 == 2 || check6 == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 4;
//                                 gamedata.chow_count += 1;
//                                 gamedata.pung_count += 1;
//                                 pung_chow(gamedata, check6);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//                             }
//                             else
//                             {
//                                 print("32 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                     else
//                     {
//                         print("33 - Hand combination not correct!");
//                     }
//                 }
//             }
//         }
//         else
//         {
//             int check3 = five_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4]);
//             if (check3 == 1)
//             {
//                 int check4 = six_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
//                 if (check4 > 3 && check4 < 9)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 3;
//                     gamedata.chow_count += 1;
//                     pung_chow(gamedata, check4);
//                     gamedata.winnable = 1; // transferhand(gamedata, 11);
//                 }
//                 else if (check4 == 10) //  12223 3344 66
//                 {
//                     if (pair_pung_chow(tiles[3], tiles[5], tiles[7]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 9;
//                         gamedata.chow_count += 3;
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else
//                     {
//                         print("34 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     int check5 = five_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9]);
//                     if (check5 == 2 && pair_pung_chow(tiles[7], tiles[9], tiles[10]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 3;
//                         gamedata.chow_count += 3;
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else if (check5 == 3 && pair_pung_chow(tiles[0], tiles[4], tiles[5]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 3;
//                         gamedata.chow_count += 2;
//                         gamedata.pung_count += 1;
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else
//                     {
//                         print("35 - Hand combination not correct!");
//                     }
//                 }
//             }
//             else if (check3 == 2 && pair_pung_chow(tiles[2], tiles[4], tiles[5]) == 3)
//             {
//                 int check4 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
//                 int check10 = pair_check(tiles[9], tiles[10]);
//                 if (check4 == 3 && check10 == 1)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 9;
//                     gamedata.chow_count += 3;
//                     gamedata.winnable = 1; // transferhand(gamedata, 11);
//                 }
//                 else if (check4 == 2)
//                 {
//                     if (check10 == 1)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 9;
//                         gamedata.chow_count += 2;
//                         gamedata.pung_count += 1;
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else if (check10 == 2 && pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 6;
//                         gamedata.chow_count += 3;
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else
//                     {
//                         print("36 - Hand combination not correct!");
//                     }
//                 }
//                 else if (check4 == 1)
//                 {
//                     int check5 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 6;
//                         gamedata.chow_count += 2;
//                         pung_chow(gamedata, check5);
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else
//                     {
//                         print("37 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     int check5 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
//                     if (check5 == 1)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 8;
//                         gamedata.chow_count += 3;
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else
//                     {
//                         print("38 - Hand combination not correct!");
//                     }
//                 }
//             }
//             else if (check3 == 3 && pair_pung_chow(tiles[0], tiles[4], tiles[5]) == 3)
//             {
//                 int check4 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
//                 int check10 = pair_check(tiles[9], tiles[10]);
//                 if (check4 == 3 && check10 == 1)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 9;
//                     gamedata.chow_count += 3;
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                     gamedata.winnable = 1; // transferhand(gamedata, 11);
//                 }
//                 else if (check4 == 2)
//                 {
//                     if (check10 == 1)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 9;
//                         gamedata.chow_count += 2;
//                         gamedata.pung_count += 1;
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                     }
//                     else if (check10 == 2 && pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 6;
//                         gamedata.chow_count += 3;
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                     }
//                     else
//                     {
//                         print("39 - Hand combination not correct!");
//                     }
//                 }
//                 else if (check4 == 1) // 1[0] 2[1] 2[2] 2[3] 2[4] 3[5] 3[6] 3[7] 4[8] 4[9] 5[10]
//                 {
//                     int check5 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 6;
//                         gamedata.chow_count += 2;
//                         pung_chow(gamedata, check5);
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else if (check5 == 1 && pair_pung_chow(tiles[3], tiles[7], tiles[9]) == 3)
//                     {
//                         if (check10 == 2 && pair_pung_chow(tiles[6], tiles[8], tiles[10]) == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 2;
//                             gamedata.chow_count += 3;
//                             gamedata.winnable = 1; // transferhand(gamedata, 11);
//                         }
//                         else
//                         {
//                             print("40 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("41 - Hand combination not correct!");
//                     }
//                 }
//                 else // 12222 3 34 666 / 789
//                 {
//                     int check5 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
//                     if (check5 == 1)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 8;
//                         gamedata.chow_count += 3;
//                         gamedata.winnable = 1; // transferhand(gamedata, 11);
//                     }
//                     else
//                     {
//                         int check6 = pair_check(tiles[6], tiles[7]);
//                         if (check6 == 2 && pair_pung_chow(tiles[3], tiles[6], tiles[7]) == 3)
//                         {
//                             int check7 = pair_pung_chow(tiles[8], tiles[9], tiles[10]);
//                             if (check7 == 2 || check7 == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 1;
//                                 gamedata.chow_count += 2;
//                                 pung_chow(gamedata, check7);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//                             }
//                             else
//                             {
//                                 print("42 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             print("43 - Hand combination not correct!");
//                         }
//                     }
//                 }
//             }
//             else
//             {
//                 print("44 - Hand combination not correct!");
//             }
//         }
//     }
// }

// void mhlgame::fourteen_rem(mhlgamedata &gamedata, vector<mhltile> tiles) // 14Rem
// {
//     int check1 = six_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4], tiles[5]);
//     if (check1 == 9 || check1 == 10)
//     {
//         int check2 = six_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10], tiles[11]);
//         if (check2 > 7 && (pair_check(tiles[12], tiles[13])) == 1)
//         {
//             gamedata.pair_count += 7;
//             gamedata.winnable = 1; // transferhand(gamedata, 14);
//         }
//         else
//         {
//             if ((pair_check(tiles[6], tiles[7])) == 1 && (pair_pung_chow(tiles[2], tiles[4], tiles[6])) == 3)
//             {
//                 int check3 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                 if (check3 > 3 && check3 < 9)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 0;
//                     gamedata.chow_count += 2;
//                     pung_chow(gamedata, check3);
//                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                 }
//                 else
//                 {
//                     int check4 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                     if (check4 == 3 && (pair_pung_chow(tiles[8], tiles[12], tiles[13])) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 0;
//                         gamedata.chow_count += 3;
//                         gamedata.pung_count += 1;
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                     }
//                     else if (check4 == 2 && (pair_pung_chow(tiles[10], tiles[12], tiles[13])) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 0;
//                         gamedata.chow_count += 4;
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("56 - Hand combination not correct!");
//                     }
//                 }
//             }
//             else
//             {
//                 print("57 - Hand combination not correct!");
//             }
//         }
//     }
//     else if (check1 == 8)
//     {
//         int check2 = six_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10], tiles[11]);
//         if (check2 > 3 && check2 < 8)
//         {
//             if ((pair_check(tiles[12], tiles[13])) == 1)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.eye_idx = 12;
//                 gamedata.chow_count += 2;
//                 pung_chow(gamedata, check2);
//                 gamedata.winnable = 1; // transferhand(gamedata, 11);
//             }
//             else if (pair_check(tiles[12], tiles[13]) == 2 && (pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3))
//             {
//                 if (check2 == 4 || check2 == 6)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 9;
//                     gamedata.pung_count -= 1;
//                     gamedata.chow_count += 3;
//                     pung_chow(gamedata, check2);
//                     gamedata.winnable = 1; // transferhand(gamedata, 11);
//                 }
//                 else
//                 {
//                     print("58 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 print("59 - Hand combination not correct!");
//             }
//         }
//         else if (check2 > 7 && check2 < 11)
//         {
//             if ((pair_check(tiles[12], tiles[13])) == 1)
//             {
//                 gamedata.pair_count += 7;
//                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//             }
//             else
//             {
//                 print("60 - Hand combination not correct!");
//             }
//         }
//         else
//         {
//             int check3 = pair_pung_chow(tiles[6], tiles[7], tiles[8]); // 112233
//             if (check3 == 2 || check3 == 3)
//             {
//                 int check4 = pair_check(tiles[9], tiles[10]);
//                 if (check4 == 1)
//                 {
//                     int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 9;
//                         gamedata.chow_count += 2;
//                         pung_chow(gamedata, check3);
//                         pung_chow(gamedata, check5);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else if (check5 == 1 && check3 == 2) // 112233 444 55 66 7
//                     {
//                         if (pair_pung_chow(tiles[8], tiles[9], tiles[11]) == 3 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 6;
//                             gamedata.chow_count += 4;
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             print("61 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("62 - Hand combination not correct!");
//                     }
//                 }
//                 else if (check4 == 2)
//                 {
//                     if (check3 == 2 && (pair_pung_chow(tiles[8], tiles[9], tiles[10])) == 3)
//                     {
//                         int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                         if (check5 == 2 || check5 == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 6;
//                             gamedata.chow_count += 3;
//                             pung_chow(gamedata, check5);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             print("63 - Hand combination not correct!");
//                         }
//                     }
//                     else if ((pair_pung_chow(tiles[11], tiles[12], tiles[13])) == 2 && (pair_pung_chow(tiles[9], tiles[10], tiles[11])) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 12;
//                         gamedata.chow_count += 3;
//                         pung_chow(gamedata, check4);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("64 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     int check5 = five_tile_check(tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]); //112233 444 56667  112233 456 78999
//                     if (check5 == 1)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 11;
//                         gamedata.chow_count += 3;
//                         pung_chow(gamedata, check3);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else if (check5 == 3 && check3 == 2) // 112233 555 66667 67777
//                     {
//                         if (pair_pung_chow(tiles[8], tiles[9], tiles[13]) == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 6;
//                             gamedata.chow_count += 3;
//                             gamedata.pung_count += 1;
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                         }
//                         else
//                         {
//                             print("65 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("66 - Hand combination not correct!");
//                     }
//                 }
//             }
//             else if (check3 == 1) // 11223344
//             {
//                 int check4 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                 if (check4 > 3 && check4 < 8)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 6;
//                     gamedata.chow_count += 2;
//                     pung_chow(gamedata, check4);
//                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                 }
//                 else
//                 {
//                     int check5 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                     if (check5 == 2 && pair_pung_chow(tiles[9], tiles[11], tiles[13]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 6;
//                         gamedata.chow_count += 4;
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else if (check5 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 6;
//                         gamedata.pung_count += 1;
//                         gamedata.chow_count += 3;
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                     }
//                     else
//                     {
//                         print("67 - Hand combination not correct!");
//                     }
//                 }
//             }
//             else
//             {
//                 int check4 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]); // 112233 56667/56777
//                 int check10 = pair_check(tiles[12], tiles[13]);
//                 if (check4 == 1)
//                 {
//                     int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 8;
//                         gamedata.chow_count += 3;
//                         pung_chow(gamedata, check5);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("68 - Hand combination not correct!");
//                     }
//                 }
//                 else if (check4 == 2 && pair_pung_chow(tiles[8], tiles[10], tiles[11]) == 3)
//                 {
//                     if (check10 == 1)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 12;
//                         gamedata.chow_count += 4;
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("69 - Hand combination not correct!");
//                     }
//                 }
//                 else if (check4 == 3 && pair_pung_chow(tiles[6], tiles[10], tiles[11]) == 3) // 112233 11112 3 34/44 /12222 3 34/44
//                 {
//                     if (check10 == 1)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 12;
//                         gamedata.chow_count += 3;
//                         gamedata.pung_count += 1;
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                     }
//                     else if (check10 == 2 && pair_pung_chow(tiles[9], tiles[12], tiles[13]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 7;
//                         gamedata.chow_count += 4;
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("70 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     print("71 - Hand combination not correct!");
//                 }
//             }
//         }
//     }
//     else if (check1 > 3 && check1 < 8)
//     {
//         int check2 = six_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10], tiles[11]);
//         if (check2 > 3 && check2 < 9)
//         {
//             int check3 = pair_check(tiles[12], tiles[13]);
//             if (check3 == 1)
//             {
//                 gamedata.pair_count += 1;
//                 gamedata.eye_idx = 12;
//                 pung_chow(gamedata, check1);
//                 pung_chow(gamedata, check2);
//                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//             }
//             else if (check3 == 2)
//             {
//                 if (pair_pung_chow(tiles[9], tiles[10], tiles[11]) == 2 && pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 9;
//                     pung_chow(gamedata, check1);
//                     pung_chow(gamedata, check2);
//                     gamedata.pung_count -= 1;
//                     gamedata.chow_count += 1;
//                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                 }
//                 else
//                 {
//                     print("72 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 print("73 - Hand combination not correct!");
//             }
//         }
//         else
//         {

//             int check3 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
//             if (check3 == 2 || check3 == 3)
//             {
//                 int check4 = pair_check(tiles[9], tiles[10]);
//                 if (check4 == 1)
//                 {
//                     int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 9;
//                         pung_chow(gamedata, check1);
//                         pung_chow(gamedata, check3);
//                         pung_chow(gamedata, check5);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("74 - Hand combination not correct!");
//                     }
//                 }
//                 else if (check4 == 2)
//                 {
//                     if (pair_pung_chow(tiles[6], tiles[7], tiles[8]) == 2 && pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 3)
//                     {
//                         int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                         if (check5 == 2 || check5 == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 6;
//                             gamedata.chow_count += 1;
//                             pung_chow(gamedata, check1);
//                             pung_chow(gamedata, check3);
//                             pung_chow(gamedata, check5);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             print("75 - Hand combination not correct!");
//                         }
//                     }
//                     else if (pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 2 && pair_pung_chow(tiles[9], tiles[10], tiles[11]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 12;
//                         gamedata.chow_count += 1;
//                         pung_chow(gamedata, check1);
//                         pung_chow(gamedata, check3);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("76 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     print("77 - Hand combination not correct!");
//                 }
//             }
//             else if (check3 == 1)
//             {
//                 int check4 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                 if (check4 > 3 && check4 < 9)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 6;
//                     pung_chow(gamedata, check1);
//                     pung_chow(gamedata, check4);
//                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                 }
//                 else
//                 {
//                     int check5 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                     if (check5 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 6;
//                         gamedata.chow_count += 2;
//                         pung_chow(gamedata, check1);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else if (check5 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 6;
//                         gamedata.chow_count += 1;
//                         gamedata.pung_count += 1;
//                         pung_chow(gamedata, check1);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                     }
//                     else
//                     {
//                         print("78 - Hand combination not correct!");
//                     }
//                 }
//             }
//             else
//             {
//                 int check4 = pair_check(tiles[6], tiles[7]);
//                 if (check4 == 2)
//                 {
//                     if (pair_pung_chow(tiles[3], tiles[4], tiles[5]) == 2 && pair_pung_chow(tiles[5], tiles[6], tiles[7]) == 3) // 12344456
//                     {
//                         int check5 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                         if (check5 > 3 && check5 < 9)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 3;
//                             gamedata.chow_count += 1;
//                             pung_chow(gamedata, check1);
//                             pung_chow(gamedata, check5);
//                             gamedata.pung_count -= 1;
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             int check6 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                             if (check6 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 3;
//                                 gamedata.chow_count += 2;
//                                 pung_chow(gamedata, check1);
//                                 gamedata.pung_count -= 1;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else if (check6 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 3;
//                                 gamedata.chow_count += 1;
//                                 gamedata.pung_count += 1;
//                                 pung_chow(gamedata, check1);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                             }
//                             else
//                             {
//                                 print("79 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                     else if (pair_pung_chow(tiles[8], tiles[9], tiles[10]) == 2 && pair_pung_chow(tiles[6], tiles[7], tiles[8]) == 3) // 11223345666
//                     {
//                         int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                         if (check5 == 2 || check5 == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 9;
//                             gamedata.chow_count += 1;
//                             pung_chow(gamedata, check1);
//                             pung_chow(gamedata, check5);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             print("80 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("81 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     print("82 - Hand combination not correct!");
//                 }
//             }
//         }
//     }
//     else
//     {
//         int check2 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
//         if (check2 == 2 || check2 == 3)
//         {
//             int check3 = pair_check(tiles[3], tiles[4]);
//             if (check3 == 1)
//             {
//                 int check4 = six_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
//                 if (check4 > 3 && check4 < 9)
//                 {
//                     int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 3;
//                         pung_chow(gamedata, check2);
//                         pung_chow(gamedata, check4);
//                         pung_chow(gamedata, check5);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("83 - Hand combination not correct!");
//                     }
//                 }
//                 else if (check4 == 10 && pair_pung_chow(tiles[3], tiles[5], tiles[7]) == 3)
//                 {
//                     int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 9;
//                         gamedata.chow_count += 2;
//                         pung_chow(gamedata, check2);
//                         pung_chow(gamedata, check5);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("84 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     int check5 = pair_pung_chow(tiles[5], tiles[6], tiles[7]); // 1112256  1235566
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         int check6 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                         if (check6 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 3;
//                             gamedata.chow_count += 2;
//                             pung_chow(gamedata, check2);
//                             pung_chow(gamedata, check5);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else if (check6 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 3;
//                             gamedata.chow_count += 1;
//                             gamedata.pung_count += 1;
//                             pung_chow(gamedata, check2);
//                             pung_chow(gamedata, check5);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                         }
//                         else
//                         {
//                             print("85 - Hand combination not correct!");
//                         }
//                     }
//                     else if (check5 == 1)
//                     {
//                         int check6 = pair_check(tiles[7], tiles[8]); // 111334455
//                         if (check6 == 1 && pair_pung_chow(tiles[3], tiles[5], tiles[7]) == 3)
//                         {
//                             int check7 = pair_pung_chow(tiles[9], tiles[10], tiles[11]);
//                             if (check7 == 3 && pair_check(tiles[12], tiles[13]) == 1)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 12;
//                                 gamedata.chow_count += 3;
//                                 pung_chow(gamedata, check2);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else if (check7 == 2)
//                             {
//                                 if (pair_check(tiles[12], tiles[13]) == 1)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 12;
//                                     gamedata.pung_count += 1;
//                                     gamedata.chow_count += 2;
//                                     pung_chow(gamedata, check2);
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else if (pair_check(tiles[12], tiles[13]) == 2 && pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 9;
//                                     gamedata.chow_count += 3;
//                                     pung_chow(gamedata, check2);
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("86 - Hand combination not correct!");
//                                 }
//                             }
//                             else if (check7 == 1)
//                             {
//                                 int check8 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                                 if (check8 == 2 || check8 == 3)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 9;
//                                     gamedata.chow_count += 2;
//                                     pung_chow(gamedata, check2);
//                                     pung_chow(gamedata, check8);
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("87 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 int check8 = five_tile_check(tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                                 if (check8 == 1)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 11;
//                                     gamedata.chow_count += 3;
//                                     pung_chow(gamedata, check2);
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("88 - Hand combination not correct!");
//                                 }
//                             }
//                         }
//                         else
//                         {
//                             print("89 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("90 - Hand combination not correct!");
//                     }
//                 }
//             }
//             else if (check3 == 2 && check2 == 2) // 11123
//             {
//                 if (pair_pung_chow(tiles[2], tiles[3], tiles[4]) == 3)
//                 {
//                     int check4 = six_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
//                     if (check4 > 3 && check4 < 9)
//                     {
//                         int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                         if (check5 == 2 || check5 == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 0;
//                             gamedata.chow_count += 1;
//                             pung_chow(gamedata, check4);
//                             pung_chow(gamedata, check5);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             print("91 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         int check5 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
//                         if (check5 == 2 || check5 == 3)
//                         {
//                             int check6 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                             if (check6 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 0;
//                                 gamedata.chow_count += 3;
//                                 pung_chow(gamedata, check5);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else if (check6 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.pung_count += 1;
//                                 gamedata.chow_count += 2;
//                                 pung_chow(gamedata, check5);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                             }
//                             else
//                             {
//                                 print("92 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             int check6 = five_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9]);
//                             int check7 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                             if (check6 == 2 && pair_pung_chow(tiles[7], tiles[9], tiles[10]) == 3)
//                             {
//                                 if (check7 == 2 || check7 == 3)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 0;
//                                     gamedata.chow_count += 3;
//                                     pung_chow(gamedata, check7);
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("93 - Hand combination not correct!");
//                                 }
//                             }
//                             else if (check6 == 3 && pair_pung_chow(tiles[5], tiles[9], tiles[10]) == 3)
//                             {
//                                 if (check7 == 2 || check7 == 3)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 0;
//                                     gamedata.pung_count += 1;
//                                     gamedata.chow_count += 2;
//                                     pung_chow(gamedata, check7);
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                     gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                                 }
//                                 else
//                                 {
//                                     print("94 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 print("95 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                 }
//                 else
//                 {
//                     print("96 - Hand combination not correct!");
//                 }
//             }
//             else
//             {
//                 print("97 - Hand combination not correct!");
//             }
//         }
//         else if (check2 == 1) // 11
//         {
//             int check3 = six_tile_check(tiles[2], tiles[3], tiles[4], tiles[5], tiles[6], tiles[7]);
//             if (check3 > 3 && check3 < 9)
//             {
//                 int check4 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                 if (check4 > 3 && check4 < 9)
//                 {
//                     gamedata.pair_count += 1;
//                     gamedata.eye_idx = 0;
//                     pung_chow(gamedata, check3);
//                     pung_chow(gamedata, check4);
//                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                 }
//                 else
//                 {
//                     int check5 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                     if (check5 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 0;
//                         gamedata.chow_count += 2;
//                         pung_chow(gamedata, check3);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else if (check5 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 0;
//                         gamedata.chow_count += 1;
//                         gamedata.pung_count += 1;
//                         pung_chow(gamedata, check3);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                     }
//                     else
//                     {
//                         print("98 - Hand combination not correct!");
//                     }
//                 }
//             }
//             else
//             {
//                 int check4 = pair_pung_chow(tiles[2], tiles[3], tiles[4]); // 11 234
//                 if (check4 == 2 || check4 == 3)
//                 {
//                     int check10 = six_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
//                     if (check10 > 3 && check10 < 9)
//                     {
//                         int check6 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                         if (check6 == 2 || check6 == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 0;
//                             gamedata.chow_count += 2;
//                             pung_chow(gamedata, check4);
//                             pung_chow(gamedata, check6);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             print("99 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         int check5 = five_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9]);
//                         if (check5 == 2 && pair_pung_chow(tiles[7], tiles[9], tiles[10]) == 3)
//                         {
//                             int check6 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                             if (check6 == 2 || check6 == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 0;
//                                 gamedata.chow_count += 2;
//                                 pung_chow(gamedata, check4);
//                                 pung_chow(gamedata, check6);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else
//                             {
//                                 print("99.5 - Hand combination not correct!");
//                             }
//                         }
//                         else if (check5 == 3 && pair_pung_chow(tiles[5], tiles[9], tiles[10]) == 3)
//                         {
//                             int check6 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                             if (check6 == 2 || check6 == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 0;
//                                 gamedata.chow_count += 1;
//                                 gamedata.pung_count += 1;
//                                 pung_chow(gamedata, check4);
//                                 pung_chow(gamedata, check6);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                             }
//                             else
//                             {
//                                 print("100 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             print("101 - Hand combination not correct!");
//                         }
//                     }
//                     // int check5 = five_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9]);
//                     // if (check5 == 2 && pair_pung_chow(tiles[7], tiles[9], tiles[10]) == 3)
//                     // {
//                     //     int check6 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                     //     if (check6 == 2 || check6 == 3)
//                     //     {
//                     //         gamedata.pair_count += 1;
//                     //         gamedata.eye_idx = 0;
//                     //         gamedata.chow_count += 2;
//                     //         pung_chow(gamedata, check4);
//                     //         pung_chow(gamedata, check6);
//                     //         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     //     }
//                     //     else
//                     //     {
//                     //         print("99 - Hand combination not correct!");
//                     //     }
//                     // }
//                     // else if (check5 == 3 && pair_pung_chow(tiles[5], tiles[9], tiles[10]) == 3)
//                     // {
//                     //     int check6 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                     //     if (check6 == 2 || check6 == 3)
//                     //     {
//                     //         gamedata.pair_count += 1;
//                     //         gamedata.eye_idx = 0;
//                     //         gamedata.chow_count += 1;
//                     //         gamedata.pung_count += 1;
//                     //         pung_chow(gamedata, check4);
//                     //         pung_chow(gamedata, check6);
//                     //         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     //         gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                     //     }
//                     //     else
//                     //     {
//                     //         print("100 - Hand combination not correct!");
//                     //     }
//                     // }
//                     // else
//                     // {
//                     //     print("101 - Hand combination not correct!");
//                     // }
//                 }
//                 else
//                 {
//                     int check5 = five_tile_check(tiles[2], tiles[3], tiles[4], tiles[5], tiles[6]);
//                     if (check5 == 1 && pair_pung_chow(tiles[6], tiles[8], tiles[10]) == 3)
//                     {
//                         if (pair_check(tiles[7], tiles[8]) == 1 && pair_check(tiles[9], tiles[10]) == 1)
//                         {
//                             int check6 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                             if (check6 == 2 || check6 == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 0;
//                                 gamedata.chow_count += 3;
//                                 pung_chow(gamedata, check6);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else
//                             {
//                                 print("102 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             print("103 - Hand combination not correct!");
//                         }
//                     }
//                     else if (check5 == 2 && pair_pung_chow(tiles[4], tiles[6], tiles[7]) == 3)
//                     {
//                         int check6 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                         if (check6 > 3 && check6 < 9)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 0;
//                             gamedata.chow_count += 2;
//                             pung_chow(gamedata, check6);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             int check7 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                             if (check7 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 0;
//                                 gamedata.chow_count += 4;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else if (check7 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 0;
//                                 gamedata.chow_count += 3;
//                                 gamedata.pung_count += 1;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else
//                             {
//                                 print("104 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                     else if (check5 == 3 && pair_pung_chow(tiles[2], tiles[6], tiles[7]) == 3)
//                     {
//                         int check6 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                         if (check6 > 3 && check6 < 9)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 0;
//                             gamedata.chow_count += 1;
//                             gamedata.pair_count += 1;
//                             pung_chow(gamedata, check6);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                         }
//                         else
//                         {
//                             int check7 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                             if (check7 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 0;
//                                 gamedata.pair_count += 1;
//                                 gamedata.chow_count += 3;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                             }
//                             else if (check7 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 0;
//                                 gamedata.chow_count += 2;
//                                 gamedata.pung_count += 2;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                             }
//                             else
//                             {
//                                 print("105 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                     else if (tiles[2].value == 9 && tiles[1].suit == tiles[2].suit) // 119
//                     {
//                         int check3 = pair_check(tiles[3], tiles[4]); // 11919
//                         int check4 = pair_check(tiles[5], tiles[6]); // 1191919
//                         if (check3 == 3 && check4 == 3)              // -----
//                         {
//                             int check5 = honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                             if (check5 == 1)
//                             {

//                                 gamedata.eye_idx = 0;
//                                 gamedata.pair_count += 1;
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else
//                             {
//                                 print("106.1 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             print("106.2 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("106.3 - Hand combination not correct!");
//                     }
//                 }
//             }
//         }
//         else
//         {
//             int check3 = five_tile_check(tiles[0], tiles[1], tiles[2], tiles[3], tiles[4]);
//             if (check3 == 1)
//             {
//                 int check4 = six_tile_check(tiles[5], tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
//                 if (check4 > 3 && check4 < 9)
//                 {
//                     int check5 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 2;
//                         gamedata.chow_count += 1;
//                         pung_chow(gamedata, check4);
//                         pung_chow(gamedata, check5);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("107 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     int check5 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         int check6 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                         if (check6 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 2;
//                             gamedata.chow_count += 3;
//                             pung_chow(gamedata, check5);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else if (check6 == 3 && pair_pung_chow(tiles[8], tiles[10], tiles[13]) == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 2;
//                             gamedata.chow_count += 2;
//                             gamedata.pung_count += 1;
//                             pung_chow(gamedata, check5);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                         }
//                         else
//                         {
//                             print("108 - Hand combination not correct!");
//                         }
//                     }
//                     else if (check5 == 1 && pair_check(tiles[7], tiles[8]) == 1) // 12223 33 44
//                     {
//                         if (pair_pung_chow(tiles[3], tiles[5], tiles[7]) == 3)
//                         {
//                             int check6 = pair_pung_chow(tiles[9], tiles[10], tiles[11]);
//                             int check7 = pair_check(tiles[12], tiles[13]);
//                             if (check6 == 3 && check7 == 1)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 12;
//                                 gamedata.chow_count += 4;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else if (check6 == 2)
//                             {
//                                 if (check7 == 1)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 12;
//                                     gamedata.chow_count += 3;
//                                     gamedata.pung_count += 1;
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else if (check7 == 2 && pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 9;
//                                     gamedata.chow_count += 4;
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("109 - Hand combination not correct!");
//                                 }
//                             }
//                             else if (check6 == 1)
//                             {
//                                 int check8 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                                 if (check8 == 2 || check8 == 3)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 9;
//                                     gamedata.chow_count += 3;
//                                     pung_chow(gamedata, check8);
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("110 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 int check8 = five_tile_check(tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                                 if (check8 == 1)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 11;
//                                     gamedata.chow_count += 4;
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("111 -Hand combination not correct!");
//                                 }
//                             }
//                         }
//                         else
//                         {
//                             print("112 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("113 - Hand combination not correct!");
//                     }
//                 }
//             }
//             else if (check3 == 2 && pair_pung_chow(tiles[2], tiles[4], tiles[5]) == 3) // 12233
//             {
//                 int check4 = six_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10], tiles[11]);
//                 if (check4 > 3 && check4 < 9)
//                 {
//                     int check5 = pair_check(tiles[12], tiles[13]);
//                     if (check5 == 1)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 12;
//                         gamedata.chow_count += 2;
//                         pung_chow(gamedata, check4);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else if (check5 == 2 && pair_pung_chow(tiles[9], tiles[10], tiles[11]) == 2)
//                     {
//                         if (pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 9;
//                             gamedata.chow_count += 3;
//                             pung_chow(gamedata, check4);
//                             gamedata.pung_count -= 1;
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             print("114 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("115 - Hand combination not correct!");
//                     }
//                 }
//                 else if (check4 == 9 && pair_check(tiles[12], tiles[13]) == 1)
//                 {
//                     if (pair_pung_chow(tiles[8], tiles[10], tiles[12]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 6;
//                         gamedata.chow_count += 4;
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("116 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     int check5 = pair_pung_chow(tiles[6], tiles[7], tiles[8]); // 122334 555 66 778
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         int check6 = pair_check(tiles[9], tiles[10]);
//                         if (check6 == 1)
//                         {
//                             int check7 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                             if (check7 == 2 || check3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 9;
//                                 gamedata.chow_count += 2;
//                                 pung_chow(gamedata, check5);
//                                 pung_chow(gamedata, check7);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else if (check7 == 1 && check5 == 2)
//                             {
//                                 if (pair_pung_chow(tiles[8], tiles[9], tiles[11]) == 3 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 6;
//                                     gamedata.chow_count += 4;
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("117 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 print("117 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             int check7 = five_tile_check(tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                             if (check7 == 1)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 11;
//                                 gamedata.chow_count += 3;
//                                 pung_chow(gamedata, check5);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else
//                             {
//                                 print("118 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                     else if (check5 == 1)
//                     {
//                         int check6 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                         if (check6 > 3 && check6 < 9)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 6;
//                             gamedata.chow_count += 2;
//                             pung_chow(gamedata, check6);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else if (check6 == 10 && pair_pung_chow(tiles[6], tiles[8], tiles[10]) == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 12;
//                             gamedata.chow_count += 4;
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             int check7 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                             if (check7 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 6;
//                                 gamedata.chow_count += 4;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else if (check7 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 6;
//                                 gamedata.chow_count += 3;
//                                 gamedata.pung_count += 1;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                             }
//                             else
//                             {
//                                 print("119 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                     else
//                     {
//                         int check6 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
//                         if (check6 == 1)
//                         {
//                             int check7 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                             if (check7 == 2 || check7 == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 8;
//                                 gamedata.chow_count += 3;
//                                 pung_chow(gamedata, check7);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else
//                             {
//                                 print("120 - Hand combination not correct!");
//                             }
//                         }
//                         else if (check6 == 2 && pair_pung_chow(tiles[8], tiles[10], tiles[11]) == 3)
//                         {
//                             if (pair_check(tiles[12], tiles[13]) == 1)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 12;
//                                 gamedata.chow_count += 4;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else
//                             {
//                                 print("121 - Hand combination not correct!");
//                             }
//                         }
//                         else if (check6 == 3 && pair_pung_chow(tiles[6], tiles[10], tiles[11]) == 3)
//                         {
//                             int check7 = pair_check(tiles[12], tiles[13]);
//                             if (check7 == 1)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 12;
//                                 gamedata.chow_count += 3;
//                                 gamedata.pung_count += 1;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                             }
//                             else if (check7 == 2 && pair_pung_chow(tiles[9], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 8;
//                                 gamedata.chow_count += 4;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                             }
//                             else
//                             {
//                                 print("122 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             print("123 - Hand combination not correct!");
//                         }
//                     }
//                 }
//             }
//             else if (check3 == 3 && pair_pung_chow(tiles[0], tiles[4], tiles[5]) == 3) // 111123     / 122223
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                 int check4 = six_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10], tiles[11]);
//                 if (check4 > 3 && check4 < 9)
//                 {
//                     int check5 = pair_check(tiles[12], tiles[13]);
//                     if (check5 == 1)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 12;
//                         gamedata.chow_count += 1;
//                         gamedata.pung_count += 1;
//                         pung_chow(gamedata, check4);
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else if (check5 == 2 && pair_pung_chow(tiles[9], tiles[10], tiles[11]) == 2)
//                     {
//                         if (pair_pung_chow(tiles[11], tiles[12], tiles[13]) == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 9;
//                             gamedata.chow_count += 2;
//                             pung_chow(gamedata, check4);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             print("124 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("125 - Hand combination not correct!");
//                     }
//                 }
//                 else if (check4 == 9 && pair_check(tiles[12], tiles[13]) == 1) /// 12222 55778899
//                 {
//                     if (pair_pung_chow(tiles[8], tiles[10], tiles[12]) == 3)
//                     {
//                         gamedata.pair_count += 1;
//                         gamedata.eye_idx = 6;
//                         gamedata.chow_count += 3;
//                         gamedata.pung_count += 1;
//                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                     }
//                     else
//                     {
//                         print("126 - Hand combination not correct!");
//                     }
//                 }
//                 else
//                 {
//                     int check5 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
//                     if (check5 == 2 || check5 == 3)
//                     {
//                         int check6 = pair_check(tiles[9], tiles[10]);
//                         if (check6 == 1)
//                         {
//                             int check7 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                             if (check7 == 2 || check3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 9;
//                                 gamedata.chow_count += 1;
//                                 gamedata.pung_count += 1;
//                                 pung_chow(gamedata, check5);
//                                 pung_chow(gamedata, check7);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else if (check7 == 1 && check5 == 2)
//                             {
//                                 if (pair_pung_chow(tiles[8], tiles[9], tiles[11]) == 3 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 6;
//                                     gamedata.chow_count += 3;
//                                     gamedata.pung_count += 1;
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("127 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 print("128 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             int check7 = five_tile_check(tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                             if (check7 == 1)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 11;
//                                 gamedata.chow_count += 2;
//                                 gamedata.pung_count += 1;
//                                 pung_chow(gamedata, check5);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else
//                             {
//                                 print("129 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                     else if (check5 == 1)
//                     {
//                         int check6 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                         if (check6 > 3 && check6 < 9)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 6;
//                             gamedata.chow_count += 1;
//                             gamedata.pung_count += 1;
//                             pung_chow(gamedata, check6);
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else if (check6 == 10 && pair_pung_chow(tiles[6], tiles[8], tiles[10]) == 3)
//                         {
//                             gamedata.pair_count += 1;
//                             gamedata.eye_idx = 12;
//                             gamedata.chow_count += 3;
//                             gamedata.pung_count += 1;
//                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                         }
//                         else
//                         {
//                             int check7 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                             if (check7 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 6;
//                                 gamedata.chow_count += 3;
//                                 gamedata.pung_count += 1;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else if (check7 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 6;
//                                 gamedata.chow_count += 2;
//                                 gamedata.pung_count += 2;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                             }
//                             else
//                             {
//                                 print("130 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                     else
//                     {
//                         if (pair_check(tiles[6], tiles[7]) == 2 && pair_pung_chow(tiles[4], tiles[6], tiles[7]) == 3) // 122223 34
//                         {
//                             int check6 = six_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                             if (check6 > 3 && check6 < 9)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.eye_idx = 3;
//                                 gamedata.chow_count += 2;
//                                 pung_chow(gamedata, check6);
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                             }
//                             else
//                             {
//                                 int check7 = five_tile_check(tiles[8], tiles[9], tiles[10], tiles[11], tiles[12]);
//                                 if (check7 == 2 && pair_pung_chow(tiles[10], tiles[12], tiles[13]) == 3)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 3;
//                                     gamedata.chow_count += 4;
//                                     pung_chow(gamedata, check6);
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else if (check7 == 3 && pair_pung_chow(tiles[8], tiles[12], tiles[13]) == 3)
//                                 {

//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 3;
//                                     gamedata.chow_count += 3;
//                                     gamedata.pung_count += 1;
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                     gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                                 }
//                                 else
//                                 {
//                                     print("131 - Hand combination not correct!");
//                                 }
//                             }
//                         }
//                         else
//                         {
//                             int check6 = five_tile_check(tiles[6], tiles[7], tiles[8], tiles[9], tiles[10]);
//                             if (check6 == 1)
//                             {
//                                 int check7 = pair_pung_chow(tiles[11], tiles[12], tiles[13]);
//                                 if (check7 == 2 || check7 == 3)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 8;
//                                     gamedata.chow_count += 2;
//                                     gamedata.pung_count += 1;
//                                     pung_chow(gamedata, check7);
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("132 - Hand combination not correct!");
//                                 }
//                             }
//                             else if (check6 == 2 && pair_pung_chow(tiles[8], tiles[10], tiles[11]) == 3)
//                             {
//                                 if (pair_check(tiles[12], tiles[13]) == 1)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 12;
//                                     gamedata.chow_count += 3;
//                                     gamedata.pung_count += 1;
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("133 - Hand combination not correct!");
//                                 }
//                             }
//                             else if (check6 == 3 && pair_pung_chow(tiles[6], tiles[10], tiles[11]) == 3)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 68);
//                                 int check7 = pair_check(tiles[12], tiles[13]);
//                                 if (check7 == 1)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 12;
//                                     gamedata.chow_count += 2;
//                                     gamedata.pung_count += 2;
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else if (check7 == 2 && pair_pung_chow(tiles[9], tiles[12], tiles[13]) == 3)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.eye_idx = 8;
//                                     gamedata.chow_count += 3;
//                                     gamedata.pung_count += 1;
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 }
//                                 else
//                                 {
//                                     print("134 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 print("135 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                 }
//             }
//             else
//             {
//                 int check10 = pair_check(tiles[0], tiles[1]);
//                 if (check10 == 3) // 19
//                 {
//                     if (pair_check(tiles[1], tiles[2]) == 1) // 199
//                     {
//                         if (pair_check(tiles[3], tiles[4]) == 3 && pair_check(tiles[5], tiles[6]) == 3)
//                         {
//                             if (honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1)
//                             {
//                                 gamedata.pair_count += 1;
//                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                             }
//                             else
//                             {
//                                 print("136.1 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             print("136.2 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         int check11 = pair_check(tiles[2], tiles[3]);
//                         if (check11 == 1 && tiles[2].value == 1) // 1911
//                         {
//                             if (tiles[4].value == 9 && tiles[3].suit == tiles[4].suit) //
//                             {
//                                 if (pair_check(tiles[5], tiles[6]) == 3 && honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                     gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                 }
//                                 else
//                                 {
//                                     print("136.3 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 print("136.4 - Hand combination not correct!");
//                             }
//                         }
//                         else if (check11 == 3) // 1919
//                         {
//                             if (pair_check(tiles[3], tiles[4]) == 1) // 19199
//                             {
//                                 if (pair_check(tiles[5], tiles[6]) == 3 && honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1)
//                                 {
//                                     gamedata.pair_count += 1;
//                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                     gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                 }
//                                 else
//                                 {
//                                     print("136.3 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 int check12 = pair_check(tiles[4], tiles[5]);
//                                 if (check12 == 1 && tiles[4].value == 1) //191911
//                                 {
//                                     if (tiles[6].value == 9 && tiles[5].suit == tiles[6].suit)
//                                     {
//                                         if (honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1)
//                                         {
//                                             gamedata.pair_count += 1;
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                         }
//                                         else
//                                         {
//                                             print("136.55 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else
//                                     {
//                                         print("136.6 - Hand combination not correct!");
//                                     }
//                                 }
//                                 else if (check12 == 3) // 191919
//                                 {
//                                     if (pair_check(tiles[5], tiles[6]) == 1)
//                                     {
//                                         if (honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1) // 1919199ESWNRWG
//                                         {
//                                             gamedata.pair_count += 1;
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                         }
//                                         else
//                                         {
//                                             print("136.7 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else
//                                     {
//                                         if (tiles[6].value == 10 && tiles[6].suit == 4)
//                                         {
//                                             if (honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]) == 1) // 191919EESWNRWG
//                                             {
//                                                 gamedata.pair_count += 1;
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                             }
//                                             else if (tiles[7].value == 10 && tiles[7].suit == 5)
//                                             {
//                                                 if (pair_check(tiles[7], tiles[8]) == 1 && (tiles[8].suit + 1) == tiles[9].suit) // 191919ESSWNRWG
//                                                 {
//                                                     if ((tiles[9].suit + 1) == tiles[10].suit && (tiles[10].suit + 1) == tiles[11].suit)
//                                                     {
//                                                         if ((tiles[11].suit + 1) == tiles[12].suit && (tiles[12].suit + 1) == tiles[13].suit)
//                                                         {
//                                                             gamedata.pair_count += 1;
//                                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                             gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                                         }
//                                                         else
//                                                         {
//                                                             print("136.8 - Hand combination not correct!");
//                                                         }
//                                                     }
//                                                     else
//                                                     {
//                                                         print("136.9 - Hand combination not correct!");
//                                                     }
//                                                 }
//                                                 else if ((tiles[7].suit + 1) == tiles[8].suit)
//                                                 {
//                                                     if (pair_check(tiles[8], tiles[9]) == 1 && (tiles[9].suit + 1) == tiles[10].suit) // 191919ESWWNRWG
//                                                     {
//                                                         if ((tiles[10].suit + 1) == tiles[11].suit && (tiles[11].suit + 1) == tiles[12].suit)
//                                                         {
//                                                             if ((tiles[12].suit + 1) == tiles[13].suit)
//                                                             {
//                                                                 gamedata.pair_count += 1;
//                                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                                             }
//                                                             else
//                                                             {
//                                                                 print("136.10 - Hand combination not correct!");
//                                                             }
//                                                         }
//                                                         else
//                                                         {
//                                                             print("136.11 - Hand combination not correct!");
//                                                         }
//                                                     }
//                                                     else if ((tiles[8].suit + 1) == tiles[9].suit)
//                                                     {
//                                                         if (pair_check(tiles[9], tiles[10]) == 1 && (tiles[10].suit + 1) == tiles[11].suit) // 191919ESWNNRWG
//                                                         {
//                                                             if ((tiles[11].suit + 1) == tiles[12].suit && (tiles[12].suit + 1) == tiles[13].suit)
//                                                             {
//                                                                 gamedata.pair_count += 1;
//                                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                                             }
//                                                             else
//                                                             {
//                                                                 print("136.12 - Hand combination not correct!");
//                                                             }
//                                                         }
//                                                         else if ((tiles[9].suit + 1) == tiles[10].suit)
//                                                         {
//                                                             if (pair_check(tiles[10], tiles[11]) == 1 && (tiles[11].suit + 1) == tiles[12].suit) // 191919ESWNRRWG
//                                                             {
//                                                                 if ((tiles[12].suit + 1) == tiles[13].suit)
//                                                                 {
//                                                                     gamedata.pair_count += 1;
//                                                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                                     gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                                                 }
//                                                                 else
//                                                                 {
//                                                                     print("136.13 - Hand combination not correct!");
//                                                                 }
//                                                             }
//                                                             else if ((tiles[10].suit + 1) == tiles[11].suit)
//                                                             {
//                                                                 if (pair_check(tiles[11], tiles[12]) == 1 && tiles[12].suit + 1 == tiles[13].suit) // 191919ESWNRWWG
//                                                                 {
//                                                                     gamedata.pair_count += 1;
//                                                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                                     gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                                                 }
//                                                                 else if ((tiles[11].suit + 1) == tiles[12].suit && pair_check(tiles[12], tiles[13]) == 1) // 191919ESWNRWGG
//                                                                 {
//                                                                     gamedata.pair_count += 1;
//                                                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                                     gamedata.score_check.insert(gamedata.score_check.begin(), 7);
//                                                                 }
//                                                                 else
//                                                                 {
//                                                                     print("136.14 - Hand combination not correct!");
//                                                                 }
//                                                             }
//                                                             else
//                                                             {
//                                                                 print("136.15 - Hand combination not correct!");
//                                                             }
//                                                         }
//                                                         else
//                                                         {
//                                                             print("136.16 - Hand combination not correct!");
//                                                         }
//                                                     }
//                                                     else
//                                                     {
//                                                         print("136.17 - Hand combination not correct!");
//                                                     }
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.18 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else
//                                             {
//                                                 print("136.19 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.20 - Hand combination not correct!");
//                                         }
//                                     }
//                                 }
//                                 else
//                                 {
//                                     print("136.21 - Hand combination not correct!");
//                                 }
//                             }
//                         }
//                         else
//                         {
//                             print("136.22 - Hand combination not correct!");
//                         }
//                     }
//                 }
//                 else
//                 {
//                     int check15 = honors_check(tiles[7], tiles[8], tiles[9], tiles[10], tiles[11], tiles[12], tiles[13]);
//                     if (check15 == 1)
//                     {
//                         int check16 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
//                         if (check16 == 4)
//                         {
//                             int check17 = pair_check(tiles[3], tiles[4]);
//                             int check18 = pair_check(tiles[5], tiles[6]);
//                             if (tiles[0].value == 1)
//                             {
//                                 if (check17 == 4 && check18 == 4)
//                                 {
//                                     if (tiles[3].value == 2 || tiles[3].value == 5)
//                                     {
//                                         if (tiles[5].value == 3 || tiles[5].value == 6)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                         }
//                                         else
//                                         {
//                                             print("136.30 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else if (tiles[3].value == 3 || tiles[3].value == 6)
//                                     {
//                                         if (tiles[5].value == 2 || tiles[5].value == 5)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                         }
//                                         else
//                                         {
//                                             print("136.31 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else
//                                     {
//                                         print("136.32 - Hand combination not correct!");
//                                     }
//                                 }
//                             }
//                             else if (tiles[0].value == 2)
//                             {
//                                 if (check17 == 4 && check18 == 4)
//                                 {
//                                     if (tiles[3].value == 1 || tiles[3].value == 4)
//                                     {
//                                         if (tiles[5].value == 3 || tiles[5].value == 6)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                         }
//                                         else
//                                         {
//                                             print("136.33 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else if (tiles[3].value == 3 || tiles[3].value == 6)
//                                     {
//                                         if (tiles[5].value == 1 || tiles[5].value == 4)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                         }
//                                         else
//                                         {
//                                             print("136.34 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else
//                                     {
//                                         print("136.35 - Hand combination not correct!");
//                                     }
//                                 }
//                             }
//                             else if (tiles[0].value == 3)
//                             {
//                                 if (check17 == 4 && check18 == 4)
//                                 {
//                                     if (tiles[3].value == 1 || tiles[3].value == 4)
//                                     {
//                                         if (tiles[5].value == 2 || tiles[5].value == 5)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                         }
//                                         else
//                                         {
//                                             print("136.36 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else if (tiles[3].value == 2 || tiles[3].value == 5)
//                                     {
//                                         if (tiles[5].value == 1 || tiles[5].value == 4)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                         }
//                                         else
//                                         {
//                                             print("136.37 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else
//                                     {
//                                         print("136.38 - Hand combination not correct!");
//                                     }
//                                 }
//                             }
//                             else
//                             {
//                                 print("136.39 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             int check17 = pair_check(tiles[0], tiles[1]);
//                             if (check17 == 4)
//                             {
//                                 if (tiles[0].value == 1 || tiles[0].value == 4)
//                                 {
//                                     int check18 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
//                                     if (check18 == 4)
//                                     {
//                                         if (tiles[2].value == 2)
//                                         {
//                                             int check19 = pair_check(tiles[5], tiles[6]);
//                                             if (check19 == 4)
//                                             {
//                                                 if (tiles[5].value == 3 || tiles[5].value == 6)
//                                                 {
//                                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                     gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.40 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else
//                                             {
//                                                 print("136.41 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else if (tiles[2].value == 3)
//                                         {
//                                             int check19 = pair_check(tiles[5], tiles[6]);
//                                             if (check19 == 4)
//                                             {
//                                                 if (tiles[5].value == 2 || tiles[5].value == 5)
//                                                 {
//                                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                     gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.42 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else
//                                             {
//                                                 print("136.43 - Hand combination not correct!");
//                                             }
//                                         }
//                                     }
//                                     else
//                                     {
//                                         int check19 = pair_check(tiles[2], tiles[3]);
//                                         if (check19 == 4)
//                                         {
//                                             if (tiles[2].value == 2 || tiles[2].value == 5)
//                                             {
//                                                 int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
//                                                 if (check20 == 4)
//                                                 {
//                                                     if (tiles[4].value == 3)
//                                                     {
//                                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                         gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                     }
//                                                     else
//                                                     {
//                                                         print("136.44 - Hand combination not correct!");
//                                                     }
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.45 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else if (tiles[2].value == 3 || tiles[2].value == 6)
//                                             {
//                                                 int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
//                                                 if (check20 == 4)
//                                                 {
//                                                     if (tiles[4].value == 2)
//                                                     {
//                                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                         gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                     }
//                                                     else
//                                                     {
//                                                         print("136.46 - Hand combination not correct!");
//                                                     }
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.47 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else
//                                             {
//                                                 print("136.48 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.49 - Hand combination not correct!");
//                                         }
//                                     }
//                                 }
//                                 else if (tiles[0].value == 2 || tiles[0].value == 5)
//                                 {
//                                     int check18 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
//                                     if (check18 == 4)
//                                     {
//                                         if (tiles[2].value == 1)
//                                         {
//                                             int check19 = pair_check(tiles[5], tiles[6]);
//                                             if (check19 == 4)
//                                             {
//                                                 if (tiles[5].value == 3 || tiles[5].value == 6)
//                                                 {
//                                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                     gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.50 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else
//                                             {
//                                                 print("136.51 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else if (tiles[2].value == 3)
//                                         {
//                                             int check19 = pair_check(tiles[5], tiles[6]);
//                                             if (check19 == 4)
//                                             {
//                                                 if (tiles[5].value == 1 || tiles[5].value == 4)
//                                                 {
//                                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                     gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.52 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else
//                                             {
//                                                 print("136.53 - Hand combination not correct!");
//                                             }
//                                         }
//                                     }
//                                     else
//                                     {
//                                         int check19 = pair_check(tiles[2], tiles[3]);
//                                         if (check19 == 4)
//                                         {
//                                             if (tiles[2].value == 1 || tiles[2].value == 4)
//                                             {
//                                                 int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
//                                                 if (check20 == 4)
//                                                 {
//                                                     if (tiles[4].value == 3)
//                                                     {
//                                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                         gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                     }
//                                                     else
//                                                     {
//                                                         print("136.54 - Hand combination not correct!");
//                                                     }
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.55 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else if (tiles[2].value == 3 || tiles[2].value == 6)
//                                             {
//                                                 int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
//                                                 if (check20 == 4)
//                                                 {
//                                                     if (tiles[4].value == 1)
//                                                     {
//                                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                         gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                     }
//                                                     else
//                                                     {
//                                                         print("136.56 - Hand combination not correct!");
//                                                     }
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.57 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else
//                                             {
//                                                 print("136.58 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.59 - Hand combination not correct!");
//                                         }
//                                     }
//                                 }
//                                 else if (tiles[0].value == 3 || tiles[0].value == 6)
//                                 {
//                                     int check18 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
//                                     if (check18 == 4)
//                                     {
//                                         if (tiles[2].value == 1)
//                                         {
//                                             int check19 = pair_check(tiles[5], tiles[6]);
//                                             if (check19 == 4)
//                                             {
//                                                 if (tiles[5].value == 2 || tiles[5].value == 5)
//                                                 {
//                                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                     gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.60 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else
//                                             {
//                                                 print("136.61 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else if (tiles[2].value == 2)
//                                         {
//                                             int check19 = pair_check(tiles[5], tiles[6]);
//                                             if (check19 == 4)
//                                             {
//                                                 if (tiles[5].value == 1 || tiles[5].value == 4)
//                                                 {
//                                                     gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                     gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.62 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else
//                                             {
//                                                 print("136.63 - Hand combination not correct!");
//                                             }
//                                         }
//                                     }
//                                     else
//                                     {
//                                         int check19 = pair_check(tiles[2], tiles[3]);
//                                         if (check19 == 4)
//                                         {
//                                             if (tiles[2].value == 1 || tiles[2].value == 4)
//                                             {
//                                                 int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
//                                                 if (check20 == 4)
//                                                 {
//                                                     if (tiles[4].value == 2)
//                                                     {
//                                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                         gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                     }
//                                                     else
//                                                     {
//                                                         print("136.64 - Hand combination not correct!");
//                                                     }
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.65 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else if (tiles[2].value == 2 || tiles[2].value == 5)
//                                             {
//                                                 int check20 = pair_pung_chow(tiles[4], tiles[5], tiles[6]);
//                                                 if (check20 == 4)
//                                                 {
//                                                     if (tiles[4].value == 1)
//                                                     {
//                                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                         gamedata.score_check.insert(gamedata.score_check.begin(), 27);
//                                                     }
//                                                     else
//                                                     {
//                                                         print("136.66 - Hand combination not correct!");
//                                                     }
//                                                 }
//                                                 else
//                                                 {
//                                                     print("136.67 - Hand combination not correct!");
//                                                 }
//                                             }
//                                             else
//                                             {
//                                                 print("136.68 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.69 - Hand combination not correct!");
//                                         }
//                                     }
//                                 }
//                                 else
//                                 {
//                                     print("136.70 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 print("136.71 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                     else if (check15 == 2)
//                     {
//                         int check16 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
//                         if (check16 == 4)
//                         {
//                             if (tiles[0].value == 1)
//                             {
//                                 int check16 = pair_pung_chow(tiles[3], tiles[4], tiles[5]);
//                                 if (check16 == 4)
//                                 {
//                                     if (tiles[3].value == 2)
//                                     {
//                                         int check17 = pair_check(tiles[6], tiles[7]);
//                                         if (check17 == 4)
//                                         {
//                                             if (tiles[6].value == 3 || tiles[6].value == 6)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.71 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else if (tiles[3].value == 3)
//                                     {
//                                         int check17 = pair_check(tiles[6], tiles[7]);
//                                         if (check17 == 4)
//                                         {
//                                             if (tiles[6].value == 3 || tiles[6].value == 6)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.71 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else
//                                     {
//                                         print("136.71 - Hand combination not correct!");
//                                     }
//                                 }
//                                 else
//                                 {
//                                     int check17 = pair_check(tiles[3], tiles[4]);
//                                     if (check17 == 4)
//                                     {
//                                         if (tiles[3].value == 2 || tiles[3].value == 5)
//                                         {
//                                             int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
//                                             if (check18 == 4 && tiles[5].value == 3)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else if (tiles[3].value == 3 || tiles[3].value == 6)
//                                         {
//                                             int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
//                                             if (check18 == 4 && tiles[5].value == 2)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.71 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else
//                                     {
//                                         print("136.71 - Hand combination not correct!");
//                                     }
//                                 }
//                             }
//                             else if (tiles[0].value == 2) // ------
//                             {
//                                 int check16 = pair_pung_chow(tiles[3], tiles[4], tiles[5]);
//                                 if (check16 == 4)
//                                 {
//                                     if (tiles[3].value == 1)
//                                     {
//                                         int check17 = pair_check(tiles[6], tiles[7]);
//                                         if (check17 == 4)
//                                         {
//                                             if (tiles[6].value == 3 || tiles[6].value == 6)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.71 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else if (tiles[3].value == 3)
//                                     {
//                                         int check17 = pair_check(tiles[6], tiles[7]);
//                                         if (check17 == 4)
//                                         {
//                                             if (tiles[6].value == 1 || tiles[6].value == 4)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.71 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else
//                                     {
//                                         print("136.71 - Hand combination not correct!");
//                                     }
//                                 }
//                                 else
//                                 {
//                                     int check17 = pair_check(tiles[3], tiles[4]);
//                                     if (check17 == 4)
//                                     {
//                                         if (tiles[3].value == 1 || tiles[3].value == 4)
//                                         {
//                                             int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
//                                             if (check18 == 4 && tiles[5].value == 3)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else if (tiles[3].value == 3 || tiles[3].value == 6)
//                                         {
//                                             int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
//                                             if (check18 == 4 && tiles[5].value == 1)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.71 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else
//                                     {
//                                         print("136.71 - Hand combination not correct!");
//                                     }
//                                 }
//                             }
//                             else if (tiles[0].value == 3)
//                             {
//                                 int check16 = pair_pung_chow(tiles[3], tiles[4], tiles[5]);
//                                 if (check16 == 4)
//                                 {
//                                     if (tiles[3].value == 2)
//                                     {
//                                         int check17 = pair_check(tiles[6], tiles[7]);
//                                         if (check17 == 4)
//                                         {
//                                             if (tiles[6].value == 1 || tiles[6].value == 4)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.71 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else if (tiles[3].value == 1)
//                                     {
//                                         int check17 = pair_check(tiles[6], tiles[7]);
//                                         if (check17 == 4)
//                                         {
//                                             if (tiles[6].value == 5 || tiles[6].value == 8)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.71 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else
//                                     {
//                                         print("136.71 - Hand combination not correct!");
//                                     }
//                                 }
//                                 else
//                                 {
//                                     int check17 = pair_check(tiles[3], tiles[4]);
//                                     if (check17 == 4)
//                                     {
//                                         if (tiles[3].value == 1 || tiles[3].value == 4)
//                                         {
//                                             int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
//                                             if (check18 == 4 && tiles[5].value == 2)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else if (tiles[3].value == 2 || tiles[3].value == 5)
//                                         {
//                                             int check18 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
//                                             if (check18 == 4 && tiles[5].value == 1)
//                                             {
//                                                 gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                             }
//                                             else
//                                             {
//                                                 print("136.71 - Hand combination not correct!");
//                                             }
//                                         }
//                                         else
//                                         {
//                                             print("136.71 - Hand combination not correct!");
//                                         }
//                                     }
//                                     else
//                                     {
//                                         print("136.71 - Hand combination not correct!");
//                                     }
//                                 }
//                             }
//                             else
//                             {
//                                 print("136.71 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             int check17 = pair_check(tiles[0], tiles[1]);
//                             if (check17 == 4)
//                             {
//                                 int check18 = pair_pung_chow(tiles[2], tiles[3], tiles[4]);
//                                 int check19 = pair_pung_chow(tiles[5], tiles[6], tiles[7]);
//                                 if (check18 == 4 && check19 == 4)
//                                 {
//                                     if (tiles[0].value == 1 || tiles[0].value == 4)
//                                     {
//                                         if (tiles[2].value == 2 && tiles[5].value == 3)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                         }
//                                         else if (tiles[2].value == 3 && tiles[5].value == 2)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                         }
//                                     }
//                                     else if (tiles[0].value == 2 || tiles[0].value == 5)
//                                     {
//                                         if (tiles[2].value == 1 && tiles[5].value == 3)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                         }
//                                         else if (tiles[2].value == 3 && tiles[5].value == 1)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                         }
//                                     }
//                                     else if (tiles[0].value == 3 || tiles[0].value == 6)
//                                     {
//                                         if (tiles[2].value == 1 && tiles[5].value == 2)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                         }
//                                         else if (tiles[2].value == 2 && tiles[5].value == 1)
//                                         {
//                                             gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                         }
//                                     }
//                                     else
//                                     {
//                                         print("136.71 - Hand combination not correct!");
//                                     }
//                                 }
//                                 else
//                                 {
//                                     print("136.71 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 print("136.71 - Hand combination not correct!");
//                             }
//                         }
//                     }
//                     else if (check15 == 3)
//                     {
//                         int check16 = pair_pung_chow(tiles[0], tiles[1], tiles[2]);
//                         int check17 = pair_pung_chow(tiles[3], tiles[4], tiles[5]);
//                         int check18 = pair_pung_chow(tiles[6], tiles[7], tiles[8]);
//                         if (check16 == 4 && check17 == 4)
//                         {
//                             if (check18 == 4)
//                             {
//                                 if (tiles[0].value == 1)
//                                 {
//                                     if (tiles[3].value == 2 && tiles[6].value == 3)
//                                     {
//                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                         // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
//                                         gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                     }
//                                     else if (tiles[3].value == 3 && tiles[6].value == 2)
//                                     {
//                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                         // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
//                                         gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                     }
//                                     else
//                                     {
//                                         print("136.72 - Hand combination not correct!");
//                                     }
//                                 }
//                                 else if (tiles[0].value == 2)
//                                 {
//                                     if (tiles[3].value == 1 && tiles[6].value == 3)
//                                     {
//                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                         // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
//                                         gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                     }
//                                     else if (tiles[3].value == 3 && tiles[6].value == 1)
//                                     {
//                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                         // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
//                                         gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                     }
//                                     else
//                                     {
//                                         print("136.23 - Hand combination not correct!");
//                                     }
//                                 }
//                                 else if (tiles[0].value == 3)
//                                 {
//                                     if (tiles[3].value == 1 && tiles[6].value == 2)
//                                     {
//                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                         // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
//                                         gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                     }
//                                     else if (tiles[3].value == 2 && tiles[6].value == 1)
//                                     {
//                                         gamedata.winnable = 1; // transferhand(gamedata, 14);
//                                         // gamedata.score_check.insert(gamedata.score_check.begin(), 35);
//                                         gamedata.score_check.insert(gamedata.score_check.begin(), 38);
//                                     }
//                                     else
//                                     {
//                                         print("136.24 - Hand combination not correct!");
//                                     }
//                                 }
//                                 else
//                                 {
//                                     print("136.25 - Hand combination not correct!");
//                                 }
//                             }
//                             else
//                             {
//                                 print("136.25 - Hand combination not correct!");
//                             }
//                         }
//                         else
//                         {
//                             print("136.26 - Hand combination not correct!");
//                         }
//                     }
//                     else
//                     {
//                         print("150 - Hand combination not correct!");
//                     }
//                 }
//             }
//         }
//     }
// }

// void mhlgame::getscore(mhlgamedata &gamedata, vector<uint8_t> & hand)
// {
//     vector<mhltile> tiles = {};
//     for (int i = 0; i < hand.size(); i++)
//     {
//         tiles.insert(tiles.end(), table_deck.at(hand[i]));
//     }
//     if (gamedata.kong_count == 0)
//     {
//         if (gamedata.pung_count == 4)
//         {
//             gamedata.score_check.insert(gamedata.score_check.begin(), 12);
//             vector<mhltile> tiles2 = {};
//             tiles2.insert(tiles2.begin(), tiles[0]);
//             tiles2.insert(tiles2.begin(), tiles[3]);
//             tiles2.insert(tiles2.begin(), tiles[6]);
//             tiles2.insert(tiles2.begin(), tiles[9]);
//             int n = tiles2.size(), i, j;
//             mhltile temp;
//             for (i = 0; i < (n - 1); i++)
//             {
//                 for (j = 0; j < (n - i - 1); j++)
//                 {
//                     if (tiles2[j].value > tiles2[j + 1].value)
//                     {
//                         temp = tiles2[j];
//                         tiles2[j] = tiles2[j + 1];
//                         tiles2[j + 1] = temp;
//                     }
//                 }
//             }
//             int checkfirst = tiles2[0].value;
//             int checkwind = 0;
//             int check1 = pung_check(tiles2[1], tiles2[2], tiles2[3]);
//             if (checkfirst == 10) // www --- --- ---
//             {
//                 if (check1 == 11) // www DDD DDD DDD --
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 2);
//                     if (tiles[12].value == 10) // www DDD DDD DDD ww
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 10);
//                         checkwind = 1;
//                     }
//                     else // www DDD DDD DDD 1/9 or www DDD DDD DDD 2-8
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         if (tiles[12].value == 1 || tiles[12].value == 9)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                             checkwind = 1;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                 }
//                 else if (check1 == 12) // www www www www --
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 1);
//                     checkwind = 1;
//                     if (tiles[12].value == 11)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 10);
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         if (tiles[12].value == 1 || tiles[12].value == 9)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                 }
//                 else if (check1 == 13) // www www DDD DDD --
//                 {
//                     if (tiles[12].value > 9) // www www DDD DDD DD or www www DDD DDD ww
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 10);
//                         if (tiles[12].value == 11)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 9);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 49);
//                         }
//                         checkwind = 1;
//                     }
//                     else // www www DDD DDD 1/9 or www www DDD DDD 2-8
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 49);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         if (tiles[12].value == 1 || tiles[12].value == 9)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                             checkwind = 1;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                 }
//                 else // www www www DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 59);
//                     if (tiles[12].value > 9) // www www www DDD DD or www www www DDD ww
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 10);
//                         if (tiles[12].value == 11)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 34);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 8);
//                         }
//                         checkwind = 1;
//                     }
//                     else // www www www DDD 1/9 or www www www DDD 2-8
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 34);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         if (tiles[12].value == 1 || tiles[12].value == 9)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                             checkwind = 1;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                 }
//             }
//             else if (checkfirst == 9) // 999 --- --- ---
//             {
//                 if (check1 == 11) // 999 DDD DDD DDD --
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 2);
//                     if (tiles[12].value == 10) // 999 DDD DDD DDD ww
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                         checkwind = 1;
//                     }
//                     else // 999 DDD DDD DDD 1/9 or 999 DDD DDD DDD 2-8
//                     {
//                         if (tiles[12].value == 1 || tiles[12].value == 9)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                             checkwind = 1;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                         if (tiles[0].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                     }
//                 }
//                 else if (check1 == 12) // 999 www www www --
//                 {
//                     if (tiles[12].value > 9)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                         checkwind = 1;
//                         if (tiles[12].value == 11)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 34);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 8);
//                         }
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 34);
//                         if (tiles[12].value == 1 || tiles[12].value == 9)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                         if (tiles[0].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                     }
//                 }
//                 else if (check1 == 13) // 999 www DDD DDD --
//                 {
//                     if (tiles[12].value > 9)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         if (tiles[12].value == 11)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 9);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 49);
//                         }
//                     }
//                     else
//                     {
//                         if (tiles[12].value == 1 || tiles[12].value == 9)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                             checkwind = 1;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                         if (tiles[0].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                     }
//                 }
//                 else if (check1 == 14) // 999 999 DDD DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                     if (tiles[12].value > 9)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         if (tiles[12].value == 11)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 9);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 49);
//                         }
//                     }
//                     else
//                     {
//                         if (tiles[12].value == 1 || tiles[12].value == 9)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                             checkwind = 1;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                         if (tiles2[0].suit == tiles[12].suit || tiles2[1].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                 }
//                 else if (check1 == 16) // 999 www www DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 59);
//                     if (tiles[12].value > 9)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                         checkwind = 1;
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                     }
//                     else
//                     {
//                         if (tiles[12].value == 1 || tiles[12].value == 9)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                             checkwind = 1;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                         if (tiles[0].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                     }
//                 }
//                 else if (check1 == 21) // 999 999 www DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 59);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                     if (tiles[12].value > 9)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                         checkwind = 1;
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                     }
//                     else
//                     {
//                         if (tiles[12].value == 1 || tiles[12].value == 9)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                             checkwind = 1;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                         if (tiles2[0].suit == tiles[12].suit || tiles2[1].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 52);
//                         }
//                     }
//                 }
//                 else if (check1 == 22) // 999 999 999 DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 31);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 59);
//                     if (tiles[12].value > 9)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                         checkwind = 1;
//                         if (tiles[12].value == 10)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 52);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                     else
//                     {
//                         if (tiles[12].value == 1)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                             checkwind = 1;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                 }
//                 else if (check1 == 23) // 999 999 www www
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                     if (tiles[12].value > 9)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         checkwind = 1;
//                     }
//                     else
//                     {
//                         if (tiles[12].value == 1 || tiles[12].value == 9)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                             checkwind = 1;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                         if (tiles2[0].suit == tiles[12].suit || tiles2[1].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                 }
//                 else //if (check1 == 24) // 999 999 999 www
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 31);
//                     if (tiles[12].value > 9)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                         checkwind = 1;
//                         if (tiles[12].value == 11)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 52);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                     else
//                     {
//                         if (tiles[12].value == 1)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 17);
//                             checkwind = 1;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                 }
//             }
//             else if (checkfirst > 1 && checkfirst < 9) // 2-8 --- --- ---
//             {
//                 int check1 = pung_check(tiles2[1], tiles2[2], tiles2[3]);
//                 if (check1 == 11) // 2-8 DDD DDD DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 2);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     if (tiles[12].value == 10) // 2-8 DDD DDD DDD ww
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                     }
//                     else // 2-8 DDD DDD DDD 1/9 or 2-8 DDD DDD DDD 2-8
//                     {
//                         if (tiles[0].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                     }
//                 }
//                 else if (check1 == 12) // 2-8 www www www
//                 {
//                     if (tiles[12].value > 9) // 2-8 www www www DD or 2-8 www www www ww
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         if (tiles[12].value == 11)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 34);
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 8);
//                             checkwind = 1;
//                         }
//                     }
//                     else // 2-8 www www www 1/9 or 2-8 www www www 2-8
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 34);
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         if (tiles[0].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                     }
//                 }
//                 else if (check1 == 13) // 2-8 www DDD DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     if (tiles[12].value > 9)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         if (tiles[12].value == 11)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 9);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 49);
//                         }
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 49);
//                         if (tiles[0].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                     }
//                 }
//                 else if (check1 == 14 || check1 == 15) // 2-8 2-8/19 DDD DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     if (tiles2[0].value == tiles2[1].value)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     }
//                     if (tiles[12].value > 9)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         if (tiles[12].value == 11)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 9);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 49);
//                         }
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 49);
//                         if (tiles[0].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                     }
//                 }
//                 else if (check1 == 16) // 2-8 www www DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 59);
//                     if (tiles[12].value > 9)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                     }
//                     else
//                     {
//                         if (tiles[0].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                     }
//                 }
//                 else if (check1 == 21 || check1 == 31) // 2-8 2-8/1/9 www DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 59);
//                     if (tiles2[0].value == tiles2[1].value)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     }
//                     if (tiles[12].value > 9)
//                     {
//                         if (tiles[0].suit == tiles[3].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                     }
//                     else
//                     {
//                         if (tiles2[0].suit != tiles2[1].suit)
//                         {
//                             if (tiles2[0].suit == tiles[12].suit || tiles2[1].suit == tiles[12].suit)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                             }
//                             else
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 52);
//                             }
//                         }
//                         else
//                         {
//                             if (tiles2[0].suit == tiles[12].suit)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                             }
//                             else
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                             }
//                         }
//                     }
//                 }
//                 else if (check1 == 22) // 2-8 999 999 DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 59);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                     if (tiles[12].value == 10)
//                     {
//                         if (tiles[0].suit == tiles2[1].suit || tiles[0].suit == tiles[2].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 52);
//                         }
//                     }
//                     else
//                     {
//                         if (tiles[0].suit == tiles2[1].suit || tiles[0].suit == tiles[2].suit)
//                         {
//                             if (tiles[12].suit == tiles2[1].suit || tiles[12].suit == tiles2[2].suit)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                             }
//                             else
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                             }
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                 }
//                 else if (check1 == 23 || check1 == 34) // 2-8 2-8/1/9 www www
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     if (tiles2[0].value == tiles2[1].value)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     }
//                     if (tiles[12].value > 9)
//                     {
//                         if (tiles[0].suit == tiles[3].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                     }
//                     else
//                     {
//                         if (tiles2[0].suit == tiles[12].suit || tiles2[1].suit == tiles[12].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 52);
//                         }
//                     }
//                 }
//                 else if (check1 == 24) // 2-8 999 999 www
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                     if (tiles[12].value == 11)
//                     {
//                         if (tiles[0].suit == tiles2[1].suit || tiles[0].suit == tiles2[2].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     }
//                 }
//                 else if (check1 == 25) // 2-8 999 999 999
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 31);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     if (tiles[12].value < 10)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 76);
//                     }
//                     else
//                     {
//                         if (tiles[0].value >= 6 && tiles[12].value >= 6)
//                         {
//                             if (tiles[0].value >= 7 && tiles[12].value >= 7)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 24);
//                             }
//                             else
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 36);
//                             }
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                 }
//                 else if (check1 == 32) // 2-8 2-8 999 DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 59);
//                     if (tiles2[0].value == tiles2[1].value)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                     }
//                     else if (tiles2[0].value + 1 == tiles2[1].value && tiles2[0].value == 7)
//                     {
//                         if (tiles2[0].suit != tiles2[1].suit && tiles2[1].suit != tiles2[2].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 41);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     }
//                     if (tiles[12].value == 10)
//                     {
//                         if (tiles[0].suit != tiles2[1].suit)
//                         {
//                             if (tiles2[1].suit != tiles2[2].suit)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 52);
//                             }
//                             else
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                             }
//                         }
//                         else
//                         {
//                             if (tiles[1].suit == tiles2[2].suit)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                             }
//                             else
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                             }
//                         }
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     }
//                 }
//                 else if (check1 == 33) // 2-8 2-8 2-8 DDD
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 59);
//                     int checktype = 0;
//                     if (tiles2[0].value + 1 == tiles2[1].value && tiles2[1].value + 1 == tiles2[2].value)
//                     {
//                         if (tiles2[0].suit == tiles2[1].suit && tiles2[1].suit == tiles2[2].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 20);
//                             checktype = 1;
//                         }
//                         else if (tiles2[0].suit != tiles2[1].suit && tiles2[1].suit != tiles2[2].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 41);
//                             checktype = 2;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                     else if (tiles2[0].value == tiles2[1].value)
//                     {
//                         if (tiles2[1].value == tiles2[2].value)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 31);
//                             checktype = 2;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                             if (tiles2[2].suit == tiles2[0].suit || tiles2[2].suit == tiles2[1].suit)
//                             {
//                                 checktype = 3;
//                             }
//                             else
//                             {
//                                 checktype = 2;
//                             }
//                         }
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     }
//                     if (tiles[12].value > 1 && tiles[12].value < 9)
//                     {
//                         if (tiles2[3].suit == GREEN)
//                         {
//                             if (tiles2[0].suit == BAMBOO && tiles2[1].suit == BAMBOO)
//                             {
//                                 if (tiles2[2].suit == BAMBOO && tiles[12].suit == BAMBOO)
//                                 {
//                                     if (tiles2[0].value % 2 == 0 || tiles2[0].value == 3)
//                                     {
//                                         if (tiles2[1].value % 2 == 0 || tiles2[1].value == 3)
//                                         {
//                                             if (tiles2[2].value % 2 == 0 || tiles2[2].value == 3)
//                                             {
//                                                 if (tiles[12].value % 2 == 0 || tiles[12].value == 3)
//                                                 {
//                                                     gamedata.score_check.insert(gamedata.score_check.begin(), 5);
//                                                 }
//                                                 else
//                                                 {
//                                                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                                                 }
//                                             }
//                                             else
//                                             {
//                                                 gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                                             }
//                                         }
//                                         else
//                                         {
//                                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                                         }
//                                     }
//                                     else
//                                     {
//                                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                                     }
//                                 }
//                                 else
//                                 {
//                                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                                 }
//                             }
//                             else
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                             }
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                     else if (tiles[12].value == 10)
//                     {
//                         if (checktype == 1)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else if (checktype == 2)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 52);
//                         }
//                         else if (checktype == 3)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     }
//                 }
//                 else if (check1 == 35) // 2-8 2-8 999 www
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     if (tiles2[0].value == tiles2[1].value)
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                     }
//                     else if (tiles2[0].value + 1 == tiles2[1].value && tiles2[0].value == 7)
//                     {
//                         if (tiles2[0].suit != tiles2[1].suit && tiles2[1].suit != tiles2[2].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 41);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     }
//                     if (tiles[12].value == 11)
//                     {
//                         if (tiles[0].suit != tiles2[1].suit)
//                         {
//                             if (tiles2[1].suit != tiles2[2].suit)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 52);
//                             }
//                             else
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                             }
//                         }
//                         else
//                         {
//                             if (tiles[1].suit == tiles2[2].suit)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                             }
//                             else
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                             }
//                         }
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     }
//                 }
//                 else if (check1 == 36) // 2-8 2-8 2-8 www
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                     int checktype = 0;
//                     if (tiles2[0].value + 1 == tiles2[1].value && tiles2[1].value + 1 == tiles2[2].value)
//                     {
//                         if (tiles2[0].suit == tiles2[1].suit && tiles2[1].suit == tiles2[2].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 20);
//                             checktype = 1;
//                         }
//                         else if (tiles2[0].suit != tiles2[1].suit && tiles2[1].suit != tiles2[2].suit)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 41);
//                             checktype = 2;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                     else if (tiles2[0].value == tiles2[1].value)
//                     {
//                         if (tiles2[1].value == tiles2[2].value)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 31);
//                             checktype = 2;
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                             if (tiles2[2].suit == tiles2[0].suit || tiles2[2].suit == tiles2[1].suit)
//                             {
//                                 checktype = 3;
//                             }
//                             else
//                             {
//                                 checktype = 2;
//                             }
//                         }
//                     }
//                     else
//                     {
//                         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                     }
//                     if (tiles[12].value == 11)
//                     {
//                         if (checktype == 1)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                         }
//                         else if (checktype == 2)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 52);
//                         }
//                         else if (checktype == 3)
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                     else
//                     {
//                         if (checktype == 1)
//                         {
//                             if (tiles[12].suit == tiles[0].suit)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 53);
//                             }
//                             else
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                             }
//                         }
//                         else if (checktype == 3)
//                         {
//                             if (tiles[12].suit == tiles[0].suit || tiles[12].suit == tiles[3].suit)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                             }
//                             else if (tiles[12].suit == tiles[6].suit)
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                             }
//                             else
//                             {
//                                 gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                             }
//                         }
//                         else
//                         {
//                             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                         }
//                     }
//                 }
//                 // else if (check1 == 41) // 2-8 2-8 999 999
//                 // {
//                 // gamedata.score_check.insert(gamedata.score_check.begin(), 66);
//                 // if (tiles2[0].value + 1 == tiles2[1].value && tiles2[1].value + 1 == tiles2[2].value)
//                 // {
//                 //     if (tiles2[0].suit == tiles2[1].suit && tiles2[1].suit == tiles2[2].suit)
//                 //     {
//                 //         gamedata.score_check.insert(gamedata.score_check.begin(), 20);
//                 //         checktype = 1;
//                 //     }
//                 //     else if (tiles2[0].suit != tiles2[1].suit && tiles2[1].suit != tiles2[2].suit)
//                 //     {
//                 //         gamedata.score_check.insert(gamedata.score_check.begin(), 41);
//                 //         checktype = 2;
//                 //     }
//                 //     else
//                 //     {
//                 //         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                 //         checktype = 3;
//                 //     }
//                 // }
//                 // else
//                 // {
//                 //     gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                 // }
//                 // if (tiles[12].value > 9)
//                 // {
//                 //     if (checktype == 1)
//                 //     {
//                 //         gamedata.score_check.insert(gamedata.score_check.begin(), 75);
//                 //     }
//                 //     else
//                 //     {
//                 //         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                 //     }
//                 // }
//                 // else
//                 // {
//                 //     gamedata.score_check.insert(gamedata.score_check.begin(), 76);
//                 //     if (tiles[12].value > 5)
//                 //     {
//                 //     }
//                 //     else
//                 //     {
//                 //         gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//                 //     }
//                 // }
//                 // }
//                 else // check1
//                 {
//                     gamedata.score_check.insert(gamedata.score_check.begin(), 56); // trial
//                 }
//             }
//             else // checkfirst == 1
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 65); // trial
//             }
//             int checksamp1 = wind_check(gamedata, tiles2[0], checkwind);
//             if (checksamp1 == 1)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 73);
//                 checkwind = 1;
//             }
//             else if (checksamp1 == 2)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 60);
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 61);
//             }
//             else if (checksamp1 == 3)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 60);
//             }
//             else if (checksamp1 == 4)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 61);
//             }
//             else
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//             }
//             int checksamp2 = wind_check(gamedata, tiles2[1], checkwind);
//             if (checksamp2 == 1)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 73);
//                 checkwind = 1;
//             }
//             else if (checksamp2 == 2)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 60);
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 61);
//             }
//             else if (checksamp2 == 3)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 60);
//             }
//             else if (checksamp2 == 4)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 61);
//             }
//             else
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//             }
//             int checksamp3 = wind_check(gamedata, tiles2[2], checkwind);
//             if (checksamp3 == 1)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 73);
//                 checkwind = 1;
//             }
//             else if (checksamp3 == 2)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 60);
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 61);
//             }
//             else if (checksamp3 == 3)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 60);
//             }
//             else if (checksamp3 == 4)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 61);
//             }
//             else
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//             }
//             int checksamp4 = wind_check(gamedata, tiles2[3], checkwind);
//             if (checksamp4 == 1)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 73);
//                 checkwind = 1;
//             }
//             else if (checksamp4 == 2)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 60);
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 61);
//             }
//             else if (checksamp4 == 3)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 60);
//             }
//             else if (checksamp4 == 4)
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 61);
//             }
//             else
//             {
//                 gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//             }
//             // vector<int> checksamp = {};
//             // for (int i = 0; i < tiles2.size(); i++)
//             // {
//             //     if (tiles2[i].value == 10)
//             //     {
//             //         checksamp[i] = wind_check(gamedata, tiles2[i], checkwind);
//             //         if (checksamp[i] == 1)
//             //         {
//             //             gamedata.score_check.insert(gamedata.score_check.begin(), 73);
//             //             checkwind = 1;
//             //         }
//             //         else if (checksamp[i] == 2)
//             //         {
//             //             gamedata.score_check.insert(gamedata.score_check.begin(), 60);
//             //             gamedata.score_check.insert(gamedata.score_check.begin(), 61);
//             //         }
//             //         else if (checksamp[i] == 3)
//             //         {
//             //             gamedata.score_check.insert(gamedata.score_check.begin(), 60);
//             //         }
//             //         else if (checksamp[i] == 4)
//             //         {
//             //             gamedata.score_check.insert(gamedata.score_check.begin(), 61);
//             //         }
//             //         else
//             //         {
//             //             gamedata.score_check.insert(gamedata.score_check.begin(), 0);
//             //         }
//             //     }
//             //     // else
//             //     // {
//             //     //     print("No wind tiles..");
//             //     // }
//             // }
//         }
//         else if (gamedata.pung_count == 3)
//         {
//             gamedata.score_check.insert(gamedata.score_check.begin(), 32);
//         }
//         else if (gamedata.pung_count == 2)
//         {
//             gamedata.score_check.insert(gamedata.score_check.begin(), 74);
//         }
//         else if (gamedata.pung_count == 1)
//         {
//             gamedata.score_check.insert(gamedata.score_check.begin(), 74);
//         }
//         else // Seven Pairs
//         {
//             gamedata.score_check.insert(gamedata.score_check.begin(), 74);
//         }
//     }
//     else if (gamedata.kong_count == 1)
//     {
//         gamedata.score_check.insert(gamedata.score_check.begin(), 74);
//     }
//     else if (gamedata.kong_count == 2)
//     {
//         gamedata.score_check.insert(gamedata.score_check.begin(), 74);
//     }
//     else if (gamedata.kong_count == 3)
//     {
//         gamedata.score_check.insert(gamedata.score_check.begin(), 18);
//     }
//     else
//     {
//         gamedata.score_check.insert(gamedata.score_check.begin(), 3);
//         gamedata.score_check.insert(gamedata.score_check.begin(), 12);
//     }
//     sumscore(gamedata);
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