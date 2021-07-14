#include "gameplay.cpp"

// function for initializing the a game
ACTION ghostquest::initialize(int id)
{
    require_auth(_self);
    // Create a record in the table if the player doesn't exist in our app yet
    auto itr = _users.find(id);

    users_table userstbl(_self, id);
    // uint64_t gameid = userstbl.available_primary_key();

    check(itr == _users.end(), "Error : Either User has Initialized a Game or has an Existing Game");

    if (itr == _users.end())
    {
        _users.emplace(_self, [&](auto &new_users) {
            new_users.id = id;
            // new_users.game_id = gameid;
            // new_users.game_id = generategameid(); // generate user game_id
        });
    }
}


// ACTION ghostquest::setsummon(int id, asset quantity, int limit) // trigger summoning after transfer transaction
// {
//     require_auth(_self);

//     action( //generate characters
//         permission_level{_self, "active"_n},
//         _self,
//         "genchar"_n,
//         std::make_tuple(username, quantity, limit))
//         .send();
// }

// ACTION ghostquest::addlife(int id, asset quantity, int key) // trigger adding life after transfer transaction
// {
//     require_auth(_self);

//     action( // add life
//         permission_level{_self, "active"_n},
//         _self,
//         "addlife"_n,
//         std::make_tuple(username, quantity, key))
//         .send();
// }


ACTION ghostquest::genchar(int id, double quantity, int limit) // generate characters after transfer transaction
{
    require_auth(_self);

    auto &user = _users.get(id, "User doesn't exist");
    auto itr = _users.find(id);
    int counter = user.game_data.characters.size();
    string hash_string = checksum256_to_string_hash();
    check(itr != _users.end(), "Game Doesn't Exist.");
    // check(user.game_data.status == INITIALIZED, "Has an existing game, can't start a new game.");
    _users.modify(user, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        for (int i = counter; i < (counter + quantity); i++) // summon character/characters and hitpoints
        {
            string key = hash_string.substr(0, 30) + to_string(i);
            character new_ghost;
            new_ghost.owner_id = id;
            new_ghost.prize = 1;
            new_ghost.battle_limit = limit;
            new_ghost.status = SUMMONED;
            new_ghost.character_life = 1;
            // new_ghost.status = STANDBY;
            base_stat tempstat = stat_deck[rng(stat_deck.size())];
            new_ghost.ghost_name = tempstat.ghost_name;
            new_ghost.ghost_id = tempstat.ghost_id;
            new_ghost.hitpoints = 101 + rng(7) + ((tempstat.hp-1)*8);
            new_ghost.rarity = tempstat.rarity;
            new_ghost.attack = 25 +rng(8) + ((tempstat.base_atk-1)*9);
            new_ghost.defense = 25 +rng(8) + ((tempstat.base_def-1)*9);
            new_ghost.speed = 25 +rng(8) + ((tempstat.base_spd-1)*9);
            new_ghost.luck = 25 +rng(8) + ((tempstat.base_lck-1)*9);
            // status formula - 24 + [(1-1)*8) + rng(1*8)
            // hitpoints formula - 100 + [(1-1)*8) + rng(1*8) 
            // new_ghost.initial_hp = 100 + rng(50);
            // gen_stat(new_ghost); // generate status for character
            game_data.characters.insert(game_data.characters.end(), pair<string, character>(key, new_ghost));
        }
    });
}


ACTION ghostquest::addlife(int id, double quantity, string key) // add life/lives after transfer transaction
{
    require_auth(_self);

    auto &user = _users.get(id, "User doesn't exist");
    auto characters = user.game_data.characters;
    map<string, character>::iterator itr = characters.find(key);
    check(itr->second.character_life > 0, "Character cease to exist.");
    itr->second.character_life += quantity;

    _users.modify(user, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.characters = characters;
    });
}


// ACTION ghostquest::battle(int id1, int ghost1_key, int id2, int ghost2_key) // battle action
// {
//     require_auth(_self);
//     auto &user1 = _users.get(id1, "User doesn't exist.");
//     auto &user2 = _users.get(id2, "User doesn't exist.");
//     auto characters1 = user1.game_data.character;
//     map<int, ghost>::iterator itr1 = characters1.find(ghost1_key);
//     auto characters2 = user2.game_data.character;
//     map<int, ghost>::iterator itr2 = characters2.find(ghost2_key);

