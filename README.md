# Donut Factory Smart Contracts
This repo contains smart contracts and guides to token creation for the Donut Factory project. 

### EOS Token Creation
#### Part 1 - Setting up the environment.
**Note that the latest version of Homebrew is suggested, but Homebrew isn't necessary to complete this tutorial.**

1. First, brew the latest version of EOSIO onto your machine.
```
brew tap eosio/eosio
brew install eosio 
```
2. Then, brew the latest version of EOSIO.cdt onto your machine.
```
brew tap eosio/eosio.cdt
brew install eosio.cdt
```
3. Now, clone the eosio.contracts repo. You will need this in order to create an EOS token. 
```
git clone https://github.com/EOSIO/eosio.contracts --branch v1.7.0 --single-branch
```
4. Next, **in a seperate window**, start keosd. Be sure to run **pkill keosd** first, to ensure that no instances of keosd are running in the background. 
```
pkill keosd
keosd &
```
5. Now, run nodeos **in your original window** and check to to make sure that it is working with the **tail -f nodeos.log** command. 
```
nodeos -e -p eosio \
--plugin eosio::producer_plugin \
--plugin eosio::producer_api_plugin \
--plugin eosio::chain_api_plugin \
--plugin eosio::http_plugin \
--plugin eosio::history_plugin \
--plugin eosio::history_api_plugin \
--filter-on="*" \
--access-control-allow-origin='*' \
--contracts-console \
--http-validate-host=false \
--verbose-http-errors >> nodeos.log 2>&1 &

tail -f nodeos.log
```
6. The following command will check that the RPC API is working correctly
```
curl http://localhost:8888/v1/chain/get_info
```
7. The following command will check of there are any wallets. There should be none. 
```
cleos wallet list
```
**Expected output:**
```
Wallets:
[]
```
8. Now we can create a wallet. It is advised that you give the wallet a name and **ensure you save the password**, as you won't be able to access the wallet otherwise.  
```
cleos wallet create -n [walletname] --to-console
```
9. Recheck the wallet list to ensure your new wallet is there. 
```
cleos wallet list
```
**Expected output:**
```
Wallets:
[
    "${your wallet name}*"
]
```
10. If there isn't an asterisk next to the name of the wallet, you need to unlock the wallet with the following command. 
```
cleos wallet unlock
```
11. Next, create a key pair for your wallet with the following command

```
cleos wallet create_key
```
12. Now that you've created a key pair for the wallet, you'll need to import the eosio development key to the wallet, too.

```
cleos wallet import
```
`eosio development private key: 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD`

13. Next, you'll need to create two test accounts in order to issue and test trasferring the token. You can create the accounts by using the commands below.

```
cleos create account eosio [name of account] [your public address]
cleos create account eosio [name of account] [your public address]
```
**Expected output:**
```
executed transaction: 40c605006de...  200 bytes  153 us
#         eosio <= eosio::newaccount            {"creator":"eosio","name":"alice","owner":{"threshold":1,"keys":[{"key":"EOS5rti4LTL53xptjgQBXv9HxyU...
warning: transaction executed locally, but may not be confirmed by the network yet    ]
```

14. You can get each of the account details by entering the following
```
cleos get account [your account name]
```
**Expected output:**
```
permissions:
     owner     1:    1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
        active     1:    1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
memory:
     quota:       unlimited  used:      3.758 KiB

net bandwidth:
     used:               unlimited
     available:          unlimited
     limit:              unlimited

cpu bandwidth:
     used:               unlimited
     available:          unlimited
     limit:              unlimited
```
<br />

#### Part 2 - Creating and issuing the tokens.
Now that we've set everything up, we can start creating, issuing, and transferring the tokens.

1. Navigate to your contracts directory.
```
cd /Users/shelleymacpherson/contracts
```
2. Now, clone the following repo into your working directory.
```
git clone https://github.com/EOSIO/eosio.contracts --branch v1.7.0 --single-branch
cd eosio.contracts/contracts/eosio.token
```
3. Create a new cleos account.
```
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
```
4. Compile the contract.
```
eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen
```
5. Deploy the contract.
```
cleos set contract eosio.token /Users/shelleymacpherson/contracts/eosio.contracts/contracts/eosio.token --abi eosio.token.abi -p eosio.token@active
```
**Expected output:**
```
Reading WASM from ...eosio.contracts/contracts/eosio.token/eosio.token.wasm...
Publishing contract...
executed transaction: a68299112725b9f2233d56e58b5392f3b37d2a4564bdf99172152c21c7dc323f  6984 bytes  6978 us
#         eosio <= eosio::setcode               {"account":"eosio.token","vmtype":0,"vmversion":0,"code":"0061736d0100000001a0011b60000060017e006002...
#         eosio <= eosio::setabi                {"account":"eosio.token","abi":"0e656f73696f3a3a6162692f312e310008076163636f756e7400010762616c616e63...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
6. Now we can create the token. Note that you will need to change the parameters to fit your desiered account name, token amount, and token name.
```
cleos push action eosio.token create '[ "user3", "10000.0000 TNEX"]' -p eosio.token@active
```
An alternative:
```
cleos push action eosio.token create '{"issuer":"user3", "maximum_supply":"10000.0000 TNEX"}' -p eosio.token@active
```
**Expected output:**
```
executed transaction: 10cfe1f7e522ed743dec39d83285963333f19d15c5d7f0c120b7db652689a997  120 bytes  1864 us
#   eosio.token <= eosio.token::create          {"issuer":"user3","maximum_supply":"10000.0000 TNEX"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
7. The tokens launched, but they aren't issued to anyone yet. They were launched using the `user3` account, but that doesn't mean that `user3` has those tokens in his wallet. You can issue the tokens as follows below:
```
cleos push action eosio.token issue '[ "user3", "50.0000 TNEX", "memo" ]' -p user3@active
```
**Expected output:**
```
executed transaction: d1466bb28eb63a9328d92ddddc660461a16c405dffc500ce4a75a10aa173347a  128 bytes  205 us
#   eosio.token <= eosio.token::issue           {"to":"user3","quantity":"50.0000 TNEX","memo":"memo"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

8. Now that the tokes are issued, we can transfer them between our two created accounts.
```
cleos push action eosio.token transfer '[ "user3", "user4", "10.0000 TNEX", "m" ]' -p user3@active
```
**Expected output:**
```
executed transaction: 800835f28659d405748f4ac0ec9e327335eae579a0d8e8ef6330e78c9ee1b67c  128 bytes  1073 us
#   eosio.token <= eosio.token::transfer        {"from":"user3","to":"user4","quantity":"10.0000 TNEX","memo":"m"}
#         user3 <= eosio.token::transfer        {"from":"user3","to":"user4","quantity":"10.0000 TNEX","memo":"m"}
#           user4 <= eosio.token::transfer        {"from":"user3","to":"user4","quantity":"10.0000 TNEX","memo":"m"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
9. Now that the transfer has been executed, you can check the balance of `TNEX` for `user4`
```
cleos get currency balance eosio.token user4 TNEX
```
10. You can also check the balance of `TNEX` for `user3` using the same command.
```
cleos get currency balance eosio.token user3 TNEX
```
