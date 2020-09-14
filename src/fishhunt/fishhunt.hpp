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
        lake_1 = 1,
        lake_2 = 5,
        lake_3 = 10,
        lake_4 = 20
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
};