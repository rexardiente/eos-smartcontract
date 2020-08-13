#include <eosio/eosio.hpp>
#include <vector>
#include <algorithm>

// #include <boost/version.hpp>
// #include <boost/config.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("treasure.v2")]] treasurev2 : public contract {
public:
    struct Tile
    {
        uint8_t panel_idx;
        uint8_t isopen;
    };

private:
    // Basics Operation
    // Table for current opened and available tile..
    //     * (1 - 16) in bool data type which be represented by (1 or 2)
    // game_id as secondary ID for history
    // auto play mode will be added once manual gameplay is done!
    // Useer will provide 16, panel set.
    // update first the table before calling `game_status` function
    // user win_tracker will be table with username and array for win details..

    // remove ticket on user value and use update function to update ticket..

    enum prize_value : int8_t
    {
        PRIZE_DEFAULT = 0,
        OPENED = 1,
        UNOPENED = 2,
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
    };

    struct game
    {
        vector<Tile> panels ={
            { 0, UNOPENED },
            { 1, UNOPENED },
            { 2, UNOPENED },
            { 3, UNOPENED },
            { 4, UNOPENED },
            { 5, UNOPENED },
            { 6, UNOPENED },
            { 7, UNOPENED },
            { 8, UNOPENED },
            { 9, UNOPENED },
            { 10, UNOPENED },
            { 11, UNOPENED },
            { 12, UNOPENED },
            { 13, UNOPENED },
            { 14, UNOPENED },
            { 15, UNOPENED } };
        vector<TilePrize> tile_prizes ={};
        uint8_t win_count = PRIZE_DEFAULT;
        uint8_t destination = MAP_DEFAULT;
        uint16_t explore_count = EXPLORE_DEFAULT;
        int8_t status = INITIALIZED;
    };

    // Tickets Table
    struct [[eosio::table]] ticket {
        name username;
        int64_t balance;

        auto primary_key() const {
            return username.value;
        }
    };

    struct [[eosio::table]] user
    {
        name username;
        uint64_t game_id;
        game game_data;
        uint64_t total_win; // total win in points (1 ticket):(1 EOS)

        auto primary_key() const {
            return username.value;
        }
    };

    struct [[eosio::table]] seed
    {
        uint64_t key   = 1; // default key of 1
        uint32_t value = 1; // default value of 1

        auto primary_key() const
        {
            return key;
        }
    };

    struct [[eosio::table]] history
    {
        uint64_t game_id = 1; // default key of 1
        name username;
        game game_data;

        auto primary_key() const
        {
            return game_id;
        }
    };

    using users_table = eosio::multi_index<"user"_n, user>;
    using tickets_table = eosio::multi_index<"ticket"_n, ticket>;
    using history_table = eosio::multi_index<"history"_n, history>;
    using seeds_table = eosio::multi_index<"seed"_n, seed>;

    users_table _users;
    tickets_table _tickets;
    seeds_table _seeds;
    history_table _history;

    // vector<results> prizeresults(game game_data, user modified_users);
    // void panel_prize();
    // void update_game();
    int rng(const int range);
    uint16_t calculate_prize(vector<TilePrize>& tile_prizes, uint8_t& win_count);
    uint64_t gen_gameid();
    void addhistory(user user_data);
    void purchase(name username, uint64_t amount);
    void ticket_update(name username, bool isdeduction, uint64_t amount);
    int64_t ticket_balance(name username);

public:
    treasurev2(name receiver, name code, datastream<const char*> ds) :
        contract(receiver, code, ds),
        _users(receiver, receiver.value),
        _tickets(receiver, receiver.value),
        _history(receiver, receiver.value),
        _seeds(receiver, receiver.value) {}

    [[eosio::action]] void authorized(name username);
    [[eosio::action]] void startgame(name username, uint8_t destination, uint16_t explore_count, vector<Tile> panel_set);
    [[eosio::action]] void genprize(name username, uint8_t panel_idx);
    [[eosio::action]] void end(name username);
    [[eosio::action]] void renew(name username, bool isreset);
};