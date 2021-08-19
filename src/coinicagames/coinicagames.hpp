#include <map>
#include <string>
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>
#include "config.hpp"

using namespace std;
using namespace eosio;

class [[eosio::contract("coinicagames")]] coinicagames : public eosio::contract
{
private:
    const symbol coinica_symbol;
    const name eosio_token;

    struct thtile
    {
        uint8_t key; // panel index(panel_idx)
        uint8_t isopen = TH_IS_OPEN_PANEL;
        uint8_t iswin = TH_IS_WIN_PANEL;
    };

        struct base_stat
    {   
        string ghost_name;
        int ghost_id;
        int rarity;
        int hp;
        int base_atk;
        int base_def;
        int base_spd;
        int base_lck;
    };



        const vector<base_stat> stat_deck = {
        {"KoTengu", 1, 1, 1, 1, 1, 2, 1},
        {"FoxGhost", 2, 1, 1, 2, 1, 1, 1},
        {"KoKappa", 3, 1, 1, 2, 1, 1, 1},
        {"MameDanuki", 4, 1, 2, 1, 2, 1, 1},      
        {"Will-o'-Wisp", 5, 1, 1, 1, 2, 1, 2},      
        {"KoHannya", 6, 1, 1, 1, 2, 2, 1},      
        {"Tsutinoko", 7, 1, 1, 2, 1, 1, 2},      
        {"KuroNeko", 8, 1, 1, 2, 1, 1, 3},      
        {"ChickMon", 9, 1, 1, 1, 1, 3, 2},      
        {"KoOni", 10, 1, 2, 3, 1, 1, 1},     
        {"Lantern", 11, 1, 3, 1, 1, 1, 1},    
        {"KameMaru", 12, 1, 2, 1, 3, 1, 1},
        {"ChickMon", 9, 1, 1, 1, 1, 3, 2},      
        {"KoOni", 10, 1, 2, 3, 1, 1, 1},     
        {"Lantern", 11, 1, 3, 1, 1, 1, 1},    
        {"KameMaru", 12, 1, 2, 1, 3, 1, 1},    
        {"PumpkinCat", 13, 2, 2, 2, 2, 2, 1}, 
        {"Kapuso", 14, 2, 3, 1, 2, 1, 1},    
        {"FoxCub", 15, 2, 1, 1, 2, 2, 2},    
        {"LivingStatue", 16, 2, 1, 3, 3, 1, 1},    
        {"Kuchisake", 17, 2, 1, 2, 1, 3, 1},    
        {"SwordPig", 18, 2, 1, 3, 1, 3, 1},    
        {"GhostCat", 19, 2, 1, 2, 1, 2, 2},    
        {"Ungaikyo", 20, 2, 2, 2, 3, 1, 2},
        {"KageWani", 21, 2, 5, 1, 1, 1, 1},
        {"Hanako", 22, 2, 1, 1, 3, 1, 3},
        {"Sunakake", 23, 2, 2, 2, 2, 1, 2},
        {"FaceDog", 24, 2, 1, 2, 2, 3, 2},
        {"Taichi", 25, 2, 3, 3, 1, 2, 2},
        {"Bakejizo", 26, 2, 4, 2, 3, 1, 1},
        {"DarkFlame", 27, 2, 2, 2, 3, 2, 3},
        {"OTengu", 28, 2, 2, 2, 2, 4, 2},
        {"OKappa", 29, 2, 3, 3, 2, 2, 2},
        {"BakeDanuki", 30, 2, 3, 2, 4, 1, 2},
        {"OHannya", 31, 2, 2, 2, 3, 2, 2},
        {"WhiteFox", 32, 2, 3, 3, 1, 3, 2},
        {"OOni", 33, 2, 4, 5, 1, 1, 2},
        {"BirdieMon", 34, 2, 2, 2, 1, 5, 3},
        {"BakeNeko", 35, 2, 2, 3, 2, 2, 4},
        {"KameGon", 36, 2, 5, 1, 4, 1, 1},
        {"BigLantern", 37, 2, 3, 2, 2, 2, 2},
        {"TsutinoBushi", 38, 2, 2, 3, 2, 3, 4},
        {"PumpkinCat", 13, 2, 2, 2, 2, 2, 1},    
        {"Kapuso", 14, 2, 3, 1, 2, 1, 1},    
        {"FoxCub", 15, 2, 1, 1, 2, 2, 2},    
        {"LivingStatue", 16, 2, 1, 3, 3, 1, 1},    
        {"Kuchisake", 17, 2, 1, 2, 1, 3, 1},    
        {"SwordPig", 18, 2, 1, 3, 1, 3, 1},    
        {"GhostCat", 19, 2, 1, 2, 1, 2, 2},    
        {"Ungaikyo", 20, 2, 2, 2, 3, 1, 2},
        {"KageWani", 21, 2, 5, 1, 1, 1, 1},
        {"Hanako", 22, 2, 1, 1, 3, 1, 3},
        {"Sunakake", 23, 2, 2, 2, 2, 1, 2},
        {"FaceDog", 24, 2, 1, 2, 2, 3, 2},
        {"Taichi", 25, 2, 3, 3, 1, 2, 2},
        {"Bakejizo", 26, 2, 4, 2, 3, 1, 1},
        {"DarkFlame", 27, 2, 2, 2, 3, 2, 3},
        {"OTengu", 28, 2, 2, 2, 2, 4, 2},
        {"OKappa", 29, 2, 3, 3, 2, 2, 2},
        {"BakeDanuki", 30, 2, 3, 2, 4, 1, 2},
        {"OHannya", 31, 2, 2, 2, 3, 2, 2},
        {"WhiteFox", 32, 2, 3, 3, 1, 3, 2},
        {"OOni", 33, 2, 4, 5, 1, 1, 2},
        {"BirdieMon", 34, 2, 2, 2, 1, 5, 3},
        {"BakeNeko", 35, 2, 2, 3, 2, 2, 4},
        {"KameGon", 36, 2, 5, 1, 4, 1, 1},
        {"BigLantern", 37, 2, 3, 2, 2, 2, 2},
        {"TsutinoBushi", 38, 2, 2, 3, 2, 3, 4},
        {"Oiwa", 39, 3, 3, 3, 3, 3, 2},
        {"Iyaya", 40, 3, 3, 3, 3, 2, 3},
        {"BackOni", 41, 3, 2, 2, 3, 3, 3},
        {"FrontOni", 42, 3, 2, 3, 2, 3, 3},
        {"Icicle", 43, 3, 4, 2, 3, 2, 2},
        {"BoneGirl", 44, 3, 2, 3, 3, 4, 2},
        {"Medochi", 45, 3, 3, 4, 3, 1, 3},
        {"SnowGrampa", 46, 3, 2, 3, 2, 4, 4},
        {"TobiTengu", 47, 3, 4, 3, 1, 2, 2},
        {"Fur", 48, 3, 5, 2, 2, 3, 2},
        {"Meido", 49, 3, 1, 4, 5, 4, 2},
        {"Ibaraki", 50, 3, 2, 5, 2, 2, 4},
        {"Amefurashi", 51, 3, 4, 5, 5, 1, 1},
        {"Okuriinu", 52, 3, 3, 3, 3, 5, 3},
        {"Neneko", 53, 3, 1, 4, 3, 3, 4},
        {"OniBlue", 54, 3, 3, 5, 2, 3, 3},
        {"FireFox", 55, 3, 3, 3, 3, 4, 2},
        {"OniBear", 56, 3, 4, 5, 2, 1, 3},
        {"Twins", 57, 3, 5, 3, 4, 2, 2},
        {"KuramaTengu", 58, 3, 1, 4, 4, 3, 3},
        {"Sagojo", 59, 3, 3, 3, 3, 3, 3},
        {"OkikuFly", 60, 3, 2, 3, 2, 5, 5},
        {"KameDon", 61, 3, 5, 3, 5, 1, 3},    
        {"WaterFox", 62, 3, 4, 3, 4, 3, 4},    
        {"OniGoblin", 63, 3, 4, 5, 2, 1, 5},    
        {"NekoMata", 64, 3, 3, 3, 2, 3, 5},    
        {"DarkHellfire", 65, 3, 3, 3, 4, 4, 4},    
        {"Medotsu", 66, 3, 4, 4, 3, 3, 3},    
        {"Tenko", 67, 3, 4, 3, 4, 4, 4},    
        {"BirdMon", 68, 3, 3, 3, 2, 5, 4},    
        {"BossHannya", 69, 3, 3, 3, 4, 4, 3},    
        {"FullDanuki", 70, 3, 4, 3, 5, 2, 3},
        {"Oiwa", 39, 3, 3, 3, 3, 3, 2},
        {"Iyaya", 40, 3, 3, 3, 3, 2, 3},
        {"BackOni", 41, 3, 2, 2, 3, 3, 3},
        {"FrontOni", 42, 3, 2, 3, 2, 3, 3},
        {"Icicle", 43, 3, 4, 2, 3, 2, 2},
        {"BoneGirl", 44, 3, 2, 3, 3, 4, 2},
        {"Medochi", 45, 3, 3, 4, 3, 1, 3},
        {"SnowGrampa", 46, 3, 2, 3, 2, 4, 4},
        {"TobiTengu", 47, 3, 4, 3, 1, 2, 2},
        {"Fur", 48, 3, 5, 2, 2, 3, 2},
        {"Meido", 49, 3, 1, 4, 5, 4, 2},
        {"Ibaraki", 50, 3, 2, 5, 2, 2, 4},
        {"Amefurashi", 51, 3, 4, 5, 5, 1, 1},
        {"Okuriinu", 52, 3, 3, 3, 3, 5, 3},
        {"Neneko", 53, 3, 1, 4, 3, 3, 4},
        {"OniBlue", 54, 3, 3, 5, 2, 3, 3},
        {"FireFox", 55, 3, 3, 3, 3, 4, 2},
        {"OniBear", 56, 3, 4, 5, 2, 1, 3},
        {"Twins", 57, 3, 5, 3, 4, 2, 2},
        {"KuramaTengu", 58, 3, 1, 4, 4, 3, 3},
        {"Sagojo", 59, 3, 3, 3, 3, 3, 3},
        {"OkikuFly", 60, 3, 2, 3, 2, 5, 5},
        {"KameDon", 61, 3, 5, 3, 5, 1, 3},    
        {"WaterFox", 62, 3, 4, 3, 4, 3, 4},    
        {"OniGoblin", 63, 3, 4, 5, 2, 1, 5},    
        {"NekoMata", 64, 3, 3, 3, 2, 3, 5},    
        {"DarkHellfire", 65, 3, 3, 3, 4, 4, 4},    
        {"Medotsu", 66, 3, 4, 4, 3, 3, 3},    
        {"Tenko", 67, 3, 4, 3, 4, 4, 4},    
        {"BirdMon", 68, 3, 3, 3, 2, 5, 4},    
        {"BossHannya", 69, 3, 3, 3, 4, 4, 3},    
        {"FullDanuki", 70, 3, 4, 3, 5, 2, 3},    
        {"OboroCar", 71, 4, 5, 5, 3, 4, 1},    
        {"Yaroka", 72, 4, 5, 5, 5, 1, 1},    
        {"Dryad", 73, 4, 5, 5, 4, 1, 2},   
        {"FariyDog", 74, 4, 3, 4, 2, 3, 5},    
        {"Nurikabe", 75, 4, 5, 5, 5, 1, 1},    
        {"Zashikiwarashi", 76, 4, 3, 4, 4, 3, 4},    
        {"KageOnna", 77, 4, 2, 4, 5, 4, 4},    
        {"LanternKid", 78, 4, 5, 3, 3, 4, 3},    
        {"Rikugo", 79, 4, 4, 5, 3, 5, 3},    
        {"Geisya", 80, 4, 3, 5, 3, 4, 5},    
        {"Maneki", 81, 4, 4, 4, 3, 4, 5},    
        {"OniRed", 82, 4, 5, 5, 2, 1, 5}, 
        {"FariyDog", 74, 4, 3, 4, 2, 3, 5},    
        {"Nurikabe", 75, 4, 5, 5, 5, 1, 1},    
        {"Zashikiwarashi", 76, 4, 3, 4, 4, 3, 4},    
        {"KageOnna", 77, 4, 2, 4, 5, 4, 4},    
        {"LanternKid", 78, 4, 5, 3, 3, 4, 3},    
        {"Rikugo", 79, 4, 4, 5, 3, 5, 3},    
        {"Geisya", 80, 4, 3, 5, 3, 4, 5},    
        {"Maneki", 81, 4, 4, 4, 3, 4, 5},    
        {"OniRed", 82, 4, 5, 5, 2, 1, 5},    
        {"OniThunder", 83, 4, 4, 5, 4, 4, 3},    
        {"Daidara", 84, 4, 5, 5, 5, 1, 4},    
        {"UmiBozu", 85, 4, 5, 4, 5, 1, 5},    
        {"QueenTengu", 86, 4, 4, 5, 5, 5, 2},    
        {"Shiranui", 87, 4, 4, 5, 5, 5, 2},    
        {"CatGirl", 88, 4, 4, 4, 4, 5, 5},    
        {"Kijyo", 89, 4, 3, 5, 4, 4, 5},    
        {"Gyuki", 90, 4, 5, 5, 4, 5, 3},    
        {"SoteiO", 91, 4, 4, 5, 5, 4, 4},    
        {"DarkFox", 92, 4, 3, 5, 5, 5, 4},    
        {"GoldenFox", 93, 4, 5, 5, 4, 5, 3},    
        {"Rasetsu", 94, 4, 3, 5, 5, 4, 5},    
        {"Ogumo", 95, 4, 5, 5, 5, 4, 4},    
        {"Syuten", 96, 4, 4, 5, 5, 4, 4},
        {"Nurari", 97, 5, 4, 5, 5, 4, 5},  
        {"IceQueen", 98, 5, 5, 5, 5, 5, 5},  
        {"Rokuro", 99, 5, 5, 5, 5, 4, 5},  
        {"Gurdian", 100, 5, 5, 5, 5, 5, 4}, 
        {"Orochi", 101, 5, 5, 5, 5, 4, 5},
        {"Kamaitachi", 102, 5, 5, 5, 4, 5, 5},    
        {"Nurari", 97, 5, 4, 5, 5, 4, 5},    
        {"IceQueen", 98, 5, 5, 5, 5, 5, 5},    
        {"Rokuro", 99, 5, 5, 5, 5, 4, 5},    
        {"Gurdian", 100, 5, 5, 5, 5, 5, 4},
        {"Orochi", 101, 5, 5, 5, 5, 4, 5},  
        {"Kamaitachi", 102, 5, 5, 5, 4, 5, 5},  
        {"Raijyu", 103, 5, 5, 5, 5, 5, 5},  
        {"Nureonnna", 104, 5, 5, 5, 5, 5, 5},  
        {"Yasha", 105, 5, 5, 5, 5, 5, 5},  
        {"Jorogumo", 106, 5, 5, 5, 5, 5, 5},  
        {"Ubume", 107, 5, 5, 5, 5, 5, 5},  
        {"Ennma", 108, 5, 5, 5, 5, 5, 5}
    };


    
    enum static_values : int64_t
    {
        GQ_DEFAULT = 0, // --> for ghostquest
        LIFE_DEFAULT = 0, // --> for ghostquest
        HP_DEFAULT = 0, // --> for ghostquest
        STAT_DEFAULT = 0, // --> for ghostquest
        BL_DEFAULT = 0, // --> for ghostquest
        LVL_DEFAULT = 0, // --> for ghostquest
        LMT_DEFAULT = 0, // --> for ghostquest
        CLASS_DEFAULT = 0, // --> for ghostquest
        GQ_SUMMONED = 1, // --> for ghostquest
        GQ_STANDBY = 2, // --> for ghostquest
        GQ_INBATTLE = 3, // --> for ghostquest
        GQ_WINNER = 4, // --> for ghostquest
        GQ_LOSER = 5, // --> for ghostquest
        GQ_ELIMINATED = 6, // --> for ghostquest
        GQ_IDLE = 7, // --> for ghostquest
        GQ_INITIALIZED = 0, // --> for ghostquest
        GQ_ONGOING = 1, // --> for ghostquest
        GQ_DONE = 2, // --> for ghostquest
        TH_DEFAULT = 0,
        TH_PANEL_SIZE = 16,
        TH_IS_OPEN_PANEL = 0,
        TH_IS_WIN_PANEL = 0,
        TH_PRIZE_DEFAULT = 0,
        TH_ENEMY_DEFAULT = 0,
        TH_INITIALIZED = 0,
        TH_ONGOING = 1,
        TH_DONE = 2,
        TH_UNOPENED = 0, // for checking
        TH_MAP_DEFAULT = 0
    };


