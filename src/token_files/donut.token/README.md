## Donut.token 

This folder contains contract code and files related to donut.token. 

Pre-requsite: EOS BIOSBOOT installed (or an alternative method.)

1. Start EOS.

```
cd ~/biosboot/genesis/
chmod 755 start.sh
./start.sh
```

2. To check that blocks are being produced, run the following command:
```
cd ~/biosboot/genesis/
tail -f ./blockchain/nodeos.log
```

3. Now that EOS is running, create an account called "donut.token"

Once created, compile the contracts using the following command:
```
eosio-cpp -I donut.token.hpp donut.token.cpp
```

4. Then move the set the contract using the following command:
```
cleos set contract donut.token donut.token -p donut.token@active
```

*Note that the code is experimental and subject to change.*
