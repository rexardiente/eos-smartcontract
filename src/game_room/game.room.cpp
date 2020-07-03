/*
--------------------------------------------------------------
Filename: game.room.cpp
Purpose: this file acts as the intro contract to the EOS Game store game room.
--------------------------------------------------------------
*/

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
// #include <eosio.cdt/eosio.hpp>

using namespace eosio;

class [[eosio::contract("game.room")]] gameroom : public contract {
  private:
  struct [[eosio::table]] user_info {
    uint16_t eos_identifier;
    name username;
    asset user_balance;
  };

  struct [[eosio::table]] game_info {
    uint16_t map_identifier;
    name game_name;
    asset destinaton_cost;
  };

  public:
  gameroom(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
    void upsert(name user,  uint64_t userid, std::string password, uint64_t auth_level) {
      require_auth(user);
      log_index login(get_first_receiver(), get_first_receiver().value);
      auto iterator = login.find(user.value);
      int guess = -1;
       checksum256 hashed = sha256(password.c_str(), password.length());
      // Convert output to hexadecimal string
      string result = to_hex(hashed);
      if( iterator == login.end() )
      {

        login.emplace(user, [&]( auto& row ) {
         row.key = user;
         row.userid = userid;
         row.password = result;
         row.auth_level = auth_level;

        });

      }
      else {
        login.modify(iterator, user, [&]( auto& row ) {
          row.key = user;
          row.userid = userid;
          row.password = result;
           row.auth_level = auth_level;

        });

      }
    }
    struct [[eosio::table]] loginformation {
    name key;
    uint64_t userid;
    std::string password;
    uint64_t auth_level;

    uint64_t primary_key() const { return key.value; }

  };
}