//     itr1->second.hitpoints = itr1->second.initial_hp;
//     itr2->second.hitpoints = itr2->second.initial_hp;
//     check(itr1->second.enemy_fought.find(itr2->first) == itr1->second.enemy_fought.end(), "Enemy already fought before.");
//     check(itr2->second.enemy_fought.find(itr1->first) == itr2->second.enemy_fought.end(), "Enemy already fought before.");
//     check(itr1->second.character_life == itr2->second.character_life, "Match not allowed.");
//     check(itr1->second.battle_count <= itr1->second.battle_limit, "Battle limit reached.");
//     check(itr2->second.battle_count <= itr2->second.battle_limit, "Battle limit reached.");
//     battle_step(itr1, itr2);                                                                                                  // whole battle step
//     itr1->second.last_match = current_time_point().elapsed._count;                                                            // time of the last match
//     itr2->second.last_match = current_time_point().elapsed._count;                                                            // time of the last match
//     itr1->second.enemy_fought.insert(itr1->second.enemy_fought.end(), pair<uint64_t, name>(itr2->first, itr2->second.owner_id)); // insert enemy fought into history
//     itr2->second.enemy_fought.insert(itr2->second.enemy_fought.end(), pair<uint64_t, name>(itr1->first, itr1->second.owner_id)); // insert enemy fought into history
//     _users.modify(user1, _self, [&](auto &modified_user) {
//         game &game_data = modified_user.game_data;
//         game_data.character = characters1;
//     });
//     _users.modify(user2, _self, [&](auto &modified_user) {
//         game &game_data = modified_user.game_data;
//         game_data.character = characters2;
//     });
// }

// ACTION ghostquest::settledpay(int id, asset prize, string memo)
// {
//     require_auth(_self);
    // auto &user = _users.get(username.value, "User doesn't exist");
//     // check(user.game_data.status == ONGOING, "Game has ended and prize is already transferred.");
// }


///////////////
// auto itr = _users.find(id);
//     // check(itr == _users.end(), "Either User has Initialized a Game or has an Existing Game");
//     auto user = _users.find(id);
//     _users.modify(user, _self, [&](auto &modified_user) {
//         game &game_data = modified_user.game_data;
//         game_data.hi_lo_balance += quantity;
//     });
//////////////

ACTION ghostquest::battleresult(string gameid, pair<string, int> winner, pair<string, int> loser) // battle action
{
    require_auth(_self);
    check(winner.second != loser.second, "Same Character are not allowed to match");
    check(winner.first != loser.first, "Same Character are not allowed to match");
    auto &winner_player = _users.get(winner.second, "User doesn't exist.");
    auto &loser_player = _users.get(loser.second, "User doesn't exist.");

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

    _users.modify(winner_player, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        itr[0]->second.character_life += 1;
        game_data.characters = player1_characters;
    });
    _users.modify(loser_player, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        itr[1]->second.character_life -= 1;
        if (itr[1]->second.character_life == 0) { itr[1]->second.status = ELIMINATED; }
        game_data.characters = player2_characters;
    });
}

ACTION ghostquest::withdraw(int id, string key)
{
    require_auth(_self);
    auto user = _users.find(id);
    check(user->game_data.characters.find(key)->second.character_life > 0, "Ghost doesn't exist.");
    check(user->game_data.characters.find(key)->second.status != 3, "Ghost is currently in battle.");
    check(user->game_data.characters.find(key)->second.battle_count > 0, "Ghost has not been in a battle.");
    // std::string feedback = "GQ Withdraw: " + name{username}.to_string() + " received " + std::to_string(user->game_data.character.find(key)->second.prize.amount); // transfer funds to user
    // action{
    //     permission_level{_self, "active"_n},
    //     eosio_token,
    //     "transfer"_n,
    //     std::make_tuple(_self, username, user->game_data.character.find(key)->second.prize, feedback)}
    //     .send();
    auto characters = user->game_data.characters;
    map<string, character>::iterator itr = characters.find(key);
    eliminated_withdrawn(itr); // modify withdrawn character
    _users.modify(user, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.characters = characters;
    });
}

ACTION ghostquest::eliminate(int id, string key) // generate stats of monsters after transfer transaction
{
    require_auth(_self);

    auto &user = _users.get(id, "User doesn't exist");
    check(user.game_data.characters.at(key).status != 3, "Character in battle.");
    check(user.game_data.characters.find(key)->second.character_life == 0, "Ghost doesn't exist.");
    _users.modify(user, _self, [&](auto &modified_user) {
        game &game_data = modified_user.game_data;
        game_data.characters.erase(key);
    });
}

ACTION ghostquest::end(int id)
{
    require_auth(_self);
    auto &user = _users.get(id, "User doesn't exist");
    // check(user.game_data.status == DONE, "End your existing game first.");
    _users.erase(user);
}

ACTION ghostquest::delall(int size)
{
    require_auth(_self);
    auto itr = _users.begin();
    while (itr != _users.end())
    {
        itr = _users.erase(itr);
    }
}