/*
-------------------------------------------------------------------------------
File name: Donut-token.hpp
Creator: Brett A. Harvey
Purpose: A header file that contains method definitions for the custom
EOS token contract "donut-token.cpp"
-------------------------------------------------------------------------------
*/

#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

#include <string>

using namespace eosio;
using namespace std;

namespace eosiosystem {
   class system_contract;
}

namespace donutToken {

   using std::string;

   /*
   ---------------------------------------------------------------------------
   Type: class
   Purpose: Defines structs and methods (create, issue, and transfer), for
   donut.token.cpp
   ---------------------------------------------------------------------------
   */
   class [[eosio::contract("donut.token")]] token : public contract {
      public:
         using contract::contract;

         /*
         ----------------------------------------------------------------------
         Type: create function
         Params:
                  1. issuer | The issuer is the account
                  2. max_supply  | The maximum supply is the maximum amount
                  of tokens that are created.
         Purpose: Creates the donut token from a specified deployment address
         and a set maximum token supply.
         ----------------------------------------------------------------------
         */
         [[eosio::on_notify("eosio.token::create")]] void donutToken::create
         (name issuer, const asset &amount) {
                print(get_self()); // donutToken
                print(get_first_receiver()); // eosio.token
         }
         
         /*
         ---------------------------------------------------------------------
         Type: issue function
         Params:
                   1. receiver | The account receiving the tokens.
                   2. amount | The amount of tokens sent in the transaction.
                   3. memo_add | The memo address of the receiver.
         Purpose: To issue tokens from the issuer to a specified receiving
         address with a set amount of tokens, and  a specific memo address.
         ---------------------------------------------------------------------
         */
         [[eosio::on_notify("eosio.token::issue")]] void donutToken::issue
         (name sender, name receiver, const asset &amount,string memo_add) {
                print(get_self()); // donutToken
                print(get_first_receiver()); // eosio.token
         }

         /*
         ---------------------------------------------------------------------
         Type: issue function
         Params:
                  1. sender | The account transferring the tokens.
                  2. receiver | The account receiving the transfer.
                  3. amount \ The amount of tokens sent in the transfer.
                  4. memo_add | The memo address of the receiver.
         Purpose: To transfer tokens between two addresses with a specified
         amount, and the specific memo address of the receiver.
         ---------------------------------------------------------------------
         */

          [[eosio::on_notify("eosio.token::transfer")]] void donutToken::transfer
          (name sender, name receiver, const asset &amount,
            string memo_add) {
                  print(get_self()); // donutToken
                  print(get_first_receiver()); // eosio.token
          }

         static asset get_balance( const name& token_contract_account, const
           name& owner, const symbol_code& sym_code )
         {
            accounts accountstable( token_contract_account, owner.value );
            const auto& ac = accountstable.get( sym_code.raw() );
            return ac.balance;
         }

         static asset get_supply( const name& token_contract_account, const
           symbol_code& sym_code )
         {
            stats statstable( token_contract_account, sym_code.raw() );
            const auto& st = statstable.get( sym_code.raw() );
            return st.supply;
         }

         using create_action = eosio::action_wrapper<"create"_n,
         &token::create>;
         using issue_action = eosio::action_wrapper<"issue"_n, &token::issue>;
         using transfer_action = eosio::action_wrapper<"transfer"_n,
         &token::transfer>;

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
   };

} // namespace donutToken
