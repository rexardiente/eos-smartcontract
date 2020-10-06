### HPP

    #include <eosio/eosio.hpp>
    #include <vector>
    #include <algorithm>

    // #include <boost/version.hpp>
    // #include <boost/config.hpp>

    using namespace std;
    using namespace eosio;

    class [[eosio::contract("treasure.v2")]] treasurev2 : public contract {
    public:
        struct Tile
        {
            uint8_t panel_idx;
            uint8_t isopen;
        };

    private:
        // Basics Operation
        // Table for current opened and available tile..
        //     * (1 - 16) in bool data type which be represented by (1 or 2)
        // game_id as secondary ID for history
        // auto play mode will be added once manual gameplay is done!
        // Useer will provide 16, panel set.
        // update first the table before calling `game_status` function
        // user win_tracker will be table with username and array for win details..

        // change panels vector to map

        enum prize_value : int8_t
        {
            PRIZE_DEFAULT = 0,
            OPENED = 1,
            UNOPENED = 2,
            WIN_LIMIT = 4
        };
        enum game_status : int8_t
        {
            DONE = 0,
            ONGOING = 1
        };
        enum game_destination : int8_t
        {
            EXPLORE_DEFAULT = 0,
            EXPLORE_1 = 1,
            EXPLORE_2 = 5,
            EXPLORE_3 = 10,
            MAP_DEFAULT = 0,
            MAP_1 = 1,
            MAP_2 = 10,
            MAP_3 = 50
        };

        struct TilePrize
        {
            uint16_t key;
            uint16_t value = PRIZE_DEFAULT;
        };

        struct game
        {
            map<uint8_t, Tile> panels ={
                { 0, { 1, UNOPENED } },
                { 1, { 2, UNOPENED } },
                { 2, { 3, UNOPENED } },
                { 3, { 4, UNOPENED } },
                { 4, { 5, UNOPENED } },
                { 5, { 6, UNOPENED } },
                { 6, { 7, UNOPENED } },
                { 7, { 8, UNOPENED } },
                { 8, { 9, UNOPENED } },
                { 9, { 10, UNOPENED } },
                { 10, { 11, UNOPENED } },
                { 11, { 12, UNOPENED } },
                { 12, { 13, UNOPENED } },
                { 13, { 14, UNOPENED } },
                { 14, { 15, UNOPENED } },
                { 15, { 16, UNOPENED } } };
            vector<TilePrize> tile_prizes ={};
            uint8_t win_count = PRIZE_DEFAULT;
            uint8_t destination = MAP_DEFAULT;
            uint16_t explore_count = EXPLORE_DEFAULT;
            int8_t status = ONGOING;
        };

        // Tickets Table
        struct [[eosio::table]] ticket {
            name username;
            int64_t balance;

            auto primary_key() const {
                return username.value;
            }
        };

        struct [[eosio::table]] user
        {
            name username;
            uint64_t game_id;
            game game_data;
            uint64_t tickets; // remaining ticket
            uint64_t total_win; // total win in points (1 ticket):(1 EOS)

            auto primary_key() const {
                return username.value;
            }
        };

        struct [[eosio::table]] seed
        {
            uint64_t key   = 1; // default key of 1
            uint32_t value = 1; // default value of 1

            auto primary_key() const
            {
                return key;
            }
        };

        struct [[eosio::table]] history
        {
            uint64_t game_id = 1; // default key of 1
            name username;
            game game_data;

            auto primary_key() const
            {
                return game_id;
            }
        };

        using users_table = eosio::multi_index<"user"_n, user>;
        using tickets_table = eosio::multi_index<"ticket"_n, ticket>;
        using history_table = eosio::multi_index<"history"_n, history>;
        using seeds_table = eosio::multi_index<"seed"_n, seed>;

        users_table _users;
        tickets_table _tickets;
        seeds_table _seeds;
        history_table _history;

        // vector<results> prizeresults(game game_data, user modified_users);
        // void panel_prize();
        // void update_game();
        int rng(const int range);
        int calculate_prize(vector<TilePrize>& tile_prizes, uint8_t& panel_idx, uint8_t& win_count);
        uint64_t gen_gameid();
        void addhistory(user user_data);
        void purchase(name username, uint64_t amount);
        int64_t ticket_balance(name username);

    public:
        treasurev2(name receiver, name code, datastream<const char*> ds) :
            contract(receiver, code, ds),
            _users(receiver, receiver.value),
            _tickets(receiver, receiver.value),
            _history(receiver, receiver.value),
            _seeds(receiver, receiver.value) {}

        [[eosio::action]] void hello(name username);
        [[eosio::action]] void authorized(name username);
        [[eosio::action]] void startgame(name username, uint8_t destination, uint16_t explore_count, map<uint8_t, Tile> panel_set);
        [[eosio::action]] void genprize(name username, uint8_t panel_idx);
        [[eosio::action]] void end(name username);
        [[eosio::action]] void renew(name username, bool isreset);
    };


