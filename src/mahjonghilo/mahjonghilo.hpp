#include <map>
#include <string>
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>
#include "config.hpp"

using namespace std;
using namespace eosio;

class [[eosio::contract("mahjonghilo")]] mahjonghilo : public eosio::contract
{
private:
    const symbol mahjonghilo_symbol;
    const name eosio_token;

    enum static_values : int8_t
    {
        MH_DEFAULT = 0,
        INITIALIZED = 0, // for mahjong game
        ONGOING = 1,     // for mahjong game
        WIN = 2,         // for mahjong game
        LOSE = 3,        // for mahjong game
        ONTRIAL = 5,     // for mahjong game
        //SKIP = 0,        //for Hi-Lo
        LOW = 1,  //for Hi-Lo
        DRAW = 2, //for Hi-Lo
        HIGH = 3  //for Hi-Lo

    };

    enum tile_suit : uint8_t
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

    struct tile
    {
        uint8_t suit;
        int value;
    };

    struct score
    {
        string score_name;
        int value;
    };

    const map<uint8_t, score> score_deck = {
        {0, {"No Score", 0}},
        {1, {"Big Four Winds", 88}},            ///
        {2, {"Big Three Dragons", 88}},         ///
        {3, {"Four Kongs", 88}},                //
        {4, {"Seven Shift Pairs", 88}},         //
        {5, {"All Green", 88}},                 ///
        {6, {"Nine Gates", 88}},                //
        {7, {"Thirteen Orphans", 88}},          ///
        {8, {"Little Four Winds", 64}},         ///
        {9, {"Little Three Dragons", 64}},      ///
        {10, {"All Honors", 64}},               ///
        {11, {"Pure Terminal Chows", 64}},      //
        {12, {"Four Concealed Pungs", 64}},     //
        {13, {"All Terminals", 64}},            //
        {14, {"Quadruple Chows", 48}},          //
        {15, {"Four Pure Shifted Pungs", 48}},  //
        {16, {"Four Pure Shifted Chows", 32}},  //
        {17, {"All Terminals and Honors", 32}}, //
        {18, {"Three Kongs", 32}},              //
        // {19, {"Pure Triple Chow", 24}}, -------------
        {20, {"Pure Shifted Pungs", 24}},               //
        {21, {"All Even Pungs", 24}},                   //
        {22, {"Seven Pairs", 24}},                      //
        {23, {"Full Flush", 24}},                       //
        {24, {"Upper Tiles", 24}},                      //
        {25, {"Middle Tiles", 24}},                     //
        {26, {"Lower Tiles", 24}},                      //
        {27, {"Greater Honors and Knitted Tiles", 24}}, //
        {28, {"Pure Shifted Chows", 16}},               //
        {29, {"Pure Straight", 16}},                    //
        {30, {"Three-suited Terminal Chows", 16}},      //
        {31, {"Triple Pungs", 16}},                     //
        {32, {"Three Concealed Pungs", 16}},            //
        // {33, {"All Fives", 16}},
        {34, {"Big Three Winds", 12}},
        // {35, {"Knitted Straight", 12}},
        {36, {"Upper Four", 12}},
        {37, {"Lower Four", 12}},
        {38, {"Lesser Honors and Knitted Tiles", 12}},
        {39, {"Mixed Triple Chow", 8}},
        {40, {"Mixed Straight", 8}},
        {41, {"Mixed Shifted Pungs", 8}},
        {42, {"Two Concealed Kongs", 8}},
        // {43, {"Out With Replacement Tile", 8}}, -------------
        // {44, {"Robbing the Kong", 8}}, -------------
        // {45, {"Last Tile Draw", 8}},
        // {46, {"Last Tile Claim", 8}}, -------------
        // {47, {"Chicken Hand", 8}}, -------------
        // {48, {"Reversible Tiles", 8}},
        {49, {"Two Dragon Pungs", 6}},
        {50, {"Mixed Shifted Chows", 6}},
        // {51, {"All Pungs", 6}},
        {52, {"All Types", 6}},
        {53, {"Half Flush", 6}},
        // {54, {"Melded Hand", 6}}, -------------
        // {55, {"Two Melded Kongs", 4}}, -------------
        {56, {"Outside Hand", 4}},
        // {57, {"Last Tile", 4}},
        // {58, {"Fully Concealed Hands", 4}},
        {59, {"Dragon Pung", 2}},
        {60, {"Prevalent Wind", 2}},
        {61, {"Seat Wind", 2}},
        {62, {"All Chows", 2}},
        // {63, {"Double Pungs", 2}},
        {64, {"Two Concealed Pungs", 2}},
        {65, {"Concealed Kong", 2}},
        {66, {"Mixed Double Pungs", 2}},
        {67, {"All Simples", 2}},
        {68, {"Tile Hog", 2}},
        {69, {"Pure Double Chows", 1}},
        {70, {"Mixed Double Chows", 1}},
        {71, {"Short Straight", 1}},
        {72, {"Two Terminal Chows", 1}},
        {73, {"Pung of Terminals or Honour", 1}},
        {74, {"No Available Score, Have a Bonus", 30}},
        {75, {"One Voided Suit", 1}},
        {76, {"No Honors", 1}}};
    // {77, {"Edge Wait", 1}}, -------------
    // {78, {"Closed Wait", 1}}, -------------
    // {79, {"Self Drawn", 1}}, -------------
    // {80, {"Single Wait", 1}}}; -------------

