#include "produttore.h"
#include <string>
#include <iostream>
#include <unistd.h> // Libreria per la fork()
#include "../../tools/msleep.cpp"
#include <math.h>
#include <sys/wait.h> // Libreria per la wait()
using namespace std;

void produttoreProcesso(Produttore& prod) {
    char* nome[] = {"Computer","Televisore","Mouse","Tastiera"};
    char* descrizione[] = {"Un computer","Un televisore", "Un Mouse","Una Tastiera"};

    int x = 0;
    long time = 1;
    pid_t pid;
    while (x < 1000) {
        msleep(time);
        int i = rand() % 4;
        prod.inserisciProdotto(nome[i], descrizione[i]);
        pid = getpid();
        cout << pid;
        cout << " Ha inserito un prodotto: ";
        x++;
        cout << x << endl;
    }
}

int main() {
    int num_produttori = 1000;
    // Creazione di processi figlio
    for (int i = 0; i < num_produttori; ++i) {
        Produttore prod(4);
        pid_t pid = fork();

        if (pid == 0) { // Processo figlio
            produttoreProcesso(prod);
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