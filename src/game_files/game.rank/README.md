## Create Test Account
- cleos create account eosio game.rank EOS8NfzQA3wfNHy844kqFcwMwdzaNEjQACRriDxb2QQnzqadBbPS5

## Contract Actions

Compile and deploy

- eosio-cpp game.rank.cpp -o game.rank.wasm
- cleos set contract game.rank ./ -p game.rank@active

Add Action

- cleos push action game.rank add '[12345, [{"id":1, "account":"user1","total_reward": 50}], 1590131419]' -p game.rank@active

Edit Action

- cleos push action game.rank edit '[12345, [{"id":1, "account":"user1","total_reward": 100}, {"id":2, "account":"user2","total_reward": 21}, {"id":3, "account":"user3","total_reward": 100}], 1590131419]' -p game.rank@active

- Check table using ID

- cleos get table game.rank game.rank ranks --lower 12345 --limit 1

Delete Action

- cleos push action game.rank del '[12345]' -p game.rank@active
