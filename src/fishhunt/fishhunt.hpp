#include <eosio/eosio.hpp>
#include <vector>
#include <algorithm>

// #include <boost/version.hpp>
// #include <boost/config.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("fishhunt")]] fishhunt : public contract
{
public:
private:
    enum prize_value : int8_t
    {
        PRIZE_DEFAULT = 0,
        OPENED = 1,
        UNOPENED = 0,
        WIN_LIMIT = 6
    };
    enum game_status : int8_t
    {
        INITIALIZED = 0,
        ONGOING = 1,
        DONE = 2
    };
    enum game_destination : int8_t
    {
        lake_DEFAULT = 0,
        river = 1,
        lake = 5,
        nearshore = 10,
        farshore = 20
    };

    struct Fish
    {
        char key;
        uint8_t value;
    };

    struct FishList
    {
        name username;
        vector<Fish> fishes = {
            {'A', 0},
            {'B', 0},
            {'C', 0},
            {'D', 0},
            {'E', 0},
            {'F', 0},
            {'G', 0},
            {'H', 0},
            {'I', 0},
            {'J', 0},
            {'K', 0},
            {'L', 0},
            {'M', 0},
            {'N', 0},
            {'O', 0},
            {'P', 0},
            {'Q', 0},
            {'R', 0},
            {'S', 0},
            {'T', 0},
            {'U', 0},
            {'V', 0},
            {'W', 0},
            {'X', 0}};

        auto primary_key() const
        {
            return username.value;
        }
    };

    struct fishPrize
    {
        uint16_t key;
        uint16_t value = PRIZE_DEFAULT;
    };

    struct game
    {
        vector<fishPrize> Fish_prizes = {};
        uint8_t win_count = PRIZE_DEFAULT;
        uint8_t destination = lake_DEFAULT;
        bool set_hunt = false;
        uint8_t status = INITIALIZED;
    };

    struct [[eosio::table]] user
    {
        name username;
        uint64_t game_id;
        game game_data;
        uint64_t total_win;

        auto primary_key() const
        {
            return username.value;
        }
    };

    struct [[eosio::table]] seed
    {
        uint64_t key = 1;
        uint32_t value = 1;

        auto primary_key() const
        {
            return key;
        }
    };

    using users_table = eosio::multi_index<"user"_n, user>;
    using seeds_table = eosio::multi_index<"seed"_n, seed>;

    users_table _users;
    seeds_table _seeds;

public:
    fishhunt(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                      _users(receiver, receiver.value),
                                                                      _seeds(receiver, receiver.value)
    {
    }
    [[eosio::action]] void renew(name username);
    [[eosio::action]] void initgames(name username);
    [[eosio::action]] void end(name username);
};