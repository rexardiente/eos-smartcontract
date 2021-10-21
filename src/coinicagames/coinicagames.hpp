#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include "config.hpp"

using namespace std;
using namespace eosio;

class [[eosio::contract("coinicagames")]] coinicagames : public eosio::contract
{
private:
    const symbol coinica_symbol;
    const name eosio_token;

    enum static_values : int64_t
    {
        DEFAULT = 0,
        GQ_SUMMONED = 1,
        GQ_STANDBY = 2,
        GQ_INBATTLE = 3,
        GQ_WINNER = 4,
        GQ_LOSER = 5,
        GQ_ELIMINATED = 6,
        GQ_IDLE = 7,
        GQ_ONGOING = 1,
        GQ_DONE = 2,
        TH_PANEL_SIZE = 16,
        TH_ONGOING = 1,
        TH_DONE = 2,
        MHL_INITIALIZED = 0, // for mahjong game
        MHL_ONGOING = 1,     // for mahjong game
        MHL_WIN = 2,         // for mahjong game
        MHL_LOSE = 3,        // for mahjong game
        MHL_ONTRIAL = 5,     // for mahjong game
        //SKIP = 0,        //for Hi-Lo
        MHL_LOW = 1,  //for Hi-Lo
        MHL_DRAW = 2, //for Hi-Lo
        MHL_HIGH = 3,  //for Hi-Lo --> for mahjong
        MHL_ISRIICHI = 1,
        MHL_RIICHILOCK = 2
    };

    enum tile_suit : int64_t
    {
        EMPTY = 0,
        CHARACTER = 1,
        DOT = 2,
        BAMBOO = 3,
        EAST = 4,
        SOUTH = 5,
        WEST = 6,
        NORTH = 7,
        RED = 8,
        WHITE = 9,
        GREEN = 10
    };

    struct thtile
    {
        uint8_t key;
        uint8_t isopen = DEFAULT;
        uint8_t iswin = DEFAULT;
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

    struct mhltile
    {
        int suit;
        int value;
    };

    struct mhlscore
    {
        string score_name;
        int value;
    };

    struct winnables
    {
        int index;
        vector<mhltile> tileswin;
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

    const vector<mhlscore> score_deck = {
        {"No Score", 0},
        {"Big Four Winds", 26},            
        {"Big Three Dragons", 13},         
        {"Four Kongs", 13},                
        {"All Green", 13},                 
        {"Nine Gates", 13},                
        {"Thirteen Orphans", 13},          
        {"Little Four Winds", 13},         
        {"All Honors", 13},               
        {"Four Concealed Pungs", 13},     
        {"All Terminals", 13},
        {"Full Flush", 6},
        {"All terminal and honor discards", 5},
        {"Two Pure Double Chow", 3},
        {"Pure Outside Hand", 3},
        {"Half Flush", 2},
        {"Three Kongs", 2},
        {"Little Three Dragons", 2},
        {"Outside Hand", 2},
        {"Pure Straight", 2},
        {"All Terminals and Honors", 2},
        {"Mixed Triple Chow", 2},
        {"Triple Pung", 2},
        {"Three Concealed Pungs", 2},
        {"All Pungs", 2},
        {"Prevail and Seat Wind", 2},
        {"Seven Pairs", 2},
        {"Double Reach", 2},
        {"Last time Draw", 1},
        {"Out with Replace- ment Tile", 1},
        {"Dragon Pung", 1},
        {"Seat Wind", 1},
        {"Prevail Wind", 1},
        {"Pure Double Chow", 1},
        {"All Chow", 1},
        {"All simples", 1},
        {"One-shot Win", 1},
        {"Reach", 1},
        {"Self drawn", 1}
    };

