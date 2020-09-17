#!/bin/bash 
# Run Command: ./run.sh --source 

# Project Setup
PROJECT_NAME="Treasure Hunt"
# Global ENV
EXECUTE="> "

UNLOCK_WALLET()
{
    cleos wallet unlock
}

# Create Wallet Acc
CREATE_ACCOUNT_WALLET() 
{   
    echo "${EXECUTE} Creating Wallet [\"admin\",\"user1\",\"user2\"] with PRIVATE_KEY=\"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV\""
    cleos create account eosio admin EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
}
# Compile Contract
COMPILE_CONTRACT() 
{
    echo "${EXECUTE} Compiling ${PROJECT_NAME}"
    eosio-cpp -o treasure.v2.wasm treasure.v2.cpp
}
# Deploy Account
DEPLOY_CONTRACT()
{
    echo "${EXECUTE} Deploying ${PROJECT_NAME} into EOSIO Server"
    cleos set contract admin ./ -p admin@active
}
SHOW_EOSIO_CONTRACT_TABLE()
{
    # Get Table Info
    echo "${EXECUTE} Fetching ${PROJECT_NAME} Tables"
    for TBL in "cleos get table admin admin user" "cleos get table admin admin ticket" "cleos get table --limit 100 admin admin history"
    do
        echo "> ${TBL}"
        ${TBL}
    done
}
# Purchase New Ticket
PURCHASE_NEW_TICKET() 
{
    echo "${EXECUTE} Purchase new 5 Tickets, and display user1 updated balances..."
    cleos push action admin purchase '["user1", 20]' -p user1@active
}
# Create user and game_defaults
INITIALIZE_GAME()
{
    echo "${EXECUTE} Creating new game for \"user1\"..."
    cleos push action admin init '["user1"]' -p user1@active
}
# Create user and game_defaults
REMOVE_EXISTING_GAME()
{
    echo "${EXECUTE} Deleting existing game for \"user1\"..."
    cleos push action admin end '["user1"]' -p user1@active
}
# Set Game Panel Sequence
SET_GAME_PANEL()
{
    echo "${EXECUTE} Configuring \"user1\" Game Panel Set."
    cleos push action admin setpanel '["user1", [[1, 0],[0, 0],[2, 0],[3, 0],[4, 0],[5, 0],[6, 0],[7, 0],
        [8, 0],[9, 0],[10, 0],[11, 0],[12, 0],[13, 0],[14, 0],[15, 0]]]' -p user1@active
}
# Set Game Destination
SET_DESTINATION()
{
    echo "${EXECUTE} Configuring \"user1\" Game Destination."
    cleos push action admin setdest '["user1", 1]' -p user1@active
}
# Set Game Destination
SET_EXPLORER()
{
    echo "${EXECUTE} Configuring \"user1\" Game Explorer Count."
    cleos push action admin setexplr '["user1", 2]' -p user1@active
}
# Enable Set Sail
SET_SAIL()
{
    echo "${EXECUTE} Trigger Set Sail Function..."
    cleos push action admin setsail '["user1", true]' -p user1@active
}
# Enable Set Sail
GET_PRIZE()
{
    echo "${EXECUTE} Trigger Generate Prize Function..."
    cleos push action admin genprize '["user1", 4]' -p user1@active
}
RENEW_MAP()
{
    echo "${EXECUTE} Trigger Renew Map Function..."
    cleos push action admin renew '["user1"]' -p user1@active
}

# UNLOCK_WALLET
# CREATE_ACCOUNT_WALLET
# COMPILE_CONTRACT
# DEPLOY_CONTRACT
# PURCHASE_NEW_TICKET
# INITIALIZE_GAME
# SET_GAME_PANEL
# SET_DESTINATION
# SET_EXPLORER
SET_SAIL
GET_PRIZE
# RENEW_MAP
# REMOVE_EXISTING_GAME
SHOW_EOSIO_CONTRACT_TABLE