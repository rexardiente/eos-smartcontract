#### Private Key:

    PW5J9srswAQKDMAhCKHer5pdKczsq5heMafi9DSqT28YrgvuDXApV

#### Compile:

    eosio-cpp ./treasurehunt.cpp -o treasurehunt.wasm --abigen

#### Deploy:

    cleos set contract treasurehunt ./ -p treasurehunt@active

#### Set Permission:
    cleos set account permission treasurehunt active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "treasurehunt","permission":"eosio.code"},"weight":1}]}'
