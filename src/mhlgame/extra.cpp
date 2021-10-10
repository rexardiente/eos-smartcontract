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