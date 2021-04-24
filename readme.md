# auto create account

set transfer memo

    accountName-ownerKey
    //or
    accountName-ownerKey-actionKey
    
    


```bash
cleos transfer cochainworld eosio.create "1.0000 CCA" "cochain11111-EOS5e2RCTAMU21HthGpsCxYigAcYqv6oYjbUwarS9r1dEGzwRRSSP"
# or 
cleos transfer cochainworld eosio.create "1.0000 CCA" "cochain11111-EOS5e2RCTAMU21HthGpsCxYigAcYqv6oYjbUwarS9r1dEGzwRRSSP-EOS5a28N3vt6hYAStHyMEMSaozFjCdT4mdaekH9BqrZ32ZCKKdezv"
```





    set symbol
    
    #define CORE_SYMBOL eosio::symbol("CCA",4)