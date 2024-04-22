#ifndef serverT_h
#define serverT_h

#include "con2redis.h"
#include "pgsql.h" // Include the header file for con2DB
#include "../../tools/src/main.h"

class ServerT {
public: 
    ServerT(const char *nome);

    char *readCommandRedis(int block);

private: 
    redisContext *c2r;
    Con2DB *db; // Declare a member for con2DB
    const char *nome;
    const char *WRITE_STREAM;
    const char *READ_STREAM;
};

#endif
