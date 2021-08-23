#include "gameplay.cpp"
// #include "coinicaextra.cpp"

// // function for adding bet
// ACTION coinicagames::mhladdbet(int id, double quantity)
// {
//     require_auth(_self);
//     auto itr = _mahjonghilos.find(id);
//     // check(itr == _mahjonghilos.end(), "Either User has Initialized a Game or has an Existing Game");
//     auto mahjonghilo = _mahjonghilos.find(id);
//     _mahjonghilos.modify(mahjonghilo, _self, [&](auto &modified_mahjonghilo) {
//         mhlgame &game_data = modified_mahjonghilo.game_data;
//         game_data.hi_lo_balance += quantity;
//     });
// }

// ACTION coinicagames::mhlinitialze(int id)
// {
//     require_auth(_self);
//     // Ensure this action is authorized by the player
//     if (_mahjonghilos.find(id) == _mahjonghilos.end())
//     {
//         _mahjonghilos.emplace(_self, [&](auto &new_mahjonghilo) { new_mahjonghilo.id = id; });
//     }

//     auto itr = _mahjonghilos.find(id);
//     check(itr != _mahjonghilos.end(), "User or Game Doesn't Exist.");
//     // uint64_t id = 0;
//     auto size = transaction_size();
//     char buf[size];
//     check(size == read_transaction(buf, size), "read_transaction failed");
//     checksum256 h = sha256(buf, size);
//     auto hbytes = h.extract_as_byte_array();
//     string hash_string = checksum256_to_string(hbytes, hbytes.size()); // convert txID arr to string
//     check(itr->game_data.status != 1, "Continue or finish your current game..");
//     _mahjonghilos.modify(itr, _self, [&](auto &modified_mahjonghilo) {
//         modified_mahjonghilo.game_count += 1;
//         mhlgame &game_data = modified_mahjonghilo.game_data;
//         const int game_count = modified_mahjonghilo.game_count;
//         // game_data.hi_lo_balance += quantity;
//         game_data.game_id = hash_string.substr(0, 30) + to_string(rng(100));
//         game_data.status = MHL_ONGOING;
//         game_data.bet_status = 1;
//         int num1 = game_count % 16;
//         int num2 = game_count % 4;
//         if (num1 > 0 && num1 < 5)
//         {
//             game_data.prevalent_wind = EAST;
//         }
//         else if (num1 > 4 && num1 < 9)
//         {
//             game_data.prevalent_wind = SOUTH;
//         }
//         else if (num1 > 8 && num1 < 13)
//         {
//             game_data.prevalent_wind = WEST;
//         }
//         else
//         {
//             game_data.prevalent_wind = NORTH;
//         }
//         if (num2 == 1)
//         {
//             game_data.seat_wind = EAST;
//         }
//         else if (num2 == 2)
//         {
//             game_data.seat_wind = SOUTH;
//         }
//         else if (num2 == 3)
//         {
//             game_data.seat_wind = WEST;
//         }
//         else
//         {
//             game_data.seat_wind = NORTH;
//         }
//         gettile(game_data);
//     });
// }

