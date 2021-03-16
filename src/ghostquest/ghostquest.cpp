#include "gameplay.cpp"

// execute this action once use has transfered EOS on smartcontract acc
ACTION ghostquest::init(name username, asset quantity, int limit)
{
    require_auth(_self);
    
    if (_users.find(username.value) == _users.end()) { _users.emplace(_self, [&](auto &new_user) { new_user.username = username; }); }

    auto itr = _users.find(username.value);
    check(itr != _users.end(), "User or Game Doesn't Exist.");
    // convert txID arr to string
    string hash_string = checksum256_to_string_hash();

    _users.modify(itr, _self, [&](auto &modified_user) {
        game &game_data = modified_user.data;
        int size = game_data.characters.size();
        auto current_time = current_time_point().time_since_epoch()._count;

        for (int i = size; i < (size + (quantity.amount / 10000)); i++) // summon character/characters and hitpoints
        {
            character ghost;
            string key = hash_string.substr(DEFAULT, 30) + to_string(i);
            ghost.GAME_LIMIT = limit;
            ghost.LIFE = 1;
            ghost.STATUS = BATTLE_STANDBY;
            ghost.HP = 100 + rng(50);
            ghost.CREATED_AT = current_time;
            gen_stat(ghost); // generate status for character
            game_data.characters.insert(game_data.characters.end(), pair<string, character>(key, ghost));
        }
    });
}

ACTION ghostquest::settledpay(const name username, const asset prize, const string memo)
{
    require_auth(_self);
    auto &user = _users.get(username.value, "User doesn't exist");
    // construct ID key using string memo
    int str_first = memo.find(GET_ID_FROM_STR_FIRST) + 4;
    int str_second = memo.find(GET_ID_FROM_STR_SECOND);
    string id = memo.substr(str_first, str_second-str_first);

    _users.modify(user, _self, [&](auto &modified_user) {
        game &game_data = modified_user.data;
        auto &characters = game_data.characters;
        auto &character = characters.at(id);

        check(character.LIFE != DEFAULT, "HP is equal to 0.");
        check(character.STATUS != WITHDRAWN, "Prize is already transferred.");
        character.STATUS = WITHDRAWN;
        character.LIFE = DEFAULT;
    });
}

ACTION ghostquest::withdraw(name username, string key)
{
    require_auth(username);
    auto &user = _users.get(username.value, "User doesn't exist");
    auto characters = user.data.characters;
    map<string, character>::iterator itr = characters.find(key);

    check(itr != characters.end(), "Doesn't exist.");
    check(itr->second.LIFE > DEFAULT, "No enough HP.");
    check(itr->second.STATUS != ELIMINATED, "Already eliminated.");
    check(itr->second.STATUS != WITHDRAWN, "Already withdrawn.");
    check(itr->second.GAME_COUNT > DEFAULT, "Has not been in a battle.");
    const string id = itr->first;
    asset prize = calculate_prize(itr);
    std::string feedback = "GQ WITHDRAW: " + name{username}.to_string() + GET_ID_FROM_STR_FIRST + id + GET_ID_FROM_STR_SECOND + " RECEIVED " + std::to_string(prize.amount);
    action{
        permission_level{_self, "active"_n},
        eosio_token,
        "transfer"_n,
        std::make_tuple(_self, username, prize, feedback)}
        .send();
}

ACTION ghostquest::end(name username)
{   
    // require_auth(_self);
    check(has_auth(_self) || has_auth(username), "Unauthorized user");
    auto &user = _users.get(username.value, "User doesn't exist");
    _users.erase(user);
}

ACTION ghostquest::eliminate(name username, string key) // generate stats of monsters after transfer transaction
{
    require_auth(_self);
    auto &user = _users.get(username.value, "User doesn't exist");
    auto characters = user.data.characters;
    map<string, character>::iterator itr = characters.find(key);

    check(itr != characters.end(), "This character doesn't belong to this user.");
    check(itr->second.LIFE == DEFAULT, "HP is not equal to 0.");
    // check(user.data.characters.at(key).STATUS != 1, "Character life is not yet empty");
    _users.modify(user, _self, [&](auto &modified_user) {
        game &game_data = modified_user.data;
        game_data.characters.erase(key);
    });
}

ACTION ghostquest::newlife(name username, asset quantity, string key) // add life/lives after transfer transaction
{
    require_auth(_self);
    auto &user = _users.get(username.value, "User doesn't exist");
    _users.modify(user, _self, [&](auto &modified_user) {
        game &game_data = modified_user.data;
        map<string, character>::iterator itr = game_data.characters.find(key);
        check(itr != game_data.characters.end(), "Character cease to exist.");
        check(itr->second.LIFE == BATTLE_STANDBY, "Already eliminated.");
        itr->second.LIFE += quantity.amount / 10000;
    });
}

ACTION ghostquest::battleresult(string gameid, pair<string, name> winner, pair<string, name> loser) // battle action
{
    require_auth(_self);
    check(winner.second != loser.second, "Same Character are not allowed to match");
    check(winner.first != loser.first, "Same Character are not allowed to match");
    auto &winner_player = _users.get(winner.second.value, "User doesn't exist.");
    auto &loser_player = _users.get(loser.second.value, "User doesn't exist.");

    map<string, character> player1_characters = winner_player.data.characters;
    map<string, character> player2_characters = loser_player.data.characters;
    vector<map<string, character>::iterator> itr{player1_characters.find(winner.first), player2_characters.find(loser.first)};

    check(itr[0]->second.LIFE != 0, winner.first + " No remaining HP.");
    check(itr[1]->second.LIFE != 0, loser.first + " No remaining HP.");
    check(itr[0]->second.GAME_COUNT <= itr[0]->second.GAME_LIMIT, winner.first + " reached battle limit.");
    check(itr[1]->second.GAME_COUNT <= itr[1]->second.GAME_LIMIT, loser.first + " reached battle limit.");

    // update each players battle details..
    for_each(itr.begin(), itr.end(), [&](map<string, character>::iterator &n) {
        // n->second.match_history.insert(n->second.match_history.end(), pair<string, battle_history>(gameid, current_battle));
        // n->second.last_match = time_executed;
        n->second.GAME_COUNT += 1;
    });

    _users.modify(winner_player, _self, [&](auto &modified_user) {
        game &game_data = modified_user.data;
        itr[0]->second.LIFE += 1;
        game_data.characters = player1_characters;
    });
    _users.modify(loser_player, _self, [&](auto &modified_user) {
        game &game_data = modified_user.data;
        itr[1]->second.LIFE -= 1;
        if (itr[1]->second.LIFE == 0) { itr[1]->second.STATUS = ELIMINATED; }
        game_data.characters = player2_characters;
    });
}