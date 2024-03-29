#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
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
        vector<tile> panel_set;
        uint8_t unopentile = PANEL_SIZE;
        uint8_t win_count = EOS_DEFAULT;
        uint8_t destination = MAP_DEFAULT;
        uint8_t status = INITIALIZED;
        uint8_t enemy_count = ENEMY_DEFAULT;
        asset prize = DEFAULT_ASSET;
        double odds = EOS_DEFAULT;
        asset nextprize = DEFAULT_ASSET;
        asset maxprize = DEFAULT_ASSET;
    };

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
    using seeds_table = eosio::multi_index<"seeds"_n, seed>;

    users_table _users;
    seeds_table _seeds;

    int rng(const int &range);
    double calculateodds(game gamedata);
    asset generateprize(game game_data);
    asset maxprize(game gamedata);
    void gameready(name username, asset quantity);
    void onsettledpay(name to, asset quantity, string memo);
    void showremainingtile(game & game_data);
    void gameupdate(game & game_data);

public:
    using contract::contract;

    treasurehunt(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                          eosio_token(MAIN_CONTRACT),
                                                                          treasurehunt_symbol(MAIN_TOKEN, PRECISION),
                                                                          _users(receiver, receiver.value),
                                                                          _seeds(receiver, receiver.value) {}

    [[eosio::on_notify("eosio.token::transfer")]] void ondeposit(name from,
                                                                 name to,
                                                                 asset quantity,
                                                                 string memo);
    ACTION initialize(name username);
    ACTION setpanel(name username, vector<uint8_t> panelset);
    ACTION destination(name username, uint8_t destination);
    ACTION setenemy(name username, uint8_t enemy_count);
    ACTION gamestart(name username, asset quantity);
    ACTION opentile(name username, uint8_t index);
    ACTION end(name username);
    ACTION withdraw(name username);
    ACTION settledpay(name to, asset prize, string memo);
    ACTION autoplay(name username, vector<uint8_t> panelset);
};