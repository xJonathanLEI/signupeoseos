//
// Created by Hongbo Tang on 2018/7/5.
//

#include "signupeoseos.hpp"
#include "abieos_numeric.hpp"

void signupeoseos::transfer(account_name from, account_name to, asset quantity, string memo) {
    if (from == _self || to != _self) {
        return;
    }
    eosio_assert(quantity.symbol == CORE_SYMBOL, "signupeoseos only accepts CORE for signup eos account");
    eosio_assert(quantity.is_valid(), "Invalid token transfer");
    eosio_assert(quantity.amount > 0, "Quantity must be positive");

    eosio_assert(memo.length() == 120 || memo.length() == 66, "Malformed Memo (not right length)");

    eosio_assert(memo[12] == ':' || memo[12] == '-', "Malformed Memo [12] == : or -");

    array<char, 33> owner_pubkey_char;
    array<char, 33> active_pubkey_char;
    const string owner_key_str = memo.substr(13, 53);
    string active_key_str;
    if(memo[66] == ':' || memo[66] == '-') {
        // active key provided
        active_key_str = memo.substr(67, 53);
    } else {
        // active key is the same as owner
        active_key_str =  owner_key_str;
    }

    const abieos::public_key owner_pubkey =
            abieos::string_to_public_key(owner_key_str);
    const abieos::public_key active_pubkey =
            abieos::string_to_public_key(active_key_str);

    copy(owner_pubkey.data.begin(), owner_pubkey.data.end(),
         owner_pubkey_char.begin());

    copy(active_pubkey.data.begin(), active_pubkey.data.end(),
         active_pubkey_char.begin());

    const account_name new_account_name(memo.substr(0, 12).c_str());

    asset stake_net(1000, CORE_SYMBOL);
    asset stake_cpu(1000, CORE_SYMBOL);
    asset buy_ram = quantity - stake_net - stake_cpu;
    eosio_assert(buy_ram.amount > 0, "Not enough balance to buy ram");

    auto get_auth = [](array<char, 33>& pubkey_char)
    {
        return authority{
                .threshold = 1,
                .keys = {
                        key_weight{
                                .key = signup_public_key{
                                        .type = (uint8_t)abieos::key_type::k1,
                                        .data = pubkey_char,
                                },
                                .weight = 1
                        }
                },
                .accounts = {},
                .waits = {}
        };
    };
    authority owner_auth = get_auth(owner_pubkey_char);
    authority active_auth = get_auth(active_pubkey_char);

    newaccount new_account = newaccount{
        .creator = _self,
        .name = new_account_name,
        .owner = owner_auth,
        .active = active_auth
    };

    action(
            permission_level{ _self, name("active") },
            name("eosio"),
            name("newaccount"),
            new_account
    ).send();

    action(
            permission_level{ _self, name("active")},
            name("eosio"),
            name("buyram"),
            make_tuple(_self, new_account_name, buy_ram)
    ).send();

    action(
            permission_level{ _self, name("active")},
            name("eosio"),
            name("delegatebw"),
            make_tuple(_self, new_account_name, stake_net, stake_cpu, true)
    ).send();
}