#include <eosio/crypto.hpp>
#include "gameplay.cpp"
#include "coinicaextra.cpp"

ACTION coinicagames::gqinitialize(int id)
{
    require_auth(_self);
    auto itr = _ghostquests.find(id);
    ghostquests_table ghostqueststbl(_self, id);
    check(itr == _ghostquests.end(), "Error : Either User has Initialized a Game or has an Existing Game");
    if (itr == _ghostquests.end())
    {
        _ghostquests.emplace(_self, [&](auto &new_ghostquests)
                             { new_ghostquests.id = id; });
    }
}

ACTION coinicagames::gqgenchar(int id, double quantity, int limit) // generate characters after transfer transaction
{
    require_auth(_self);
    auto &ghostquest = _ghostquests.get(id, "User doesn't exist");
    auto itr = _ghostquests.find(id);
    int counter = ghostquest.game_data.characters.size();
    string hash_string = checksum256_to_string_hash();
    check(itr != _ghostquests.end(), "Game Doesn't Exist.");
    // check(ghostquest.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    _ghostquests.modify(ghostquest, _self, [&](auto &modified_ghostquest)
                        {
                            gqgamedata &game_data = modified_ghostquest.game_data;
                            auto current_time = current_time_point().time_since_epoch()._count;

                            for (int i = counter; i < (counter + quantity); i++) // summon character/characters and hitpoints
                            {
                                string key = hash_string.substr(0, 30) + to_string(i);
                                character new_ghost;
                                new_ghost.owner_id = id;
                                new_ghost.prize = 1;
                                new_ghost.battle_limit = limit;
                                new_ghost.status = GQ_SUMMONED;
                                new_ghost.character_life = 1;
                                base_stat tempstat = stat_deck[random(stat_deck.size())];
                                new_ghost.ghost_name = tempstat.ghost_name;
                                new_ghost.ghost_id = tempstat.ghost_id;
                                new_ghost.hitpoints = 101 + random(7) + ((tempstat.hp - 1) * 8);
                                new_ghost.rarity = tempstat.rarity;
                                new_ghost.attack = 25 + random(8) + ((tempstat.base_atk - 1) * 9);
                                new_ghost.defense = 25 + random(8) + ((tempstat.base_def - 1) * 9);
                                new_ghost.speed = 25 + random(8) + ((tempstat.base_spd - 1) * 9);
                                new_ghost.luck = 25 + random(8) + ((tempstat.base_lck - 1) * 9);
                                new_ghost.created_at = current_time;
                                game_data.characters.insert(game_data.characters.end(), pair<string, character>(key, new_ghost));
                                current_time += 1;
                            }
                        });
}

ACTION coinicagames::gqaddlife(int id, double quantity, string key) // add life/lives after transfer transaction
{
    require_auth(_self);
    auto &ghostquest = _ghostquests.get(id, "User doesn't exist");
    auto characters = ghostquest.game_data.characters;
    map<string, character>::iterator itr = characters.find(key);
    check(itr->second.character_life > 0, "Character cease to exist.");
    itr->second.character_life += quantity;

    _ghostquests.modify(ghostquest, _self, [&](auto &modified_ghostquest)
                        {
                            gqgamedata &game_data = modified_ghostquest.game_data;
                            game_data.characters = characters;
                        });
}

