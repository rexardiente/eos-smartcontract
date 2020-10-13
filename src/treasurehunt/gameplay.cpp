#include "treasurehunt.hpp"
#include <eosio/transaction.hpp>

void treasurehunt::ondeposit(name from,
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
    check(quantity.symbol == treasurehunt_symbol, "Invalid EOS Token");

    gameready(from, quantity);
}

void treasurehunt::onsettledpay(name username, asset quantity, string memo)
{
    require_auth(_self);
    action(
        permission_level{_self, "active"_n},
        _self,
        "settledpay"_n,
        std::make_tuple(username, quantity, memo))
        .send();
}

void treasurehunt::gameready(name username, asset quantity)
{
    require_auth(username);
    action(
        permission_level{_self, "active"_n},
        _self,
        "gamestart"_n,
        std::make_tuple(username, quantity))
        .send();
}

int treasurehunt::rng(const int &range)
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

treasurehunt::game treasurehunt::showremainingtile(game gamedata)
{
    game game_data = gamedata;

    int random_result;
    int remaining_prizes = game_data.unopentile - game_data.enemy_count;
    if (game_data.status == DONE)
    {
        remaining_prizes++;
    }
    while (remaining_prizes > 0)
    {
        random_result = rng(15);
        if (game_data.panel_set.at(random_result).isopen == 0)
        {
            game_data.panel_set.at(random_result).isopen = 1;
            game_data.panel_set.at(random_result).iswin = 1;
            remaining_prizes--;
        }
    }
    // open all panels and show treasures and enemies
    for (int i = 0; i < PANEL_SIZE; i++)
    {
        if (game_data.panel_set.at(i).isopen == 0 && game_data.panel_set.at(i).iswin == 0)
        {
            game_data.panel_set.at(i).isopen = 1;
        }
    }
    game_data.unopentile = 0;
    return game_data;
}

float treasurehunt::prizegen(game game_data)
{
    double rem = (double)game_data.unopentile - (double)game_data.enemy_count;
    double odds = (double)game_data.unopentile / (double)rem;
    float current_prize_amount = (game_data.prize.amount * odds) * 0.98;
    return current_prize_amount;
}

float treasurehunt::nextprizegen(game gamedata)
{
    double rem = (double)gamedata.unopentile - (double)gamedata.enemy_count;
    double odds = ((double)gamedata.unopentile) / (double)rem;
    float current_prize_amount = (gamedata.prize.amount * odds) * 0.98;
    return current_prize_amount;
}

asset treasurehunt::maxprizegen(game gamedata)
{
    asset game_prize = gamedata.prize;
    float rem = gamedata.panel_set.size() - gamedata.enemy_count;
    for (int i = 0; i < rem; i++)
    {
        float odds = (float)(gamedata.panel_set.size() - i) / (float)(rem - i);
        float current_prize_amount = (game_prize.amount * odds) * 0.98;

        game_prize.amount = current_prize_amount;
    }

    return game_prize;
}