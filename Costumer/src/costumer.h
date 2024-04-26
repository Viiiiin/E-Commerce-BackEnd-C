#ifndef costumer_h
#define costumer_h

#include "con2redis.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class Costumer {
public:
    Costumer(int id, const char *nome, const char *cognome);
    string acquistaProdotto(int prod);

private:
    redisContext *c2r;
    const char *READ_STREAM;
    const char *WRITE_STREAM;
    const char *nome;
    const char *cognome;
    int id;
};

#endif
