#include "game.rank.hpp"

using namespace eosio;

void gamerank::add(uint64_t id, std::vector<user> data, uint64_t created_at)
{
  // Check if the user is authorized to action
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  rank_index gamerank(get_self(), get_self().value);
  auto itr = gamerank.find(id);
  check(itr == gamerank.end(), "Rank already exist.");

  gamerank.emplace(get_self(), [&](auto &row) {
    row.id = id;
    row.data = data;
    row.created_at = created_at;
  });
};

void gamerank::edit(uint64_t id, std::vector<user> data, uint64_t created_at)
{
  // Check if the user is authorized to action
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  rank_index gamerank(get_self(), get_self().value);
  auto itr = gamerank.find(id);
  check(itr != gamerank.end(), "Rank doesn't exist.");

  gamerank.modify(itr, get_self(), [&](auto &row) {
    row.id = id;
    row.data = data;
    row.created_at = created_at;
  });
};

void gamerank::del(uint64_t id)
{
  // Auth Validation
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  rank_index gamerank(get_self(), get_self().value);

  // Check if game exits
  auto itr = gamerank.find(id);
  check(itr != gamerank.end(), "Rank doesn't exits.");

  // If all validations successful, remove the game from the list
  gamerank.erase(itr);
};
