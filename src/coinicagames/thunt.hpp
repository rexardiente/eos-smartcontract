#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>
#include <string>
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
        TH_DEFAULT = 0,
        PANEL_SIZE = 16,
        IS_OPEN_PANEL = 0,
        IS_WIN_PANEL = 0,
        PRIZE_DEFAULT = 0,
        ENEMY_DEFAULT = 0,
        TH_INITIALIZED = 0,
        TH_ONGOING = 1,
        TH_DONE = 2,
        TH_UNOPENED = 0, // for checking
        MAP_DEFAULT = 0,
    };

    struct thtile
    {
        uint8_t key; // panel index(panel_idx)
        uint8_t isopen = IS_OPEN_PANEL;
        uint8_t iswin = IS_WIN_PANEL;
    };

    struct thgame
    {
        string game_id;
        vector<thtile> panel_set;
        uint8_t unopentile = PANEL_SIZE;
        uint8_t win_count = EOS_DEFAULT;
        uint8_t destination = MAP_DEFAULT;
        uint8_t status = INITIALIZED;
        uint8_t enemy_count = ENEMY_DEFAULT;
        double prize = EOS_DEFAULT;
        double odds = EOS_DEFAULT;
        double nextprize = EOS_DEFAULT;
        double maxprize = EOS_DEFAULT;
    };

    struct [[eosio::table]] thunt
    {
        // string user_id;
        int id;
        thgame game_data;

        auto primary_key() const
        {
            return id;
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


    using seeds_table = eosio::multi_index<"seeds"_n, seed>;

    seeds_table _seeds;

    // int rng(const int &range);
    // double calculateodds(thgame gamedata);
    double generateprize(thgame game_data);
    double maxprize(thgame gamedata);
    // void gameready(string user_id, double quantity);
    // void onsettledpay(name to, asset quantity, string memo);
    void showremainingtile(thgame & game_data);
    void gameupdate(thgame & game_data);
    // string checksum256_to_string(std::array<uint8_t, 32UL> arr, size_t size);

public:
    using contract::contract;

    treasurehunt(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                          eosio_token(MAIN_CONTRACT),
                                                                          treasurehunt_symbol(MAIN_TOKEN, PRECISION),
                                                                          _users(receiver, receiver.value),
                                                                          _seeds(receiver, receiver.value) {}

//     [[eosio::on_notify("eosio.token::transfer")]] void ondeposit(name from,
//                                                                  name to,
//                                                                  asset quantity,
//                                                                  string memo);
    ACTION thinitialize(int id);
    ACTION thsetpanel(int id, vector<uint8_t> panelset);
    ACTION thdstination(int id, uint8_t destination);
    ACTION thsetenemy(int id, uint8_t enemy_count);
    ACTION thgamestart(int id, double quantity);
    ACTION thopentile(int id, uint8_t index);
    ACTION thend(int id);
    ACTION thwithdraw(int id);
    // ACTION thsettledpay(name to, asset prize, string memo);
    ACTION thautoplay(int id, vector<uint8_t> panelset);
    ACTION thdel(int size);
};