// ACTION coinicagames::mhlplayhilo(int id, int option)
// {
//     require_auth(_self);
//     auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
//     int checking = 0;
//     tile checktile = table_deck.at(mahjonghilo.game_data.standard_tile);
//     if (checktile.value == 1 && option == 1)
//     {
//         checking = 5;
//     }
//     else if (checktile.value == 11 && option == 3)
//     {
//         checking = 10;
//     }
//     else if(mahjonghilo.game_data.sumofvalue[checktile.value-1]==0 && option == 3)
//     {
//         checking = 15;
//     }
//     else
//     {
//         checking = 1;
//     }
//     // check(, "Max number of draws reached.");
//     check(mahjonghilo.game_data.hand_player.size() < (14 + mahjonghilo.game_data.kong_count - mahjonghilo.game_data.reveal_kong.size()), "Discard a tile to draw a new one.");
//     check(mahjonghilo.game_data.draw_count < 34, " Draw limit reached..");
//     check(mahjonghilo.game_data.status == 1, "No ongoing game.");
//     check(mahjonghilo.game_data.option_status == 1, "No bet yet.");
//     check(checking != 5, "Cannot choose low.");
//     check(checking != 10, "Cannot choose high.");
//     check(checking != 15, "Cannot choose high.");
//     _mahjonghilos.modify(mahjonghilo, _self, [&](auto &modified_mahjonghilo) {
//         mhlgame &game_data = modified_mahjonghilo.game_data;
//         float current_stake = game_data.hi_lo_stake;
//         // if(game_data.hi_lo_result==2)
//         // {
//         //     game_data.hi_lo_bet = game_data.hi_lo_stake;
//         // }
//         // else
//         // {
//         //     game_data.hi_lo_bet = 1;
//         // }
//         gettile(game_data);
//         if (game_data.hand_player.size() <= 13)
//         {
//             sorthand(game_data.hand_player);
//         }
//         game_data.prediction = option;
//         // float bet = 1.0000;
//         const auto standard_tile = table_deck.at(game_data.standard_tile);
//         const auto current_tile = table_deck.at(game_data.current_tile);
//         game_data.hi_lo_stake *= hilo_step(game_data, standard_tile.value, current_tile.value);
//         if (game_data.hi_lo_stake != 0)
//         {
//             game_data.hi_lo_result = MHL_WIN;
//         }
//         else
//         {
//             game_data.hi_lo_result = MHL_LOSE;
//             game_data.hi_lo_bet = current_stake;
//         }
//         // game_data.hi_lo_winnings += game_data.hi_lo_prize;
//         // uint8_t tile_var = gettile(game_data);
//         // const auto hilo_tile = table_deck.at(standard_tile);
//         // hilo_step(game_data, standard_tile.value, current_tile.value, option, bet);
//         // game_data.standard_tile = game_data.current_tile;
//         // get_odds(game_data, current_tile.value);
//         if (game_data.hand_player.size() == (14 + mahjonghilo.game_data.kong_count - mahjonghilo.game_data.reveal_kong.size()))
//         {
//             // sorthand(game_data.hand_player);
//             winhand_check(game_data, game_data.hand_player);
//             if (game_data.winnable == 1)
//             {
//                 print("You have a winning hand.");
//             }
//             else
//             {
//                 if (game_data.draw_count >= 33)
//                 {
//                     game_data.hi_lo_balance += game_data.hi_lo_stake;
//                     game_data.hi_lo_stake = 0.0000;
//                     game_data.status = MHL_LOSE;
//                     game_data.bet_status = 1;
//                     print("Draw limits reached.");
//                 }
//                 else
//                 {
//                     print("Keep playing and try again..");
//                 }
//             }
//         }
//         // game_data.hi_lo_bet = 0;
//         game_data.bet_status = 1;
//         game_data.option_status = 0;
//     });
// }

// ACTION coinicagames::mhlresetbet(int id)
// {
//     require_auth(_self);

//     auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
//     // check(idx <= 13, "Index should be below 14.");
//     check(mahjonghilo.game_data.status == MHL_ONGOING, "Game already ended.");
//     check(mahjonghilo.game_data.hi_lo_result == MHL_LOSE, "Game already ended.");
//     _mahjonghilos.modify(mahjonghilo, _self, [&](auto &modified_mahjonghilo) {
//         mhlgame &game_data = modified_mahjonghilo.game_data;
//         game_data.hi_lo_bet = 0.0000;
//         // print(temptile.suit);
//     });
// }

// ACTION coinicagames::mhldscrdtile(int id, int idx)
// {
//     require_auth(_self);

