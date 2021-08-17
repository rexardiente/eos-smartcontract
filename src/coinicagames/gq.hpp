#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>
#include <eosio/transaction.hpp>
#include<string>
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

    // const map<uint8_t, base_stat> stat_deck = {
    //         {0, {"KoTengu", 1, 1, 1, 1, 1, 2, 1}},      
    //         {1, {"FoxGhost", 2, 1, 1, 2, 1, 1, 1}},      
    //         {2, {"KoKappa", 3, 1, 1, 2, 1, 1, 1}},      
    //         {3, {"MameDanuki", 4, 1, 2, 1, 2, 1, 1}},      
    //         {4, {"Will-o'-Wisp", 5, 1, 1, 1, 2, 1, 2}},      
    //         {5, {"KoHannya", 6, 1, 1, 1, 2, 2, 1}},      
    //         {6, {"Tsutinoko", 7, 1, 1, 2, 1, 1, 2}},      
    //         {7, {"KuroNeko", 8, 1, 1, 2, 1, 1, 3}},      
    //         {8, {"ChickMon", 9, 1, 1, 1, 1, 3, 2}},      
    //         {9, {"KoOni", 10, 1, 2, 3, 1, 1, 1}},     
    //         {10, {"Lantern", 11, 1, 3, 1, 1, 1, 1}},    
    //         {11, {"KameMaru", 12, 1, 2, 1, 3, 1, 1}},    
    //         {12, {"PumpkinCat", 13, 2, 2, 2, 2, 2, 1}},    
    //         {13, {"Kapuso", 14, 2, 3, 1, 2, 1, 1}},    
    //         {14, {"FoxCub", 15, 2, 1, 1, 2, 2, 2}},    
    //         {15, {"LivingStatue", 16, 2, 1, 3, 3, 1, 1}},    
    //         {16, {"Kuchisake", 17, 2, 1, 2, 1, 3, 1}},    
    //         {17, {"SwordPig", 18, 2, 1, 3, 1, 3, 1}},    
    //         {18, {"GhostCat", 19, 2, 1, 2, 1, 2, 2}},    
    //         {19, {"Ungaikyo", 20, 2, 2, 2, 3, 1, 2}},    
    //         {20, {"KageWani", 21, 2, 5, 1, 1, 1, 1}},    
    //         {21, {"Hanako", 22, 2, 1, 1, 3, 1, 3}},    
    //         {22, {"Sunakake", 23, 2, 2, 2, 2, 1, 2}},    
    //         {23, {"FaceDog", 24, 2, 1, 2, 2, 3, 2}},    
    //         {24, {"Taichi", 25, 2, 3, 3, 1, 2, 2}},    
    //         {25, {"Bakejizo", 26, 2, 4, 2, 3, 1, 1}},    
    //         {26, {"DarkFlame", 27, 2, 2, 2, 3, 2, 3}},    
    //         {27, {"OTengu", 28, 2, 2, 2, 2, 4, 2}},    
    //         {28, {"OKappa", 29, 2, 3, 3, 2, 2, 2}},    
    //         {29, {"BakeDanuki", 30, 2, 3, 2, 4, 1, 2}},    
    //         {30, {"OHannya", 31, 2, 2, 2, 3, 2, 2}},    
    //         {31, {"WhiteFox", 32, 2, 3, 3, 1, 3, 2}},    
    //         {32, {"OOni", 33, 2, 4, 5, 1, 1, 2}},    
    //         {33, {"BirdieMon", 34, 2, 2, 2, 1, 5, 3}},    
    //         {34, {"BakeNeko", 35, 2, 2, 3, 2, 2, 4}},    
    //         {35, {"KameGon", 36, 2, 5, 1, 4, 1, 1}},    
    //         {36, {"BigLantern", 37, 2, 3, 2, 2, 2, 2}},    
    //         {37, {"TsutinoBushi", 38, 2, 2, 3, 2, 3, 4}},    
    //         {38, {"Oiwa", 39, 3, 3, 3, 3, 3, 2}},    
    //         {39, {"Iyaya", 40, 3, 3, 3, 3, 2, 3}},    
    //         {40, {"BackOni", 41, 3, 2, 2, 3, 3, 3}},    
    //         {41, {"FrontOni", 42, 3, 2, 3, 2, 3, 3}},    
    //         {42, {"Icicle", 43, 3, 4, 2, 3, 2, 2}},    
    //         {43, {"BoneGirl", 44, 3, 2, 3, 3, 4, 2}},    
    //         {44, {"Medochi", 45, 3, 3, 4, 3, 1, 3}},    
    //         {45, {"SnowGrampa", 46, 3, 2, 3, 2, 4, 4}},    
    //         {46, {"TobiTengu", 47, 3, 4, 3, 1, 2, 2}},    
    //         {47, {"Fur", 48, 3, 5, 2, 2, 3, 2}},    
    //         {48, {"Meido", 49, 3, 1, 4, 5, 4, 2}},    
    //         {49, {"Ibaraki", 50, 3, 2, 5, 2, 2, 4}},    
    //         {50, {"Amefurashi", 51, 3, 4, 5, 5, 1, 1}},    
    //         {51, {"Okuriinu", 52, 3, 3, 3, 3, 5, 3}},    
    //         {52, {"Neneko", 53, 3, 1, 4, 3, 3, 4}},    
    //         {53, {"OniBlue", 54, 3, 3, 5, 2, 3, 3}},    
    //         {54, {"FireFox", 55, 3, 3, 3, 3, 4, 2}},    
    //         {55, {"OniBear", 56, 3, 4, 5, 2, 1, 3}},    
    //         {56, {"Twins", 57, 3, 5, 3, 4, 2, 2}},    
    //         {57, {"KuramaTengu", 58, 3, 1, 4, 4, 3, 3}},    
    //         {58, {"Sagojo", 59, 3, 3, 3, 3, 3, 3}},    
    //         {59, {"OkikuFly", 60, 3, 2, 3, 2, 5, 5}},    
    //         {60, {"KameDon", 61, 3, 5, 3, 5, 1, 3}},    
    //         {61, {"WaterFox", 62, 3, 4, 3, 4, 3, 4}},    
    //         {62, {"OniGoblin", 63, 3, 4, 5, 2, 1, 5}},    
    //         {63, {"NekoMata", 64, 3, 3, 3, 2, 3, 5}},    
    //         {64, {"DarkHellfire", 65, 3, 3, 3, 4, 4, 4}},    
    //         {65, {"Medotsu", 66, 3, 4, 4, 3, 3, 3}},    
    //         {66, {"Tenko", 67, 3, 4, 3, 4, 4, 4}},    
    //         {67, {"BirdMon", 68, 3, 3, 3, 2, 5, 4}},    
    //         {68, {"BossHannya", 69, 3, 3, 3, 4, 4, 3}},    
    //         {69, {"FullDanuki", 70, 3, 4, 3, 5, 2, 3}},    
    //         {70, {"OboroCar", 71, 4, 5, 5, 3, 4, 1}},    
    //         {71, {"Yaroka", 72, 4, 5, 5, 5, 1, 1}},    
    //         {72, {"Dryad", 73, 4, 5, 5, 4, 1, 2}},    
    //         {73, {"FariyDog", 74, 4, 3, 4, 2, 3, 5}},    
    //         {74, {"Nurikabe", 75, 4, 5, 5, 5, 1, 1}},    
    //         {75, {"Zashikiwarashi", 76, 4, 3, 4, 4, 3, 4}},    
    //         {76, {"KageOnna", 77, 4, 2, 4, 5, 4, 4}},    
    //         {77, {"LanternKid", 78, 4, 5, 3, 3, 4, 3}},    
    //         {78, {"Rikugo", 79, 4, 4, 5, 3, 5, 3}},    
    //         {79, {"Geisya", 80, 4, 3, 5, 3, 4, 5}},    
    //         {80, {"Maneki", 81, 4, 4, 4, 3, 4, 5}},    
    //         {81, {"OniRed", 82, 4, 5, 5, 2, 1, 5}},    
    //         {82, {"OniThunder", 83, 4, 4, 5, 4, 4, 3}},    
    //         {83, {"Daidara", 84, 4, 5, 5, 5, 1, 4}},    
    //         {84, {"UmiBozu", 85, 4, 5, 4, 5, 1, 5}},    
    //         {85, {"QueenTengu", 86, 4, 4, 5, 5, 5, 2}},    
    //         {86, {"Shiranui", 87, 4, 4, 5, 5, 5, 2}},    
    //         {87, {"CatGirl", 88, 4, 4, 4, 4, 5, 5}},    
    //         {88, {"Kijyo", 89, 4, 3, 5, 4, 4, 5}},    
    //         {89, {"Gyuki", 90, 4, 5, 5, 4, 5, 3}},    
    //         {90, {"SoteiO", 91, 4, 4, 5, 5, 4, 4}},    
    //         {91, {"DarkFox", 92, 4, 3, 5, 5, 5, 4}},    
    //         {92, {"GoldenFox", 93, 4, 5, 5, 4, 5, 3}},    
    //         {93, {"Rasetsu", 94, 4, 3, 5, 5, 4, 5}},    
    //         {94, {"Ogumo", 95, 4, 5, 5, 5, 4, 4}},    
    //         {95, {"Syuten", 96, 4, 4, 5, 5, 4, 4}},    
            // {96, {"Nurari", 97, 5, 4, 5, 5, 4, 5}},    
            // {97, {"IceQueen", 98, 5, 5, 5, 5, 5, 5}},    
            // {98, {"Rokuro", 99, 5, 5, 5, 5, 4, 5}},    
            // {99, {"Gurdian", 100, 5, 5, 5, 5, 5, 4}},   
            // {100, {"Orochi", 101, 5, 5, 5, 5, 4, 5}},  
            // {101, {"Kamaitachi", 102, 5, 5, 5, 4, 5, 5}},  
    //         {102, {"Raijyu", 103, 5, 5, 5, 5, 5, 5}},  
    //         {103, {"Nureonnna", 104, 5, 5, 5, 5, 5, 5}},  
    //         {104, {"Yasha", 105, 5, 5, 5, 5, 5, 5}},  
    //         {105, {"Jorogumo", 106, 5, 5, 5, 5, 5, 5}},  
    //         {106, {"Ubume", 107, 5, 5, 5, 5, 5, 5}},  
    //         {107, {"Ennma", 108, 5, 5, 5, 5, 5, 5}}};
    

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

    struct game
    {
        map<string, character> characters;
        // uint8_t status = INITIALIZED;
    };

    struct [[eosio::table]] ghostquest
    {
        int id;
        // uint64_t game_id;
        game game_data;

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

    using ghostquests_table = eosio::multi_index<"ghostquest"_n, ghostquest>;
    using seeds_table = eosio::multi_index<"seeds"_n, seed>;

    qhostquests_table _ghostquests;
    seeds_table _seeds;

    int rng(const int &range);
    string checksum256_to_string_hash();
    // void summon_ready(name username, asset quantity, int limit);
    // void onsettledpay(name to, asset quantity, string memo);
    // void gen_stat(ghost & initghost);
    // void battle_step(map<int, ghost>::iterator & ghost1, map<int, ghost>::iterator & ghost2);
    // void damage_step(map<int, ghost>::iterator & attacker, map<int, ghost>::iterator & defender, int round);
    // void result_step(map<int, ghost>::iterator & loser, map<int, ghost>::iterator & winner);
    // void set_add_life(name username, asset quantity, int key);
    void calculate_prize(map<string, character>::iterator & character);
    void eliminated_withdrawn(map<string, character>::iterator & character);

public:
    using contract::contract;

    ghostquest(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                        eosio_token(MAIN_CONTRACT),
                                                                        ghostquest_symbol(MAIN_TOKEN, PRECISION),
                                                                        _users(receiver, receiver.value),
                                                                        _seeds(receiver, receiver.value) {}

    // [[eosio::on_notify("eosio.token::transfer")]] void ondeposit(name from,
    //                                                              name to,
    //                                                              asset quantity,
    //                                                              string memo);
    ACTION gqinitialize(int id);
    ACTION gqbttlrslt(string gameid, pair<string, int> winner, pair<string, int> loser); // battle action
    ACTION gqwithdraw(int id, string key);
    ACTION gqgenchar(int id, double quantity, int limit);
    ACTION gqaddlife(int id, double quantity, string key);
    ACTION gqeliminate(int id, string key);
    ACTION gqend(int id);
    ACTION gqdelall(int size);
};