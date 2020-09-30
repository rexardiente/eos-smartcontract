#include <eosio/eosio.hpp>
#include <vector>
#include <algorithm>

// #include <boost/version.hpp>
// #include <boost/config.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("treasurehuntv3")]] treasurehuntv3 : public contract
{
public:
    struct Tile
    {
        uint8_t panel_idx;
        uint8_t isopen;
        uint8_t iswin;
    };

private:
    enum game_status : int8_t
    {
        INITIALIZED = 0,
        ONGOING = 1,
        DONE = 2
    };

    enum game_destination : int8_t
    {
        MAP_DEFAULT = 0,
        MAP_1 = 1,
        MAP_2 = 10,
        MAP_3 = 50
    };

    struct game
    {
        vector<Tile> panels;
        uint8_t destination = MAP_DEFAULT;
        bool set_sail = false;
        uint8_t status = INITIALIZED;
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
        game game_data;

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

    struct [[eosio::table]] history
    {
        uint64_t game_id = 1;
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

    using users_table = eosio::multi_index<"user"_n, user>;
    using tickets_table = eosio::multi_index<"ticket"_n, ticket>;
    using history_table = eosio::multi_index<"history"_n, history, indexed_by<"byusername"_n, const_mem_fun<history, uint64_t, &history::secondary_indice>>>;
    using seeds_table = eosio::multi_index<"seed"_n, seed>;

    users_table _users;
    tickets_table _tickets;
    seeds_table _seeds;
    history_table _history;
};