# Treasure Hunt | Smart Contract Documentation

![](https://i.imgur.com/q5O2tTg.jpg)

### Overview & Goals
This repository contains files, code, and resources for smart contracts associated with the Treasure Hunt game. 

#### Note:
    - We can add Game_ID as parameter in every action as an option.
    - Automatically end game once Explore count is empty and add to history.

#### Private Key: 
    PW5J9srswAQKDMAhCKHer5pdKczsq5heMafi9DSqT28YrgvuDXApV
    
#### Get Table Info
    cleos get table treasurev2 treasurev2 user
    cleos get table treasurev2 treasurev2 ticket
    cleos get table treasurev2 treasurev2 history

### Testing Process
1. Create ticket inorder to use the game using purchase action.
2. Create User/Login using init action.
3. Start the game using startgame action.
4. set sail button will allow user user to select any available panel, once selected a panel it generate prize using genprize action
5. renew map using renew action
6. quit or remove current game setting using end action

### Test Action
```
void treasurev2::test(name username) {
    vector<treasurev2::opened_panel> set ={
        { 1, UNOPENED },
        { 0, UNOPENED },
        { 2, OPENED },
        { 3, OPENED },
        { 4, OPENED },
        { 5, UNOPENED },
        { 6, UNOPENED },
        { 7, UNOPENED },
        { 8, UNOPENED },
        { 9, UNOPENED },
        { 10, UNOPENED },
        { 11, UNOPENED },
        { 12, UNOPENED },
        { 13, UNOPENED },
        { 14, UNOPENED },
        { 15, UNOPENED } };

    <!-- login or create user -->
    init(username);
    <!-- Start game -->
    startgame(username, MAP_1, EXPLORE_1, set);
    <!-- remove current user.. -->
    end(username);
    <!-- renew map -->
    renew(username, true);
    <!-- purchase new ticket and view balance -->
    purchase(username, 200);
    ticket_balance(username);

    <!-- generate prize -->
    genprize(username, 6);
};
