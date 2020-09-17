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
        lake_DEFAULT = 0,
        lake_1 = 1,
        lake_2 = 5,
        lake_3 = 10,
        lake_4 = 20
    };

    struct TilePrize
    {
        uint16_t key;
        uint16_t value = PRIZE_DEFAULT;
    };

    struct game
    {
        vector<Tile> panels;
        vector<TilePrize> tile_prizes = {};
        uint8_t win_count = PRIZE_DEFAULT;
        uint8_t destination = lake_DEFAULT;
        bool set_hunt = false;
        uint8_t status = INITIALIZED;
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

    struct [[eosio::table]] user
    {
        name username;
        uint64_t game_id;
        game game_data;
        uint64_t total_win;

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

   using users_table = eosio::multi_index<"user"_n, user>;
    using tickets_table = eosio::multi_index<"ticket"_n, ticket>;
    using history_table = eosio::multi_index<"history"_n, history>;
    using seeds_table = eosio::multi_index<"seed"_n, seed>;

    users_table _users;
    tickets_table _tickets;
    seeds_table _seeds;
    history_table _history;

    int rng(const int range);
    uint64_t gen_gameid();
    void addhistory(user user_data);
     void ticket_update(name username, bool isdeduction, uint64_t amount);
   int64_t ticket_balance(name username);
    
public:
    fishhunt(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                         _tickets(receiver, receiver.value),
                                                                        _history(receiver, receiver.value),
                                                                      _users(receiver, receiver.value),
                                                                      _seeds(receiver, receiver.value)
    {
    }
    [[eosio::action]] void playerticket(name username, uint64_t amount);
    [[eosio::action]] void renew(name username);
    [[eosio::action]] void initgames(name username);
    [[eosio::action]] void setuppanel(name username, vector<Tile> panel_set);

};