//     auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
//     // check(idx <= 13, "Index should be below 14.");
//     check(mahjonghilo.game_data.status == MHL_ONGOING, "Game already ended.");
//     check(mahjonghilo.game_data.draw_count < 34, "Your hand is for declaration(win/lose).");
//     check(mahjonghilo.game_data.hand_player.size() == (14 + mahjonghilo.game_data.kong_count - mahjonghilo.game_data.reveal_kong.size()), "Have a complete hand before discarding a tile.");
//     _mahjonghilos.modify(mahjonghilo, _self, [&](auto &modified_mahjonghilo) {
//         mhlgame &game_data = modified_mahjonghilo.game_data;
//         game_data.winnable = 0;
//         game_data.pung_count = MHL_DEFAULT;
//         game_data.pair_count = MHL_DEFAULT;
//         game_data.chow_count = MHL_DEFAULT;
//         game_data.discarded_tiles.insert(game_data.discarded_tiles.begin(), game_data.hand_player[idx]);
//         game_data.hand_player.erase(game_data.hand_player.begin() + idx); // Remove the card from the hand
//         sorthand(game_data.hand_player);
//         // tile temptile = table_deck.at(game_data.hand_player[idx]);
//         // print(temptile.value);
//         // print(temptile.suit);
//     });
// }

// ACTION coinicagames::mhlstartbet(int id)
// {
//     require_auth(_self);

//     auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
//     check(mahjonghilo.game_data.hi_lo_balance >= 1 || mahjonghilo.game_data.hi_lo_stake != 0, "Not sufficient balance on account..");
//     check(mahjonghilo.game_data.hand_player.size() < (14 + mahjonghilo.game_data.kong_count - mahjonghilo.game_data.reveal_kong.size()), "Discard a tile to play Hi-Low..");
//     check(mahjonghilo.game_data.bet_status == 1, "Bet in place.");
//     _mahjonghilos.modify(mahjonghilo, _self, [&](auto &modified_mahjonghilo) {
//         mhlgame &game_data = modified_mahjonghilo.game_data;
//         // if(game_data.hi_lo_result==3 && game_data.hi_lo_stake!=0)
//         // {
//         //     game_data.hi_lo_stake = 0.0000;
//         // }
//         // else
//         // {        
//             if(game_data.hi_lo_result==2 && game_data.hi_lo_stake != 0)
//                 {
//                     game_data.hi_lo_bet = game_data.hi_lo_stake;
//                 }
//             else
//                 {
//                     // game_data.hi_lo_bet = 1;
//                     game_data.hi_lo_balance -= 1;
//                     game_data.hi_lo_bet += 1.0000;
//                     game_data.hi_lo_stake = game_data.hi_lo_bet;
//                 }
//         // }
//         // if (game_data.hi_lo_stake == 0)
//         // {
//         //     game_data.hi_lo_balance -= 10000;
//         //     game_data.hi_lo_stake += 1.0000;
//         // }
//         // else
//         // {
//         //     game_data.hi_lo_stake = game_data.hi_lo_stake;
//         // }
//         game_data.bet_status = 0;
//         game_data.option_status = 1;
//         game_data.hi_lo_outcome = MHL_DEFAULT;
//         game_data.standard_tile = game_data.current_tile;
//         const auto hilo_tile = table_deck.at(game_data.standard_tile);
//         get_odds(game_data, hilo_tile.value);
//     });
// }

// ACTION coinicagames::mhlwintrnsfr(int id)
// {
//     require_auth(_self);

//     auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
//     check(mahjonghilo.game_data.status != MHL_INITIALIZED, "Game haven't started.");
//     check(mahjonghilo.game_data.bet_status == 1, "Bet in place.");
//     // check(mahjonghilo.game_data.hi_lo_result != 3, "Last hi-lo was a lost.");
//     check(mahjonghilo.game_data.hi_lo_stake > 0.0000, "No winnings found.");
//     _mahjonghilos.modify(mahjonghilo, _self, [&](auto &modified_mahjonghilo) {
//         mhlgame &game_data = modified_mahjonghilo.game_data;
//         game_data.hi_lo_balance += game_data.hi_lo_stake;
//         game_data.hi_lo_stake = 0.0000;
//         game_data.hi_lo_bet = 0.0000;
//     });
// }