ACTION coinicagames::gqbttlrslt(string gameid, pair<string, int> winner, pair<string, int> loser) // battle action
{
    require_auth(_self);
    check(winner.second != loser.second, "Same Character are not allowed to match");
    check(winner.first != loser.first, "Same Character are not allowed to match");
    auto &winner_player = _ghostquests.get(winner.second, "User doesn't exist.");
    auto &loser_player = _ghostquests.get(loser.second, "User doesn't exist.");

    map<string, character> player1_characters = winner_player.game_data.characters;
    map<string, character> player2_characters = loser_player.game_data.characters;
    vector<map<string, character>::iterator> itr{player1_characters.find(winner.first), player2_characters.find(loser.first)};

    check(itr[0]->second.character_life != 0, winner.first + " No remaining HP.");
    check(itr[1]->second.character_life != 0, loser.first + " No remaining HP.");
    check(itr[0]->second.battle_count <= itr[0]->second.battle_limit, winner.first + " reached battle limit.");
    check(itr[1]->second.battle_count <= itr[1]->second.battle_limit, loser.first + " reached battle limit.");

    // update each players battle details..
    for_each(itr.begin(), itr.end(), [&](map<string, character>::iterator &n)
             {
                 // n->second.match_history.insert(n->second.match_history.end(), pair<string, battle_history>(gameid, current_battle));
                 // n->second.last_match = time_executed;
                 n->second.battle_count += 1;
             });

    _ghostquests.modify(winner_player, _self, [&](auto &modified_ghostquest)
                        {
                            gqgamedata &game_data = modified_ghostquest.game_data;
                            itr[0]->second.character_life += 1;
                            game_data.characters = player1_characters;
                        });
    _ghostquests.modify(loser_player, _self, [&](auto &modified_ghostquest)
                        {
                            gqgamedata &game_data = modified_ghostquest.game_data;
                            itr[1]->second.character_life -= 1;
                            if (itr[1]->second.character_life == 0)
                            {
                                itr[1]->second.status = GQ_ELIMINATED;
                            }
                            game_data.characters = player2_characters;
                        });
}

ACTION coinicagames::gqwithdraw(int id, string key)
{
    require_auth(_self);
    auto user = _ghostquests.find(id);
    check(user->game_data.characters.find(key)->second.character_life > 0, "Ghost doesn't exist.");
    check(user->game_data.characters.find(key)->second.status != 3, "Ghost is currently in battle.");
    check(user->game_data.characters.find(key)->second.battle_count > 0, "Ghost has not been in a battle.");
    auto characters = user->game_data.characters;
    map<string, character>::iterator itr = characters.find(key);
    eliminated_withdrawn(itr); // modify withdrawn character
    _ghostquests.modify(user, _self, [&](auto &modified_ghostquest)
                        {
                            gqgamedata &game_data = modified_ghostquest.game_data;
                            game_data.characters = characters;
                        });
}

ACTION coinicagames::gqeliminate(int id, string key) // generate stats of monsters after transfer transaction
{
    require_auth(_self);

    auto &ghostquest = _ghostquests.get(id, "User doesn't exist");
    check(ghostquest.game_data.characters.at(key).status != 3, "Character in battle.");
    check(ghostquest.game_data.characters.find(key)->second.character_life == 0, "Ghost doesn't exist.");
    _ghostquests.modify(ghostquest, _self, [&](auto &modified_ghostquest)
                        {
                            gqgamedata &game_data = modified_ghostquest.game_data;
                            game_data.characters.erase(key);
                        });
}

ACTION coinicagames::gqend(int id)
{
    require_auth(_self);
    auto &ghostquest = _ghostquests.get(id, "User doesn't exist");
    // check(ghostquest.game_data.status == DONE, "End your existing game first.");
    _ghostquests.erase(ghostquest);
}

ACTION coinicagames::gqdelall(int size)
{
    require_auth(_self);
    auto itr = _ghostquests.begin();
    while (itr != _ghostquests.end())
    {
        itr = _ghostquests.erase(itr);
    }
}

ACTION coinicagames::thinitialize(int id, uint8_t destination, uint8_t enemy_count, vector<uint8_t> panels, double quantity)
{
    require_auth(_self);
    auto itr = _thunts.find(id);
    check(itr == _thunts.end(), "Error: Either User has Initialized a Game or has an Existing Game");
    check(enemy_count > 0 && enemy_count < 16, "Please input valid enemy count.");
    check(itr->game_data.enemy_count < TH_PANEL_SIZE, "Can't have enemy greater than or equal to 16");
    check(itr->game_data.panel_set.empty(), "Error: Either User has an Existing Game");

    if (itr == _thunts.end())
    {
        thunts_table thuntstbl(_self, id);
        _thunts.emplace(_self, [&](auto &new_thunts)
                        {
                            new_thunts.id = id;
                            thgamedata game_data = new_thunts.game_data;
                            game_data.destination = destination;
                            game_data.enemy_count = enemy_count;
                            // process tiles sequences..
                            vector<thtile> new_panel_set;
                            for (int i = 0; i < TH_PANEL_SIZE; i++)
                            {
                                thtile new_tile;
                                new_tile.key = panels[i];
                                new_panel_set.insert(new_panel_set.end(), new_tile);
                            }
                            game_data.panel_set = new_panel_set;
                            game_data.prize = quantity;
                            game_data.game_id = checksum256_to_string_hash().substr(0, 30);
                            game_data.maxprize = maxprize(game_data);
                            game_data.status = TH_ONGOING;

                            new_thunts.game_data = game_data;
                            gameupdate(new_thunts.game_data);
                        });
    }
}

