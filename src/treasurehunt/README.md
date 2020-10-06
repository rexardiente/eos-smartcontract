
#### Private Key: 
    PW5J9srswAQKDMAhCKHer5pdKczsq5heMafi9DSqT28YrgvuDXApV

#### Compile: 
    eosio-cpp ./treasurehunt.cpp -o treasurehunt.wasm --abigen

#### Deploy: 
    cleos set contract treasurehunt ./ -p treasurehunt@active


cleos transfer treasurehunt user1 "10 EOS"