// ACTION coinicagames::mhldclrkong(int id, vector<int> idx)
// {
//     require_auth(_self);

//     auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
//     vector<tile> kongtile{};
//     // check(idx.size() == 4, "Array size should be equal to 4 to declare a kong.")
//     check(mahjonghilo.game_data.status == MHL_ONGOING, "No ongoing game..");
//     // check(mahjonghilo.game_data.hand_player.size() == (14 + mahjonghilo.game_data.kong_count - mahjonghilo.game_data.reveal_kong.size()), "Must have a complete hand to declare a kong.");
//     _mahjonghilos.modify(mahjonghilo, _self, [&](auto &modified_mahjonghilo) {
//         mhlgame &game_data = modified_mahjonghilo.game_data;
//         for (int i = 0; i < 4; i++)
//         {
//             kongtile[i] = table_deck.at(game_data.hand_player[idx[i]]);
//         }
//         // const auto kongtile1 = table_deck.at(game_data.hand_player[idx[0]]);
//         // const auto kongtile2 = table_deck.at(game_data.hand_player[idx[1]]);
//         // const auto kongtile3 = table_deck.at(game_data.hand_player[idx[2]]);
//         // const auto kongtile4 = table_deck.at(game_data.hand_player[idx[3]]);
//         int check1 = pair_check(kongtile[0], kongtile[1]);
//         int check2 = pair_check(kongtile[2], kongtile[3]);
//         check(check1 == 1 && check2 == 1, "Tiles are not of the same suit and value.");
//         if (pair_check(kongtile[0], kongtile[2]) == 1)
//         {
//             game_data.winnable = 0;
//             for (int i = 0; i < 4; i++)
//             {
//                 game_data.reveal_kong.insert(game_data.reveal_kong.begin(), game_data.hand_player[idx[i]]);
//             }
//             for (int i = 3; i >= 0; i--)
//             {
//                 game_data.hand_player.erase(game_data.hand_player.begin() + idx[i]);
//             }
//             game_data.kong_count += 1;
//         }
//         else
//         {
//             print("Tiles are not of the same suit and value.");
//         }
//         sorthand(game_data.reveal_kong);
//     });
// }

// ACTION coinicagames::mhldclrwnhnd(int id)
// {
//     require_auth(_self);
//     auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
//     check(mahjonghilo.game_data.status == MHL_ONGOING, "Game already ended.");
//     _mahjonghilos.modify(mahjonghilo, _self, [&](auto &modified_mahjonghilo) {
//         mhlgame &game_data = modified_mahjonghilo.game_data;
//         if (game_data.winnable == 1)
//         {
//             transferhand(game_data, game_data.hand_player.size());
//             vector<uint8_t> temp_hand = game_data.winning_hand;
//             sorteye(temp_hand, game_data.eye_idx);
//             getscore(game_data, temp_hand);
//             for (int i = 0; i < game_data.reveal_kong.size(); i++)
//             {
//                 game_data.winning_hand.insert(game_data.winning_hand.end(), game_data.reveal_kong[i]);
//             }
//             for (int i = 0; i < game_data.reveal_kong.size(); i++)
//             {
//                 game_data.reveal_kong.erase(game_data.reveal_kong.begin());
//             }
//             game_data.status = MHL_WIN;
//             // sorteye(temp_hand, game_data.eye_idx);
//             // getscore(game_data, temp_hand);
//             print("Well played!");
//         }
//         // else
//         // {
//         //     print("Your hand didn't win..");
//         // }
//         // if (game_data.hand_player.size() == 0)
//         // {
//         //     vector<uint8_t> temp_hand = game_data.winning_hand;
//         //     sorteye(temp_hand, game_data.eye_idx);
//         //     getscore(game_data, temp_hand);
//         //     for (int i = 0; i < game_data.reveal_kong.size(); i++)
//         //     {
//         //         game_data.winning_hand.insert(game_data.winning_hand.end(), game_data.reveal_kong[i]);
//         //     }
//         //     for (int i = 0; i < game_data.reveal_kong.size(); i++)
//         //     {
//         //         game_data.reveal_kong.erase(game_data.reveal_kong.begin());
//         //     }
//         //     game_data.status = WIN;
//         //     // sorteye(temp_hand, game_data.eye_idx);
//         //     // getscore(game_data, temp_hand);
//         //     print("Well played!");
//         // }
//         else
//         {
//             print("Your hand didn't win..");
//         }
//     });
// }

