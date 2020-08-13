# Main Contract
## Create Test Account
```
cleos create account eosio maincontract EOS8NfzQA3wfNHy844kqFcwMwdzaNEjQACRriDxb2QQnzqadBbPS5
```

## Contract compilation and deployment

**Compile contract**
```
eosio-cpp main.cpp -o main.wasm
```
**Deploy contract**
```
cleos set contract maincontract ./ -p maincontract@active