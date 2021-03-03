#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>
#include <eosio/transaction.hpp>
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
        DEFAULT = 0,
        NUM_1 = 1,
        BATTLE_STANDBY = 1,
        ELIMINATED = 2,
        WITHDRAWN = 3
    };

    struct character
    {
        uint64_t LIFE = DEFAULT;
        int HP = DEFAULT;
        uint64_t CLASS = DEFAULT;
        uint64_t LEVEL = DEFAULT;
        uint64_t STATUS = DEFAULT;
        uint64_t ATTACK = DEFAULT;
        uint64_t DEFENSE = DEFAULT;
        uint64_t SPEED = DEFAULT;
        uint64_t LUCK = DEFAULT;
        int GAME_LIMIT = DEFAULT;
        uint64_t GAME_COUNT = DEFAULT;
        bool IS_NEW = true; // change to false once validated by server API
        uint64_t CREATED_AT = DEFAULT;
    };

    struct game
    {
        map<string, character> characters; // id and character details
    };

    struct [[eosio::table]] user
    {
        name username;
        game data;
        auto primary_key() const { return username.value; }
    };

    struct [[eosio::table]] seed
    {
        uint64_t key = 1;
        uint32_t value = 1;
        auto primary_key() const { return key; }
    };

    using users_table = eosio::multi_index<"users"_n, user>;
    using seeds_table = eosio::multi_index<"seeds"_n, seed>;
    users_table _users;
    seeds_table _seeds;

    void gen_stat(character & initial_ghost);
    int rng(const int &range);
    string checksum256_to_string_hash();
    asset calculate_prize(map<string, character>::iterator character);
    // transfer related functions..
    void oninit(name username, asset quantity, int limit);
    void onaddnewlife(name username, asset quantity, string key);
    void onsettledpay(name username, asset quantity, string memo);

public:
    using contract::contract;

    ghostquest(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                        eosio_token(MAIN_CONTRACT),
                                                                        ghostquest_symbol(MAIN_TOKEN, PRECISION),
                                                                        _users(receiver, receiver.value),
                                                                        _seeds(receiver, receiver.value) {}

    [[eosio::on_notify("eosio.token::transfer")]] void ondeposit(name from, name to, asset quantity, string memo);
    ACTION settledpay(name to, asset prize, string memo);
    ACTION withdraw(name username, string key);

    // New: merge init and generate character..
    ACTION init(name username, asset quantity, int limit);
    ACTION battleresult(string gameid, pair<string, name> winner, pair<string, name> loser);
    ACTION newlife(name username, asset quantity, string key);
    ACTION eliminate(name username, string key);
    ACTION end(name username);
};
