#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("game.rank")]] gamerank : public contract
{
  /* Game Rank Table
  *
  *  ID - Primary Key hostname.
  *  game - Secondary key Game title.
  *  platform - String name of the Platform.
  *  symbol - symbol is member of an asset instance.
  *  info - JSON string for list of top users [{user: 'name type', total_reward: 'asset', symbol: 'asset.symbol.raw'}].
  *  created_at - timestamp datetime execution.
  */
private:
  struct [[eosio::table]] rank
  {
    name id;
    std::string game;
    std::string platform;
    std::string info;
    uint32_t created_at;

    auto primary_key() const { return id.value; }
    uint32_t secondary_key() const { return created_at; }

    EOSLIB_SERIALIZE(rank, (id)(platform)(game)(info)(created_at))
  };

  typedef eosio::multi_index<"ranks"_n, rank> rank_index;

public:
  using contract::contract;

  gamerank(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

  // [[eosio::action]] void getbyid( name id);
  [[eosio::action]] void erase(name id);
  [[eosio::action]] void byid(name id);
  [[eosio::action]] void create(name id,
                                std::string game,
                                std::string platform,
                                std::string info,
                                uint32_t created_at);
  [[eosio::action]] void update(name id,
                                std::string game,
                                std::string platform,
                                std::string info,
                                uint32_t created_at);

  using create_action = action_wrapper<"create"_n, &gamerank::create>;
  using update_action = action_wrapper<"update"_n, &gamerank::update>;
  using erase_action = action_wrapper<"erase"_n, &gamerank::erase>;
};