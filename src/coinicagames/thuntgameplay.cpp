#include "coinicagames.hpp"
// void coinicagames::ondeposit(name from,
//                              name to,
//                              asset quantity,
//                              string memo)
// {
//     if (from == _self)
//     {
//         if (memo.find(HAS_ON_SETTLE_PAY) != std::string::npos)
//         {
//             onsettledpay(to, quantity, memo);
//         }

//         // we're sending money, do nothing additional
//         return;
//     }
//     check(to == _self, "Not to our contract");
//     check(quantity.symbol.is_valid(), "Invalid quantity");
//     check(quantity > 0, "Only positive quantity allowed");
//     check(quantity.symbol == coinicagames_symbol, "Invalid EOS Token");

//     gameready(from, quantity);
// }

// void coinicagames::onsettledpay(name username, asset quantity, string memo)
// {
//     require_auth(_self);
//     action(
//         permission_level{_self, "active"_n},
//         _self,
//         "settledpay"_n,
//         std::make_tuple(username, quantity, memo))
//         .send();
// }

// void coinicagames::gameready(name username, asset quantity)
// {
//     require_auth(username);
//     action(
//         permission_level{_self, "active"_n},
//         _self,
//         "gamestart"_n,
//         std::make_tuple(username, quantity))
//         .send();
// }

// int coinicagames::rng(const int &range)
// {
//     // Find the existing seed
//     auto seed_iterator = _seeds.begin();

//     // Initialize the seed with default value if it is not found
//     if (seed_iterator == _seeds.end())
//     {
//         seed_iterator = _seeds.emplace(_self, [&](auto &seed) {});
//     }

//     // Generate new seed value using the existing seed value
//     int prime = 65537;
//     auto new_seed_value = (seed_iterator->value + current_time_point().elapsed.count()) % prime;

//     // Store the updated seed value in the table
//     _seeds.modify(seed_iterator, _self, [&](auto &s) {
//         s.value = new_seed_value;
//     });

//     // Get the random result in desired range
//     int random_result = new_seed_value % range;
//     return random_result;
// }

void coinicagames::showremainingtile(thgame &thgame_data)
{
    check(thgame_data.status == TH_DONE, "Game hasn't ended yet.");
    // thgame &thgame_data = user_info.game_data;

    int available_tile = thgame_data.unopentile - thgame_data.enemy_count;

    // need optimization because if RNG result is always OPENED status,
    // then it will take time to finish
    while (available_tile > 0)
    {
        int random_result = rng(15);
        if (thgame_data.panel_set.at(random_result).isopen == 0)
        {
            thgame_data.panel_set.at(random_result).isopen = 1;
            thgame_data.panel_set.at(random_result).iswin = 1;
            available_tile--;
        }
    }

    // open all panels and show treasures and enemies
    for (int i = 0; i < TH_PANEL_SIZE; i++)
    {
        if (thgame_data.panel_set.at(i).isopen == 0 && thgame_data.panel_set.at(i).iswin == 0)
        {
            thgame_data.panel_set.at(i).isopen = 1;
        }
    }
}


double coinicagames::generateprize(thgame thgamedata)
{
    double game_prize = thgamedata.prize;
    double odds = calculateodds(thgamedata);
    game_prize = (game_prize * odds) * 0.98;
    return game_prize;
}

double coinicagames::calculateodds(thgame thgamedata)
{
    double rem_panel = (double)thgamedata.unopentile - (double)thgamedata.enemy_count;
    return ((double)thgamedata.unopentile) / (double)rem_panel;
}

double coinicagames::maxprize(thgame thgamedata)
{
    double game_prize = thgamedata.prize;
    float rem_panel = thgamedata.panel_set.size() - thgamedata.enemy_count;
    for (int i = 0; i < rem_panel; i++)
    {
        float odds = (float)(gamedata.panel_set.size() - i) / (float)(rem_panel - i);
        float current_prize = (game_prize * odds) * 0.98;

        game_prize = current_prize;
    }

    return game_prize;
}

void coinicagames::gameupdate(thgame &thgame_data)
{

    if (thgame_data.status == TH_INITIALIZED)
    {
        thgame_data.maxprize = maxprize(game_data);
        thgame_data.status = TH_ONGOING;
    }

    if (thgame_data.status == TH_ONGOING)
    {
        if(thgame_data.win_count!=16-thgame_data.enemy_count)
        {
        thgame_data.nextprize = generateprize(thgame_data);
        thgame_data.odds = calculateodds(thgame_data);
        }
        else
        {
        thgame_data.nextprize = TH_DEFAULT;
        thgame_data.odds = TH_DEFAULT;
        }
        
    }
    else
    {
        showremainingtile(game_data);
        thgame_data.prize = TH_DEFAULT;
        thgame_data.nextprize = TH_DEFAULT;
        thgame_data.odds = TH_DEFAULT;
        thgame_data.unopentile = TH_DEFAULT; // reset unopentile to empty
    }
}