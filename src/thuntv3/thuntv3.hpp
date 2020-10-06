#include <eosio/eosio.hpp>
#include <vector>
#include <algorithm>
#include <eosio/asset.hpp>
#include <eosio/print.hpp>
#include <eosio/singleton.hpp>

// #include <boost/version.hpp>
// #include <boost/config.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("thuntv3")]] thuntv3 : public contract
{

private:
    enum Defaults : int64_t
    {
        EOS_DEFAULT = 0,
        PANEL_SIZE = 16,
        IS_OPEN_PANEL = 0, // for default value
        IS_WIN_PANEL = 0,  // for default value
        PRIZE_DEFAULT = 0,
        ENEMY_DEFAULT = 0,
        INITIALIZED = 0,
        ONGOING = 1,
        DONE = 2,
        UNOPENED = 0, // for checking
        MAP_DEFAULT = 0,
        MAP_1 = 1,
        MAP_2 = 10,
        MAP_3 = 20
    };

    // // index 0 = 1EOS
    // // index 1 = 10EOS
    // // index 2 = 20EOS
    // struct [[eosio::table]] ticket
    // {
    //     name username;
    //     vector<uint64_t> balance = {
    //         EOS_DEFAULT,
    //         EOS_DEFAULT,
    //         EOS_DEFAULT};

    //     auto primary_key() const
    //     {
    //         return username.value;
    //     }
    // };

    struct tile
    {
        uint8_t key; // panel index(panel_idx)
        uint8_t isopen = IS_OPEN_PANEL;
        uint8_t iswin = IS_WIN_PANEL;
    };

    // struct list
    // {
    //     uint8_t key;
    // };

    struct game
    {
        vector<tile> panel_set;
        // vector<list> tilelist;
        uint8_t unopentile = PANEL_SIZE;
        uint8_t win_count = EOS_DEFAULT;
        uint8_t destination = MAP_DEFAULT;
        uint8_t status = INITIALIZED;
        uint8_t enemy_count = ENEMY_DEFAULT;
        uint64_t prize = PRIZE_DEFAULT;
        bool isready = false;
        // bool isdone = false;
    };

    // struct [[eosio::table]] history
    // {
    //     uint64_t game_id = 1; // default key of 1
    //     name username;
    //     game game_data;

    //     auto primary_key() const
    //     {
    //         return game_id;
    //     };

    //     uint64_t secondary_indice() const
    //     {
    //         return username.value;
    //     };
    // };

    struct [[eosio::table]] user
    {
        name username;
        uint64_t game_id;
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
    // using history_table = eosio::multi_index<"history"_n, history, indexed_by<"byusername"_n, const_mem_fun<history, uint64_t, &history::secondary_indice>>>;
    using seeds_table = eosio::multi_index<"seeds"_n, seed>;
    // using tickets_tbl = eosio::multi_index<"tickets"_n, ticket>;

    users_table _users;
    seeds_table _seeds;
    // history_table _history;
    // tickets_tbl _tickets;

    // void check_asset(asset quantity, uint64_t minvalue, uint8_t nonfloat);
    // void ticket_add(name from, uint64_t quantity, uint64_t destination);
    // void ticket_update(name username, const bool &isdeduction, const bool &destination, const uint64_t &amount);
    // uint64_t ticket_balances(name username, const int &ticket_destination);

    uint64_t gen_gameid();
    int rndm(const int &range);
    float roundoff(float value);
    // void genresult(name username, const uint8_t &enemy_count, const uint8_t &unopentile);
    // void addhistory(user user_data);
    void game_update(name username);

public:
    thuntv3(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                     _users(receiver, receiver.value),
                                                                     _seeds(receiver, receiver.value) {}
    //  _history(receiver, receiver.value) {}
    //  _tickets(receiver, receiver.value) {}

    // static symbol eosio_symbol() { return symbol("EOS", 4); }
    // static name eosio_contract() { return name("eosio.token"); }

    // void ondeposit(eosio::name from,
    //                eosio::name to,
    //                eosio::asset quantity,
    //                std::string memo);

    ACTION intgame(name username);
    ACTION setpanel(name username, vector<uint8_t> panelset);
    ACTION destination(name username, uint8_t destination);
    ACTION setenemy(name username, uint8_t enemy_count);
    ACTION gamestart(name username);
    ACTION slcttile(name username, uint8_t index);
    ACTION end(name username);
    ACTION withdraw(name username);
};