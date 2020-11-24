#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include "config.hpp"
// #include <chrono>

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
        name ghost_id;
        uint8_t key = GQ_DEFAULT;
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
        uint64_t battle_limit = LMT_DEFAULT;
        uint64_t battle_count = GQ_DEFAULT;
        // std::chrono::time_point<std::chrono::steady_clock> last_battle;
        // vector<fight_log> battle_log;

        // auto primary_key() const
        // {
        //     return key;
        // }
    };

    struct game
    {
        vector<ghost> character;
        uint64_t monster_count = GQ_DEFAULT;
        uint64_t summon_count = GQ_DEFAULT;
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

    void gameready(name username, asset quantity);
    void onsettledpay(name to, asset quantity, string memo);
    void genstat(ghost & initghost);
    void battle(ghost & ghost1, ghost & ghost2);
    void damage_step(ghost & ghost1, ghost & ghost2, int round);
    void calculate_prize(ghost & ghost);

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
    ACTION summoncount(name username, uint64_t summoncount, uint64_t battlelimit);
    ACTION findmatch(name username, uint64_t char1, uint64_t char2);
    ACTION withdraw(name username, uint64_t idx);
    ACTION settledpay(name to, asset prize, string memo);
    ACTION getstat(name username, asset quantity);
    ACTION end(name username);
};