#include "games.hpp"

using namespace eosio;

void games::add(uint64_t id, std::string title, std::string description)
{
  // Check if the user is authorized to action
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  game_index games(get_self(), get_self().value);
  auto itr = games.find(id);
  check(itr == games.end(), "Game already exist.");

  games.emplace(get_self(), [&](auto &row) {
    row.id = id;
    row.title = title;
    row.description = description;
  });
};

void games::edit(uint64_t id, std::string title, std::string description)
{
  // Check if the user is authorized to action
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  game_index games(get_self(), get_self().value);
  auto itr = games.find(id);

  check(itr != games.end(), "Game doesn't exist.");

  games.modify(itr, get_self(), [&](auto &row) {
    row.id = id;
    row.title = title;
    row.description = description;
  });
};

void games::del(uint64_t id)
{
  // Auth Validation
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  game_index games(get_self(), get_self().value);

  // Check if game exits
  auto itr = games.find(id);
  check(itr != games.end(), "Game doesn't exits.");

  // If all validations successful, remove the game from the list
  games.erase(itr);
};

void games::addconfig(uint64_t id)
{
  // Auth Validation
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  config_index games(get_self(), get_self().value);

  // Check if game exits
  auto itr = games.find(id);
  check(itr == games.end(), "Game config already exits.");

  // If all validations successful, add config on the list.
  games.emplace(get_self(), [&](auto &row) {
    row.id = id;
  });
};

void games::editconfig(uint64_t id)
{
  // Auth Validation
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  config_index games(get_self(), get_self().value);

  // Check if game exits
  auto itr = games.find(id);
  check(itr != games.end(), "Game config doesn't exits.");

  // If all validations successful, update config on the list.
  games.modify(itr, get_self(), [&](auto &row) {
    row.id = id;
  });
};

void games::delconfig(uint64_t id)
{
  // Auth Validation
  check(has_auth(get_self()), "User with admin access only authorized.");
  require_auth(get_self());

  config_index games(get_self(), get_self().value);

  // Check if game exits
  auto itr = games.find(id);
  check(itr != games.end(), "Game config doesn't exits.");

  // If all validations successful, erase config on the list.
  games.erase(itr);
};