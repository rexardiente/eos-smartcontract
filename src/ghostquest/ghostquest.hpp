#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include <map>
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
        LIFE_DEFAULT = 0,
        HP_DEFAULT = 0,
        STAT_DEFAULT = 0,
        BL_DEFAULT = 0,
        LVL_DEFAULT = 0,
        LMT_DEFAULT = 0,
        CLASS_DEFAULT = 0,
        SUMMONED = 1,
        STANDBY = 2,
        INBATTLE = 3,
        WINNER = 4,
        LOSER = 5,
        ELIMINATED = 6,
        IDLE = 7,
        INITIALIZED = 0,
        ONGOING = 1,
        DONE = 2
    };

    // struct fight_log
    // {
    //     string gameplay_log;
    // };

    struct ghost
    {
        name owner;
        uint64_t ghost_id;
        uint64_t character_life = LIFE_DEFAULT;
        int initial_hp = HP_DEFAULT;
        int hitpoints = HP_DEFAULT;
        uint64_t ghost_class = CLASS_DEFAULT;
        uint64_t ghost_level = LVL_DEFAULT;
        uint64_t status = GQ_DEFAULT;
        uint64_t attack = STAT_DEFAULT;
        uint64_t defense = STAT_DEFAULT;
        uint64_t speed = STAT_DEFAULT;
        uint64_t luck = STAT_DEFAULT;
        asset prize = DEFAULT_ASSET;
        int battle_limit = LMT_DEFAULT;
        uint64_t battle_count = GQ_DEFAULT;
        uint64_t last_match = GQ_DEFAULT;
        map<uint64_t, name> enemy_fought;
        // std::chrono::time_point<std::chrono::steady_clock> last_battle;
        // vector<fight_log> battle_log;

        // auto primary_key() const
        // {
        //     return key;
        // }
    };

    struct game
    {
        map<int, ghost> character;
        uint8_t status = INITIALIZED;
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
    uint64_t genghostid();
    void game_ready(name username, asset quantity, int limit);
    void onsettledpay(name to, asset quantity, string memo);
    void gen_stat(ghost & initghost);
    void battle_step(map<int, ghost>::iterator & ghost1, map<int, ghost>::iterator & ghost2);
    void damage_step(map<int, ghost>::iterator & attacker, map<int, ghost>::iterator & defender, int round);
    void result_step(map<int, ghost>::iterator & loser, map<int, ghost>::iterator & winner);
    void set_add_life(name username, asset quantity, int key);
    void calculate_prize(map<int, ghost>::iterator & ghost);
    void eliminated_withdrawn(map<int, ghost>::iterator & ghost);

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
    ACTION battle(name username1, int ghost1_key, name username2, int ghost2_key);
    ACTION withdraw(name username, int key);
    ACTION settledpay(name to, asset prize, string memo);
    ACTION genchar(name username, asset quantity, int limit);
    ACTION addlife(name username, asset quantity, int key);
    ACTION end(name username);
};