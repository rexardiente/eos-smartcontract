Compile and deploy

- eosio-cpp overall.cpp -o overall.wasm
- cleos set contract overall ./ -p overall@active

Add Action

- cleos push action overall add '[12345, [{"id":1, "account":"user1","total_reward": 50}], 1590131419]' -p overall@active

Edit Action

- cleos push action overall edit '[12345, [{"id":1, "account":"user1","total_reward": 100}, {"id":2, "account":"user2","total_reward": 21}, {"id":3, "account":"user3","total_reward": 100}], 1590131419]' -p overall@active

- Check table using ID

- cleos get table overall overall ranks --lower 12345 --limit 1

Delete Action

- cleos push action overall del '[12345]' -p overall@active