    const vector<mhltile> table_deck = {
        {0, 0},
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 2},
        {1, 2},
        {1, 2},
        {1, 2},
        {1, 3},
        {1, 3},
        {1, 3},
        {1, 3},
        {1, 4},
        {1, 4},
        {1, 4},
        {1, 4},
        {1, 5},
        {1, 5},
        {1, 5},
        {1, 5},
        {1, 6},
        {1, 6},
        {1, 6},
        {1, 6},
        {1, 7},
        {1, 7},
        {1, 7},
        {1, 7},
        {1, 8},
        {1, 8},
        {1, 8},
        {1, 8},
        {1, 9},
        {1, 9},
        {1, 9},
        {1, 9},
        {2, 1},
        {2, 1},
        {2, 1},
        {2, 1},
        {2, 2},
        {2, 2},
        {2, 2},
        {2, 2},
        {2, 3},
        {2, 3},
        {2, 3},
        {2, 3},
        {2, 4},
        {2, 4},
        {2, 4},
        {2, 4},
        {2, 5},
        {2, 5},
        {2, 5},
        {2, 5},
        {2, 6},
        {2, 6},
        {2, 6},
        {2, 6},
        {2, 7},
        {2, 7},
        {2, 7},
        {2, 7},
        {2, 8},
        {2, 8},
        {2, 8},
        {2, 8},
        {2, 9},
        {2, 9},
        {2, 9},
        {2, 9},
        {3, 1},
        {3, 1},
        {3, 1},
        {3, 1},
        {3, 2},
        {3, 2},
        {3, 2},
        {3, 2},
        {3, 3},
        {3, 3},
        {3, 3},
        {3, 3},
        {3, 4},
        {3, 4},
        {3, 4},
        {3, 4},
        {3, 5},
        {3, 5},
        {3, 5},
        {3, 5},
        {3, 6},
        {3, 6},
        {3, 6},
        {3, 6},
        {3, 7},
        {3, 7},
        {3, 7},
        {3, 7},
        {3, 8},
        {3, 8},
        {3, 8},
        {3, 8},
        {3, 9},
        {3, 9},
        {3, 9},
        {3, 9},
        {4, 10},
        {4, 10},
        {4, 10},
        {4, 10},
        {5, 10},
        {5, 10},
        {5, 10},
        {5, 10},
        {6, 10},
        {6, 10},
        {6, 10},
        {6, 10},
        {7, 10},
        {7, 10},
        {7, 10},
        {7, 10},
        {8, 11},
        {8, 11},
        {8, 11},
        {8, 11},
        {9, 11},
        {9, 11},
        {9, 11},
        {9, 11},
        {10, 11},
        {10, 11},
        {10, 11},
        {10, 11}
    };

    struct thgamedata
    {
        string game_id;
        vector<thtile> panel_set;
        uint8_t unopentile = TH_PANEL_SIZE;
        uint8_t win_count = DEFAULT;
        uint8_t destination = DEFAULT;
        uint8_t status = DEFAULT;
        uint8_t enemy_count = DEFAULT;
        double prize = DEFAULT;
        double odds = DEFAULT;
        double nextprize = DEFAULT;
        double maxprize = DEFAULT;
    };

    struct character
    {
        int owner_id;
        string ghost_name;
        int ghost_id;
        int rarity;
        uint64_t character_life = DEFAULT;
        int hitpoints = DEFAULT;
        uint64_t created_at;
        uint64_t status = DEFAULT;
        int attack = DEFAULT;
        int defense = DEFAULT;
        int speed = DEFAULT;
        int luck = DEFAULT;
        double prize = DEFAULT;
        int battle_limit = DEFAULT;
        uint64_t battle_count = DEFAULT;
        uint64_t last_match = DEFAULT;
        map<uint64_t, string> enemy_fought;
    };

    struct gqgamedata
    {
        map<string, character> characters;
    };

    struct mhlgamedata
    {
        string game_id;
        vector<int> deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
        int status = DEFAULT;
        double hi_lo_balance = DEFAULT;
        int prediction = DEFAULT;
        int hi_lo_result = DEFAULT;
        int hi_lo_outcome = DEFAULT;
        // float hi_lo_prize = DEFAULT;
        float hi_lo_bet = DEFAULT;
        float hi_lo_stake = DEFAULT;
        double low_odds = DEFAULT;
        double draw_odds = DEFAULT;
        double high_odds = DEFAULT;
        int bet_status = DEFAULT;
        int option_status = DEFAULT;
        int riichi_status = DEFAULT;
        vector<int> sumofvalue = {12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 12};
        int prevalent_wind;
        int seat_wind;
        int current_tile;
        int standard_tile;
        int eye_idx;
        // int winnable = DEFAULT;
        // int highest;
        // int suit_count;
        // int type_count;
        int pair_count;
        int pung_count;
        int chow_count;
        int kong_count;
        int draw_count;
        vector<uint8_t> hand_player = {};
        vector<uint8_t> discarded_tiles = {};
        vector<uint8_t> reveal_kong = {};
        vector<uint8_t> winning_hand = {};
        vector<uint8_t> score_check = {};
        vector<mhlscore> score_type = {};
        vector<winnables> wintiles = {};
        // map<uint8_t, vector<mhltile> > wintiles = {};
        int final_score;
    };

    struct [[eosio::table]] ghostquest
    {
        int id;
        gqgamedata game_data;
        auto primary_key() const
        {
            return id;
        };
    };

    struct [[eosio::table]] thunt
    {
        // string user_id;
        int id;
        thgamedata game_data;
        auto primary_key() const
        {
            return id;
        };
    };