// // ACTION coinicagames::mhlsettledpay(int id, asset prize, string memo)
// // {
// //     require_auth(_self);
// //     auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
// //     check(mahjonghilo.game_data.status == ONGOING, "Game has ended and prize is already transferred.");
// // }

// ACTION coinicagames::mhlwithdraw(int id)
// {
//     require_auth(_self);
//     auto mahjonghilo = _mahjonghilos.find(id);
//     check(mahjonghilo->game_data.status != MHL_INITIALIZED, "Game has ended and prize is already transferred or you are on trial.");
//     check(mahjonghilo->game_data.hi_lo_balance > 0, "You have no prize money.");
//     check(mahjonghilo->game_data.hi_lo_stake == 0, "Transfer your winnings to withdraw.");
//     // std::string feedback = "MHL Withdraw: " + name{id}.to_string() + " received " + std::to_string(mahjonghilo->game_data.hi_lo_balance); // transfer funds to user
//     // action{
//     //     permission_level{_self, "active"_n},
//     //     eosio_token,
//     //     "transfer"_n,
//     //     std::make_tuple(_self, id, mahjonghilo->game_data.hi_lo_balance, feedback)}
//     //     .send();

//     _mahjonghilos.modify(mahjonghilo, _self, [&](auto &modified_mahjonghilo) {
//         mhlgame &game_data = modified_mahjonghilo.game_data;
//         game_data.hi_lo_balance = MHL_DEFAULT;
//         game_data.hi_lo_stake = MHL_DEFAULT;
//         game_data.hi_lo_bet = MHL_DEFAULT;
//     });
// }

// ACTION coinicagames::mhlendgame(int id)
// {
//     require_auth(_self);

//     auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
//     // check(mahjonghilo.game_data.status == ONGOING, "Game may haven't started yet, may on an ongoing trial, or may already ended.");
//     // // check(mahjonghilo.game_data.drawn_tiles.size() < 34, "Your hand is for declaration(win/lose).");
//     // // check(mahjonghilo.game_data.hand_player.size() == (14 + mahjonghilo.game_data.kong_count - mahjonghilo.game_data.reveal_kong.size()), "Have a complete hand before discarding a tile.");
//     // _mahjonghilos.modiuseritr, _self, [&](auto &modified_mahjonghilo) {
//     //     mhlgame &game_data = modified_mahjonghilo.game_data;