    const map<uint8_t, tile> table_deck = {
        {0, {EMPTY, 0}},
        {1, {CHARACTER, 1}},
        {2, {CHARACTER, 1}},
        {3, {CHARACTER, 1}},
        {4, {CHARACTER, 1}},
        {4, {CHARACTER, 2}},
        {5, {CHARACTER, 2}},
        {6, {CHARACTER, 2}},
        {8, {CHARACTER, 2}},
        {7, {CHARACTER, 3}},
        {8, {CHARACTER, 3}},
        {9, {CHARACTER, 3}},
        {12, {CHARACTER, 3}},
        {10, {CHARACTER, 4}},
        {11, {CHARACTER, 4}},
        {12, {CHARACTER, 4}},
        {16, {CHARACTER, 4}},
        {13, {CHARACTER, 5}},
        {14, {CHARACTER, 5}},
        {19, {CHARACTER, 5}},
        {20, {CHARACTER, 5}},
        {21, {CHARACTER, 6}},
        {22, {CHARACTER, 6}},
        {23, {CHARACTER, 6}},
        {24, {CHARACTER, 6}},
        {25, {CHARACTER, 7}},
        {26, {CHARACTER, 7}},
        {27, {CHARACTER, 7}},
        {28, {CHARACTER, 7}},
        {29, {CHARACTER, 8}},
        {30, {CHARACTER, 8}},
        {31, {CHARACTER, 8}},
        {32, {CHARACTER, 8}},
        {33, {CHARACTER, 9}},
        {34, {CHARACTER, 9}},
        {35, {CHARACTER, 9}},
        {36, {CHARACTER, 9}},
        {37, {DOT, 1}},
        {38, {DOT, 1}},
        {39, {DOT, 1}},
        {40, {DOT, 1}},
        {41, {DOT, 2}},
        {42, {DOT, 2}},
        {43, {DOT, 2}},
        {44, {DOT, 2}},
        {45, {DOT, 3}},
        {46, {DOT, 3}},
        {47, {DOT, 3}},
        {48, {DOT, 3}},
        {49, {DOT, 4}},
        {50, {DOT, 4}},
        {51, {DOT, 4}},
        {52, {DOT, 4}},
        {53, {DOT, 5}},
        {54, {DOT, 5}},
        {55, {DOT, 5}},
        {56, {DOT, 5}},
        {57, {DOT, 6}},
        {58, {DOT, 6}},
        {59, {DOT, 6}},
        {60, {DOT, 6}},
        {61, {DOT, 7}},
        {62, {DOT, 7}},
        {63, {DOT, 7}},
        {64, {DOT, 7}},
        {65, {DOT, 8}},
        {66, {DOT, 8}},
        {67, {DOT, 8}},
        {68, {DOT, 8}},
        {69, {DOT, 9}},
        {70, {DOT, 9}},
        {71, {DOT, 9}},
        {72, {DOT, 9}},
        {73, {BAMBOO, 1}},
        {74, {BAMBOO, 1}},
        {75, {BAMBOO, 1}},
        {76, {BAMBOO, 1}},
        {77, {BAMBOO, 2}},
        {78, {BAMBOO, 2}},
        {79, {BAMBOO, 2}},
        {80, {BAMBOO, 2}},
        {81, {BAMBOO, 3}},
        {82, {BAMBOO, 3}},
        {83, {BAMBOO, 3}},
        {84, {BAMBOO, 3}},
        {85, {BAMBOO, 4}},
        {86, {BAMBOO, 4}},
        {87, {BAMBOO, 4}},
        {88, {BAMBOO, 4}},
        {89, {BAMBOO, 5}},
        {90, {BAMBOO, 5}},
        {91, {BAMBOO, 5}},
        {92, {BAMBOO, 5}},
        {93, {BAMBOO, 6}},
        {94, {BAMBOO, 6}},
        {95, {BAMBOO, 6}},
        {96, {BAMBOO, 6}},
        {97, {BAMBOO, 7}},
        {98, {BAMBOO, 7}},
        {99, {BAMBOO, 7}},
        {100, {BAMBOO, 7}},
        {101, {BAMBOO, 8}},
        {102, {BAMBOO, 8}},
        {103, {BAMBOO, 8}},
        {104, {BAMBOO, 8}},
        {105, {BAMBOO, 9}},
        {106, {BAMBOO, 9}},
        {107, {BAMBOO, 9}},
        {108, {BAMBOO, 9}},
        {109, {EAST, 10}},
        {110, {EAST, 10}},
        {111, {EAST, 10}},
        {112, {EAST, 10}},
        {113, {SOUTH, 10}},
        {114, {SOUTH, 10}},
        {115, {SOUTH, 10}},
        {116, {SOUTH, 10}},
        {117, {WEST, 10}},
        {118, {WEST, 10}},
        {119, {WEST, 10}},
        {120, {WEST, 10}},
        {121, {NORTH, 10}},
        {122, {NORTH, 10}},
        {123, {NORTH, 10}},
        {124, {NORTH, 10}},
        {125, {RED, 11}},
        {126, {RED, 11}},
        {127, {RED, 11}},
        {128, {RED, 11}},
        {129, {WHITE, 11}},
        {130, {WHITE, 11}},
        {131, {WHITE, 11}},
        {132, {WHITE, 11}},
        {133, {GREEN, 11}},
        {134, {GREEN, 11}},
        {135, {GREEN, 11}},
        {136, {GREEN, 11}}};

