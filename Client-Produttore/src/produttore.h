#ifndef produttore_h
#define produttore_h

#include "con2redis.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

struct Prezzo {
    char valuta[4]; // Campo valuta di 3 char + terminatore null
    int prezzo;
};

struct Prodotto {
    const char* nome;
    const char* descrizione;
    Prezzo prezzo;
};

class Produttore {
    public: 

        Produttore(const int id);

        int inserisciProdotto(const char *nome, const char *descrizione,Prezzo prezzo);
    private: 
        int id;
        redisContext *c2r;
        const char *READ_STREAM;
        const char *WRITE_STREAM;
};



#endif