//     // });
//     // uint64_t id = 0;
//     auto size = transaction_size();
//     char buf[size];
//     check(size == read_transaction(buf, size), "read_transaction failed");
//     checksum256 h = sha256(buf, size);
//     auto hbytes = h.extract_as_byte_array();
//     string hash_string = checksum256_to_string(hbytes, hbytes.size()); // convert txID arr to string
//     check(mahjonghilo.game_data.status != 0, "No ongoing game..");
//     check(mahjonghilo.game_data.option_status == 0, "Bet in place..");
//     _mahjonghilos.modify(mahjonghilo, _self, [&](auto &modified_mahjonghilo) {
//         modified_mahjonghilo.game_count += 1;
//         mhlgame &game_data = modified_mahjonghilo.game_data;
//         const int game_count = modified_mahjonghilo.game_count;
//         game_data.deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
//         game_data.game_id = hash_string.substr(0, 30) + to_string(rng(100));
//         game_data.sumofvalue = {12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 12};
//         game_data.status = MHL_ONGOING;
//         game_data.hi_lo_balance += game_data.hi_lo_stake;
//         game_data.hi_lo_stake = 0.0000;
//         game_data.bet_status = 1;
//         game_data.option_status = 0;
//         game_data.hi_lo_result = MHL_DEFAULT;
//         // game_data.hi_lo_prize = MHL_DEFAULT;
//         game_data.low_odds = MHL_DEFAULT;
//         game_data.draw_odds = MHL_DEFAULT;
//         game_data.high_odds = MHL_DEFAULT;
//         game_data.winnable = MHL_DEFAULT;
//         game_data.hand_player = {};
//         game_data.discarded_tiles = {};
//         game_data.reveal_kong = {};
//         game_data.winning_hand = {};
//         game_data.score_check = {};
//         game_data.score_type = {};
//         game_data.pair_count = MHL_DEFAULT;
//         game_data.pung_count = MHL_DEFAULT;
//         game_data.chow_count = MHL_DEFAULT;
//         game_data.kong_count = MHL_DEFAULT;
//         game_data.draw_count = MHL_DEFAULT;
//         game_data.final_score = MHL_DEFAULT;
//         game_data.current_tile = MHL_DEFAULT;
//         game_data.standard_tile = MHL_DEFAULT;
//         int num1 = game_count % 16;
//         int num2 = game_count % 4;
//         if (num1 > 0 && num1 < 5)
//         {
//             game_data.prevalent_wind = EAST;
//         }
//         else if (num1 > 4 && num1 < 9)
//         {
//             game_data.prevalent_wind = SOUTH;
//         }
//         else if (num1 > 8 && num1 < 13)
//         {
//             game_data.prevalent_wind = WEST;
//         }
//         else
//         {
//             game_data.prevalent_wind = NORTH;
//         }
//         if (num2 == 1)
//         {
//             game_data.seat_wind = EAST;
//         }
//         else if (num2 == 2)
//         {
//             game_data.seat_wind = SOUTH;
//         }
//         else if (num2 == 3)
//         {
//             game_data.seat_wind = WEST;
//         }
//         else
//         {
//             game_data.seat_wind = NORTH;
//         }
//         gettile(game_data);
//         // const auto hilo_tile = table_deck.at(game_data.standard_tile);
//         // get_odds(game_data, hilo_tile.value);
//     });
// }

// ACTION coinicagames::mhlend(int id)
// {
//     require_auth(_self);
//     // check(has_auth(_self) || has_auth(id), "Unauthorized user");
//     auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
//     check(mahjonghilo.game_data.hi_lo_balance == 0.0000 && mahjonghilo.game_data.hi_lo_stake == 0.0000, "Withdraw your balance before you can end.");
//     _mahjonghilos.erase(mahjonghilo);
// }

// ACTION coinicagames::mhldel(int size)
// {
//     require_auth(_self);
//     auto itr = _mahjonghilos.begin();
//     while (itr != _mahjonghilos.end())
//     {
//         itr = _mahjonghilos.erase(itr);
//     }
// }
// // ACTION coinicagames::mhlendall(int size)
// // {
// //     require_auth(_self);
// //     // check(has_auth(_self) || has_auth(id), "Unauthorized user");
// //     // auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
// //     // check(mahjonghilo.game_data.hi_lo_balance == 0.0000 && mahjonghilo.game_data.hi_lo_winnings == 0.0000, "Withdraw your balance before you can end.");
// //     for (int i = 0; i < 5; i++)
// //     {
// //         auto itr = _mahjonghilos.begin();
// //         _mahjonghilos.erase(itr);
// //     }
// // }