ACTION coinicagames::thopentile(int id, uint8_t index)
{
    require_auth(_self);
    auto itr = _thunts.find(id);
    check(itr->game_data.win_count != (TH_PANEL_SIZE - itr->game_data.enemy_count), "You already found all treasures, feel free to withdraw.");
    check(itr->game_data.status == TH_ONGOING, "Either game has ended or not yet configured.");
    check(itr->game_data.panel_set.at(index).isopen == DEFAULT, "Tile already opened!");

    _thunts.modify(itr, _self, [&](auto &modified_thunt)
                   {
                       thgamedata &game_data = modified_thunt.game_data;
                       // float available = TH_PANEL_SIZE - game_data.enemy_count - game_data.win_count;
                       // float chance = available / (TH_PANEL_SIZE - game_data.win_count) * 100;
                       if (random(TH_PANEL_SIZE) % 2)
                       {
                           game_data.prize = generateprize(game_data);
                           game_data.panel_set[index].iswin = 1;
                           game_data.win_count++; // count number of chest found
                           game_data.unopentile--;
                       }
                       else
                       {
                           game_data.status = TH_DONE;
                       }
                       game_data.panel_set[index].isopen = 1;
                       gameupdate(game_data);
                   });
}

ACTION coinicagames::thend(int id)
{
    require_auth(_self);
    auto itr = _thunts.find(id);
    check(itr->game_data.status == TH_DONE, "End your existing game first.");
    _thunts.erase(itr);
}

ACTION coinicagames::thwithdraw(int id)
{
    require_auth(_self);
    // auto user = _thunts.find(user_id);
    auto itr = _thunts.find(id);
    check(itr->game_data.win_count > 0, "You have not found any treasure yet.");
    check(itr->game_data.prize > 0, "You've already lost.");
    check(itr->game_data.status == TH_ONGOING, "Game has ended and prize is already transferred.");

    _thunts.modify(itr, _self, [&](auto &modified_thunt)
                   {
                       thgamedata &game_data = modified_thunt.game_data;
                       game_data.prize = DEFAULT;
                       game_data.status = TH_DONE;
                   });
}

ACTION coinicagames::thautoplay(int id, vector<uint8_t> to_open_panel)
{
    require_auth(_self);
    auto itr = _thunts.find(id);
    check(itr->game_data.status == TH_ONGOING, "Either game has ended or not yet configured.");
    _thunts.modify(itr, _self, [&](auto &modified_thunt)
                   {
                       thgamedata &game_data = modified_thunt.game_data;
                       for (size_t i = 0; i < to_open_panel.size(); i++)
                       {
                           thtile &thtile = game_data.panel_set[to_open_panel[i]];
                           if (thtile.isopen == DEFAULT)
                           {
                               thtile.isopen = 1;
                               float available = TH_PANEL_SIZE - game_data.enemy_count - game_data.win_count;
                               float chance = available / (TH_PANEL_SIZE - game_data.win_count) * 100;
                               if (random(100) < chance)
                               {
                                   game_data.prize = generateprize(game_data);
                                   game_data.win_count++; // count number of chest found
                                   thtile.iswin = 1;
                               }
                               else
                               {
                                   game_data.status = TH_DONE;
                                   game_data.nextprize = DEFAULT;
                                   game_data.prize = DEFAULT;
                                   game_data.odds = DEFAULT;
                                   break;
                               }
                               game_data.unopentile--;
                               game_data.odds = calculateodds(game_data);
                               game_data.nextprize = generateprize(game_data);
                           }
                       }
                   });
}

ACTION coinicagames::mhladdbet(int id, double quantity)
{
    require_auth(_self);
    auto itr = _mjhilos.find(id);
    // check(itr == _mjhilos.end(), "Either User has Initialized a Game or has an Existing Game");
    // auto user = _mjhilos.find(id);
    _mjhilos.modify(itr, _self, [&](auto &modified_mjhilo)
                    {
                        mhlgamedata &game_data = modified_mjhilo.game_data;
                        game_data.hi_lo_balance += quantity;
                    });
}

