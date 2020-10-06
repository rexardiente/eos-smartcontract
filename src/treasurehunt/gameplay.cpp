#include "treasurehunt.hpp"
#include <eosio/transaction.hpp>

// Wallet Destination = {0:1, 1:10, 2:50}
// memo must have "Selected Wallet Destination: 0"
void treasurehunt::ondeposit(name from,
                             name to,
                             asset quantity,
                             string memo)
{
    if (from == _self)
    {
        // we're sending money, do nothing additional
        return;
    }

    check(to == _self, "Not to our contract");
    check(quantity.symbol.is_valid(), "Invalid quantity");
    check(quantity.amount > 0, "Only positive quantity allowed");

    check(quantity.symbol == eosio_symbol(), "Invalid EOS Token");
    // eosio::print("Update current Ticket Balance.");

    // Get Selected Destination
    int findCharPos = memo.find(":");
    string str_dest = memo.substr(findCharPos + 2); // get from "live" to the end
    uint64_t newdestination = stoi(str_dest);

    // Set 1EOS as default wallet
    if (findCharPos < 0 || newdestination > 2)
    {
        newdestination = 0;
    }

    uint64_t convertedquantity = quantity.amount / 10000;

    ticket_add(from, convertedquantity, newdestination);
}

void treasurehunt::ticket_add(name from, uint64_t quantity, uint64_t destination)
{
    auto itr = _tickets.find(from.value);
    uint64_t ticket_tyle = MAP_1;

    switch (destination)
    {
    case 1:
        ticket_tyle = MAP_2;
        break;

    case 2:
        ticket_tyle = MAP_3;
        break;

    default:
        break;
    }

    if (itr == _tickets.end())
    {
        _tickets.emplace(from, [&](auto &new_ticket) {
            new_ticket.username = from;
            new_ticket.balance[destination] = (quantity / ticket_tyle);
        });
    }
    else
    {
        _tickets.modify(itr, from, [&](auto &modify_ticket) {
            modify_ticket.balance[destination] += (quantity / ticket_tyle);
        });
    }
}

void treasurehunt::check_asset(asset quantity, uint64_t minvalue, uint8_t nonfloat)
{
    check(quantity.symbol.is_valid(), "invalid symbol name");
    check(quantity.symbol == eosio_symbol(), "token symbol not match");
    check(quantity.is_valid(), "invalid quantity");
    check(quantity.amount > 0, "quantity must > 0");
    check(quantity.amount >= minvalue, "quantity not reach minimum allowed");
    if (nonfloat > 0)
        check((quantity.amount % 10000) == 0, "quantity must integer");
}

uint64_t treasurehunt::gen_gameid()
{
    // get current time
    uint64_t current_time = current_time_point().elapsed._count;
    // get current size of the table
    int size = std::distance(_users.begin(), _users.end());

    return (size + rng(1000)) + current_time;
}

int treasurehunt::rng(const int &range)
{
    // Find the existing seed
    auto seed_iterator = _seeds.begin();

    // Initialize the seed with default value if it is not found
    if (seed_iterator == _seeds.end())
    {
        seed_iterator = _seeds.emplace(_self, [&](auto &seed) {});
    }

    // Generate new seed value using the existing seed value
    int prime = 65537;
    auto new_seed_value = (seed_iterator->value + current_time_point().elapsed.count()) % prime;

    // Store the updated seed value in the table
    _seeds.modify(seed_iterator, _self, [&](auto &s) {
        s.value = new_seed_value;
    });

    // Get the random result in desired range
    int random_result = new_seed_value % range;
    return random_result;
}

void treasurehunt::ticket_update(name username, const bool &isdeduction, const bool &destination, const uint64_t &amount)
{
    require_auth(username);
    auto &ticket = _tickets.get(username.value, "Ticket doesn't exist");
    _tickets.modify(ticket, username, [&](auto &modified_ticket) {
        if (isdeduction)
            modified_ticket.balance[destination] -= amount;
        else
            modified_ticket.balance[destination] += amount;
    });
}

uint64_t treasurehunt::ticket_balances(name username, const int &ticket_destination)
{
    require_auth(username);
    auto &ticket = _tickets.get(username.value, "Ticket doesn't exist");
    if (ticket.balance[ticket_destination] > 0)
        return ticket.balance[ticket_destination];
    return 0;
}

uint64_t treasurehunt::iswinning(const uint8_t &opened_tiles, const uint8_t &win_count)
{
    const uint8_t unopened = 16 - opened_tiles;
    const int win_rate = rng(10);

    if (unopened == (4 - win_count))
        return multiplier();
    if (win_rate > 6)
        return multiplier();
    else
        return 0;
}

// chest multiplier
uint64_t treasurehunt::multiplier()
{
    const int genChest = rng(1000);
    const int genTier = rng(100);
    if (genTier < 26)
    {
        return 1;
    }
    else if (genTier >= 26 && genTier <= 50)
    {
        return 2;
    }
    else if (genTier >= 51 && genTier <= 75)
    {
        if (genChest >= 1 && genChest <= 700)
        {
            return 3;
        }
        else
        {
            return 4;
        }
    }
    else
    {
        if (genChest >= 1 && genChest <= 700)
        {
            return 4;
        }
        else if (genChest >= 701 && genChest <= 850)
        {
            return 8;
        }
        else if (genChest >= 851 && genChest <= 950)
        {
            return 16;
        }
        else if (genChest >= 951 && genChest <= 990)
        {
            return 32;
        }
        else if (genChest >= 991 && genChest <= 999)
        {
            return 64;
        }
        else
        {
            return 128;
        }
    }
}
