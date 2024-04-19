
#include "costumer.h"
#include <string>
#include <iostream>

using namespace std;

int main(){    
    int res;
    int i=1;
    int x=0;
    Costumer cost(1, "Mario", "Rossi");
    int prodotto[] = {1,2,3};
    while(x<100){
        i = rand() % 3;
        res = cost.acquistaProdotto(prodotto[i]);
        if (res==0){
        cout << " Ha acquistato un prodotto: ";
        }
        else {
            cout << " Acquisto del prodotto non riuscito n: ";
        }
        x++;
        cout << x << endl;
    }
    return 0;
}