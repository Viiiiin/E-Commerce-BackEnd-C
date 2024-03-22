
#include "produttore.h"
#include <string>
#include <iostream>
using namespace std;

int main(){
    int i=1;
    int x=0;
    Produttore prod(4);
    char* nome[] = {"Computer","Televisore","Mouse","Tastiera"};
    char* descrizione[] = {"Un computer","Un televisore", "Un Mouse","Una Tastiera"};
    while(x<100){
        i= (i+5^2+7)%4;
        prod.inserisciProdotto(nome[i],descrizione[i]);
        cout << "Ha inserito un prodotto: ";
        cout << x << endl;
        x++;
    }
    return 0;
}