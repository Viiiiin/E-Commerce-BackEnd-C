
#include "costumer.h"
#include <string>
#include <iostream>

using namespace std;

int main(){    
    string res;
    int i=1;
    int x=0;
    Costumer cost(1, "Mario", "Rossi");
    int prodotto[] = {1,2,3,4,5,6};
    while(x<10){
        i = rand() % 6;
        res = cost.acquistaProdotto(prodotto[i]);
        cout << res << endl;
        x++;
        cout << x << endl;
    }
    return 0;
}