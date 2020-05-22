#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
// #include <to_json.hpp>

using namespace eosio;

class [[eosio::contract("overall")]] overall : public contract
{
public:
  /*
  *  rank - number
  *  data - user ID
  *  asset - total amount win
  */
  struct user
  {
    uint64_t rank_num;
    name account_name;
    uint64_t total_reward;
  };

private:
  /* Game Overall Table
  *
  *  ID - Primary Key.
  *  data - JSON string for list of top users [{user: 'name type', total_reward: 'uint64_t'}].
  *  created_at - timestamp datetime execution.
  */
  struct [[eosio::table]] rank
  {
    uint64_t id;
    std::vector<user> data; // List of JSON String format
    uint64_t created_at;

    uint64_t primary_key() const { return id; }
    uint64_t secondary_key() const { return created_at; }

    EOSLIB_SERIALIZE(rank, (id)(data)(created_at))
  };

  typedef eosio::multi_index<"ranks"_n, rank> rank_index;

public:
  using contract::contract;

  overall(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

  [[eosio::action]] void del(uint64_t id);
  [[eosio::action]] void add(uint64_t id,
                             std::vector<user> data,
                             uint64_t created_at);
  [[eosio::action]] void edit(uint64_t id,
                              std::vector<user> data,
                              uint64_t created_at);

  using del_action = action_wrapper<"del"_n, &overall::del>;
  using add_action = action_wrapper<"add"_n, &overall::add>;
  using edit_action = action_wrapper<"edit"_n, &overall::edit>;
};