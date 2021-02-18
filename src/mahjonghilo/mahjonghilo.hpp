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
        ONTRIAL = 7, // for mahjong game
        SKIP = 0,        //for Hi-Lo
        LOW = 1,         //for Hi-Lo
        DRAW = 2,        //for Hi-Lo
        HIGH = 3
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
        GREEN = 9,
        WHITE = 10
    };

    struct tile
    {
        uint8_t suit;
        int value;
    };

    const map<uint8_t, tile> table_deck = {
        {0, {EMPTY, 0}},
        {1, {CHARACTER, 1}},
        {2, {CHARACTER, 1}},
        {3, {CHARACTER, 1}},
        {4, {CHARACTER, 1}},
        {5, {CHARACTER, 2}},
        {6, {CHARACTER, 2}},
        {7, {CHARACTER, 2}},
        {8, {CHARACTER, 2}},
        {9, {CHARACTER, 3}},
        {10, {CHARACTER, 3}},
        {11, {CHARACTER, 3}},
        {12, {CHARACTER, 3}},
        {13, {CHARACTER, 4}},
        {14, {CHARACTER, 4}},
        {15, {CHARACTER, 4}},
        {16, {CHARACTER, 4}},
        {17, {CHARACTER, 5}},
        {18, {CHARACTER, 5}},
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
        {80, {BAMBOO, 4}},
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
        uint8_t status;
        asset hi_lo_prize = DEFAULT_ASSET;
        float low_odds = MH_DEFAULT;
        float draw_odds = MH_DEFAULT;
        float high_odds = MH_DEFAULT;
        uint8_t current_tile;
        uint8_t standard_tile;
        int pair_count;
        int pung_count;
        int chow_count;
        int kong_count;
        vector<uint8_t> deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
        vector<uint8_t> hand_player = {};
        vector<uint8_t> discarded_tiles = {};
        vector<uint8_t> reveal_kong = {};
        vector<uint8_t> winning_hand = {};
    };

    struct [[eosio::table]] user
    {
        name username;
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
    void onsettledpay(name to, asset quantity, string memo);
    void gettile(game & gamedata);
    void sorthand(vector<uint8_t> & hand);
    void two_rem(game & gamedata, vector<tile> tiles);
    void five_rem(game & gamedata, vector<tile> tiles);
    void eight_rem(game & gamedata, vector<tile> tiles);
    void eleven_rem(game & gamedata, vector<tile> tiles);
    void fourteen_rem(game & gamedata, vector<tile> tiles);
    void transferhand(game & gamedata, int size);
    void pung_chow(game & gamedata, int check);
    void get_odds(game & gamedata, int value);
    void hilo_step(game & gamedata, int prev_tile, int current_tile, int option);
    int pair_pung_chow(tile tile1, tile tile2, tile tile3);
    int pair_check(tile tile1, tile tile2);
    int five_tile_check(tile tile1, tile tile2, tile tile3, tile tile4, tile tile5);
    int six_tile_check(tile tile1, tile tile2, tile tile3, tile tile4, tile tile5, tile tile6);
    void winhand_check(game & gamedata, vector<uint8_t> & hand);
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
    ACTION startgame(name username);
    ACTION starttrial(name username, vector<int> idx);
    ACTION playhilo(name username, int option);
    ACTION discardtile(name username, int idx);
    ACTION dclrkong(name username, vector<int> idx);
    ACTION dclrwinhand(name username);
    ACTION withdraw(name username);
    ACTION settledpay(name to, asset prize, string memo);
    ACTION end(name username);
};