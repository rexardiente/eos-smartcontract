#!/bin/bash 
# for authority of shell command : chmod +x run.sh 
# Run Command: ./run.sh --source 

# Project Setup
PROJECT_NAME="Ghost Quest"
# Global ENV
EXECUTE="> "

# Cleos Wallet Password
# PW5KgmdCJdBjcgfBazCCKvYB8Lbzgrs6AvMhXamKe7FvwQQWYxt1P
# PW5KNqqcca9vDQ1UBDYwejsY78GwLay3RP7qWG9EaabsjvdCozFWF -- DonutFactory1 MacBook


UNLOCK_WALLET()
{
    cleos wallet unlock
}

# Create Wallet Acc
CREATE_ACCOUNT_WALLET() 
{   
    echo "${EXECUTE} Creating Wallet [\"treasurev2\",\"user1\",\"user2\"] with PRIVATE_KEY=\"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV\""
    cleos create account eosio ghostquest EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
}

SET_PERMISSION() 
{
    echo "${EXECUTE} Setting Permission for [\"ghostquest\"] with PUBLICK_KEY=\"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV\""
    cleos set account permission ghostquest active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "ghostquest","permission":"eosio.code"},"weight":1}]}'
}
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
    cleos set contract ghostquest ./ -p ghostquest@active
}
SHOW_EOSIO_CONTRACT_TABLE()
{
    # Get Table Info
    echo "${EXECUTE} Fetching ${PROJECT_NAME} Tables"
    for TBL in "cleos get table ghostquest ghostquest users"
    do
        echo "> ${TBL}"
        ${TBL}
    done
}

# Create user and game_defaults
INITIALIZE_GAME()
{
    echo "${EXECUTE} Creating new game for \"user1, user2\"..."
    cleos push action ghostquest initialize '["1"]' -p ghostquest@active #for user1
    # cleos push action ghostquest initialize '["user2"]' -p user2@active #for user2
    
}

# generate character
GEN_CHAR()
{
    cleos push action ghostquest genchar '["1", 1, 5]' -p ghostquest@active 
}



# add life to characters

ADD_LIFE()
{
    cleos push action ghostquest addlife '["1", 1, "67f5a86fe2278fcd6a4b352fb4e0cb0"]' -p ghostquest@active 
}

# Create user and game_defaults
REMOVE_EXISTING_GAME()
{
    echo "${EXECUTE} Deleting existing game for \"user1, user2\"..."
    cleos push action ghostquest end '["1"]' -p ghostquest@active #for user1
    # cleos push action ghostquest end '["user2"]' -p user2@active #for user2
}

# SET_SUMMON_COUNT()
# {
#     echo "${EXECUTE} Configuring \"user1\" Game Destination."
#     cleos push action ghostquest summoncount '["1", 4, 10]' -p ghostquest@active #for user1
#     cleos push action ghostquest summoncount '["user2", 4, 10]' -p user2@active #for user2
# }

# TRANSFER()
# {
#     # cleos push action eosio.token transfer '[ "1", "ghostquest", "5.0000 EOS", "BTTL_LMT=10" ]' -p ghostquest@active
#     # cleos push action eosio.token transfer '[ "user2", "ghostquest", "5.0000 EOS", "BTTL_LMT=10" ]' -p user2@active
#     # cleos push action eosio.token transfer '[ "1", "ghostquest", "2.0000 EOS", "ADD_LIFE=1" ]' -p ghostquest@active
#     # cleos push action eosio.token transfer '[ "user2", "ghostquest", "2.0000 EOS", "ADD_LIFE=1" ]' -p user2@active
# }


BATTLE()
{
    cleos push action ghostquest battle '[ "1", 1, "user2", 1]' -p ghostquest@active
}

WITHDRAW()
{
    cleos push action ghostquest withdraw '[ "1", 1]' -p ghostquest@active
    # cleos push action ghostquest withdraw '[ "user2", 1]' -p user2@active
}

ELIMINATE()
{
    cleos push action ghostquest eliminate '[ "1", "67f5a86fe2278fcd6a4b352fb4e0cb0"]' -p ghostquest@active
}

GET_CURRENCY()
{
    cleos get currency balance eosio.token ghostquest EOS 
    cleos get currency balance eosio.token user1 EOS
    cleos get currency balance eosio.token user2 EOS
}

ERASE_ALL()
{
    cleos push action ghostquest delall '[5]' -p ghostquest@active
}

# cleos set account permission ghostquest active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "ghostquest","permission":"eosio.code"},"weight":1}]}'


# ERASE_ALL
# UNLOCK_WALLET
# CREATE_ACCOUNT_WALLET
# SET_PERMISSION
# COMPILE_CONTRACT
DEPLOY_CONTRACT
# INITIALIZE_GAME
# GEN_CHAR
# ADD_LIFE
# TRANSFER   # note : transfer has two types, for summon and for add life
# BATTLE
# WITHDRAW
# ELIMINATE
# GET_CURRENCY 
# REMOVE_EXISTING_GAME
SHOW_EOSIO_CONTRACT_TABLE