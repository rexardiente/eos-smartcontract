/*
--------------------------------------------------------------
Filename: game.room.cpp
Purpose: this file acts as the intro contract to the EOS Game store game room.
--------------------------------------------------------------
*/

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>


using namespace eosio;

class [[eosio::contract("gameroom")]] gameroom : public contract {
  private:
  // Configure the Multi-Index Table
   struct [[eosio::table]] user_info {
   uint16_t user_identifier;
   name username;
   asset user_balance;
    uint64_t primary_key() const { return user_identifier; }
  };
  typedef eosio::multi_index<"userinfo"_n, user_info> userinfo_index;
 
  struct [[eosio::table]] games_info {
     uint16_t game_identifier;
    name game_title;
    asset game_balance;
    uint64_t primary_key() const { return game_identifier; }
  };
  
  struct [[eosio::table]] game_room {
    
    uint16_t id;
    
    name username;
    asset user_balance;
     uint16_t primary_key() const { return id; }
  };
  
  

  public:
      using contract::contract;
      gameroom(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

      [[eosio::action]]
      void loguser (name user, uint16_t user_identifier,name username,asset user_balance) {
        require_auth(user);

        userinfo_index userinfo(get_self(), get_self().value);
        auto itr = userinfo.find(user_identifier);
        if(itr==userinfo.end())
            {
                
               userinfo.emplace(user, [&]( auto& row ) {
               row.user_identifier =user_identifier;
                row.username = username;
                row.user_balance = user_balance;
       
             });
            
           
             
             
            }
            else
            {
              userinfo.modify(itr, get_self(), [&](auto &row) {
               
                row.user_balance += user_balance;
              });
            }
            
      
      
       
      }

      
  
};