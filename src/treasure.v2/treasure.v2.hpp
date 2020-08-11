#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("treasure.v2")]] treasurev2 : public contract {
public:
    struct opened_panel
    {
        uint16_t panel_number;
        uint16_t isopen;
    };

private:
    // Basics Operation
    // Table for current opened and available tile..
    //     * (1 - 16) in bool data type which be represented by (1 or 2)
    // game_id as secondary ID for history
    // auto play mode will be added once manual gameplay is done!
    // Useer will provide 16, panel set.
    // update first the table before calling `game_status` function

    enum prize_value : int8_t
    {
        PRIZE_DEFAULT = 0,
        OPENED = 2,
        UNOPENED = 1,
    };
    enum game_status : int8_t
    {
        DONE = 0,
        ONGOING = 1
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

    struct win_prize
    {
        uint16_t key;
        uint16_t value = PRIZE_DEFAULT;
    };

    struct game
    {
        vector<opened_panel> panels ={
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
        vector<win_prize> win_prizes ={};
        uint8_t destination = MAP_DEFAULT;
        uint16_t explore_count = EXPLORE_DEFAULT;
        int8_t status = ONGOING;
    };

    // Tickets Table
    // struct [[eosio::table]] ticket {

    // }

    struct [[eosio::table]] user
    {
        name username;
        string game_id;
        game game_data;
        uint64_t tickets; // remaining ticket
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

    using users_table = eosio::multi_index<"user"_n, user>;
    using seeds_table = eosio::multi_index<"seed"_n, seed>;

    users_table _users;
    seeds_table _seeds;

    int rng(const int range);
    int calculate_prize(); // triggered inside panel_prize

    void panel_prize();
    void update_game();

public:
    treasurev2(name receiver, name code, datastream<const char*> ds) :
        contract(receiver, code, ds),
        _users(receiver, receiver.value),
        _seeds(receiver, receiver.value) {}

    using contract::contract; // not sure what is the use of this..

    [[eosio::action]] void hello(name username);
    [[eosio::action]] void authorized(name username);
    [[eosio::action]] void startgame(name username, uint8_t destination, uint16_t explore_count, vector<treasurev2::opened_panel> panel_set);
    [[eosio::action]] void end(name username);
    [[eosio::action]] void removeuser(name user);
    [[eosio::action]] void renew(name username, bool isreset);
};