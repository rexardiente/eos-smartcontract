#!/bin/bash 
# for authority of shell command : chmod +x run.sh 
# Run Command: ./run.sh --source 

# Project Setup
PROJECT_NAME="Treasure Hunt"
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
    echo "${EXECUTE} Creating Wallet [\"treasurev2\",\"user1\",\"user2\"] with PRIVATE_KEY=\"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV\""
    cleos create account eosio treasurehunt EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
}


# Set Permission
# cleos set account permission treasurehunt active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "mahjonghilo","permission":"eosio.code"},"weight":1}]}'
# Compile Contract
COMPILE_CONTRACT() 
{
    echo "${EXECUTE} Compiling ${PROJECT_NAME}"
    eosio-cpp -o treasurehunt.wasm treasurehunt.cpp
}
# Deploy Account
DEPLOY_CONTRACT()
{
    echo "${EXECUTE} Deploying ${PROJECT_NAME} into EOSIO Server"
    cleos set contract treasurehunt ./ -p treasurehunt@active
}
SHOW_EOSIO_CONTRACT_TABLE()
{
    # Get Table Info
    echo "${EXECUTE} Fetching ${PROJECT_NAME} Tables"
    for TBL in "cleos get table treasurehunt treasurehunt users"
    do
        echo "> ${TBL}"
        ${TBL}
    done
}

# Create user and game_defaults
INITIALIZE_GAME()
{
    echo "${EXECUTE} Creating new game for \"user1\"..."
    cleos push action treasurehunt initialize '["1"]' -p treasurehunt@active
}
# Create user and game_defaults
REMOVE_EXISTING_GAME()
{
    echo "${EXECUTE} Deleting existing game for \"user1\"..."
    cleos push action treasurehunt end '["1"]' -p treasurehunt@active
}
# Set Game Panel Sequence
SET_GAME_PANEL()
{
    echo "${EXECUTE} Configuring \"user1\" Game Panel Set."
    cleos push action treasurehunt setpanel '["1", [0,1,2,3,4,5,6,7,
        8,9,10,11,12,13,14,15]]' -p treasurehunt@active
}
# Set Game Destination
SET_DESTINATION()
{
    echo "${EXECUTE} Configuring \"user1\" Game Destination."
    cleos push action treasurehunt destination '["1", 1]' -p treasurehunt@active
}

# Set Game Destination
SET_ENEMY()
{
    echo "${EXECUTE} Configuring \"user1\" Game Destination."
    cleos push action treasurehunt setenemy '["1", 5]' -p treasurehunt@active
}


GAME_START()
{
    echo "${EXECUTE} Trigger Set Sail Function..."
    cleos push action treasurehunt gamestart '["1", "1"]' -p treasurehunt@active
}

SELECT_TILE()
{
    echo "${EXECUTE} Trigger Generate Prize Function..."
    cleos push action treasurehunt opentile '["1", 7]' -p treasurehunt@active
}

TRANSFER()
{
    cleos push action eosio.token transfer '[ "1", "treasurehunt", "1.0000 EOS", "m" ]' -p user1@active
}


WITHDRAW()
{
    cleos push action treasurehunt withdraw '["1"]' -p treasurehunt@active
}

GET_CURRENCY()
{
    cleos get currency balance eosio.token treasurehunt EOS
    cleos get currency balance eosio.token user1 EOS
}

AUTOPLAY()
{
    echo "${EXECUTE} Trigger AutoPlay Function..."
    cleos push action treasurehunt autoplay '["1", [1,5]]' -p treasurehunt@active
}

# UNLOCK_WALLET
# CREATE_ACCOUNT_WALLET
# COMPILE_CONTRACT
# DEPLOY_CONTRACT 
# INITIALIZE_GAME
# SET_DESTINATION
# SET_GAME_PANEL
# SET_ENEMY
# GAME_START
# TRANSFER
# AUTOPLAY
# SELECT_TILE
# WITHDRAW
# GET_CURRENCY
REMOVE_EXISTING_GAME
SHOW_EOSIO_CONTRACT_TABLE