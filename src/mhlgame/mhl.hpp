#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include "config.hpp"

using namespace std;
using namespace eosio;

class [[eosio::contract("mhlgame")]] mhlgame : public eosio::contract
{
private:
    const symbol mhl_symbol;
    const name eosio_token;

    enum static_values : int64_t
    {
        DEFAULT = 0,
        MHL_INITIALIZED = 0, // for mahjong game
        MHL_ONGOING = 1,     // for mahjong game
        MHL_WIN = 2,         // for mahjong game
        MHL_LOSE = 3,        // for mahjong game
        MHL_ONTRIAL = 5,     // for mahjong game
        //SKIP = 0,        //for Hi-Lo
        MHL_LOW = 1,  //for Hi-Lo
        MHL_DRAW = 2, //for Hi-Lo
        MHL_HIGH = 3, //for Hi-Lo --> for mahjong
        MHL_ISRIICHI = 1,
        MHL_RIICHILOCK = 2
    };

    enum tile_suit : int64_t
    {
        EMPTY = 0,
        CHARACTER = 1,
        DOT = 2,
        BAMBOO = 3,
        EAST = 4,
        SOUTH = 5,
        WEST = 6,
        NORTH = 7,
        RED = 8,
        WHITE = 9,
        GREEN = 10
    };

    struct mhltile
    {
        int suit;
        int value;
    };

    struct winnables
    {
        int index;
        vector<mhltile> tileswin;
    };

    struct mhlscore
    {
        string score_name;
        int value;
    };

    const vector<mhlscore> score_deck = {
        {"No Score", 0},
        {"Big Four Winds", 26},
        {"Big Three Dragons", 13},
        {"Four Kongs", 13},
        {"All Green", 13},
        {"Nine Gates", 13},
        {"Thirteen Orphans", 13},
        {"Little Four Winds", 13},
        {"All Honors", 13},
        {"Four Concealed Pungs", 13},
        {"All Terminals", 13},
        {"Full Flush", 6},
        {"All terminal and honor discards", 5},
        {"Two Pure Double Chow", 3},
        {"Pure Outside Hand", 3},
        {"Half Flush", 2},
        {"Three Kongs", 2},
        {"Little Three Dragons", 2},
        {"Outside Hand", 2},
        {"Pure Straight", 2},
        {"All Terminals and Honors", 2},
        {"Mixed Triple Chow", 2},
        {"Triple Pung", 2},
        {"Three Concealed Pungs", 2},
        {"All Pungs", 2},
        {"Prevail and Seat Wind", 2},
        {"Seven Pairs", 2},
        {"Double Reach", 2},
        {"Last time Draw", 1},
        {"Out with Replace- ment Tile", 1},
        {"Dragon Pung", 1},
        {"Seat Wind", 1},
        {"Prevail Wind", 1},
        {"Pure Double Chow", 1},
        {"All Chow", 1},
        {"All simples", 1},
        {"One-shot Win", 1},
        {"Reach", 1},
        {"Self drawn", 1}};

