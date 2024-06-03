#ifndef serverP_h
#define serverP_h

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

class ServerP {
    public:
        ServerP(const char *nome);

        // Reads the last message sent by the client and extracts the command
        Cmd_Reply readCommandRedis(int block);

        // Reads the product characteristics from Redis and saves the product in the database
        void inserisciProdotto(int block, redisReply *reply);

        // Removes a product from the database
        void rimuoviProdotto(int block, redisReply *reply);
   
    private: 
        int monitor(char *idProdotto, char *produttore);
        const char *nome;
        redisContext *c2r;
        Con2DB db;
        const char *READ_STREAM;
        const char *WRITE_STREAM;
};

#endif // serverP_h
