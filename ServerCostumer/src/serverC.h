#ifndef serverC_h
#define serverC_h

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
class ServerC {
    public:
        ServerC(const char *nome);

        // Legge l' ultimo messaggio  inviato da client e ne ricava il comando
        Cmd_Reply readCommandRedis(int block);

        // Legge le caratteristiche del prodotto da redis e salva prodotto nel database
        int acquistaProdotto(int block,redisReply *reply);
   
    private: 
        const char *nome;
        redisContext *c2r;
        const char *READ_STREAM;
        const char *WRITE_STREAM;
        Con2DB db;
};


#endif
