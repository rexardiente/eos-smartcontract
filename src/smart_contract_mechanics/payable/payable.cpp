#include <eosio/eosio.hpp>
// #include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>

using namespace eosio;

class [[eosio::contract("payable")]] payable : public eosio::contract
{

private:
  const symbol egs_symbol;
  const uint32_t withdrawal_limit = withdrawal_time();

  struct [[eosio::table]] balance
  {
    asset funds;
    uint32_t limit; // Can be used to plug a withdrawal time on every account transaction.
    uint64_t primary_key() const { return funds.symbol.raw(); }
  };

  using balance_table = eosio::multi_index<"balance"_n, balance>;

  uint32_t now()
  {
    return current_time_point().sec_since_epoch();
  }

  // add 1 min from current time
  uint32_t withdrawal_time()
  {
    return (now() + (1 * 60));
  };

public:
  using contract::contract;

  payable(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds), egs_symbol("SYS", 4) {}

  // The important thing to note is the deposit function will actually be triggered by the eosio.token contract.
  // To understand this behaviour we need to understand the on_notify attribute.
  [[eosio::on_notify("eosio.token::transfer")]] void deposit(name from, name to, eosio::asset quantity, std::string memo)
  {
    // checks that the contract is not transferring to itself:
    if (from == get_self() || to != get_self())
      return;

    /* Then checks a few other conditions:
     * - The incoming transfer has a valid amount of tokens
     * - The incoming transfer uses the token we specify in the constructor
     */
    check(quantity.amount > 0, "Insufficient amount.");
    check(quantity.symbol == egs_symbol, "Not acceptable token");

    // If all constraints are passed, the action updates the balances accordingly:
    balance_table balance(get_self(), from.value);
    auto itr = balance.find(egs_symbol.raw());

    if (itr != balance.end())
      balance.modify(itr, get_self(), [&](auto &row) {
        row.funds += quantity;
        row.limit += withdrawal_limit;
      });
    else
      balance.emplace(get_self(), [&](auto &row) {
        row.funds = quantity;
        row.limit = withdrawal_limit;
      });
  }

  [[eosio::action]] void withdraw(name acc)
  {
    // Check the authority of payable account.
    require_auth(acc);

    balance_table balance(get_self(), acc.value);
    auto itr = balance.find(egs_symbol.raw());

    // Get withdrawal_limit time value.
    uint32_t to_withdraw;
    to_withdraw = itr->limit;

    check(now() > to_withdraw, "Balance is not yet available for withdraw.");
    // Make sure amount is greater that 0.
    check(itr->funds.amount > 0, "Insufecient balance.");
    // Make sure the holder is in the table
    check(itr != balance.end(), "You're not allowed to withdraw.");

    action{
        permission_level{get_self(), "active"_n},
        "eosio.token"_n,
        "transfer"_n,
        std::make_tuple(get_self(), acc, itr->funds, std::string("Successfully withdraw."))}
        .send();

    balance.erase(itr);
  };
};