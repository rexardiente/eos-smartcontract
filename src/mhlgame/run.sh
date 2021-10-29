#!/bin/bash 
# for authority of shell command : chmod +x run.sh 
# Run Command: ./run.sh --source 

# Project Setup
PROJECT_NAME="Mahjong-Hi/Lo"
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
    echo "${EXECUTE} Creating Wallet [\"mhlgame\",\"user1\",\"user2\"] with PRIVATE_KEY=\"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV\""
    cleos create account eosio mhlgame EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active

}

SET_PERMISSION() 
{
    echo "${EXECUTE} Setting Permission for [\"mhlgame\"] with PUBLICK_KEY=\"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV\""
    cleos set account permission mhlgame active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "mhlgame","permission":"eosio.code"},"weight":1}]}'
}
# Compile Contract
COMPILE_CONTRACT() 
{
    echo "${EXECUTE} Compiling ${PROJECT_NAME}"
    eosio-cpp -o mhlgame.wasm mhlgame.cpp
}
# Deploy Account
DEPLOY_CONTRACT()
{
    echo "${EXECUTE} Deploying ${PROJECT_NAME} into EOSIO Server"
    cleos set contract mhlgame ./ -p mhlgame@active
}
SHOW_EOSIO_CONTRACT_TABLE()
{
    # Get Table Info
    echo "${EXECUTE} Fetching ${PROJECT_NAME} Tables"
    for TBL in "cleos get table mhlgame mhlgame mjhilos"
    do
        echo "> ${TBL}"
        ${TBL}
    done
}

# Create user and game_defaults
INITIALIZE_GAME()
{
    echo "${EXECUTE} Creating new account for \"user1\"..."
    cleos push action mhlgame mhlinitialze '["1"]' -p mhlgame@active
    # cleos push action mhlgame mhlinitialze '["2"]' -p mhlgame@active #for user2
    
}

END_GAME()
{
    echo "${EXECUTE} Creating new account for \"user1\"..."
    cleos push action mhlgame mhlend '["7"]' -p mhlgame@active
    # cleos push action mhlgame initialize '["user2"]' -p user2@active #for user2
    
}

NEW_GAME()
{
    echo "${EXECUTE} Creating new account for \"user1\"..."
    cleos push action mhlgame mhlresetacc '["1"]' -p mhlgame@active
}

# RESET_GAME()
# {
#     echo "${EXECUTE} Creating new account for \"user1\"..."
#     cleos push action mhlgame reset '["1"]' -p mhlgame@active #for user1
#     # cleos push action mhlgame initialize '["user2"]' -p user2@active #for user2
# }

# Create user and game_defaults
REMOVE_EXISTING_GAME()
{
    echo "${EXECUTE} Deleting existing account for \"user1\"..."
    cleos push action mhlgame end '["1"]' -p mhlgame@active #for user1
    # cleos push action mhlgame end '["user2"]' -p mhlgame@active #for user2
}

PLAYHILO() ## function : playhilo -> to draw a tile, parameters : name 'user', uint8_t 'option'(0=skip,1=low,2=draw,3=high)     note : players can play hi-lo with or without deposit, all drawn tiles will be within gamedata
{
    cleos push action mhlgame mhlplayhilo '["1", 3]' -p mhlgame@active
}

DISCARD() ## function : discardtile -> to discard a tile, parameters : name 'user', uint8_t 'index'(index of the tile desired to be discarded from hand)    note : there will be a message if a player hands reaches certain amount of tiles
{
    cleos push action mhlgame mhldscrdtile '[ "1", 13]' -p mhlgame@active
}

KONGDEC() ## function : dclrkong -> to declare a kong, parameters : name 'user', array<uint8_t> 'index'(indexes of the 4 tiles designated as kong)      note : declaring a kong will automatically draw a tile which is also possible for Hi-Lo
{
    cleos push action mhlgame dclrkong '[ "1", [10,11,12,13]]' -p mhlgame@active
}

TRIALMODE() ## function : starttrial -> to test out winning hand combinations, parameters : name 'user', array<uint8_t> 'index'(full hand array)
{
    cleos push action mhlgame starttrial '["1", 10, [77,78,79,81,82,83,85,86,87,133,134,135,94,95]]' -p mhlgame@active
}

WINDEC() ## function : dclrwinhand -> to declare a winning hand, parameters : name 'user'   note : game data of the user will be used for checking
{ 
    cleos push action mhlgame dclrwinhand '["1"]' -p mhlgame@active
}

TRANSFER()
{
    cleos push action eosio.token transfer '[ "1", "mhlgame", "5.0000 EOS", "MHL Deposit" ]' -p mhlgame@active
    # cleos push action eosio.token transfer '[ "user2", "mhlgame", "2.0000 EOS", "BTTL_LMT=10" ]' -p user2@active
}

START_BET()
{
    cleos push action mhlgame mhlstartbet '[ "1"]' -p mhlgame@active
    # cleos push action mhlgame withdraw '[ "user2"]' -p user2@active
}

TRANSFER_WINNINGS()
{
    cleos push action mhlgame mhlwintrnsfr '[ "1"]' -p mhlgame@active
    # cleos push action mhlgame withdraw '[ "user2"]' -p user2@active
}

ADD_BET()
{
    echo "${EXECUTE} Trigger Set Sail Function..."
    cleos push action mhlgame mhladdbet '["1", "1"]' -p mhlgame@active
}


WITHDRAW()
{
    cleos push action mhlgame mhlwithdraw '[ "1"]' -p mhlgame@active
    # cleos push action mhlgame withdraw '[ "user2"]' -p user2@active
}

RESETBET()
{
    cleos push action mhlgame mhlresetbet '[ "1"]' -p mhlgame@active
    # cleos push action mhlgame withdraw '[ "user2"]' -p user2@active
}

ERASE_ALL()
{
    cleos push action mhlgame del '[5]' -p mhlgame@active
    # cleos push action mhlgame withdraw '[ "user2"]' -p user2@active
}
# ACTION mhlgame::resetbet(int id)


GET_CURRENCY()
{
    cleos get currency balance eosio.token mhlgame EOS 
    cleos get currency balance eosio.token user1 EOS
    # cleos get currency balance eosio.token user2 EOS
}

# cleos set account permission coinicagames active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "coinicagames","permission":"eosio.code"},"weight":1}]}'

# ERASE_ALL
# UNLOCK_WALLET
# CREATE_ACCOUNT_WALLET
# SET_PERMISSION
# COMPILE_CONTRACT
# DEPLOY_CONTRACT
# INITIALIZE_GAME
# ADD_BET
# START_BET
# PLAYHILO
# DISCARD
# KONGDEC
# WINDEC
# RESETBET
# TRANSFER_WINNINGS 
# WITHDRAW
# TRIALMODE
# TRANSFER
END_GAME
# NEW_GAME
# GET_CURRENCY 
# REMOVE_EXISTING_GAME
SHOW_EOSIO_CONTRACT_TABLE