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

// Function to simulate the process of a costumer
void costumerProcesso(Costumer& cost, long time, int numero_operazioni) {      
    int x = 0;
    int i = 0;
    string res;
    while (x < numero_operazioni) {
        msleep(time); // Sleep for a given time
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, numero_operazioni);
        
        // Generate a random number
        i = dis(gen);
        // Perform an operation of purchasing a product
        res = cost.acquistaProdotto(i);
        x++; // Increment the operation counter
        cout << res << ": " << x << endl; // Print the result of the operation
    }
}

int main(int argc,char *argv[]){
    // Declaration and initialization of variables
    int numero_costumer = 0;
    long time_to_sleep = 0;
    int numero_operazioni = 0;
    char c;
    
    // Parse command line arguments
    while ((c = getopt(argc, argv, "c:t:n:")) != -1) {
        switch (c) {
            case 'c':    
              numero_costumer = atoi(optarg);
              break;
            case 't':    
              time_to_sleep = atoi(optarg);
              break;
            case 'n': 
              numero_operazioni = atoi(optarg);
              break;
        }
    }
    
    // Create child processes for each costumer
    for (int i = 1; i <= numero_costumer; ++i) {
        Costumer cost(i);
        pid_t pid = fork();

        if (pid == 0) { // Child process
            costumerProcesso(cost, time_to_sleep, numero_operazioni);
            exit(0); // Terminate the child process after completing the work
        } else if (pid < 0) { // Error in creating the child process
            cerr << "Error in fork()" << endl;
            exit(1);
        }
    }

    // Wait for all child processes to finish before terminating the main process
    int status;
    pid_t pid;
    while ((pid = wait(&status)) > 0);

    return 0;
} 