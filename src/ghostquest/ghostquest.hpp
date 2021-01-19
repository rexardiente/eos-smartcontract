#include <map>
#include <string>
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>
#include "config.hpp"

using namespace std;
using namespace eosio;

class [[eosio::contract("ghostquest")]] ghostquest : public eosio::contract
{
private:
    const symbol ghostquest_symbol;
    const name eosio_token;

    enum Defaults : int64_t
    {
        GQ_DEFAULT = 0,
        SUMMONED = 1,
        STANDBY = 2,
        INBATTLE = 3,
        WINNER = 4,
        LOSER = 5,
        ELIMINATED = 6,
        IDLE = 7,
        ONGOING = 1,
        DONE = 2
    };

    struct battle_history
    {
        name enemy;
        string enemy_id;
        uint64_t time_executed;
        vector<string> gameplay_log = {};
        bool isWin;
    };

    struct ghost
    {
        name owner;
        // uint64_t ghost_id; move as ID Key
        uint64_t character_life = GQ_DEFAULT;
        int initial_hp = GQ_DEFAULT;
        uint64_t ghost_class = GQ_DEFAULT;
        uint64_t ghost_level = GQ_DEFAULT;
        uint64_t status = GQ_DEFAULT;
        uint64_t attack = GQ_DEFAULT;
        uint64_t defense = GQ_DEFAULT;
        uint64_t speed = GQ_DEFAULT;
        uint64_t luck = GQ_DEFAULT;
        asset prize = DEFAULT_ASSET;
        int battle_limit = GQ_DEFAULT;
        uint64_t battle_count = GQ_DEFAULT;
        uint64_t last_match = GQ_DEFAULT;
        map<string, battle_history> match_history;
        uint64_t created_at = GQ_DEFAULT;
    };

    struct game
    {
        map<string, ghost> character;
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
    // void battle_step(map<string, ghost>::iterator & ghost1, map<string, ghost>::iterator & ghost2, battle_history & current_battle);
    // void damage_step(map<string, ghost>::iterator & attacker, map<string, ghost>::iterator & defender, int round, battle_history &current_battle, int damage);
    // void result_step(map<string, ghost>::iterator & loser, map<string, ghost>::iterator & winner, battle_history & current_battle);
    void set_add_life(name username, asset quantity, string key);
    void calculate_prize(map<string, ghost>::iterator & ghost);
    void eliminated_withdrawn(map<string, ghost>::iterator & ghost);
    string checksum256_to_string(std::array<uint8_t, 32UL> arr, size_t size);

public:
    using contract::contract;

    ghostquest(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                        eosio_token(MAIN_CONTRACT),
                                                                        ghostquest_symbol(MAIN_TOKEN, PRECISION),
                                                                        _users(receiver, receiver.value),
                                                                        _seeds(receiver, receiver.value) {}

    [[eosio::on_notify("eosio.token::transfer")]] void ondeposit(name from,
                                                                 name to,
                                                                 asset quantity,
                                                                 string memo);
    ACTION initialize(name username);
    ACTION battle(string gameid, pair<string, name> winner, pair<string, name> loser, vector<string> logs);
    ACTION withdraw(name username, string key);
    ACTION settledpay(name to, asset prize, string memo);
    ACTION genchar(name username, asset quantity, int limit);
    ACTION addlife(name username, asset quantity, string key);
    ACTION eliminate(name username, string key);
    ACTION end(name username);
};