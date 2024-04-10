
#include "costumer.h"
#include <string>
#include <iostream>

using namespace std;

int main(){
    int i=1;
    int x=0;
    Costumer cost(1, "Mario", "Rossi");
    int prodotto[] = {1,2,3};
    while(x<100){
        i = rand() % 3;
        cost.acquistaProdotto(prodotto[i]);
        cout << "Acquistato prodotto: ";
        cout << x << endl;
        x++;
    }
    return 0;
}