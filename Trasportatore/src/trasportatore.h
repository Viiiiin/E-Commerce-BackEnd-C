#ifndef trasportatore_h
#define trasportatore_h

#include "con2redis.h"



class Trasportatore {
    public: 
        Trasportatore(
            const int id,
            const char *ragioneSociale
        ); 
    private: 
        redisContext *c2r;
        const char *READ_STREAM;
        const char *WRITE_STREAM;
};



#endif
