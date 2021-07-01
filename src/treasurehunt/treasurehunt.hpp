#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>
#include <string>
#include "config.hpp"

using namespace std;
using namespace eosio;

class [[eosio::contract("treasurehunt")]] treasurehunt : public eosio::contract
{
private:
    const symbol treasurehunt_symbol;
    const name eosio_token;

    enum Defaults : int64_t
    {
        EOS_DEFAULT = 0,
        PANEL_SIZE = 16,
        IS_OPEN_PANEL = 0,
        IS_WIN_PANEL = 0,
        PRIZE_DEFAULT = 0,
        ENEMY_DEFAULT = 0,
        INITIALIZED = 0,
        ONGOING = 1,
        DONE = 2,
        UNOPENED = 0, // for checking
        MAP_DEFAULT = 0,
    };

    struct tile
    {
        uint8_t key; // panel index(panel_idx)
        uint8_t isopen = IS_OPEN_PANEL;
        uint8_t iswin = IS_WIN_PANEL;
    };

    struct game
    {
        string game_id;
        vector<tile> panel_set;
        uint8_t unopentile = PANEL_SIZE;
        uint8_t win_count = EOS_DEFAULT;
        uint8_t destination = MAP_DEFAULT;
        uint8_t status = INITIALIZED;
        uint8_t enemy_count = ENEMY_DEFAULT;
        double prize = EOS_DEFAULT;
        double odds = EOS_DEFAULT;
        double nextprize = EOS_DEFAULT;
        double maxprize = EOS_DEFAULT;
    };

    struct [[eosio::table]] user
    {
        // string user_id;
        int id;
        game game_data;

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

    using users_table = eosio::multi_index<"users"_n, user>;
    using seeds_table = eosio::multi_index<"seeds"_n, seed>;

    users_table _users;
    seeds_table _seeds;

    int rng(const int &range);
    double calculateodds(game gamedata);
    double generateprize(game game_data);
    double maxprize(game gamedata);
    // void gameready(string user_id, double quantity);
    // void onsettledpay(name to, asset quantity, string memo);
    void showremainingtile(game & game_data);
    void gameupdate(game & game_data);
    string checksum256_to_string(std::array<uint8_t, 32UL> arr, size_t size);

public:
    using contract::contract;

    treasurehunt(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                          eosio_token(MAIN_CONTRACT),
                                                                          treasurehunt_symbol(MAIN_TOKEN, PRECISION),
                                                                          _users(receiver, receiver.value),
                                                                          _seeds(receiver, receiver.value) {}

//     [[eosio::on_notify("eosio.token::transfer")]] void ondeposit(name from,
//                                                                  name to,
//                                                                  asset quantity,
//                                                                  string memo);
    ACTION initialize(int id);
    ACTION setpanel(int id, vector<uint8_t> panelset);
    ACTION destination(int id, uint8_t destination);
    ACTION setenemy(int id, uint8_t enemy_count);
    ACTION gamestart(int id, double quantity);
    ACTION opentile(int id, uint8_t index);
    ACTION end(int id);
    ACTION withdraw(int id);
    // ACTION settledpay(name to, asset prize, string memo);
    ACTION autoplay(int id, vector<uint8_t> panelset);
};