#include <map>
#include <string>
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include "config.hpp"

using namespace std;
using namespace eosio;

class [[eosio::contract("mahjonghilo")]] mahjonghilo : public eosio::contract
{
private:
    const symbol mahjonghilo_symbol;
    const name eosio_token;

    enum Defaults : int64_t
    {
        MAHJONGHILO_DEFAULT = 0,
        INITIALIZED = 0,
        ONGOING = 1,
        DONE = 2
    };

    struct tile
    {
        int hi_lo_value;
        name tile_class;
        uint8_t status;
    };

    struct game
    {
        map<uint64_t, ghost> character;
        // uint8_t status = INITIALIZED;
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
    void summon_ready(name username, asset quantity, int limit);
    void onsettledpay(name to, asset quantity, string memo);
    void gen_stat(ghost & initghost);
    void battle_step(map<uint64_t, ghost>::iterator & ghost1, map<uint64_t, ghost>::iterator & ghost2, battle_history & current_battle);
    void damage_step(map<uint64_t, ghost>::iterator & attacker, map<uint64_t, ghost>::iterator & defender, int round, battle_history &current_battle);
    void result_step(map<uint64_t, ghost>::iterator & loser, map<uint64_t, ghost>::iterator & winner, battle_history & current_battle);
    void set_add_life(name username, asset quantity, int key);
    void calculate_prize(map<uint64_t, ghost>::iterator & ghost);
    void eliminated_withdrawn(map<uint64_t, ghost>::iterator & ghost);

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
    ACTION battle(vector<pair<uint64_t, name>> & players, string gameid);
    ACTION withdraw(name username, int key);
    ACTION settledpay(name to, asset prize, string memo);
    ACTION genchar(name username, asset quantity, int limit);
    ACTION addlife(name username, asset quantity, uint64_t key);
    ACTION eliminate(name username, int key);
    ACTION end(name username);
};