#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace std;
using namespace eosio;

class [[eosio::contract("treasurehunt")]] treasurehunt : public contract
{

private:
    enum Defaults : int64_t
    {
        EOS_DEFAULT = 0,
        PANEL_SIZE = 16,
        IS_OPEN_PANEL = 0, // for default value
        IS_WIN_PANEL = 0,  // for default value
        PRIZE_DEFAULT = 0,
        ENEMY_DEFAULT = 0,
        INITIALIZED = 0,
        ONGOING = 1,
        DONE = 2,
        UNOPENED = 0, // for checking
        MAP_DEFAULT = 0,
        MAP_1 = 1,
        MAP_2 = 10,
        MAP_3 = 20
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
        uint64_t prize = PRIZE_DEFAULT;
        bool isready = false;
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

    // void game_update(name username);
    uint64_t generategameid();
    int rng(const int &range);
    float roundoff(float value);

public:
    treasurehunt(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                          _users(receiver, receiver.value),
                                                                          _seeds(receiver, receiver.value) {}
    static symbol eosio_symbol() { return symbol("EOS", 4); }
    static name eosio_contract() { return name("eosio.token"); }
    void ontransfernotification(eosio::name from,
                               eosio::name to,
                               eosio::asset quantity,
                               std::string memo);

    ACTION initialize(name username);
    ACTION setpanel(name username, vector<uint8_t> panelset);
    ACTION destination(name username, uint8_t destination);
    ACTION setenemy(name username, uint8_t enemy_count);
    ACTION gamestart(name username);
    ACTION opentile(name username, uint8_t index);
    ACTION end(name username);
};

extern "C" void apply(uint64_t receiver, uint64_t code, uint64_t action)
{
#if DEBUG
    eosio::print("receiver:");
    eosio::print(receiver);
    eosio::print("code:");
    eosio::print(code);
    eosio::print("action:");
    eosio::print(action);
    eosio::print("\n");
#endif

    if (code == treasurehunt::eosio_contract().value && action == "transfer"_n.value)
    {
        //proses transfer from user to our contract account
        eosio::execute_action(eosio::name(receiver), eosio::name(code), &treasurehunt::ontransfernotification);
    }
    else if (code == receiver)
    {
        switch (action)
        {
            EOSIO_DISPATCH_HELPER(treasurehunt, (initialize)(destination)(setpanel)(setenemy)(gamestart)(opentile)(end))
        }
    }
}