    struct thgame
    {
        string game_id;
        vector<thtile> panel_set;
        uint8_t unopentile = TH_PANEL_SIZE;
        uint8_t win_count = TH_DEFAULT;
        uint8_t destination = TH_MAP_DEFAULT;
        uint8_t status = TH_INITIALIZED;
        uint8_t enemy_count = TH_ENEMY_DEFAULT;
        double prize = TH_DEFAULT;
        double odds = TH_DEFAULT;
        double nextprize = TH_DEFAULT;
        double maxprize = TH_DEFAULT;
    };

    struct character
    {
        int owner_id;
        string ghost_name;
        int ghost_id;
        int rarity;
        uint64_t character_life = LIFE_DEFAULT;
        // int initial_hp = HP_DEFAULT;
        int hitpoints = HP_DEFAULT;
        uint64_t created_at;
        // uint64_t ghost_class = CLASS_DEFAULT;
        // uint64_t ghost_level = LVL_DEFAULT;
        uint64_t status = GQ_DEFAULT;
        int attack = STAT_DEFAULT;
        int defense = STAT_DEFAULT;
        int speed = STAT_DEFAULT;
        int luck = STAT_DEFAULT;
        double prize = GQ_DEFAULT;
        int battle_limit = LMT_DEFAULT;
        uint64_t battle_count = GQ_DEFAULT;
        uint64_t last_match = GQ_DEFAULT;
        map<uint64_t, string> enemy_fought;
        // std::chrono::time_point<std::chrono::steady_clock> last_battle;
        // vector<fight_log> battle_log;

