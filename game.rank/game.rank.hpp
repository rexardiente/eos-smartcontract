#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("game.rank")]] gamerank : public contract
{
public:
  typedef __int128 uint128_t;
  using contract::contract;
  gamerank(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}
  /* Game Rank Table
   *
   *  ID - Primary Key hostname.
   *  game - Secondary key Game title.
   *  platform - String name of the Platform.
   *  symbol - symbol is member of an asset instance.
   *  info - JSON string for list of top users [{user: 'name type', total_reward: 'asset', symbol: 'asset.symbol.raw'}].
   *  created_at - timestamp datetime execution.
   */
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

  [[eosio::action]] void create(name auth_user,
                                name id,
                                std::string game,
                                std::string platform,
                                std::string info,
                                uint32_t created_at);
  // [[eosio::action]] void getbyid(name auth_user, name id);
  [[eosio::action]] void erase(name auth_user, name id);
  [[eosio::action]] void update(name auth_user,
                                name id,
                                std::string game,
                                std::string platform,
                                std::string info,
                                uint32_t created_at);
};