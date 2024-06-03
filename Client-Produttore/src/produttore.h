#ifndef produttore_h
#define produttore_h

#include "con2redis.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

// Structure to represent the price of a product
struct Prezzo {
    char valuta[4]; // Currency field of 3 characters + null terminator
    int prezzo;     // Price of the product
};

// Structure to represent product details
struct Prodotto {
    const char* nome;         // Name of the product
    const char* descrizione;  // Description of the product
    Prezzo prezzo;            // Price of the product
};

// Class declaration for Produttore (Producer)
class Produttore {
public: 
    // Constructor to initialize a Produttore object with an ID
    Produttore(const int id);

    // Method to insert a product into the system
    string inserisciProdotto(const char *nome, const char *descrizione, Prezzo prezzo);
    
    // Method to remove a product from the system
    string rimuoviProdotto(const int id);
    
private: 
    int id;                  // ID of the producer
    redisContext *c2r;       // Pointer to the Redis context
    const char *READ_STREAM; // Pointer to the read stream name
    const char *WRITE_STREAM;// Pointer to the write stream name
};

#endif