        // auto primary_key() const
        // {
        //     return key;
        // }
    };

    struct gqgame
    {
        map<string, character> characters;
        // uint8_t status = INITIALIZED;
    };

    struct [[eosio::table]] ghostquest
    {
        int id;
        // uint64_t game_id;
        gqgame gqgame_data;

        auto primary_key() const
        {
            return id;
        };
    };

    struct [[eosio::table]] thunt
    {
        // string user_id;
        int id;
        thgame thgame_data;

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

    // using mahjonghilo_table = eosio::multi_index<"mahjonghilos"_n, mahjonghilo>;
    using ghostquests_table = eosio::multi_index<"ghostquests"_n, ghostquest>;
    using thunts_table = eosio::multi_index<"thunts"_n, thunt>;
    using seeds_table = eosio::multi_index<"seeds"_n, seed>;

    // mahjonghilo_table _mahjonghilos;
    ghostquests_table _ghostquests;
    thunts_table _thunts;
    seeds_table _seeds;

    int rng(const int &range);
    string checksum256_to_string_hash();
    void calculate_prize(map<string, character>::iterator & character);
    void eliminated_withdrawn(map<string, character>::iterator & character);
    double calculateodds(thgame thgamedata);
    double generateprize(thgame thgame_data);
    double maxprize(thgame thgamedata);
    // void gameready(string user_id, double quantity);
    // void onsettledpay(name to, asset quantity, string memo);
    void showremainingtile(thgame &thgame_data);
    void gameupdate(thgame &thgame_data);
    

public:
    using contract::contract;

    coinicagames(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                         eosio_token(MAIN_CONTRACT),
                                                                         coinica_symbol(MAIN_TOKEN, PRECISION),
                                                                         _ghostquests(receiver, receiver.value),
                                                                         _thunts(receiver, receiver.value),
                                                                         _seeds(receiver, receiver.value) {}
    ACTION gqinitialize(int id);
    ACTION gqbttlrslt(string gameid, pair<string, int> winner, pair<string, int> loser); // battle action
    ACTION gqwithdraw(int id, string key);
    ACTION gqgenchar(int id, double quantity, int limit);
    ACTION gqaddlife(int id, double quantity, string key);
    ACTION gqeliminate(int id, string key);
    ACTION gqend(int id);
    ACTION gqdelall(int size);
    ACTION thinitialize(int id);
    ACTION thsetpanel(int id, vector<uint8_t> panelset);
    ACTION thdstination(int id, uint8_t destination);
    ACTION thsetenemy(int id, uint8_t enemy_count);
    ACTION thgamestart(int id, double quantity);
    ACTION thopentile(int id, uint8_t index);
    ACTION thend(int id);
    ACTION thwithdraw(int id);
    ACTION thautoplay(int id, vector<uint8_t> panelset);
    ACTION thdel(int size);
};
