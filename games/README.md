## Games contract Contents:
- Games table
  - Add
  - Edit
  - Delete

- Game config table
  - Add
  - Edit
  - Delete

## Usage

First create an accountf for game contract.

- cleos create account eosio games EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

Add game and config table:

- cleos push action games add '[12345, "eosgamestore", "1000000000.0000 SYS"]' -p games@active
- cleos push action games addconfig '[12345]' -p games@active

Check games and configs table

- cleos get table games games games 
- cleos get table games games configs 

Try `Edit` and `Delete` action.