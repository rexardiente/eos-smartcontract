##### Private Key: 
> PW5JMmjNXNGCZqyMNbskJ1NJBehUPF8AJqf6aSNodsTQwC1o73tv8

##### Create Account
> cleos create account eosio treasurev2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active

##### Compile: 
> eosio-cpp ./treasure.v2.cpp -o treasure.v2.wasm --abigen

##### Deploy: 
> cleos set contract treasurev2 ./ -p treasurev2@active

##### Test: 
> cleos push action treasurev2 hello '["treasurev2"]' -p treasurev2@active