ACTION coinicagames::mhlinitialze(int id)
{
    require_auth(_self);
    // Ensure this action is authorized by the player
    if (_mjhilos.find(id) == _mjhilos.end())
    {
        _mjhilos.emplace(_self, [&](auto &new_mjhilo)
                         { new_mjhilo.id = id; });
    }

    auto itr = _mjhilos.find(id);
    check(itr != _mjhilos.end(), "User or Game Doesn't Exist.");
    // uint64_t id = 0;
    check(itr->game_data.status != 1, "Continue or finish your current game..");
    check(itr->game_data.option_status == 0, "Bet in place..");
    _mjhilos.modify(itr, _self, [&](auto &modified_mjhilo)
                    {
                        modified_mjhilo.game_count += 1;
                        mhlgamedata &game_data = modified_mjhilo.game_data;
                        const int game_count = modified_mjhilo.game_count;
                        // game_data.hi_lo_balance += quantity;
                        game_data.game_id = checksum256_to_string_hash().substr(0, 30);
                        game_data.status = MHL_ONGOING;
                        // if (game_data.hi_lo_stake > 0)
                        // {
                        //     game_data.hi_lo_balance += game_data.hi_lo_stake;
                        // }
                        game_data.hi_lo_stake = DEFAULT;
                        game_data.hi_lo_bet = DEFAULT;
                        game_data.bet_status = 1;
                        if (game_data.deck_player.size() < 136)
                        {
                            game_data.deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
                            game_data.sumofvalue = {12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 12};
                            game_data.hi_lo_stake = 0.0000;
                            game_data.option_status = 0;
                            game_data.hi_lo_result = DEFAULT;
                            // game_data.hi_lo_prize = DEFAULT;
                            game_data.low_odds = DEFAULT;
                            game_data.draw_odds = DEFAULT;
                            game_data.high_odds = DEFAULT;
                            game_data.riichi_status = DEFAULT;
                            while (game_data.hand_player.size() != 0)
                            {
                                game_data.hand_player.erase(game_data.hand_player.begin());
                            }
                            while (game_data.discarded_tiles.size() != 0)
                            {
                                game_data.discarded_tiles.erase(game_data.discarded_tiles.begin());
                            }
                            while (game_data.reveal_kong.size() != 0)
                            {
                                game_data.reveal_kong.erase(game_data.reveal_kong.begin());
                            }
                            while (game_data.winning_hand.size() != 0)
                            {
                                game_data.winning_hand.erase(game_data.winning_hand.begin());
                            }
                            while (game_data.score_check.size() != 0)
                            {
                                game_data.score_check.erase(game_data.score_check.begin());
                            }
                            while (game_data.score_type.size() != 0)
                            {
                                game_data.score_type.erase(game_data.score_type.begin());
                            }
                            while (game_data.wintiles.size() != 0)
                            {
                                game_data.wintiles.erase(game_data.wintiles.begin());
                            }
                            // game_data.discarded_tiles = {};
                            // game_data.reveal_kong = {};
                            // game_data.winning_hand = {};
                            // game_data.score_check = {};
                            // game_data.score_type = {};
                            game_data.pair_count = DEFAULT;
                            game_data.pung_count = DEFAULT;
                            game_data.chow_count = DEFAULT;
                            game_data.kong_count = DEFAULT;
                            game_data.draw_count = DEFAULT;
                            game_data.final_score = DEFAULT;
                            game_data.current_tile = DEFAULT;
                            game_data.standard_tile = DEFAULT;
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

ACTION coinicagames::mhlplayhilo(int id, int option)
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
    else if (mjhilo.game_data.sumofvalue[checktile.value - 1] == 0 && option == 3)
    {
        checking = 15;
    }
    else
    {
        checking = 1;
    }
    // check(, "Max number of draws reached.");
    check(mjhilo.game_data.hand_player.size() < (14 + mjhilo.game_data.kong_count - mjhilo.game_data.reveal_kong.size()), "Discard a tile to draw a new one.");
    check(mjhilo.game_data.hi_lo_balance >= 1 || mjhilo.game_data.hi_lo_stake != 0, "Not sufficient balance on account..");
    check(mjhilo.game_data.draw_count < 34, " Draw limit reached..");
    check(mjhilo.game_data.status == 1, "No ongoing game.");
    check(mjhilo.game_data.option_status == 1, "No bet yet.");
    check(checking != 5, "Cannot choose low.");
    check(checking != 10, "Cannot choose high.");
    check(checking != 15, "Cannot choose high.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo)
                    {
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
                            // riichi_check(game_data, game_data.hand_player);
                            // if (game_data.riichi_status != 0)
                            // {
                            //     print("You have a winning hand.");
                            // }
                            // else
                            // {
                            //     if (game_data.draw_count >= 33)
                            //     {
                            //         game_data.hi_lo_balance += game_data.hi_lo_stake;
                            //         game_data.hi_lo_stake = 0.0000;
                            //         game_data.status = MHL_LOSE;
                            //         game_data.bet_status = 1;
                            //         print("Draw limits reached.");
                            //     }
                            //     else
                            //     {
                            //         print("Keep playing and try again..");
                            //     }
                            // }
                            game_data.riichi_status = 1;
                            winnables tempwin;
                            tempwin.index = 5;
                            vector<mhltile> tiles;
                            tiles.insert(tiles.begin(), table_deck.at(random(game_data.deck_player.size())));
                            tempwin.tileswin = tiles;
                            game_data.wintiles.insert(game_data.wintiles.end(), tempwin);
                        }
                        // game_data.hi_lo_bet = 0;
                        game_data.bet_status = 1;
                        game_data.option_status = 0;
                    });
}

ACTION coinicagames::mhlresetbet(int id)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    // check(idx <= 13, "Index should be below 14.");
    check(mjhilo.game_data.status == MHL_ONGOING, "Game already ended.");
    check(mjhilo.game_data.hi_lo_result == MHL_LOSE, "Game already ended.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo)
                    {
                        mhlgamedata &game_data = modified_mjhilo.game_data;
                        game_data.hi_lo_bet = 0.0000;
                    });
}

ACTION coinicagames::mhldscrdtile(int id, int idx)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    // check(idx <= 13, "Index should be below 14.");
    check(mjhilo.game_data.status == MHL_ONGOING, "Game already ended.");
    check(mjhilo.game_data.draw_count < 34, "Your hand is for declaration(win/lose).");
    check(mjhilo.game_data.hand_player.size() == (14 + mjhilo.game_data.kong_count - mjhilo.game_data.reveal_kong.size()), "Have a complete hand before discarding a tile.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo)
                    {
                        mhlgamedata &game_data = modified_mjhilo.game_data;
                        // game_data.riichi_status = 0;
                        if (game_data.riichi_status == 2)
                        {
                            game_data.discarded_tiles.insert(game_data.discarded_tiles.begin(), game_data.hand_player[idx]);
                            game_data.hand_player.erase(game_data.hand_player.end()); // Remove the card from the hand
                        }
                        else
                        {
                            game_data.pung_count = DEFAULT;
                            game_data.pair_count = DEFAULT;
                            game_data.chow_count = DEFAULT;
                            for (int i = 0; i < game_data.wintiles.size(); i++)
                            {
                                game_data.wintiles.erase(game_data.wintiles.begin());
                            }
                            if (game_data.riichi_status == 1)
                            {
                                game_data.riichi_status = 0;
                            }
                            game_data.discarded_tiles.insert(game_data.discarded_tiles.begin(), game_data.hand_player[idx]);
                            game_data.hand_player.erase(game_data.hand_player.begin() + idx); // Remove the card from the hand
                            sorthand(game_data.hand_player);
                        }
                    });
}