// // ACTION coinicagames::mhlstarttrial(int id, int numgames, vector<int> idx)
// // {
// //     require_auth(_self);
// //     auto &mahjonghilo = _mahjonghilos.get(id, "User doesn't exist");
// //     // check(mahjonghilo.game_data.status == INITIALIZED || mahjonghilo.game_data.status == ONTRIAL, "Cannot do a trial. Initialize a new game.");
// //     _mahjonghilos.userify(itr, _self, [&](auto &modified_mahjonghilo) {
// //         mhlgame &game_data = modified_mahjonghilo.game_data;
// //         game_data.status = ONTRIAL;
// //         game_data.hand_player = {};
// //         game_data.winning_hand = {};
// //         game_data.score_check = {};
// //         game_data.score_type = {};
// //         game_data.deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
// //         // int num1 = numgames % 16;
// //         // int num2 = numgames % 4;
// //         // if (num1 > 0 && num1 < 5)
// //         // {
// //         //     game_data.prevalent_wind = EAST;
// //         // }
// //         // else if (num1 > 4 && num1 < 9)
// //         // {
// //         //     game_data.prevalent_wind = SOUTH;
// //         // }
// //         // else if (num1 > 8 && num1 < 13)
// //         // {
// //         //     game_data.prevalent_wind = WEST;
// //         // }
// //         // else
// //         // {
// //         //     game_data.prevalent_wind = NORTH;
// //         // }
// //         // if (num2 == 1)
// //         // {
// //         //     game_data.seat_wind = EAST;
// //         // }
// //         // else if (num2 == 2)
// //         // {
// //         //     game_data.seat_wind = SOUTH;
// //         // }
// //         // else if (num2 == 3)
// //         // {
// //         //     game_data.seat_wind = WEST;
// //         // }
// //         // else
// //         // {
// //         //     game_data.seat_wind = NORTH;
// //         // }
// //         // if (game_data.hand_player.size() != 0)
// //         // {
// //         //     int size5 = game_data.hand_player.size();
// //         //     for (int i = 0; i < size5; i++)
// //         //     {
// //         //         game_data.hand_player.erase(game_data.hand_player.begin());
// //         //     }
// //         // }
// //         // // else
// //         // // {
// //         // if (game_data.winning_hand.size() != 0)
// //         // {
// //         //     int size = game_data.winning_hand.size();
// //         //     for (int i = 0; i < size; i++)
// //         //     {
// //         //         game_data.winning_hand.erase(game_data.winning_hand.begin());
// //         //     }
// //         //     game_data.pair_count = 0;
// //         //     game_data.pung_count = 0;
// //         //     game_data.chow_count = 0;
// //         //     game_data.final_score = 0;
// //         //     int size2 = game_data.score_check.size();
// //         //     game_data.deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
// //         //     for (int i = 0; i < size2; i++)
// //         //     {
// //         //         game_data.score_check.erase(game_data.score_check.begin());
// //         //     }
// //         //     int size3 = game_data.score_type.size();
// //         //     for (int i = 0; i < size3; i++)
// //         //     {
// //         //         game_data.score_type.erase(game_data.score_type.begin());
// //         //     }
// //         // }
// //         //     else
// //         //     {
// //         //         int size = game_data.hand_player.size();
// //         //         for (int i = 0; i < size; i++)
// //         //         {
// //         //             game_data.hand_player.erase(game_data.hand_player.begin());
// //         //         }
// //         //     }
// //         for (int i = 0; i < idx.size(); i++)
// //         {
// //             game_data.hand_player.insert(game_data.hand_player.begin(), game_data.deck_player[idx[i] - 1]); // Assign the tile to the first empty slot in the hand
// //         }
// //         sorthand(game_data.hand_player);
// //         winhand_check(game_data, game_data.hand_player);
// //         if (game_data.winnable == 1)
// //         {
// //             transferhand(game_data, game_data.hand_player.size());
// //             sorteye(game_data.winning_hand, game_data.eye_idx);
// //             getscore(game_data, game_data.winning_hand);
// //             sorthand(game_data.score_check);
// //             print("Well played!");
// //             // print(game_data.final_score);
// //         }
// //         else
// //         {
// //             print("Your hand didn't win..");
// //         }
// //         // }
// //         game_data.winnable = 0;
// //     });
// // }
