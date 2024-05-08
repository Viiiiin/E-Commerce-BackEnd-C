
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