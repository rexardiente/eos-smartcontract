#include <eosio/crypto.hpp>
#include "gameplay.cpp"

ACTION coinicagames::gqinitialize(int id)
{
    require_auth(_self);
    auto itr = _ghostquests.find(id);
    ghostquests_table ghostqueststbl(_self, id);
    check(itr == _ghostquests.end(), "Error : Either User has Initialized a Game or has an Existing Game");
    if (itr == _ghostquests.end())
    {
        _ghostquests.emplace(_self, [&](auto &new_ghostquests) {
            new_ghostquests.id = id;
        });
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
    _ghostquests.modify(ghostquest, _self, [&](auto &modified_ghostquest) {
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
            new_ghost.hitpoints = 101 + random(7) + ((tempstat.hp-1)*8);
            new_ghost.rarity = tempstat.rarity;
            new_ghost.attack = 25 + random(8) + ((tempstat.base_atk-1)*9);
            new_ghost.defense = 25 + random(8) + ((tempstat.base_def-1)*9);
            new_ghost.speed = 25 + random(8) + ((tempstat.base_spd-1)*9);
            new_ghost.luck = 25 + random(8) + ((tempstat.base_lck-1)*9);
            new_ghost.created_at = current_time;
            game_data.characters.insert(game_data.characters.end(), pair<string, character>(key, new_ghost));
            current_time +=1;
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

    _ghostquests.modify(ghostquest, _self, [&](auto &modified_ghostquest) {
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
    for_each(itr.begin(), itr.end(), [&](map<string, character>::iterator &n) {
        // n->second.match_history.insert(n->second.match_history.end(), pair<string, battle_history>(gameid, current_battle));
        // n->second.last_match = time_executed;
        n->second.battle_count += 1;
    });

    _ghostquests.modify(winner_player, _self, [&](auto &modified_ghostquest) {
        gqgamedata &game_data = modified_ghostquest.game_data;
        itr[0]->second.character_life += 1;
        game_data.characters = player1_characters;
    });
    _ghostquests.modify(loser_player, _self, [&](auto &modified_ghostquest) {
        gqgamedata &game_data = modified_ghostquest.game_data;
        itr[1]->second.character_life -= 1;
        if (itr[1]->second.character_life == 0) { itr[1]->second.status = GQ_ELIMINATED; }
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
    _ghostquests.modify(user, _self, [&](auto &modified_ghostquest) {
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
    _ghostquests.modify(ghostquest, _self, [&](auto &modified_ghostquest) {
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
    check(enemy_count>0&&enemy_count<16, "Please input valid enemy count.");
    check(itr->game_data.enemy_count < TH_PANEL_SIZE, "Can't have enemy greater than or equal to 16");
    check(itr->game_data.panel_set.empty(), "Error: Either User has an Existing Game");

    if (itr == _thunts.end())
    {
        thunts_table thuntstbl(_self, id);
        _thunts.emplace(_self, [&](auto &new_thunts) {
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

    _thunts.modify(itr, _self, [&](auto &modified_thunt) {
        thgamedata &game_data = modified_thunt.game_data;
        // float available = TH_PANEL_SIZE - game_data.enemy_count - game_data.win_count;
        // float chance = available / (TH_PANEL_SIZE - game_data.win_count) * 100;
        if(random(TH_PANEL_SIZE) % 2)
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

    _thunts.modify(itr, _self, [&](auto &modified_thunt) {
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
    _thunts.modify(itr, _self, [&](auto &modified_thunt) {
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