/*
-------------------------------------------------------------------------------
File name: Donut-token.hpp
Creator: Brett A. Harvey
Purpose: A header file that contains method definitions for the custom
EOS token contract "donut-token.cpp"
-------------------------------------------------------------------------------
*/

#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("donut.token")]] donuttoken : public contract
{
private:
  struct [[eosio::table]] account {
     asset    balance;
     uint64_t primary_key()const { return balance.symbol.code().raw(); }
  };

  struct [[eosio::table]] currency_stats {
     asset    supply;
     asset    max_supply;
     name     issuer;
     uint64_t primary_key()const { return supply.symbol.code().raw(); }
  };

  typedef eosio::multi_index< "accounts"_n, account > accounts;
  typedef eosio::multi_index< "stat"_n, currency_stats > stats;

  void sub_balance( const name& owner, const asset& value );
  void add_balance( const name& owner, const asset& value, const name&
    ram_payer );

public:
  using contract::contract;

  donuttoken(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

  [[eosio::action]] void create(const name& issuer, const asset&  max_supply);

  [[eosio::action]] void issue(const name& receiver, const asset& amount,
    const string& memo_add);

  [[eosio::action]] void transfer(onst name& sender, const name& receiver,
    const asset& amount, const string&  memo_add );

    static asset get_balance( const name& token_contract_account, const
      name& owner, const symbol_code& sym_code);

    static asset get_supply( const name& token_contract_account, const
      symbol_code& sym_code );

  // Action Wrappers
  using create_action = action_wrapper<"create"_n, &donuttoken::create>;
  using issue_action = action_wrapper<"issue"_n, &donuttoken::issue>;
  using transfer_action = action_wrapper<"transfer"_n, &donuttoken::transfer>;
};
