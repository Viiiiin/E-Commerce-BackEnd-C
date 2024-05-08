#include "costumer.h"
#include <string>
#include <iostream>
#include <unistd.h> // Libreria per la fork()
#include "../../tools/src/main.h"
#include <math.h>
#include <time.h>
#include <random>
#include <sys/wait.h> // Libreria per la wait()
using namespace std;

void costumerProcesso(Costumer& cost, long time, int numero_operazioni) {      
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
        res = cost.acquistaProdotto(i);
        x++;
        cout << res <<": "<< x << endl;
       
    }
}



int main(int argc,char *argv[]){
	
	//Dichiarazione e inizializzazione delle variabili
	int numero_costumer = 0;
	long time_to_sleep = 0;
    int numero_operazioni = 0;
    char c;
	
    while ((c = getopt(argc, argv, "c:t:n:")) != -1) {
        switch (c) {
            case 'c':    
              numero_costumer = atoi(optarg);
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
    for (int i = 1; i <= numero_costumer; ++i) {
        Costumer cost(i);
        pid_t pid = fork();

        if (pid == 0) { // Processo figlio
            costumerProcesso(cost,time_to_sleep,numero_operazioni);
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

/*
#include "costumer.h"
#include <string>
#include <iostream>
#include <random>

using namespace std;

int main(){    
    string res;
    int i=1;
    int x=0;
    Costumer cost1 (1, "Mario", "Rossi");
    int prodotto1[] = {1,2,3,4,5,6,7,444,555,777,888};
    while(x<100){
        // Generatore di numeri casuali
        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::uniform_int_distribution<> dis(0, 6);
        
        // Estrazione di un numero casuale
        i = dis(gen);
        res = cost1.acquistaProdotto(prodotto1[i]);
        cout << res << endl;
        x++;
        cout << x << endl;
    }
    Costumer cost2 (2, "Luigi", "Bianchi");
    int prodotto2[] = {8,9,10,11,12,13,14,000,333,222};
    while(x<200){
        // Generatore di numeri casuali
        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::uniform_int_distribution<> dis(0, 7);
        
        // Estrazione di un numero casuale
        i = dis(gen);
        res = cost2.acquistaProdotto(prodotto2[i]);
        cout << res << endl;
        x++;
        cout << x << endl;
    }
    return 0;
}
*/