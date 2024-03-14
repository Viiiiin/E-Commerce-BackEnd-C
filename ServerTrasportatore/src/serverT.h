#ifndef serverT_h
#define serverT_h

#include "con2redis.h"

class ServerT {
    public: 
        ServerT( const char *nome);

        char *readCommandRedis(int block);

    private: 
        redisContext *c2r;
        const char *nome;
        const char *WRITE_STREAM;
        const char *READ_STREAM;
};



#endif