ACTION coinicagames::mhlstartbet(int id)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    check(mjhilo.game_data.hi_lo_balance >= 1 || mjhilo.game_data.hi_lo_stake != 0, "Not sufficient balance on account..");
    check(mjhilo.game_data.hand_player.size() < (14 + mjhilo.game_data.kong_count - mjhilo.game_data.reveal_kong.size()), "Discard a tile to play Hi-Low..");
    check(mjhilo.game_data.bet_status == 1, "Bet in place.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo)
                    {
                        mhlgamedata &game_data = modified_mjhilo.game_data;
                        // if(game_data.hi_lo_result==3 && game_data.hi_lo_stake!=0)
                        // {
                        //     game_data.hi_lo_stake = 0.0000;
                        // }
                        // else
                        // {
                        if (game_data.hi_lo_result == 2 && game_data.hi_lo_stake != 0)
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

ACTION coinicagames::mhlwintrnsfr(int id)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    check(mjhilo.game_data.status != MHL_INITIALIZED, "Game haven't started.");
    check(mjhilo.game_data.bet_status == 1, "Bet in place.");
    // check(mjhilo.game_data.hi_lo_result != 3, "Last hi-lo was a lost.");
    check(mjhilo.game_data.hi_lo_stake > 0.0000, "No winnings found.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo)
                    {
                        mhlgamedata &game_data = modified_mjhilo.game_data;
                        game_data.hi_lo_balance += game_data.hi_lo_stake;
                        game_data.hi_lo_stake = 0.0000;
                        game_data.hi_lo_bet = 0.0000;
                    });
}

ACTION coinicagames::mhldclrkong(int id, vector<int> idx)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    vector<mhltile> kongtile{};
    // check(idx.size() == 4, "Array size should be equal to 4 to declare a kong.")
    check(mjhilo.game_data.status == MHL_ONGOING, "No ongoing game..");
    // check(mjhilo.game_data.hand_player.size() == (14 + mjhilo.game_data.kong_count - mjhilo.game_data.reveal_kong.size()), "Must have a complete hand to declare a kong.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo)
                    {
                        mhlgamedata &game_data = modified_mjhilo.game_data;
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
                            game_data.riichi_status = 0;
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

ACTION coinicagames::mhldclrwnhnd(int id)
{
    require_auth(_self);
    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    check(mjhilo.game_data.status == MHL_ONGOING, "Game already ended.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo)
                    {
                        mhlgamedata &game_data = modified_mjhilo.game_data;
                        if (game_data.riichi_status != 0)
                        {
                            // transferhand(game_data, game_data.hand_player.size());
                            vector<uint8_t> temp_hand = game_data.winning_hand;
                            // sorteye(temp_hand, game_data.eye_idx);
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

ACTION coinicagames::mhlwithdraw(int id)
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

    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo)
                    {
                        mhlgamedata &game_data = modified_mjhilo.game_data;
                        game_data.hi_lo_balance = DEFAULT;
                        game_data.hi_lo_stake = DEFAULT;
                        game_data.hi_lo_bet = DEFAULT;
                    });
}

ACTION coinicagames::mhlend(int id)
{
    require_auth(_self);
    // check(has_auth(_self) || has_auth(id), "Unauthorized user");
    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    check(mjhilo.game_data.option_status == 0, "Bet in place..");
    // check(mjhilo.game_data.hi_lo_bet == 0 && mjhilo.game_data.hi_lo_stake == 0, "Bet in place.");
    // check(mjhilo.game_data.hi_lo_balance == 0.0000 && mjhilo.game_data.hi_lo_stake == 0.0000, "Withdraw your balance before you can end.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo)
                    {
                        mhlgamedata &game_data = modified_mjhilo.game_data;
                        if (game_data.hi_lo_stake > 0)
                        {
                            game_data.hi_lo_balance += game_data.hi_lo_stake;
                        }
                        game_data.hi_lo_stake = DEFAULT;
                        game_data.hi_lo_bet = DEFAULT;
                        game_data.status = 3;
                    });
}

ACTION coinicagames::mhlreachlock(int id)
{
    require_auth(_self);

    auto &mjhilo = _mjhilos.get(id, "User doesn't exist");
    check(mjhilo.game_data.riichi_status == 1, "Hand not reach.");
    _mjhilos.modify(mjhilo, _self, [&](auto &modified_mjhilo)
                    {
                        mhlgamedata &game_data = modified_mjhilo.game_data;
                        game_data.riichi_status = 2;
                    });
}

ACTION coinicagames::mhldel(int size)
{
    require_auth(_self);
    auto itr = _mjhilos.begin();
    while (itr != _mjhilos.end())
    {
        itr = _mjhilos.erase(itr);
    }
}