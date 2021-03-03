eosio-cpp -o ghostquest.wasm ghostquest.cpp

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos set account permission ghostquest active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "ghostquest","permission":"eosio.code"},"weight":1}]}'
Default key: PW5J9srswAQKDMAhCKHer5pdKczsq5heMafi9DSqT28YrgvuDXApV

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos wallet unlock
/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos set contract ghostquest ./ -p ghostquest@active

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio.token transfer '[ "user1", "ghostquest", "1.0000 EOS", "BTTL_LMT=10" ]' -p user1@active
/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio.token transfer '[ "user2", "ghostquest", "1.0000 EOS", "BTTL_LMT=10" ]' -p user2@active
/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio.token transfer '[ "user2", "ghostquest", "1.0000 EOS", "ADD_LIFE=16c959d5aa37402471eb2a892c2a460" ]' -p user2@active

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action ghostquest battleresult '["ba8b8ba1-3512-467f-bc6d-5172efe409a", ["c35d3a7ea8cb27dbc37c44166007850","user1"], ["0db67bc0cac810ca977e39244f4a6b0","user2"]]' -p ghostquest@active

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos get table ghostquest ghostquest users

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action ghostquest end '["user1"]' -p user1@active
/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action ghostquest end '["user2"]' -p user2@active

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action ghostquest eliminate '["user2", "0db67bc0cac810ca977e39244f4a6b0"]' -p ghostquest@active