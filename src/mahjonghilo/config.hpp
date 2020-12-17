#define MAIN_TOKEN "EOS" //EOS/TLOS/BOS/MEETONE
#define PRECISION 4
#define MAIN_CONTRACT "eosio.token"
#define DELAYSETTLE 1 //1 second, adjust to prevent result attack
#define HAS_ON_SETTLE_PAY "Mahjong Withdraw:"
#define DEFAULT_ASSET asset(0, symbol(MAIN_TOKEN, PRECISION))