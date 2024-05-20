#include "trasportatore.h"
#include <string>
#include <iostream>
#include <random>

using namespace std;

int main(){  

    /*  
    string res;
    Trasportatore trasp(1, "BARTOLINI");
    int* prodottiNonConsegnati; 
    prodottiNonConsegnati = trasp.getNonConsegnati();
    int j = 0;
    while (prodottiNonConsegnati[j] != -1 && j < 100) {
        res = trasp.consegnaProdotto(prodottiNonConsegnati[j]);
        cout << res << endl;
        j++;
    }
    return 0;
    */

    string res;
    int i=1;
    int x=0;
    Trasportatore trasp(1, "BARTOLINI");
    while(x<10){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(100, 150);
        i = dis(gen);
        cout << "Id estratto: ";
        cout << i << endl;
        res = trasp.consegnaProdotto(i);
        cout << res << endl;
        x++;
        cout << x << endl;
    }
    return 0;

}

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

void trasportatoreProcess(Trasportatore& trasp, long time, int numero_operazioni) {      
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
            case 'c':    
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
            costumerProcesso(trasp,time_to_sleep,numero_operazioni);
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

