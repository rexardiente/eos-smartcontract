#### Private Key:

    PW5J9srswAQKDMAhCKHer5pdKczsq5heMafi9DSqT28YrgvuDXApV

#### Compile:

    eosio-cpp ./treasurehunt.cpp -o treasurehunt.wasm --abigen

#### Deploy:

    cleos set contract treasurehunt ./ -p treasurehunt@active

#### Set Permission:

    cleos set account permission treasurehunt active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "treasurehunt","permission":"eosio.code"},"weight":1}]}'

#### Compile, Deploy, Create and Issue Token:

    eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen
    cleos set contract eosio.token ./ --abi eosio.token.abi -p eosio.token@active
    cleos push action eosio.token create '[ "treasurehunt", "1000000000.0000 EOS"]' -p eosio.token@active
    cleos push action eosio.token issue '[ "treasurehunt", "10000.0000 EOS", "memo" ]' -p treasurehunt@active

#### TRANSFER Balance to Users

    cleos transfer treasurehunt user1 "200 EOS"
    cleos transfer treasurehunt user2 "200 EOS"