    const vector<mhltile> table_deck = {
        {0, 0},
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 2},
        {1, 2},
        {1, 2},
        {1, 2},
        {1, 3},
        {1, 3},
        {1, 3},
        {1, 3},
        {1, 4},
        {1, 4},
        {1, 4},
        {1, 4},
        {1, 5},
        {1, 5},
        {1, 5},
        {1, 5},
        {1, 6},
        {1, 6},
        {1, 6},
        {1, 6},
        {1, 7},
        {1, 7},
        {1, 7},
        {1, 7},
        {1, 8},
        {1, 8},
        {1, 8},
        {1, 8},
        {1, 9},
        {1, 9},
        {1, 9},
        {1, 9},
        {2, 1},
        {2, 1},
        {2, 1},
        {2, 1},
        {2, 2},
        {2, 2},
        {2, 2},
        {2, 2},
        {2, 3},
        {2, 3},
        {2, 3},
        {2, 3},
        {2, 4},
        {2, 4},
        {2, 4},
        {2, 4},
        {2, 5},
        {2, 5},
        {2, 5},
        {2, 5},
        {2, 6},
        {2, 6},
        {2, 6},
        {2, 6},
        {2, 7},
        {2, 7},
        {2, 7},
        {2, 7},
        {2, 8},
        {2, 8},
        {2, 8},
        {2, 8},
        {2, 9},
        {2, 9},
        {2, 9},
        {2, 9},
        {3, 1},
        {3, 1},
        {3, 1},
        {3, 1},
        {3, 2},
        {3, 2},
        {3, 2},
        {3, 2},
        {3, 3},
        {3, 3},
        {3, 3},
        {3, 3},
        {3, 4},
        {3, 4},
        {3, 4},
        {3, 4},
        {3, 5},
        {3, 5},
        {3, 5},
        {3, 5},
        {3, 6},
        {3, 6},
        {3, 6},
        {3, 6},
        {3, 7},
        {3, 7},
        {3, 7},
        {3, 7},
        {3, 8},
        {3, 8},
        {3, 8},
        {3, 8},
        {3, 9},
        {3, 9},
        {3, 9},
        {3, 9},
        {4, 10},
        {4, 10},
        {4, 10},
        {4, 10},
        {5, 10},
        {5, 10},
        {5, 10},
        {5, 10},
        {6, 10},
        {6, 10},
        {6, 10},
        {6, 10},
        {7, 10},
        {7, 10},
        {7, 10},
        {7, 10},
        {8, 11},
        {8, 11},
        {8, 11},
        {8, 11},
        {9, 11},
        {9, 11},
        {9, 11},
        {9, 11},
        {10, 11},
        {10, 11},
        {10, 11},
        {10, 11}};

    struct mhlgamedata
    {
        string game_id;
        vector<int> deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
        int status = DEFAULT;
        double hi_lo_balance = DEFAULT;
        int prediction = DEFAULT;
        int hi_lo_result = DEFAULT;
        int hi_lo_outcome = DEFAULT;
        // float hi_lo_prize = DEFAULT;
        float hi_lo_bet = DEFAULT;
        float hi_lo_stake = DEFAULT;
        double low_odds = DEFAULT;
        double draw_odds = DEFAULT;
        double high_odds = DEFAULT;
        int bet_status = DEFAULT;
        int option_status = DEFAULT;
        int riichi_status = DEFAULT;
        vector<int> sumofvalue = {12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 12};
        int prevalent_wind;
        int seat_wind;
        int current_tile;
        int standard_tile;
        // int eye_idx;
        // int highest;
        // int suit_count;
        // int type_count;
        // int pair_count;
        // int pung_count;
        // int chow_count;
        int kong_count;
        int draw_count;
        vector<uint8_t> hand_player = {};
        vector<uint8_t> hand_melds = {};
        vector<uint8_t> hand_wait = {};
        vector<uint8_t> hand_eye = {};
        vector<uint8_t> discarded_tiles = {};
        vector<uint8_t> reveal_kong = {};
        vector<uint8_t> winning_hand = {};
        vector<uint8_t> score_check = {};
        vector<mhlscore> score_type = {};
        vector<winnables> wintiles = {};
        int final_score;
    };

    struct [[eosio::table]] mjhilo
    {
        int id;
        int game_count;
        mhlgamedata game_data;
        auto primary_key() const
        {
            return id;
        };
    };

    struct [[eosio::table]] seed
    {
        uint64_t key = 1;   // default key '1'
        uint32_t value = 1; // default value of 1
        auto primary_key() const
        {
            return key;
        }
    };

    using mjhilos_table = eosio::multi_index<"mjhilos"_n, mjhilo>;
    using seeds_table = eosio::multi_index<"seeds"_n, seed>;

    mjhilos_table _mjhilos;
    seeds_table _seeds;

    int random(const int range);
    string checksum256_to_string_hash();
    double roundoff(double var);
    void gettile(mhlgamedata & gamedata);
    void sorthand(vector<uint8_t> & hand);
    void tile_insert(mhlgamedata & gamedata, vector<mhltile> tiles, uint8_t idx);
    void riichi_check(mhlgamedata & gamedata, vector<uint8_t> hand);
    void sumscore(mhlgamedata & gamedata);
    void get_odds(mhlgamedata & gamedata, int value);
    float hilo_step(mhlgamedata & gamedata, int prev_tile, int current_tile);
    int meld_check(mhltile tile1, mhltile tile2, mhltile tile3);
    int pair_check(mhltile tile1, mhltile tile2);
    // void four_check(mhlgamedata & gamedata, vector<mhltile> tiles, uint8_t idx);
    // void seven_check(mhlgamedata & gamedata, vector<mhltile> tiles, uint8_t idx);
    // void ten_check(mhlgamedata & gamedata, vector<mhltile> tiles, uint8_t idx);
    void thirteen_check(mhlgamedata & gamedata, vector<mhltile> tiles, uint8_t idx);
    int five_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5);
    // void winhand_check(mhlgamedata & gamedata, vector<uint8_t> & hand);
    // void getscore(mhlgamedata & gamedata, vector<uint8_t> & hand);
    // void five_rem(mhlgamedata & gamedata, vector<mhltile> tiles);
    // void eight_rem(mhlgamedata & gamedata, vector<mhltile> tiles);
    // void eleven_rem(mhlgamedata & gamedata, vector<mhltile> tiles);
    // void transferhand(mhlgamedata & gamedata, int size);
    // void pung_chow(mhlgamedata & gamedata, int check);
    // int pung_check(mhltile tile1, mhltile tile2, mhltile tile3);
    // int wind_check(mhlgamedata gamedata, mhltile tile1, int check1);
    // int five_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5);
    // int six_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5, mhltile tile6);
    // int honors_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5, mhltile tile6, mhltile tile7);

public:
    using contract::contract;

    mhlgame(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                     eosio_token(MAIN_CONTRACT),
                                                                     mhl_symbol(MAIN_TOKEN, PRECISION),
                                                                     _mjhilos(receiver, receiver.value),
                                                                     _seeds(receiver, receiver.value) {}

    ACTION mhlinitialze(int id);
    ACTION mhlresetbet(int id);
    ACTION mhlresetacc(int id);
    ACTION mhladdbet(int id, double quantity);
    ACTION mhlplayhilo(int id, int option);
    ACTION mhldscrdtile(int id, int idx);
    ACTION mhlstartbet(int id);
    ACTION mhlwintrnsfr(int id);
    ACTION mhldclrkong(int id, vector<int> idx);
    ACTION mhldclrwnhnd(int id);
    ACTION mhlwithdraw(int id);
    ACTION mhlend(int id);
    ACTION mhlrchilock(int id);
    // ACTION mhlendgame(int id);
    // ACTION mhldel(int size);
};