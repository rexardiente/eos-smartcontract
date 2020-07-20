/*
--------------------------------------------------------------
Filename: game.room.cpp
Purpose: this file acts as the intro contract to the EOS Game store game room.
--------------------------------------------------------------
*/

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

class [[eosio::contract("game.room")]] gameroom : public contract {
  private:
  struct [[eosio::table]] user_info {
    eosio::uint16_t user_identifier;
    eosio::name username;
    eosio::asset user_balance;
    uint64_t primary_key() const { return user_identifier; }
  };

  struct [[eosio::table]] games_info {
    eosio::uint16_t game_identifier;
    eosio::name game_title;
    eosio::asset game_balance;
    uint64_t primary_key() const { return game_identifier; }
  };

  public:
  gameroom(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  void loguser(username user,uint16_t user_identifier,name username,asset user_balance) {
    user_index userlog(get_first_receiver(), get_first_receiver().value);
    require_auth(user);

    auto iter=userlog.find(user);
    if(iter==userlog.end()) {
      userlog.emplace(user, [&]( auto& row ) {
        row.key = user;
        row.user_identifier =user_identifier;
        row.username = username;
        row.user_balance = user_balance;
      });
    } else {
      uint64_t getuser;
      getuser=iter->username;
      print("data already exist ",name{getuser}," \t");
    }
  }
  void getBalance(username owner){
    eosio::token t(N(eosio.token));
    const auto sym_name = eosio::symbol_type(S(4,EOS)).name();
    const auto my_balance = t.get_balance(N(owner), sym_name );
    eosio::print("My balance is ", my_balance);
  }
}

EOSIO_DISPATCH(gameroom, (loguser)(getBalance));
