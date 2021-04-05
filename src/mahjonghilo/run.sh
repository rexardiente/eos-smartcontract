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
    echo "${EXECUTE} Creating Wallet [\"mahjonghilo\",\"user1\",\"user2\"] with PRIVATE_KEY=\"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV\""
    cleos create account eosio mahjonghilo EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active

}

SET_PERMISSION() 
{
    echo "${EXECUTE} Setting Permission for [\"mahjonghilo\"] with PUBLICK_KEY=\"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV\""
    cleos set account permission mahjonghilo active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "mahjonghilo","permission":"eosio.code"},"weight":1}]}'
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
    echo "${EXECUTE} Creating new account for \"user1\"..."
    cleos push action mahjonghilo initialize '["user1"]' -p mahjonghilo@active #for user1
    # cleos push action mahjonghilo initialize '["user2"]' -p user2@active #for user2
    
}

END_GAME()
{
    echo "${EXECUTE} Creating new account for \"user1\"..."
    cleos push action mahjonghilo endgame '["user1"]' -p user1@active #for user1
    # cleos push action mahjonghilo initialize '["user2"]' -p user2@active #for user2
    
}

# RESET_GAME()
# {
#     echo "${EXECUTE} Creating new account for \"user1\"..."
#     cleos push action mahjonghilo reset '["user1"]' -p mahjonghilo@active #for user1
#     # cleos push action mahjonghilo initialize '["user2"]' -p user2@active #for user2
    
# }

# Create user and game_defaults
REMOVE_EXISTING_GAME()
{
    echo "${EXECUTE} Deleting existing account for \"user1\"..."
    cleos push action mahjonghilo end '["user1"]' -p mahjonghilo@active #for user1
    # cleos push action mahjonghilo end '["user2"]' -p user2@active #for user2
}

PLAYHILO() ## function : playhilo -> to draw a tile, parameters : name 'user', uint8_t 'option'(0=skip,1=low,2=draw,3=high)     note : players can play hi-lo with or without deposit, all drawn tiles will be within gamedata
{
    cleos push action mahjonghilo playhilo '["user1", 1]' -p user1@active
}

DISCARD() ## function : discardtile -> to discard a tile, parameters : name 'user', uint8_t 'index'(index of the tile desired to be discarded from hand)    note : there will be a message if a player hands reaches certain amount of tiles
{
    cleos push action mahjonghilo discardtile '[ "user1", 5]' -p user1@active
}

KONGDEC() ## function : dclrkong -> to declare a kong, parameters : name 'user', array<uint8_t> 'index'(indexes of the 4 tiles designated as kong)      note : declaring a kong will automatically draw a tile which is also possible for Hi-Lo
{
    cleos push action mahjonghilo dclrkong '[ "user1", [0,1,2,3]]' -p user1@active
}

TRIALMODE() ## function : starttrial -> to test out winning hand combinations, parameters : name 'user', array<uint8_t> 'index'(full hand array)
{
    cleos push action mahjonghilo starttrial '["user1", 10, [35,36,70,72,71,103,102,101,106,107,108,123,122,121]]' -p user1@active
}

WINDEC() ## function : dclrwinhand -> to declare a winning hand, parameters : name 'user'   note : game data of the user will be used for checking
{ 
    cleos push action mahjonghilo dclrwinhand '["user1"]' -p user1@active
}

TRANSFER()
{
    cleos push action eosio.token transfer '[ "user1", "mahjonghilo", "5.0000 EOS", "MHL Deposit" ]' -p user1@active
    # cleos push action eosio.token transfer '[ "user2", "mahjonghilo", "2.0000 EOS", "BTTL_LMT=10" ]' -p user2@active
}


WITHDRAW()
{
    cleos push action mahjonghilo withdraw '[ "user1"]' -p user1@active
    # cleos push action mahjonghilo withdraw '[ "user2"]' -p user2@active
}


GET_CURRENCY()
{
    cleos get currency balance eosio.token mahjonghilo EOS 
    cleos get currency balance eosio.token user1 EOS
    # cleos get currency balance eosio.token user2 EOS
}

# cleos set account permission mahjonghilo active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "mahjonghilo","permission":"eosio.code"},"weight":1}]}'


# UNLOCK_WALLET
# CREATE_ACCOUNT_WALLET
# SET_PERMISSION
# COMPILE_CONTRACT
# DEPLOY_CONTRACT
# INITIALIZE_GAME
# TRANSFER
# PLAYHILO
# DISCARD
# KONGDEC
# WINDEC
# END_GAME 
# WITHDRAW
# TRIALMODE
# GET_CURRENCY 
REMOVE_EXISTING_GAME
SHOW_EOSIO_CONTRACT_TABLE