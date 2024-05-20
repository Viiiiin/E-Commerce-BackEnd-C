
#include "trasportatore.h"
#include <string>
#include <iostream>
#include <unistd.h> // Libreria per la fork()
#include "../../tools/src/main.h"
#include <math.h>
#include <time.h>
#include <random>
#include <sys/wait.h> // Libreria per la wait()
using namespace std;

void trasportatoreProcesso(Trasportatore& trasp, long time, int numero_operazioni) {      
    int x = 0;
    int i = 0;
    string res;
    while (x < numero_operazioni) {
        msleep(time);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, numero_operazioni);
        
        // Estrazione di un numero casuale
        i = dis(gen);
        res = trasp.consegnaProdotto(i);
        x++;
        cout << res <<": "<< x << endl;
       
    }
}



int main(int argc,char *argv[]){
	
	//Dichiarazione e inizializzazione delle variabili
	int numero_trasportatori = 0;
	long time_to_sleep = 0;
    int numero_operazioni = 0;
    char c;
	
    while ((c = getopt(argc, argv, "r:t:n:")) != -1) {
        switch (c) {
            case 'r':    
              numero_trasportatori = atoi(optarg);
              break;
            case 't':    
              time_to_sleep = atoi(optarg);
              break;
            case 'n': 
              numero_operazioni= atoi(optarg);
              break;
              
        }
    }
    // Creazione di processi figlio
    for (int i = 1; i <= numero_trasportatori; ++i) {
        Trasportatore trasp(i);
        pid_t pid = fork();

        if (pid == 0) { // Processo figlio
            trasportatoreProcesso(trasp,time_to_sleep,numero_operazioni);
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

