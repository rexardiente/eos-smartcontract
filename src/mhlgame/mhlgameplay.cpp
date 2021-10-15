#include "mhl.hpp"
#include <eosio/transaction.hpp>

int mhlgame::random(const int range)
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
    _seeds.modify(seed_iterator, _self, [&](auto &s)
                  { s.value = new_seed_value; });

    // Get the random result in desired range
    int random_result = new_seed_value % range;
    return random_result;
}

string mhlgame::checksum256_to_string_hash()
{
    auto size = transaction_size();
    char buf[size];
    check(size == read_transaction(buf, size), "read_transaction failed");
    checksum256 sha = sha256(buf, size);
    auto hbytes = sha.extract_as_byte_array();
    std::string hash_id;
    const char *to_hex = "0123456789abcdef";
    for (uint32_t i = 0; i < hbytes.size(); ++i)
    {
        (hash_id += to_hex[(hbytes[i] >> 4)]) += to_hex[(hbytes[i] & 0x0f)];
    }
    return hash_id;
}

double mhlgame::roundoff(double var)
{
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67
    float value = (int)(var * 10 + .5);
    return (float)value / 10;
}

void mhlgame::gettile(mhlgamedata &gamedata)
{
    uint8_t deck_tile_idx = random(gamedata.deck_player.size()); // Pick a random tile from the deck
    // uint8_t deck_tile_idx = 1; //  Pick a random tile from the deck
    // uint8_t deck_tile_idx = 64;
    gamedata.hand_player.insert(gamedata.hand_player.end(), gamedata.deck_player[deck_tile_idx]); // Assign the mhltile to the first empty slot in the hand
    gamedata.current_tile = gamedata.deck_player[deck_tile_idx];
    gamedata.deck_player.erase(gamedata.deck_player.begin() + deck_tile_idx); // Remove the tile from the deck
    // sorthand(gamedata.hand_player);
    mhltile num = table_deck.at(gamedata.current_tile);
    gamedata.sumofvalue[num.value - 1] -= 1;
    gamedata.draw_count += 1;
    // if(sampctr%36<4)
    // {
    //     sampctr++;
    // }
    // else
    // {
    //     sampctr+=32;
    // }
    // gamedata.drawn_tiles.insert(gamedata.drawn_tiles.end(), gamedata.current_tile);
    // return gamedata.current_tile;
}

void mhlgame::get_odds(mhlgamedata &gamedata, int value)
{
    // gamedata.sumofvalue[value - 1] -= 1;
    double sum, sum1, sum2, num1, num2, num3;
    for (int i = 0; i < gamedata.sumofvalue.size(); i++)
    {
        sum += gamedata.sumofvalue[i];
    }
    // var = (y < 10) ? 30 : 40;
    num2 = sum > 0 ? (sum / gamedata.sumofvalue[value - 1]) * 0.9 : 0;
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

float mhlgame::hilo_step(mhlgamedata &gamedata, int prev_tile, int current_tile)
{
    // int option = gamedata.prediction;
    if (prev_tile > current_tile)
    {
        gamedata.hi_lo_outcome = 1;
        if (gamedata.prediction == 1)
        {
            return gamedata.low_odds;
        }
        else
        {
            return 0;
        }
    }
    else if (prev_tile == current_tile)
    {
        gamedata.hi_lo_outcome = 2;
        if (gamedata.prediction == 2)
        {
            return gamedata.draw_odds;
        }
        else
        {
            return 0;
        }
    }
    else if (prev_tile < current_tile)
    {
        gamedata.hi_lo_outcome = 3;
        if (gamedata.prediction == 3)
        {
            return gamedata.high_odds;
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

void mhlgame::sorthand(vector<uint8_t> &hand)
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

// void mhlgame::sorteye(vector<uint8_t> &hand, int idx)
// {
//     int temp = hand[idx];
//     hand.erase(hand.begin() + idx);
//     hand.erase(hand.begin() + idx);
//     hand.insert(hand.end(), temp);
//     hand.insert(hand.end(), temp += 1);
// }

void mhlgame::sumscore(mhlgamedata &gamedata)
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

void mhlgame::riichi_check(mhlgamedata &gamedata, vector<uint8_t> hand)
{
    vector<mhltile> remtiles = {};
    sorthand(hand);
    for (int i = 0; i < gamedata.hand_player.size(); i++)
    {
        remtiles.insert(remtiles.end(), table_deck.at(hand[i]));
    }
    if (remtiles.size() == 2)
    {
        for (int i = 0; i < remtiles.size(); i++)
        {
            vector<mhltile> temptiles = remtiles;
            temptiles.erase(temptiles.begin() + i);
            gamedata.riichi_status = 1;
            gamedata.wintiles.insert(gamedata.wintiles.end(), pair<uint8_t, vector<mhltile> >(i, temptiles));
        }
    }
    // else if (remtiles.size() == 5)
    // {

    //     for (int i = 0; i < remtiles.size(); i++)
    //     {
    //         vector<mhltile> temptiles = remtiles;
    //         temptiles.erase(temptiles.begin() + i);
    //         four_check(gamedata, temptiles, i);
    //     }
    // }
    // else if (remtiles.size() == 8)
    // {

    //     for (int i = 0; i < remtiles.size(); i++)
    //     {
    //         vector<mhltile> temptiles = remtiles;
    //         temptiles.erase(temptiles.begin() + i);
    //         seven_check(gamedata, temptiles, i);
    //     }
    // }
    // else if (remtiles.size() == 11)
    // {

    //     for (int i = 0; i < remtiles.size(); i++)
    //     {
    //         vector<mhltile> temptiles = remtiles;
    //         temptiles.erase(temptiles.begin() + i);
    //         ten_check(gamedata, temptiles, i);
    //     }
    // }
    else
    {

        for (int i = 0; i < remtiles.size(); i++)
        {
            vector<mhltile> temptiles = remtiles;
            temptiles.erase(temptiles.begin() + i);
            thirteen_check(gamedata, temptiles, i);
        }
    }
}