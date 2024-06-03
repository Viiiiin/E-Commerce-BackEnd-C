#ifndef serverT_h
#define serverT_h

#include "con2redis.h"
#include "pgsql.h" // Include the header file for con2DB
#include <string>
#include <iostream>
#include "../../tools/src/main.h"

using namespace std;

// Structure to hold the command and its reply
struct Cmd_Reply{
    string cmd;
    redisReply *reply;
};

class ServerT {
public:
    // Constructor
    ServerT(const char *nome);

    // Read the last message sent by the client and extract the command
    Cmd_Reply readCommandRedis(int block);

    // Check if the transport is valid for the given purchase and deliver the product
    void consegnaProdotto(int block, redisReply *reply);

private:
    // Monitor the status of a purchase for delivery
    int monitor(char *idAcquisto, char *trasportatore);

    // Member variables
    const char *nome;
    redisContext *c2r;
    Con2DB db;
    const char *READ_STREAM;
    const char *WRITE_STREAM;
};

#endif // serverT_h
