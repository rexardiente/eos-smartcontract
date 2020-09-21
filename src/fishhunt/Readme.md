cleos create account eosio fishhunt  EOS6MRyAjQq8ud7hVCNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV -p eosio@active
 cleos set contract fishhunt  ./ -p fishhunt@active
cleos create account eosio bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV                    

cleos push action fishhunt  initgames '[ "bob"]' -p bob@active                                          
cleos push action fishhunt  playerticket '[ "bob",10]' -p bob@active                  
cleos push action fishhunt setuppanel '["bob", [[1, 0],[0, 0],[2, 0],[3, 0],[4, 0],[5, 0],[6, 0],[7, 0],
        [8, 0],[9, 0],[10, 0],[11, 0],[12, 0],[13, 0],[14, 0],[15, 0],[16,0],[17,0],[18,0]],[19,0],[20,0],[21,0],[23,0],24,0]]]' -p bob@active
cleos push action fishhunt  destlake '[ "bob",1]' -p bob@active                        
cleos push action fishhunt  sethuntfish '[ "bob",true]' -p bob@active      
