

    // struct mhltile
    // {
    //     int suit;
    //     int value;
    // };

    // enum tile_suit : int64_t
    // {
    //     EMPTY = 0,
    //     CHARACTER = 1,
    //     DOT = 2,
    //     BAMBOO = 3,
    //     EAST = 4,
    //     SOUTH = 5,
    //     WEST = 6,
    //     NORTH = 7,
    //     RED = 8,
    //     WHITE = 9,
    //     GREEN = 10
    // };

    // const vector<mhlscore> score_deck = {
    //     {"No Score", 0},
    //     {"Seven Shift Pairs", 88},         
    //     {"Pure Terminal Chows", 64},
    //     {"Quadruple Chows", 48},         
    //     {"Four Pure Shifted Pungs", 48}, 
    //     {"Four Pure Shifted Chows", 32}, 
    //     {"Pure Shifted Pungs", 24},              
    //     {"All Even Pungs", 24},                  
    //     {"Seven Pairs", 24},                     
    //     {"Upper Tiles", 24},                     
    //     {"Middle Tiles", 24},                    
    //     {"Lower Tiles", 24},                     
    //     {"Greater Honors and Knitted Tiles", 24},
    //     {"Pure Shifted Chows", 16},              
    //     {"Pure Straight", 16},                   
    //     {"Three-suited Terminal Chows", 16},     
    //     {"Triple Pungs", 16},                    
    //     {"Three Concealed Pungs", 16},           
    //     {"Big Three Winds", 12},
    //     {"Upper Four", 12},
    //     {"Lower Four", 12},
    //     {"Lesser Honors and Knitted Tiles", 12},
    //     {"Mixed Triple Chow", 8},
    //     {"Mixed Straight", 8},
    //     {"Mixed Shifted Pungs", 8},
    //     {"Two Concealed Kongs", 8},
    //     {"Two Dragon Pungs", 6},
    //     {"Mixed Shifted Chows", 6},
    //     {"All Types", 6},
    //     {"Dragon Pung", 2},
    //     {"Prevalent Wind", 2},
    //     {"Seat Wind", 2},
    //     {"All Chows", 2},
    //     {"Two Concealed Pungs", 2},
    //     {"Concealed Kong", 2},
    //     {"Mixed Double Pungs", 2},
    //     {"All Simples", 2},
    //     {"Tile Hog", 2},
    //     {"Mixed Double Chows", 1},
    //     {"Short Straight", 1},
    //     {"Two Terminal Chows", 1},
    //     {"Pung of Terminals or Honour", 1},
    //     {"No Available Score, Have a Bonus", 5},
    //     {"One Voided Suit", 1},
    //     {"No Honors", 1}};

    //     const vector<mhltile> table_deck = {
    //     {0, 0},
    //     {1, 1},
    //     {1, 1},
    //     {1, 1},
    //     {1, 1},
    //     {1, 2},
    //     {1, 2},
    //     {1, 2},
    //     {1, 2},
    //     {1, 3},
    //     {1, 3},
    //     {1, 3},
    //     {1, 3},
    //     {1, 4},
    //     {1, 4},
    //     {1, 4},
    //     {1, 4},
    //     {1, 5},
    //     {1, 5},
    //     {1, 5},
    //     {1, 5},
    //     {1, 6},
    //     {1, 6},
    //     {1, 6},
    //     {1, 6},
    //     {1, 7},
    //     {1, 7},
    //     {1, 7},
    //     {1, 7},
    //     {1, 8},
    //     {1, 8},
    //     {1, 8},
    //     {1, 8},
    //     {1, 9},
    //     {1, 9},
    //     {1, 9},
    //     {1, 9},
    //     {2, 1},
    //     {2, 1},
    //     {2, 1},
    //     {2, 1},
    //     {2, 2},
    //     {2, 2},
    //     {2, 2},
    //     {2, 2},
    //     {2, 3},
    //     {2, 3},
    //     {2, 3},
    //     {2, 3},
    //     {2, 4},
    //     {2, 4},
    //     {2, 4},
    //     {2, 4},
    //     {2, 5},
    //     {2, 5},
    //     {2, 5},
    //     {2, 5},
    //     {2, 6},
    //     {2, 6},
    //     {2, 6},
    //     {2, 6},
    //     {2, 7},
    //     {2, 7},
    //     {2, 7},
    //     {2, 7},
    //     {2, 8},
    //     {2, 8},
    //     {2, 8},
    //     {2, 8},
    //     {2, 9},
    //     {2, 9},
    //     {2, 9},
    //     {2, 9},
    //     {3, 1},
    //     {3, 1},
    //     {3, 1},
    //     {3, 1},
    //     {3, 2},
    //     {3, 2},
    //     {3, 2},
    //     {3, 2},
    //     {3, 3},
    //     {3, 3},
    //     {3, 3},
    //     {3, 3},
    //     {3, 4},
    //     {3, 4},
    //     {3, 4},
    //     {3, 4},
    //     {3, 5},
    //     {3, 5},
    //     {3, 5},
    //     {3, 5},
    //     {3, 6},
    //     {3, 6},
    //     {3, 6},
    //     {3, 6},
    //     {3, 7},
    //     {3, 7},
    //     {3, 7},
    //     {3, 7},
    //     {3, 8},
    //     {3, 8},
    //     {3, 8},
    //     {3, 8},
    //     {3, 9},
    //     {3, 9},
    //     {3, 9},
    //     {3, 9},
    //     {4, 10},
    //     {4, 10},
    //     {4, 10},
    //     {4, 10},
    //     {5, 10},
    //     {5, 10},
    //     {5, 10},
    //     {5, 10},
    //     {6, 10},
    //     {6, 10},
    //     {6, 10},
    //     {6, 10},
    //     {7, 10},
    //     {7, 10},
    //     {7, 10},
    //     {7, 10},
    //     {8, 11},
    //     {8, 11},
    //     {8, 11},
    //     {8, 11},
    //     {9, 11},
    //     {9, 11},
    //     {9, 11},
    //     {9, 11},
    //     {10, 11},
    //     {10, 11},
    //     {10, 11},
    //     {10, 11}};

        // MHL_DEFAULT = 0,  // --> for mahjong
        // MHL_INITIALIZED = 0, // for mahjong game
        // MHL_ONGOING = 1,     // for mahjong game
        // MHL_WIN = 2,         // for mahjong game
        // MHL_LOSE = 3,        // for mahjong game
        // MHL_ONTRIAL = 5,     // for mahjong game
        // //SKIP = 0,        //for Hi-Lo
        // MHL_LOW = 1,  //for Hi-Lo
        // MHL_DRAW = 2, //for Hi-Lo
        // MHL_HIGH = 3,  //for Hi-Lo --> for mahjong

    //         struct mhlgame
    // {
    //     string game_id;
    //     vector<uint8_t> deck_player = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
    //     uint8_t status;
    //     double hi_lo_balance = MHL_DEFAULT;
    //     int prediction = MHL_DEFAULT;
    //     int hi_lo_result = MHL_DEFAULT;
    //     int hi_lo_outcome = MHL_DEFAULT;
    //     // float hi_lo_prize = MHL_DEFAULT;
    //     float hi_lo_bet = MHL_DEFAULT;
    //     float hi_lo_stake = MHL_DEFAULT;
    //     double low_odds = MHL_DEFAULT;
    //     double draw_odds = MHL_DEFAULT;
    //     double high_odds = MHL_DEFAULT;
    //     int bet_status = MHL_DEFAULT;
    //     int option_status = MHL_DEFAULT;
    //     vector<int> sumofvalue = {12, 12, 12, 12, 12, 12, 12, 12, 12, 16, 12};
    //     uint8_t prevalent_wind;
    //     uint8_t seat_wind;
    //     uint8_t current_tile;
    //     uint8_t standard_tile;
    //     int eye_idx;
    //     int winnable = MHL_DEFAULT;
    //     // int highest;
    //     // int suit_count;
    //     // int type_count;
    //     int pair_count;
    //     int pung_count;
    //     int chow_count;
    //     int kong_count;
    //     int draw_count;
    //     vector<uint8_t> hand_player = {};
    //     vector<uint8_t> discarded_tiles = {};
    //     vector<uint8_t> reveal_kong = {};
    //     vector<uint8_t> winning_hand = {};
    //     vector<uint8_t> score_check = {};
    //     vector<score> score_type = {};
    //     int final_score;
    // };

    //     struct [[eosio::table]] mahjonghilo
    // {
    //     // name username;
    //     int id;
    //     int game_count;
    //     mhlgame game_data;

    //     auto primary_key() const
    //     {
    //         return id;
    //     };
    //     EOSLIB_SERIALIZE(mahjonghilo,(id));
    // };

    // int rng(const int &range);
    // string checksum256_to_string(std::array<uint8_t, 32UL> arr, size_t size);
    // void depositbet(name to, asset quantity);
    // void onsettledpay(name to, asset quantity, string memo);
    // double roundoff(double var);
    // void gettile(mhlgame & gamedata);
    // void sorthand(vector<uint8_t> & hand);
    // void sorteye(vector<uint8_t> & hand, int idx);
    // void getscore(mhlgame & gamedata, vector<uint8_t> & hand);
    // void sumscore(mhlgame & gamedata);
    // void two_rem(mhlgame & gamedata, vector<mhltile> tiles);
    // void five_rem(mhlgame & gamedata, vector<mhltile> tiles);
    // void eight_rem(mhlgame & gamedata, vector<mhltile> tiles);
    // void eleven_rem(mhlgame & gamedata, vector<mhltile> tiles);
    // void fourteen_rem(mhlgame & gamedata, vector<mhltile> tiles);
    // void winhand_check(mhlgame & gamedata, vector<uint8_t> & hand);
    // void transferhand(mhlgame & gamedata, int size);
    // void pung_chow(mhlgame & gamedata, int check);
    // void get_odds(mhlgame & gamedata, int value);
    // float hilo_step(mhlgame & gamedata, int prev_tile, int current_tile);
    // int pair_pung_chow(mhltile tile1, mhltile tile2, mhltile tile3);
    // int pung_check(mhltile tile1, mhltile tile2, mhltile tile3);
    // int pair_check(mhltile tile1, mhltile tile2);
    // int wind_check(mhlgame gamedata, mhltile tile1, int check1);
    // int five_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5);
    // int six_tile_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5, mhltile tile6);
    // int honors_check(mhltile tile1, mhltile tile2, mhltile tile3, mhltile tile4, mhltile tile5, mhltile tile6, mhltile tile7);

    // ACTION mhlinitialze(int id);
    // ACTION mhlresetbet(int id);
    // ACTION mhladdbet(int id, double quantity);
    // ACTION mhlplayhilo(int id, int option);
    // ACTION mhldscrdtile(int id, int idx);
    // ACTION mhlstartbet(int id);
    // ACTION mhlwintrnsfr(int id);
    // ACTION mhldclrkong(int id, vector<int> idx);
    // ACTION mhldclrwnhnd(int id);
    // ACTION mhlwithdraw(int id);
    // ACTION mhlendgame(int id);
    // ACTION mhlend(int id);
    // ACTION mhldel(int size);