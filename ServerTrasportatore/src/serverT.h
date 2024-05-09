#ifndef serverT_h
#define serverT_h


#include "con2redis.h"
#include "pgsql.h" // Include the header file for con2DB
#include <string>
#include <iostream>
#include "../../tools/src/main.h"

using namespace std;

struct Cmd_Reply{
    string cmd;
    redisReply *reply;
};
class ServerT {
public: 
    ServerT(const char *nome);

    Cmd_Reply readCommandRedis(int block);
    
    void consegnaProdotto(int block,redisReply *reply);
    // void getNonConsegnati(int block,redisReply *reply);

private: 
    int monitor(char* idAcquisto,char *trasportatore);
    redisContext *c2r;
    Con2DB db; // Declare a member for con2DB
    const char *nome;
    const char *WRITE_STREAM;
    const char *READ_STREAM;
};

#endif
