#!/bin/bash 
# for authority of shell command : chmod +x run.sh 
# Run Command: ./run.sh --source 

# Project Setup
PROJECT_NAME="Ghost Quest"
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
    # cleos create account eosio ghostquest EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user3 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user4 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user5 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user11 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user12 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user13 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user14 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user15 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user21 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
    cleos create account eosio user22 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active

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
    cleos push action ghostquest initialize '["user1"]' -p user1@active #for user1
    cleos push action ghostquest initialize '["user2"]' -p user2@active #for user2
    cleos push action ghostquest initialize '["user3"]' -p user3@active #for user1
    cleos push action ghostquest initialize '["user4"]' -p user4@active #for user2
    cleos push action ghostquest initialize '["user5"]' -p user5@active #for user2
    cleos push action ghostquest initialize '["user11"]' -p user11@active #for user2
    cleos push action ghostquest initialize '["user12"]' -p user12@active #for user2
    cleos push action ghostquest initialize '["user13"]' -p user13@active #for user2
    cleos push action ghostquest initialize '["user14"]' -p user14@active #for user2
    cleos push action ghostquest initialize '["user15"]' -p user15@active #for user2
    cleos push action ghostquest initialize '["user21"]' -p user21@active #for user2
    cleos push action ghostquest initialize '["user22"]' -p user22@active #for user2
    # cleos push action ghostquest initialize '["user3"]' -p user3@active #for user3
    # cleos push action ghostquest initialize '["user4"]' -p user4@active #for user4
    
}
# Create user and game_defaults
REMOVE_EXISTING_GAME()
{
echo "${EXECUTE} Deleting existing game for \"user1, user2\"..."
cleos push action ghostquest end '["user1"]' -p user1@active #for user1
cleos push action ghostquest end '["user2"]' -p user2@active #for user2
cleos push action ghostquest end '["user3"]' -p user3@active #for user1
cleos push action ghostquest end '["user4"]' -p user4@active #for user2
cleos push action ghostquest end '["user5"]' -p user5@active #for user2
cleos push action ghostquest end '["user11"]' -p user11@active #for user2
cleos push action ghostquest end '["user12"]' -p user12@active #for user2
cleos push action ghostquest end '["user13"]' -p user13@active #for user2
cleos push action ghostquest end '["user14"]' -p user14@active #for user2
cleos push action ghostquest end '["user15"]' -p user15@active #for user2
cleos push action ghostquest end '["user21"]' -p user21@active #for user2
cleos push action ghostquest end '["user22"]' -p user22@active #for user2
}

# SET_SUMMON_COUNT()
# {
#     echo "${EXECUTE} Configuring \"user1\" Game Destination."
#     cleos push action ghostquest summoncount '["user1", 4, 10]' -p user1@active #for user1
#     cleos push action ghostquest summoncount '["user2", 4, 10]' -p user2@active #for user2
# }

TRANSFER()
{
    cleos push action eosio.token transfer '[ "user1", "ghostquest", "1.0000 EOS", "BTTL_LMT=10" ]' -p user1@active

    cleos push action eosio.token transfer '[ "user1", "ghostquest", "3.0000 EOS", "BTTL_LMT=10" ]' -p user1@active
    cleos push action eosio.token transfer '[ "user2", "ghostquest", "3.0000 EOS", "BTTL_LMT=10" ]' -p user2@active
    cleos push action eosio.token transfer '[ "user3", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user3@active
    cleos push action eosio.token transfer '[ "user4", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user4@active
    cleos push action eosio.token transfer '[ "user5", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user5@active
    cleos push action eosio.token transfer '[ "user11", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user11@active
    cleos push action eosio.token transfer '[ "user12", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user12@active
    cleos push action eosio.token transfer '[ "user13", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user13@active
    cleos push action eosio.token transfer '[ "user14", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user14@active
    cleos push action eosio.token transfer '[ "user15", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user15@active
    cleos push action eosio.token transfer '[ "user21", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user21@active
    cleos push action eosio.token transfer '[ "user22", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user22@active

    # cleos push action eosio.token transfer '[ "user3", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user3@active
    # cleos push action eosio.token transfer '[ "user4", "ghostquest", "2.0000 EOS", "BTTL_LMT=10" ]' -p user4@active
    # cleos push action eosio.token transfer '[ "user1", "ghostquest", "1.0000 EOS", "ADD_LIFE=1" ]' -p user1@active
    # cleos push action eosio.token transfer '[ "user2", "ghostquest", "1.0000 EOS", "ADD_LIFE=1" ]' -p user2@active
    # cleos push action eosio.token transfer '[ "user3", "ghostquest", "1.0000 EOS", "ADD_LIFE=1" ]' -p user3@active
    # cleos push action eosio.token transfer '[ "user4", "ghostquest", "1.0000 EOS", "ADD_LIFE=1" ]' -p user4@active
}


BATTLE()
{
    cleos push action ghostquest battle '["ba8b8ba1-3512-467f-bc6d-5172efe409a", ["c8bb55ef276eab413040ece9b459b92","user1"], ["0228519bf708701ec24108a35c6e910","user2"], ["FIRST ROUND", "SECOND ROUND"]]' -p ghostquest@active
    # cleos push action ghostquest battle '[ [[1,"user1"], [1,"user2"]], "ba8b8ba1-3512-467f-bc6d-5172efe4097b"]' -p ghostquest@active
    # cleos push action ghostquest battle '[ [[1,"user4"], [1,"user2"]], "ba8b8ba1-3512-467f-bc6d-5172efe4097d"]' -p ghostquest@active
    # cleos push action ghostquest battle '[ [[2,"user4"], [2,"user3"]], "ba8b8ba1-3512-467f-bc6d-5172efe4097e"]' -p ghostquest@active
}

WITHDRAW()
{
    cleos push action ghostquest withdraw '[ "user11", 1]' -p user1@active
    cleos push action ghostquest withdraw '[ "user2", 1]' -p user2@active
}

ELIMINATE()
{
    cleos push action ghostquest eliminate '[ "user1", 1]' -p user1@active
}

GET_CURRENCY()
{
    cleos get currency balance eosio.token ghostquest EOS 
    cleos get currency balance eosio.token user1 EOS
    cleos get currency balance eosio.token user2 EOS
    cleos get currency balance eosio.token user3 EOS
    cleos get currency balance eosio.token user4 EOS
}

# cleos set account permission ghostquest active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor": "ghostquest","permission":"eosio.code"},"weight":1}]}'


#UNLOCK_WALLET
# CREATE_ACCOUNT_WALLET
# SET_PERMISSION
COMPILE_CONTRACT
DEPLOY_CONTRACT
# INITIALIZE_GAME
# TRANSFER   # note : transfer has two types, for summon and for add life
# BATTLE
# WITHDRAW
# ELIMINATE
# GET_CURRENCY 
# SHOW_EOSIO_CONTRACT_TABLE
# REMOVE_EXISTING_GAME