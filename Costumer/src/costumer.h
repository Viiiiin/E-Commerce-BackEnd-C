#ifndef costumer_h
#define costumer_h

#include "con2redis.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class Costumer {
public:
    // Constructor to initialize a costumer with an ID
    Costumer(int id);

    // Method to simulate the purchase of a product by a costumer
    string acquistaProdotto(int prod);

private:
    redisContext *c2r;    // Redis connection context
    const char *READ_STREAM;   // Name of the read stream
    const char *WRITE_STREAM;  // Name of the write stream
    const char *nome;     // Costumer's first name
    const char *cognome;  // Costumer's last name
    int id;               // Costumer's ID
};


#endif
