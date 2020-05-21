## Goal

Contract `Payable` actions are actions that require to transfer some tokens. Also, the EOSIO asset type is defined.

- Withdraw action
- Deposit action

Getting Started:

First create account to deploy

- cleos create account eosio payable EOS8NfzQA3wfNHy844kqFcwMwdzaNEjQACRriDxb2QQnzqadBbPS5
- eosio-cpp payable.cpp -o payable.wasm
- cleos set contract payable ./ -p payable@active

Add this contract an `eosio.code` permission:

- cleos set account permission payable active --add-code

Create testing account, after creating transfer some token issued from previouus accounts.

- cleos create account eosio han EOS8NfzQA3wfNHy844kqFcwMwdzaNEjQACRriDxb2QQnzqadBbPS5

Finally, transfer some SYS tokens to the `payable` contract from `han's` account.

- cleos transfer han payable '0.0001 SYS' 'Payable!' -p han@active

Withdrawingthe funds

- cleos push action payable party '["han"]' -p han@activ

Check user `han` available balances on `balance` table.

- cleos get table payable han balance
