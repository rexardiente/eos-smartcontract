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
    cleos push action treasurehunt initialize '["user1"]' -p user1@active
}
# Create user and game_defaults
REMOVE_EXISTING_GAME()
{
    echo "${EXECUTE} Deleting existing game for \"user1\"..."
    cleos push action treasurehunt end '["user1"]' -p user1@active
}
# Set Game Panel Sequence
SET_GAME_PANEL()
{
    echo "${EXECUTE} Configuring \"user1\" Game Panel Set."
    cleos push action treasurehunt setpanel '["user1", [1,0,2,3,4,5,6,7,
        8,9,10,11,12,13,14,15]]' -p user1@active
}
# Set Game Destination
SET_DESTINATION()
{
    echo "${EXECUTE} Configuring \"user1\" Game Destination."
    cleos push action treasurehunt destination '["user1", 10]' -p user1@active
}

# Set Game Destination
SET_ENEMY()
{
    echo "${EXECUTE} Configuring \"user1\" Game Destination."
    cleos push action treasurehunt setenemy '["user1", 5]' -p user1@active
}

# Enable Set Sail
GAME_START()
{
    echo "${EXECUTE} Trigger Set Sail Function..."
    cleos push action treasurehunt gamestart '["user1", ]' -p user1@active
}

SELECT_TILE()
{
    echo "${EXECUTE} Trigger Generate Prize Function..."
    cleos push action treasurehunt opentile '["user1", 1]' -p user1@active
}

TRANSFER()
{
    cleos push action eosio.token transfer '[ "user1", "treasurehunt", "10.0000 EOS", "m" ]' -p user1@active
}

WITHDRAW()
{
    cleos push action treasurehunt withdraw '["user1"]' -p user1@active
}

GET_CURRENCY()
{
    cleos get currency balance eosio.token treasurehunt EOS
    cleos get currency balance eosio.token user1 EOS
}

# cleos set account permission treasurehunt active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "treasurehunt","permission":"eosio.code"},"weight":1}]}'

# RENEW_MAP()
# {
#     echo "${EXECUTE} Trigger Renew Map Function..."
#     cleos push action treasurev2 renew '["user1"]' -p user1@active
# }

# UNLOCK_WALLET
# CREATE_ACCOUNT_WALLET
# COMPILE_CONTRACT
# DEPLOY_CONTRACT
# INITIALIZE_GAME
# SET_GAME_PANEL
# SET_DESTINATION
# SET_ENEMY
# TRANSFER
# SELECT_TILE
# WITHDRAW
GET_CURRENCY
# REMOVE_EXISTING_GAME
# SHOW_EOSIO_CONTRACT_TABLE