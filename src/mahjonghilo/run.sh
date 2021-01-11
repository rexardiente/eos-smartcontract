#!/bin/bash 
# for authority of shell command : chmod +x run.sh 
# Run Command: ./run.sh --source 

# Project Setup
PROJECT_NAME="Mahjong-Hi/Lo Quest"
# Global ENV
EXECUTE="> "

# Cleos Wallet Password
# PW5KgmdCJdBjcgfBazCCKvYB8Lbzgrs6AvMhXamKe7FvwQQWYxt1P

UNLOCK_WALLET()
{
    cleos wallet unlock
}

# Create Wallet Acc
CREATE_ACCOUNT_WALLET() 
{   
    echo "${EXECUTE} Creating Wallet [\"mahjonghilo\",\"user1\",\"user2\"] with PRIVATE_KEY=\"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV\""
    cleos create account eosio mahjonghilo EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    # cleos create account eosio user1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    # cleos create account eosio user2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active

}

SET_PERMISSION() 
{
    echo "${EXECUTE} Setting Permission for [\"ghostquest\"] with PUBLICK_KEY=\"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV\""
    cleos set account permission mahjonghilo active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "ghostquest","permission":"eosio.code"},"weight":1}]}'
}
# Compile Contract
COMPILE_CONTRACT() 
{
    echo "${EXECUTE} Compiling ${PROJECT_NAME}"
    eosio-cpp -o mahjonghilo.wasm mahjonghilo.cpp
}
# Deploy Account
DEPLOY_CONTRACT()
{
    echo "${EXECUTE} Deploying ${PROJECT_NAME} into EOSIO Server"
    cleos set contract mahjonghilo ./ -p mahjonghilo@active
}
SHOW_EOSIO_CONTRACT_TABLE()
{
    # Get Table Info
    echo "${EXECUTE} Fetching ${PROJECT_NAME} Tables"
    for TBL in "cleos get table mahjonghilo mahjonghilo users"
    do
        echo "> ${TBL}"
        ${TBL}
    done
}

# Create user and game_defaults
INITIALIZE_GAME()
{
    echo "${EXECUTE} Creating new game for \"user1, user2\"..."
    cleos push action mahjonghilo initialize '["user1"]' -p user1@active #for user1
    # cleos push action mahjonghilo initialize '["user2"]' -p user2@active #for user2
    
}
# Create user and game_defaults
REMOVE_EXISTING_GAME()
{
    echo "${EXECUTE} Deleting existing game for \"user1, user2\"..."
    cleos push action mahjonghilo end '["user1"]' -p user1@active #for user1
    # cleos push action mahjonghilo end '["user2"]' -p user2@active #for user2
}

STARTGAME()
{
    cleos push action mahjonghilo startgame '["user1"]' -p user1@active
}

PLAYHILO()
{
    cleos push action mahjonghilo playhilo '["user1", 0]' -p user1@active
}

DISCARD()
{
    cleos push action mahjonghilo discardtile '[ "user1", 0]' -p user1@active
}

KONGDEC()
{
    cleos push action mahjonghilo dclrkong '[ "user1", [2,3,4,5]]' -p user1@active
}

TRANSFER()
{
    cleos push action eosio.token transfer '[ "user1", "mahjonghilo", "2.0000 EOS", "BTTL_LMT=10" ]' -p user1@active
    cleos push action eosio.token transfer '[ "user2", "mahjonghilo", "2.0000 EOS", "BTTL_LMT=10" ]' -p user2@active
}


WITHDRAW()
{
    cleos push action mahjonghilo withdraw '[ "user1", 1]' -p user1@active
    cleos push action mahjonghilo withdraw '[ "user2", 1]' -p user2@active
}


GET_CURRENCY()
{
    cleos get currency balance eosio.token mahjonghilo EOS 
    cleos get currency balance eosio.token user1 EOS
    cleos get currency balance eosio.token user2 EOS
}

# cleos set account permission ghostquest active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "ghostquest","permission":"eosio.code"},"weight":1}]}'


# UNLOCK_WALLET
# CREATE_ACCOUNT_WALLET
# SET_PERMISSION
COMPILE_CONTRACT
DEPLOY_CONTRACT
# INITIALIZE_GAME
# STARTGAME
# PLAYHILO
# DISCARD
# KONGDEC
# TRANSFER 
# WITHDRAW
# GET_CURRENCY 
REMOVE_EXISTING_GAME
SHOW_EOSIO_CONTRACT_TABLE