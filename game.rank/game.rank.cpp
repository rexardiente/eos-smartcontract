#include "game.rank.hpp"

using namespace eosio;

// This action will insert an entry in the table game rank.
// If a record does not exist, a new record is created.
// The data is stored in the multi index table.
// The RAM costs are paid by the smart contract.
void gamerank::create(name auth_user,
                      name id,
                      std::string game,
                      std::string platform,
                      std::string info,
                      uint32_t created_at)
{
  require_auth(auth_user);
  rank_index gamerank(get_first_receiver(), get_first_receiver().value);
  gamerank.emplace(auth_user, [&](auto &row) {
    row.id = id;
    row.game = game;
    row.platform = platform;
    row.info = info;
    row.created_at = created_at;
  });
};

// This action will remove an entry from the table game rank
// if an entry in the multi index table exists with the specified name.
void gamerank::erase(name auth_user, name id)
{
  require_auth(auth_user);
  rank_index gamerank(get_first_receiver(), get_first_receiver().value);
  auto iterator = gamerank.find(id.value);
  check(iterator != gamerank.end(), "Record does not exist");
  gamerank.erase(iterator);
};

// This action will update an entry in the table game rank.
void gamerank::update(name auth_user,
                      name id,
                      std::string game,
                      std::string platform,
                      std::string info,
                      uint32_t created_at)
{
  require_auth(auth_user);
  rank_index gamerank(get_self(), get_first_receiver().value);
  auto iterator = gamerank.find(id.value);
  gamerank.modify(iterator, auth_user, [&](auto &row) {
    row.id = id;
    row.game = game;
    row.platform = platform;
    row.info = info;
    row.created_at = created_at;
  });
};
