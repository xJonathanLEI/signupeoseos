//
// Created by Hongbo Tang on 2018/7/5.
//

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/action.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/crypto.hpp>
#include <cstring>
using namespace eosio;
using namespace std;

typedef eosio::name account_name;
typedef uint16_t weight_type;
#define CORE_SYMBOL eosio::symbol("CCA",4)


class [[eosio::contract("signupeoseos")]] signupeoseos: public contract {
public:
    signupeoseos(name s, name code, datastream<const char*> ds): eosio::contract(s, code, ds) {};

    [[eosio::action]]
    void transfer(account_name from, account_name to, asset quantity, string memo);
private:
    struct signup_public_key {
        uint8_t        type;
        array<char,33> data;
    };
    struct permission_level_weight {
        permission_level permission;
        weight_type weight;
    };
    struct key_weight {
        signup_public_key key;
        weight_type weight;
    };
    struct wait_weight {
        uint32_t wait_sec;
        weight_type weight;
    };
    struct authority {
        uint32_t threshold;
        vector<key_weight> keys;
        vector<permission_level_weight> accounts;
        vector<wait_weight> waits;
    };
    struct newaccount {
        account_name creator;
        account_name name;
        authority owner;
        authority active;
    };
};

#define N(act)  eosio::name(act).value

#define EOSIO_DISPATCH_EX( TYPE, MEMBERS ) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
        if (action == N("onerror")) \
            eosio_assert(code == N("eosio"), "onerror action's are only valid from the \"eosio\" system account"); \
        if( (code == N("eosio.token") && action == N("transfer") ) ) { \
            switch( action ) { \
                EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
            } \
      } \
   } \
} \


EOSIO_DISPATCH_EX(signupeoseos, (transfer))