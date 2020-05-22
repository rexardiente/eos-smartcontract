Compile and deploy

- eosio-cpp overall.cpp -o overall.wasm
- cleos set contract overall ./ -p overall@active

Add Action

- cleos push action overall add '[12345, [{"rank":1, "user":"user1","total_reward": 50}], 1590131419]' -p overall@active

Edit Action

- cleos push action overall edit '[12345, [{"rank":1, "user":"user1","total_reward": 100}, {"rank":2, "user":"user2","total_reward": 21}, {"rank":3, "user":"user3","total_reward": 100}], 1590131419]' -p overall@active

- Check table using ID

- cleos get table overall overall ranks --lower 12345 --limit 1

Delete Action

- cleos push action overall del '[12346]' -p overall@active
