#include <eosio/eosio.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("treasure.v2")]] treasurev2 : public contract {
private:

    struct game {

    };

    struct [[eosio::table]] user
    {
        name username;
        game game_data;
        uint16_t tickets; // remaining ticket
        uint16_t total_win; // total win in points (1 ticket):(1 EOS)

        auto primary_key() const {
            return username.value;
        }
    };


    struct [[eosio::table]] seed {
        uint64_t key   = 1; // default key of 1
        uint32_t value = 1; // default value of 1

        auto primary_key() const {
            return key;
        }
    };

    using user_table = eosio::multi_index<"user"_n, user>;
    using seed_table = eosio::multi_index<"seed"_n, seed>;

    user_table _user;
    seed_table _seed;

    int rng(const int range);
    int calculate_prize(); // triggered inside panel_prize

    void panel_prize();
    void update_game();

public:
    treasurev2(name receiver, name code, datastream<const char*> ds) :
        contract(receiver, code, ds),
        _user(receiver, receiver.value),
        _seed(receiver, receiver.value) {}

    using contract::contract; // not sure what is the use of this..

    [[eosio::action]] void hello(name username);
    [[eosio::action]] void login(name username);
    [[eosio::action]] void startgame(name username);
    [[eosio::action]] void end(name username);
    [[eosio::action]] void reset(name username);
};