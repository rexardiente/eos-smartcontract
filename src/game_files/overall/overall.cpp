#include "overall.hpp"

using namespace eosio;

void overall::add(uint64_t id, std::vector<user> data, uint64_t created_at)
{
  // Check if the user is authorized to action
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  rank_index overall(get_self(), get_self().value);
  auto itr = overall.find(id);
  check(itr == overall.end(), "Rank already exist.");

  overall.emplace(get_self(), [&](auto &row) {
    row.id = id;
    row.data = data;
    row.created_at = created_at;
  });
};

void overall::edit(uint64_t id, std::vector<user> data, uint64_t created_at)
{
  // Check if the user is authorized to action
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  rank_index overall(get_self(), get_self().value);
  auto itr = overall.find(id);
  check(itr != overall.end(), "Rank doesn't exist.");

  overall.modify(itr, get_self(), [&](auto &row) {
    row.id = id;
    row.data = data;
    row.created_at = created_at;
  });
};

void overall::del(uint64_t id)
{
  // Auth Validation
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  rank_index overall(get_self(), get_self().value);

  // Check if game exits
  auto itr = overall.find(id);
  check(itr != overall.end(), "Rank doesn't exits.");

  // If all validations successful, remove the game from the list
  overall.erase(itr);
};
