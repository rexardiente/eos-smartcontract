#### Private Key:

    PW5J9srswAQKDMAhCKHer5pdKczsq5heMafi9DSqT28YrgvuDXApV

#### Compile:

    eosio-cpp ./treasurehunt.cpp -o treasurehunt.wasm --abigen

#### Deploy:

    cleos set contract treasurehunt ./ -p treasurehunt@active

cleos transfer treasurehunt user1 "10 EOS"

cleos push action eosio.token create '[ "treasurehunt", "1000000000.0000 EOS"]' -p eosio.token@active

cleos push action eosio.token transfer '[ "user1", "treasurehunt", "25.0000 EOS", "m" ]' -p user1@active

cleos set contract eosio.token ./ --abi eosio.token.abi -p eosio.token@active