    struct [[eosio::table]] mjhilo
    {
        int id;
        int game_count;
        mhlgamedata game_data;
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

    using ghostquests_table = eosio::multi_index<"ghostquests"_n, ghostquest>;
    using thunts_table = eosio::multi_index<"thunts"_n, thunt>;
    using mjhilos_table = eosio::multi_index<"mjhilos"_n, mjhilo>;
    using seeds_table = eosio::multi_index<"seeds"_n, seed>;
    
    ghostquests_table _ghostquests;
    thunts_table _thunts;
    mjhilos_table _mjhilos;
    seeds_table _seeds;

    int random(const int range);
    string checksum256_to_string_hash();
    void calculate_prize(map<string, character>::iterator & character);
    void eliminated_withdrawn(map<string, character>::iterator & character);
    double calculateodds(thgamedata game_data);
    double generateprize(thgamedata game_data);
    double maxprize(thgamedata game_data);
    void showremainingtile(thgamedata &game_data);
    void gameupdate(thgamedata &game_data);
    double roundoff(double var);
    void gettile(mhlgamedata & gamedata);
    void sorthand(vector<uint8_t> & hand);
    void sorteye(vector<uint8_t> & hand, int idx);
    void getscore(mhlgamedata & gamedata, vector<uint8_t> & hand);
    // void sumscore(mhlgamedata & gamedata);
    // void two_rem(mhlgamedata & gamedata, vector<mhltile> tiles);
    // void five_rem(mhlgamedata & gamedata, vector<mhltile> tiles);
    // void eight_rem(mhlgamedata & gamedata, vector<mhltile> tiles);
    // void eleven_rem(mhlgamedata & gamedata, vector<mhltile> tiles);
    // void fourteen_rem(mhlgamedata & gamedata, vector<mhltile> tiles);
    // void winhand_check(mhlgamedata & gamedata, vector<uint8_t> & hand);
    void transferhand(mhlgamedata & gamedata, int size);
    void pung_chow(mhlgamedata & gamedata, int check);
    void get_odds(mhlgamedata & gamedata, int value);
    float hilo_step(mhlgamedata & gamedata, int prev_tile, int current_tile);
    int meld_check(mhltile tile1, mhltile tile2, mhltile tile3);
    int pair_check(mhltile tile1, mhltile tile2);
    void thirteen_check(mhlgamedata & gamedata, vector<mhltile> tiles, uint8_t idx);
    int five_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5);
    void tile_insert(mhlgamedata & gamedata, vector<mhltile> tiles, uint8_t idx);
    void riichi_check(mhlgamedata & gamedata, vector<uint8_t> hand);
    // int pung_check(mhltile tile1, mhltile tile2, mhltile tile3);
    // int wind_check(mhlgamedata gamedata, mhltile tile1, int check1);
    // int five_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5);
    // int six_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5, mhltile tile6);
    // int honors_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5, mhltile tile6, mhltile tile7);

public:
    using contract::contract;

    coinicagames(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                         eosio_token(MAIN_CONTRACT),
                                                                         coinica_symbol(MAIN_TOKEN, PRECISION),
                                                                         _ghostquests(receiver, receiver.value),
                                                                         _thunts(receiver, receiver.value),
                                                                         _mjhilos(receiver, receiver.value),
                                                                         _seeds(receiver, receiver.value) {}
    ACTION gqinitialize(int id);
    ACTION gqbttlrslt(string gameid, pair<string, int> winner, pair<string, int> loser); // battle action
    ACTION gqwithdraw(int id, string key);
    ACTION gqgenchar(int id, double quantity, int limit);
    ACTION gqaddlife(int id, double quantity, string key);
    ACTION gqeliminate(int id, string key);
    ACTION gqend(int id);
    ACTION gqdelall(int size);
    ACTION thinitialize(int id, uint8_t destination, uint8_t enemy_count, vector<uint8_t> panels, double quantity);
    ACTION thopentile(int id, uint8_t index);
    ACTION thend(int id);
    ACTION thwithdraw(int id);
    ACTION thautoplay(int id, vector<uint8_t> panelset);
    ACTION mhlinitialze(int id);
    ACTION mhlresetbet(int id);
    ACTION mhladdbet(int id, double quantity);
    ACTION mhlplayhilo(int id, int option);
    ACTION mhldscrdtile(int id, int idx);
    ACTION mhlstartbet(int id);
    ACTION mhlwintrnsfr(int id);
    ACTION mhldclrkong(int id, vector<int> idx);
    ACTION mhldclrwnhnd(int id);
    ACTION mhlwithdraw(int id);
    // ACTION mhlendgame(int id);
    ACTION mhlrchilock(int id);
    ACTION mhlend(int id);
    // ACTION mhldel(int size);
};