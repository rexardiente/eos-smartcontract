/*
-------------------------------------------------------------------------------
File name: Donut-token.cpp
Creator: Brett A. Harvey
Purpose: A source file that contains methods and structs for Donut token.
-------------------------------------------------------------------------------
*/

#include "donut-token.hpp"

namespace donutToken {

void token::create( const name&   issuer,
                    const asset&  max_supply )
{
    require_auth( get_self() );

    auto sym = max_supply.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( max_supply.is_valid(), "invalid supply");
    check( max_supply.amount > 0, "max-supply must be positive");

    stats statstable( get_self(), sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( get_self(), [&]( auto& s ) {
       s.supply.symbol = max_supply.symbol;
       s.max_supply    = max_supply;
       s.issuer        = issuer;
    });
}


void token::issue( const name& receiver, const asset& amount, const string&
  memo_add )
{
    auto sym = amount.symbol;
    check( sym.is_valid(), "invalid symbol name" );
    check( memo_add.size() <= 256, "memo address has more than 256 bytes" );

    stats statstable( get_self(), sym.code().raw() );
    auto existing = statstable.find( sym.code().raw() );
    check( existing != statstable.end(), "token with symbol does not exist,
    create token before issue" );
    const auto& st = *existing;
    check( receiver == st.issuer, "tokens can only be issued to issuer account" );

    require_auth( st.issuer );
    check( amount.is_valid(), "invalid amount" );
    check( amount.amount > 0, "must issue positive amount" );

    check( amount.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( amount.amount <= st.max_supply.amount - st.supply.amount,
      "amount exceeds available supply");

    statstable.modify( st, same_payer, [&]( auto& s ) {
       s.supply += amount;
    });

    add_balance( st.issuer, amount, st.issuer );
}


void token::transfer( const name&    sender,
                      const name&    receiver,
                      const asset&   amount,
                      const string&  memo_add )
{
    check( sender != receiver, "cannot transfer to self" );
    require_auth( sender );
    check( is_account( receiver ), "to account does not exist");
    auto sym = amount.symbol.code();
    stats statstable( get_self(), sym.raw() );
    const auto& st = statstable.get( sym.raw() );

    require_recipient( sender );
    require_recipient( receiver );

    check( amount.is_valid(), "invalid amount" );
    check( amount.amount > 0, "must transfer positive quantity" );
    check( amount.symbol == st.supply.symbol, "symbol precision mismatch" );
    check( memo_add.size() <= 256, "memo address has more than 256 bytes" );

    auto payer = has_auth( receiver ) ? receiver : sender;

    sub_balance( sender, amount );
    add_balance( receiver, amount, payer );
}

void token::sub_balance( const name& owner, const asset& value ) {
   accounts from_acnts( get_self(), owner.value );

   const auto& sender = from_acnts.get( value.symbol.code().raw(),
   "no balance object found" );
   check( sender.balance.amount >= value.amount, "overdrawn balance" );

   from_acnts.modify( from, owner, [&]( auto& a ) {
         a.balance -= value;
      });
}

void token::add_balance( const name& owner, const asset& value,
  const name& ram_payer )
{
   accounts to_acnts( get_self(), owner.value );
   auto sender = to_acnts.find( value.symbol.code().raw() );
   if( sender == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });
   } else {
      to_acnts.modify( sender, same_payer, [&]( auto& a ) {
        a.balance += value;
      });
   }
}
} /// namespace donutToken
