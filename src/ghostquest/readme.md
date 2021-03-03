eosio-cpp -o ghostquest.wasm ghostquest.cpp

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos set account permission ghostquest active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "ghostquest","permission":"eosio.code"},"weight":1}]}'
Default key: PW5J9srswAQKDMAhCKHer5pdKczsq5heMafi9DSqT28YrgvuDXApV

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos wallet unlock
/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos set contract ghostquest ./ -p ghostquest@active

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio.token transfer '[ "user1", "ghostquest", "1.0000 EOS", "BTTL_LMT=10" ]' -p user1@active
/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio.token transfer '[ "user2", "ghostquest", "1.0000 EOS", "BTTL_LMT=10" ]' -p user2@active
/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio.token transfer '[ "user2", "ghostquest", "1.0000 EOS", "ADD_LIFE=8e2bfc33ea0ef75ac29deadbf4a7f80" ]' -p user2@active

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action ghostquest battleresult '["ba8b8ba1-3512-467f-bc6d-5172efe409a", ["4afc9251fddbcd70b4b60be64eff372","user1"], ["d74ea680e036a3ac1edc2bd5f761ab1","user2"]]' -p ghostquest@active

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos get table ghostquest ghostquest users

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action ghostquest end '["user1"]' -p user1@active
/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action ghostquest end '["user2"]' -p user2@active

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action ghostquest withdraw '["user1", "4afc9251fddbcd70b4b60be64eff372"]' -p user1@active

/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action ghostquest eliminate '["user2", "0db67bc0cac810ca977e39244f4a6b0"]' -p ghostquest@active