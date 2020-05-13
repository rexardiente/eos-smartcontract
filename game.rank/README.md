## Contract Actions

### Create Action

- cleos push action game.rank create '["eosgamestore", "eosgamestore", "PANEL GAME" , "EOS GAME STORE", "infomation", 1589312200]' -p eosgamestore@active

### Check Database Table

- cleos get table game.rank game.rank ranks --lower eosgamestor1 --limit 1

### Update Action

- cleos push action game.rank update '["eosgamestore", "eosgamestore", "PANEL GAME" , "EOS GAME STORE", "infomation", 1589312201]' -p eosgamestore@active

### Delete Action

- cleos push action game.rank erase '["game.rank", "eosgamestore"]' -p game.rank@active

## Testing: Compile and Deploy

Make sure wallets are unlock, default development key, and testing acccounts

eosio-cpp game.rank.cpp -o game.rank.wasm
cleos set contract game.rank ../game.rank -p game.rank@active

## Create Test Account

- cleos create account eosio eosgamestore EOS8NfzQA3wfNHy844kqFcwMwdzaNEjQACRriDxb2QQnzqadBbPS5
- cleos create account eosio game.rank EOS8NfzQA3wfNHy844kqFcwMwdzaNEjQACRriDxb2QQnzqadBbPS5

## Create The Token

- cleos push action eosio.token create '[ "eosgamestore", "1000000000.0000 SYS"]' -p eosio.token@active

## Issue Tokens

> To inspect the transaction, try using the -d -j options, which indicate "don't broadcast" and "return the transaction as json", which you may find useful during development. Now "eosgamestore is ready to transfer some of the tokens to other account.

- cleos push action eosio.token issue '[ "eosgamestore", "100.0000 SYS", "game rank memo" ]' -p eosgamestore@active -d -j

## Transfer Tokens

- cleos push action eosio.token transfer '[ "eosgamestore", "game.rank", "25.0000 SYS", "m" ]' -p eosgamestore@active

cleos push action game.rank getbyid '["eosgamestore", "eosgamestor1"]' -p eosgamestore@active
