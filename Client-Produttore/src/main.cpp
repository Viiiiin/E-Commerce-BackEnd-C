#include "produttore.h"
#include <string>
#include <iostream>
#include <unistd.h> // Libreria per la fork()
#include "../../tools/src/main.h"
#include <math.h>
#include <time.h>
#include <random>
#include <sys/wait.h> // Libreria per la wait()
using namespace std;

// Function to simulate the producer process
void produttoreProcesso(Produttore& prod, long time, int numero_operazioni) {
    // Array of sample products
    Prodotto prodotti[] = {
        {"Maglietta", "Maglietta di cotone blu con logo stampato", {"EUR", 19}},
        {"Pantaloni", "Pantaloni jeans regular fit", {"EUR", 39}},
        {"Scarpe", "Scarpe sportive leggere e traspiranti", {"EUR", 59}},
        {"Cappello", "Cappello vintage anni 60", {"USD", 25}}
    };
        
    int x = 0;
    int i=0;
    string res;
    while (x < numero_operazioni) {
        msleep(time); // Sleep for a specified time
        int j = rand() % 2; // Randomly choose between inserting and removing a product
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, numero_operazioni); // Uniform distribution between 0 and number of operations
        
        // Extract a random number
        i = dis(gen);
        switch (j)
        {
            case 0:
                i = i % 4;
                res = prod.inserisciProdotto(prodotti[i].nome, prodotti[i].descrizione,prodotti[i].prezzo); // Insert a product
                break;
            case 1:
                res = prod.rimuoviProdotto(i); // Remove a product
                break;
            default:
                break;
        }
        x++;
        cout << res <<": "<< x << endl; // Output the result of the operation
    }
}

// Main function
int main(int argc,char *argv[]){
    // Declaration and initialization of variables
    int numero_produttori= 0;
    long time_to_sleep= 0;
    int numero_operazioni=0;
    char c;
    
    // Parse command line arguments
    while ((c = getopt(argc, argv, "p:t:n:")) != -1) {
        switch (c) {
            case 'p':    
                numero_produttori = atoi(optarg);
                break;
            case 't':    
                time_to_sleep = atoi(optarg);
                break;
            case 'n': 
                numero_operazioni= atoi(optarg);
                break;
        }
    }
    
    // Create child processes for producers
    for (int i = 1; i <= numero_produttori; ++i) {
        Produttore prod(i);
        pid_t pid = fork();

        if (pid == 0) { // Child process
            produttoreProcesso(prod,time_to_sleep,numero_operazioni); // Start producer process
            exit(0); // Terminate the child process after completing the work
        } else if (pid < 0) { // Error in creating child process
            cerr << "Errore nella fork()" << endl;
            exit(1);
        }
    }

    // Wait for all child processes to terminate before terminating the main process
    int status;
    pid_t pid;
    while ((pid = wait(&status)) > 0);

    return 0;
} 
