#!/bin/sh
# https://developers.eos.io/manuals/eosio.cdt/v1.8/how-to-guides/how-to-return-values-from-actions
# The returned values from actions are only available to the clients sending the action via the RPC API. 
# Currently, there is no support for an inline action to be able to use the return value, because inline actions don't execute synchronously.
# Project Setup
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos
PROJECT_NAME="Ghost Quest"
# Global ENV
EXECUTE="> "
CLEOS=/Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos 
CONTRACTS_PATH=/Users/rexardiente/Development/contracts/eosio.contracts/contracts/eosio.token
# Compile Contract
COMPILE_CONTRACT() 
{
    echo "${EXECUTE} Compiling ${PROJECT_NAME}"
    eosio-cpp -o ghostquest.wasm ghostquest.cpp
}
# Deploy Account
DEPLOY_CONTRACT()
{
    echo "${EXECUTE} Deploying ${PROJECT_NAME} into EOSIO Server"
    $CLEOS set contract ghostquest ./ -p ghostquest@active
}

CREATE_ACCOUNT_WITH_BALANCE()
{
    # EOSIO.TOKEN FOR ACCOUNT CREATION
    # eosio-cpp -I include -o $CONTRACTS_PATH/eosio.token.wasm $CONTRACTS_PATH/src/eosio.token.cpp --abigen
    # $CLEOS set contract eosio.token $CONTRACTS_PATH -p eosio.token@active
    
    # $CLEOS create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    # $CLEOS create account eosio egs.main EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    # $CLEOS create account eosio treasurehunt EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    # $CLEOS create account eosio ghostquest EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    # $CLEOS create account eosio user1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    # $CLEOS create account eosio user2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active

    $CLEOS push action eosio.token create '[ "egs.main", "1000000000.0000 EOS"]' -p eosio.token@active
    $CLEOS push action eosio.token issue '[ "egs.main", "100000.0000 EOS", "memo" ]' -p egs.main@active

    $CLEOS transfer egs.main treasurehunt "200 EOS"
    $CLEOS transfer egs.main ghostquest "200 EOS"
    $CLEOS transfer egs.main user1 "200 EOS"
    $CLEOS transfer egs.main user2 "200 EOS"
}

AUTOMATION()
{
    # echo "${EXECUTE} Compiling ${PROJECT_NAME}"
    # $CLEOS push action ghostquest automation  '["test"]' -p user1@active
    # $CLEOS push action ghostquest rtmp '[]'  -p user1@active
    $CLEOS push action ghostquest rstring '["test"]' -p user1@active -j
}

# COMPILE_CONTRACT
# DEPLOY_CONTRACT
# CREATE_ACCOUNT_WITH_BALANCE
AUTOMATION




### ACTIVE PROTOCOLS
# echo KV_DATABASE
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["825ee6288fb1373eab1b5187ec2f04f6eacb39cb3a97f356a07c91622dd61d16"]' -p eosio
# echo ACTION_RETURN_VALUE
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["c3a6138c5061cf291310887c0b5c71fcaffeab90d5deb50d3b9e687cead45071"]' -p eosio
# echo CONFIGURABLE_WASM_LIMITS
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["bf61537fd21c61a60e542a5d66c3f6a78da0589336868307f94a82bccea84e88"]' -p eosio
# echo BLOCKCHAIN_PARAMETERS
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["5443fcf88330c586bc0e5f3dee10e7f63c76c00249c87fe4fbf7f38c082006b4"]' -p eosio
# echo GET_SENDER
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["f0af56d2c5a48d60a4a5b5c903edfb7db3a736a94ed589d0b797df33ff9d3e1d"]' -p eosio
# echo FORWARD_SETCODE
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["2652f5f96006294109b3dd0bbde63693f55324af452b799ee137a81a905eed25"]' -p eosio
# echo ONLY_BILL_FIRST_AUTHORIZER
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["8ba52fe7a3956c5cd3a656a3174b931d3bb2abb45578befc59f283ecd816a405"]' -p eosio
# echo RESTRICT_ACTION_TO_SELF
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["ad9e3d8f650687709fd68f4b90b41f7d825a365b02c23a636cef88ac2ac00c43"]' -p eosio
# echo DISALLOW_EMPTY_PRODUCER_SCHEDULE
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["68dcaa34c0517d19666e6b33add67351d8c5f69e999ca1e37931bc410a297428"]' -p eosio
#  echo FIX_LINKAUTH_RESTRICTION
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["e0fb64b1085cc5538970158d05a009c24e276fb94e1a0bf6a528b48fbc4ff526"]' -p eosio
#  echo REPLACE_DEFERRED
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["ef43112c6543b88db2283a2e077278c315ae2c84719a8b25f25cc88565fbea99"]' -p eosio
# echo NO_DUPLICATE_DEFERRED_ID
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["4a90c00d55454dc5b059055ca213579c6ea856967712a56017487886a4d4cc0f"]' -p eosio
# echo ONLY_LINK_TO_EXISTING_PERMISSION
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["1a99a59d87e06e09ec5b028a9cbb7749b4a5ad8819004365d02dc4379a8b7241"]' -p eosio
# echo RAM_RESTRICTIONS
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["4e7bf348da00a945489b2a681749eb56f5de00b900014e137ddae39f48f69d67"]' -p eosio
# echo WEBAUTHN_KEY
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["4fca8bd82bbd181e714e283f83e1b45d95ca5af40fb89ad3977b653c448f78c2"]' -p eosio
# echo WTMSIG_BLOCK_SIGNATURES
# /Users/rexardiente/Development/contracts/eosio/eos/build/programs/cleos/cleos push action eosio activate '["299dcb6af692324b899b39f16d5a530a33062804e41f09dc97e9f156b4476707"]' -p eosio