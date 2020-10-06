cleos create account eosio treasurev2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV  -p eosio@active    
 cleos create account eosio bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV  -p eosio@active
 cleos set contract treasurev2 ./ -p treasurev2@active 
 cleos push action treasurev2 init '["bob"]' -p bob@active  
     cleos push action treasurev2 gamestart '["bob", [[1, 0],[0, 0],[2, 0],[3, 0],[4, 0],[5, 0],[6, 0],[7, 0],
        [8, 0],[9, 0],[10, 0],[11, 0],[12, 0],[13, 0],[14, 0],[15, 0]],5]' -p bob@active  
   cleos get table treasurev2 treasurev2 user
        