    struct game
    {
        string game_id;
        vector<uint8_t> deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
        uint8_t status;
        asset hi_lo_balance = DEFAULT_ASSET;
        int hi_lo_result = MH_DEFAULT;
        // float hi_lo_prize = MH_DEFAULT;
        float hi_lo_stake = MH_DEFAULT;
        double low_odds = MH_DEFAULT;
        double draw_odds = MH_DEFAULT;
        double high_odds = MH_DEFAULT;
        int bet_status = MH_DEFAULT;
        int option_status = MH_DEFAULT;
        vector<int> sumofvalue = {12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 12};
        uint8_t prevalent_wind;
        uint8_t seat_wind;
        uint8_t current_tile;
        uint8_t standard_tile;
        int eye_idx;
        int winnable = MH_DEFAULT;
        // int highest;
        // int suit_count;
        // int type_count;
        int pair_count;
        int pung_count;
        int chow_count;
        int kong_count;
        int draw_count;
        vector<uint8_t> hand_player = {};
        vector<uint8_t> drawn_tiles = {};
        vector<uint8_t> reveal_kong = {};
        vector<uint8_t> winning_hand = {};
        vector<uint8_t> score_check = {};
        vector<score> score_type = {};
        int final_score;
    };

    struct [[eosio::table]] user
    {
        name username;
        int game_count;
        game game_data;

        auto primary_key() const
        {
            return username.value;
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

    using users_table = eosio::multi_index<"users"_n, user>;
    using seeds_table = eosio::multi_index<"seeds"_n, seed>;

    users_table _users;
    seeds_table _seeds;

    int rng(const int &range);
    double roundoff(double var);
    void depositbet(name to, asset quantity);
    void onsettledpay(name to, asset quantity, string memo);
    void gettile(game & gamedata);
    void sorthand(vector<uint8_t> & hand);
    void sorteye(vector<uint8_t> & hand, int idx);
    void getscore(game & gamedata, vector<uint8_t> & hand);
    void sumscore(game & gamedata);
    void two_rem(game & gamedata, vector<tile> tiles);
    void five_rem(game & gamedata, vector<tile> tiles);
    void eight_rem(game & gamedata, vector<tile> tiles);
    void eleven_rem(game & gamedata, vector<tile> tiles);
    void fourteen_rem(game & gamedata, vector<tile> tiles);
    void winhand_check(game & gamedata, vector<uint8_t> & hand);
    void transferhand(game & gamedata, int size);
    void pung_chow(game & gamedata, int check);
    void get_odds(game & gamedata, int value);
    float hilo_step(game gamedata, int prev_tile, int current_tile, int option);
    int pair_pung_chow(tile tile1, tile tile2, tile tile3);
    int pung_check(tile tile1, tile tile2, tile tile3);
    int pair_check(tile tile1, tile tile2);
    int wind_check(game gamedata, tile tile1, int check1);
    int five_tile_check(tile tile1, tile tile2, tile tile3, tile tile4, tile tile5);
    int six_tile_check(tile tile1, tile tile2, tile tile3, tile tile4, tile tile5, tile tile6);
    int honors_check(tile tile1, tile tile2, tile tile3, tile tile4, tile tile5, tile tile6, tile tile7);
    string checksum256_to_string(std::array<uint8_t, 32UL> arr, size_t size);

public:
    using contract::contract;

    mahjonghilo(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                         eosio_token(MAIN_CONTRACT),
                                                                         mahjonghilo_symbol(MAIN_TOKEN, PRECISION),
                                                                         _users(receiver, receiver.value),
                                                                         _seeds(receiver, receiver.value) {}

    [[eosio::on_notify("eosio.token::transfer")]] void ondeposit(name from,
                                                                 name to,
                                                                 asset quantity,
                                                                 string memo);
    ACTION initialize(name username);
    ACTION acceptbet(name username, asset quantity);
    // ACTION starttrial(name username, int numgames, vector<int> idx);
    ACTION playhilo(name username, int option);
    ACTION discardtile(name username, int idx);
    ACTION startbet(name username);
    ACTION wintransfer(name username);
    ACTION dclrkong(name username, vector<int> idx);
    ACTION dclrwinhand(name username);
    ACTION withdraw(name username);
    ACTION settledpay(name to, asset prize, string memo);
    ACTION endgame(name username);
    ACTION end(name username);
    ACTION del(int size);
};