### CPP

    #include "gameplay.cpp"

    using namespace eosio;

    void treasurev2::hello(name username) {
        map<uint8_t, Tile> panel_set ={
            { 0, { 1, OPENED } },
            { 1, { 2, OPENED } },
            { 2, { 3, OPENED } },
            { 3, { 4, OPENED } },
            { 4, { 5, OPENED } },
            { 5, { 6, OPENED } },
            { 6, { 7, OPENED } },
            { 7, { 8, OPENED } },
            { 8, { 9, OPENED } },
            { 9, { 10, OPENED } },
            { 10, { 11, OPENED } },
            { 11, { 12, OPENED } },
            { 12, { 13, OPENED } },
            { 13, { 14, OPENED } },
            { 14, { 15, OPENED } },
            { 15, { 16, OPENED } } };

        // authorized(username);
        // purchase(username, 200);
        // startgame(username, MAP_1, EXPLORE_1, panel_set);
        // renew(username, true);
        end(username);
        // genprize(username, 1);
    }

    void treasurev2::genprize(name username, uint8_t panel_idx) {
        require_auth(username);
        auto itr = _users.find(username.value);

        check(ticket_balance(username) > 0, "No enough balance to play the game.");
        check(itr->game_data.win_count < 5, "No more available prizes.");

        _users.modify(itr, username, [&](auto& modified_user) {
            game game_data = modified_user.game_data;

            // for (auto gdp : game_data.panels) {
            //     //  Check the current index and update once found
            //     if (gdp.panel_idx == panel_idx) {
            //         if (gdp.isopen == UNOPENED) {
            //             game_data.panels.at(panel_idx).isopen = OPENED;
            //             // calculate_prize(game_data.tile_prizes, panel_idx, game_data.win_count);
            //             // gdp.isopen = OPENED;
            //         }
            //     };
            // }

            // calculate_prize(game_data, panel_idx);


            // int result = calculate_prize(game_data, panel_idx);

            // if generation failed, rerun function
            // if (result > 0) genprize(username, panel_idx);

            // it will generate 1 to 10 random result as temp prize

            // find current data..
            // game_data.panels.begin();
            // treasurev2::contains(game_data.panels, panel_idx);

            // vector<Tile> panels = game_data.panels;
            // Tile opp ={ 1, UNOPENED };

            // for (auto gdp : game_data.panels) {
            //     //  Check the current index and update once found..
            //     if (gdp.panel_idx == panel_idx) {
            //         // change to opened tile
            //         game_data.panels[panel_idx].isopen = OPENED;
            //         // Add to prize results
            //         game_data.tile_prizes.insert(game_data.tile_prizes.begin(), {
            //             panel_idx, prize
            //             });
            //     }
            // }

            modified_user.game_data = game_data;
            });
    }

    void treasurev2::end(name username) {
        require_auth(username);
        auto& user = _users.get(username.value, "User doesn't exist");
        auto iterator = _users.find(username.value);
        _users.erase(iterator);
        // _users.modify(user, username, [&](auto& modified_user) {
        //     modified_user.game_data = game();
        //     });
    }

    // Initialized the game components
    void treasurev2::startgame(name username, uint8_t destination, uint16_t explore_count, map<uint8_t, treasurev2::Tile> panel_set) {
        require_auth(username);

        auto& user = _users.get(username.value, "User doesn't exist");
        uint64_t user_balance = ticket_balance(username);

        check(user_balance > explore_count && user_balance > 0, "No enough balance to play the game.");
        _users.modify(user, username, [&](auto& modified_user)
            {
                // Initialized game_data
                game game_data;

                // update game data 
                game_data.panels = panel_set;
                game_data.destination = destination;
                game_data.explore_count = explore_count;

                modified_user.total_win = 0;
                modified_user.tickets = ticket_balance(username); // create another table for tickets
                modified_user.game_id = gen_gameid(); // generate user game_id

                modified_user.game_data = game_data;
            });
    }

    void treasurev2::renew(name username, bool isreset) {
        require_auth(username);
        auto& user = _users.get(username.value, "User doesn't exist");

        // Before renewing current user game data, add it to the history..
        // and check if the current game is inserted into the history or else fail.
        addhistory(user);

        auto history = _history.find(user.game_id);
        check(history != _history.end(), "Game Cannot be renewed. Please try again!");

        _users.modify(user, username, [&](auto& modified_user)
            {
                // Initialized game_data
                game game_data;

                if (!isreset) {
                    // load default values
                    modified_user.game_data = game();
                }
                else {
                    // initialized using the current data
                    // generate new panel with unopened as default value..
                    game_data.panels = modified_user.game_data.panels;
                    for (uint8_t i = 0; i < modified_user.game_data.panels.size(); i++) {
                        // Update only into UNOPENED value..
                        // game_data.panels[i].isopen = UNOPENED;
                        game_data.panels.at(i).isopen = UNOPENED;
                    }
                    // reuse current game user destination
                    game_data.destination = modified_user.game_data.destination;
                    game_data.explore_count = modified_user.game_data.explore_count;
                }

                // generate new game ID
                modified_user.game_id = gen_gameid();
                modified_user.game_data = game_data;
                // print("Panel Successfully Renewed or Reset!\n");
            });
    }

    void treasurev2::addhistory(user user_data) {
        name username = user_data.username;
        require_auth(username);

        uint64_t game_id = user_data.game_id;
        auto itr = _history.find(game_id);

        // check if the game_id doesn't exist
        if (itr == _history.end()) {
            _history.emplace(username, [&](auto& history) {
                history.game_id = game_id;
                history.username = user_data.username;
                history.game_data = user_data.game_data;
                history.game_data.status = DONE;
                });
        }
    }

    // After the game ended, remove the user on the current users list...
    void treasurev2::authorized(name username) {
        require_auth(username);
        // Create a record in the table if the player doesn't exist in our app yet
        auto itr = _users.find(username.value);

        if (itr == _users.end())
        {
            _users.emplace(username, [&](auto& new_users) {
                new_users.username = username;
                });
        }
    }
