#include "produttore.h"
#include <string>
#include <iostream>
#include <unistd.h> // Libreria per la fork()
#include "../../tools/src/main.h"
#include <math.h>
#include <sys/wait.h> // Libreria per la wait()
using namespace std;

void produttoreProcesso(Produttore& prod, long time) {
       Prodotto prodotti[] = {
        {"Maglietta", "Maglietta di cotone blu con logo stampato", {"EUR", 19}},
        {"Pantaloni", "Pantaloni jeans regular fit", {"EUR", 39}},
        {"Scarpe", "Scarpe sportive leggere e traspiranti", {"EUR", 59}},
        {"Cappello", "Cappello vintage anni 60", {"USD", 25}}
        };
        
    int x = 0;
    pid_t pid;
    string res;
    while (x < 100) {
        msleep(time);
        int j = rand() % 2;
        int i = rand() %100000;
        switch (j)
        {
        case 0:
            i= i%4;
            res = prod.inserisciProdotto(prodotti[i].nome, prodotti[i].descrizione,prodotti[i].prezzo);
            break;
        case 1:
            res = prod.rimuoviProdotto(i);
        default:
            break;
        }
        x++;
        cout << res <<": "<< x << endl;
       
    }
}



int main(int argc,char *argv[]){
	
	//Dichiarazione e inizializzazione delle variabili
	int numero_produttori= 0;
	long time_to_sleep= 0;
    char c;
	
    while ((c = getopt(argc, argv, "p:t:")) != -1) {
        switch (c) {
            case 'p':    /* numero produttori */
              numero_produttori = atoi(optarg);
              break;
            case 't':    /* tempo di dormita */
              time_to_sleep = atoi(optarg);
              break;
        }
    }
    // Creazione di processi figlio
    for (int i = 1; i <= numero_produttori; ++i) {
        Produttore prod(i);
        pid_t pid = fork();

        if (pid == 0) { // Processo figlio
            produttoreProcesso(prod,time_to_sleep);
            exit(0); // Termina il processo figlio dopo aver completato il lavoro
        } else if (pid < 0) { // Errore nella creazione del processo figlio
            cerr << "Errore nella fork()" << endl;
            exit(1);
        }
    }

    // Attendi che tutti i processi figlio terminino prima di terminare il processo principale
    int status;
    pid_t pid;
    while ((pid = wait(&status)) > 0);

    return 0;
}