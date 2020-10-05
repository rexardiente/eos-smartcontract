#include <eosio/eosio.hpp>
#include <vector>
#include <algorithm>

// #include <boost/version.hpp>
// #include <boost/config.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("treasure.v2")]] treasurev2 : public contract
{
public:
    struct Tile
    {
        uint8_t panel_idx;
        uint8_t isopen;
    };

private:
    enum prize_value : int8_t
    {
        PRIZE_DEFAULT = 0,
        UNOPENED = 0,
        OPENED = 1,
        WIN_LIMIT = 4
    };
    enum game_status : int8_t
    {
        INITIALIZED = 0,
        ONGOING = 1,
        DONE = 2
    };
    enum game_destination : int8_t
    {
        EXPLORE_DEFAULT = 0,
        EXPLORE_1 = 1,
        EXPLORE_2 = 5,
        EXPLORE_3 = 10,
        MAP_DEFAULT = 0,
        MAP_1 = 1,
        MAP_2 = 10,
        MAP_3 = 50
    };
    
    struct TilePrize
    {
        uint16_t key;
        uint16_t value = PRIZE_DEFAULT;
        uint16_t multiplier;
    };

    struct game
    {
        vector<Tile> panels;
        vector<TilePrize> tile_prizes = {};
        uint8_t win_count = PRIZE_DEFAULT;
        uint8_t destination = MAP_DEFAULT;
        uint8_t status = INITIALIZED;
        uint8_t enemy_count =0;
        int64_t bet_amount;
        
    };

    // Tickets Table
    struct [[eosio::table]] ticket
    {
        name username;
        int64_t balance;

        auto primary_key() const
        {
            return username.value;
        }
    };

    struct [[eosio::table]] user
    {
        name username;
        uint64_t game_id;
        int64_t cash_balance;
        game game_data;
        
        uint64_t total_win; // total win in points (1 ticket):(1 EOS)
      
        auto primary_key() const
        {
            return username.value;
        }
    };

    struct [[eosio::table]] seed
    {
        uint64_t key = 1;   // default key of 1
        uint32_t value = 1; // default value of 1

        auto primary_key() const
        {
            return key;
        }
    };

    

    using users_table = eosio::multi_index<"user"_n, user>;
    using seeds_table = eosio::multi_index<"seed"_n, seed>;

    users_table _users;
    seeds_table _seeds;
    
    int rng(const int &range);
    uint16_t iswinning(const vector<TilePrize> &tile_prizes, const uint8_t &win_count);
    uint64_t gen_gameid();
    void game_update(name username);
    uint64_t multiplier();

public:
    treasurev2(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                        _users(receiver, receiver.value),
                                                                        _seeds(receiver, receiver.value) {}
    [[eosio::action]] void init(name username);
    [[eosio::action]] void end(name username);
    [[eosio::action]] void gamestart(name username,vector<Tile> panel_set,uint8_t enemy_count,int64_t bet_amount);
};