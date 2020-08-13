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
  struct [[eosio::table]] user {
    eosio::uint16_t user_ID;
    eosio::name username;
    eosio::asset user_balance;
    uint64_t primary_key() const { return user_ID; }
  };

  struct [[eosio::table]] game {
    eosio::uint16_t game_ID;
    eosio::name game_title;
    eosio::asset game_balance;
    uint64_t primary_key() const { return game_ID; }
  };

  struct [[eosio::table]] history {
    // TO DO
  };

  struct [[eosio::table]] ticket {
    eosio::uint16_t ticket_ID;
    eosio::name ticket_name;
    uint64_t primary_key() const { return ticket_ID; }
  };

  typedef eosio::multi_index<"user"_n, user> user_index;
  typedef eosio::multi_index<"game"_n, game> game_index;
  typedef eosio::multi_index<"history"_n, history> histroy_index;
  typedef eosio::multi_index<"ticket"_n, ticket> ticket_index;


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
