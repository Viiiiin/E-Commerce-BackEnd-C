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
