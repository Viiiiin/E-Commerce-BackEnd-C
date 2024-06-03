#include "trasportatore.h"
#include <string>
#include <iostream>
#include <unistd.h>     // Library for fork()
#include "../../tools/src/main.h"
#include <math.h>
#include <time.h>
#include <random>
#include <sys/wait.h>   // Library for wait()
using namespace std;

// Function to simulate the delivery process of a transporter
void trasportatoreProcesso(Trasportatore& trasp, long time, int numero_operazioni) {      
    int x = 0;
    int i = 0;
    string res;
    while (x < numero_operazioni) {
        msleep(time);   // Simulate sleep time between operations
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, numero_operazioni);
        
        // Generate a random number
        i = dis(gen);
        res = trasp.consegnaProdotto(i);    // Perform product delivery
        x++;
        cout << res << ": " << x << endl;   // Output delivery result
    }
}

int main(int argc, char *argv[]) {
	
	// Declaration and initialization of variables
	int numero_trasportatori = 0;   // Number of transporters
	long time_to_sleep = 0;         // Sleep time between operations
    int numero_operazioni = 0;      // Number of delivery operations
    char c;
	
    // Parse command line arguments
    while ((c = getopt(argc, argv, "r:t:n:")) != -1) {
        switch (c) {
            case 'r':    
                numero_trasportatori = atoi(optarg);
                break;
            case 't':    
                time_to_sleep = atoi(optarg);
                break;
            case 'n': 
                numero_operazioni = atoi(optarg);
                break;
        }
    }
    
    // Create child processes for each transporter
    for (int i = 1; i <= numero_trasportatori; ++i) {
        Trasportatore trasp(i);     // Create a new transporter object
        pid_t pid = fork();         // Fork a new process
        
        if (pid == 0) {             // Child process
            trasportatoreProcesso(trasp, time_to_sleep, numero_operazioni);  // Perform delivery operations
            exit(0);                 // Terminate the child process after completing the work
        } else if (pid < 0) {       // Error in forking a child process
            cerr << "Errore nella fork()" << endl;
            exit(1);
        }
    }

    // Wait for all child processes to finish before terminating the main process
    int status;
    pid_t pid;
    while ((pid = wait(&status)) > 0);

    return 0;
} 
