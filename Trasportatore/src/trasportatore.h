#ifndef trasportatore_h
#define trasportatore_h

#include "con2redis.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class Trasportatore {
    public: 
        Trasportatore(
            const int id
        ); 

        // Delivery a product
        string consegnaProdotto(int idAcquisto);

        // int* getNonConsegnati();

    private: 
        redisContext *c2r;
        const char *READ_STREAM;
        const char *WRITE_STREAM;
        int id;
};



#endif
