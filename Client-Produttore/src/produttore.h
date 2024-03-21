#ifndef produttore_h
#define produttore_h

#include "con2redis.h"

class Produttore {
    public: 

        Produttore(const int id);

        void inserisciProdotto(char *nome, char *descrizione);
    private: 
        int id;
        redisContext *c2r;
        const char *READ_STREAM;
        const char *WRITE_STREAM;
};



#endif
