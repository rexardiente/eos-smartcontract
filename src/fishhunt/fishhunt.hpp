#include <eosio/eosio.hpp>
#include <vector>
#include <algorithm>

// #include <boost/version.hpp>
// #include <boost/config.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("fishhunt")]] fishhunt : public contract
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
        OPENED = 1,
        UNOPENED = 0,
        WIN_LIMIT = 6
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
        EXPLORE_4 = 20,
        lake_DEFAULT = 0,
        river = 1,
        lake = 5,
        nearshore = 10,
        farshore = 20
    };

    struct Fish
    {
        char key;
        uint8_t value;
    };

    struct fishlist
    {
        vector<Fish> fishes = {
            {'A', 0},
            {'B', 0},
            {'C', 0},
            {'D', 0},
            {'E', 0},
            {'F', 0},
            {'G', 0},
            {'H', 0},
            {'I', 0},
            {'J', 0},
            {'K', 0},
            {'L', 0},
            {'M', 0},
            {'N', 0},
            {'O', 0},
            {'P', 0},
            {'Q', 0},
            {'R', 0},
            {'S', 0},
            {'T', 0},
            {'U', 0},
            {'V', 0},
            {'W', 0},
            {'X', 0}};
    };

    struct fishPrize
    {
        uint16_t key;
        uint16_t value = PRIZE_DEFAULT;
    };

    struct game
    {
        vector<Tile> panels;
        vector<fishPrize> Fish_prizes = {};
        uint8_t win_count = PRIZE_DEFAULT;
        uint8_t destination = lake_DEFAULT;
        bool set_hunt = false;
        uint8_t status = INITIALIZED;
        uint16_t explore_count = EXPLORE_DEFAULT;
    };

    struct [[eosio::table]] user
    {
        name username;
        uint64_t game_id;
        game game_data;
        uint64_t total_win;
        fishlist fish_caught;

        auto primary_key() const
        {
            return username.value;
        }
    };

    struct [[eosio::table]] seed
    {
        uint64_t key = 1;
        uint32_t value = 1;

        auto primary_key() const
        {
            return key;
        }
    };
    struct [[eosio::table]] ticket
    {
        name username;
        int64_t balance;

        auto primary_key() const
        {
            return username.value;
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

        uint64_t secondary_indice() const
        {
            return username.value;
        }
    };

    using fishlists_table = eosio::multi_index<"fishlist"_n, fishlist>;
    using users_table = eosio::multi_index<"user"_n, user>;
    using tickets_table = eosio::multi_index<"ticket"_n, ticket>;
    using history_table = eosio::multi_index<"history"_n, history>;
    using seeds_table = eosio::multi_index<"seed"_n, seed>;

    fishlists_table _fishlists;
    users_table _users;
    seeds_table _seeds;
    tickets_table _tickets;
    history_table _history;

    int rng(const int range);
    int rngTwo(const int range);
    uint64_t gen_gameid();
    uint16_t iswinning(const vector<fishPrize> &Fish_prizes, const uint8_t &win_count, Fish fishcatch);
    void addhistory(user user_data);
    void ticket_update(name username, bool isdeduction, uint64_t amount);
    void game_update(name username);
    int64_t ticket_balance(name username);
    uint64_t multiplier(Fish fishcatch);

public:
    fishhunt(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                      _tickets(receiver, receiver.value),
                                                                      _history(receiver, receiver.value),
                                                                      _users(receiver, receiver.value),
                                                                      _seeds(receiver, receiver.value)
    {
    }
    [[eosio::action]] void renew(name username);
    [[eosio::action]] void initgames(name username);
    [[eosio::action]] void setuppanel(name username, vector<Tile> panel_set);
    [[eosio::action]] void playerticket(name username, uint64_t amount);
    [[eosio::action]] void destlake(name username, uint8_t lakechoice);
    [[eosio::action]] void sethuntfish(name username, bool ready);
    [[eosio::action]] void genprize(name username, uint8_t panel_idx);
};