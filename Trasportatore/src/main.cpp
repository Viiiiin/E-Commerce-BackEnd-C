#include "trasportatore.h"
#include <string>
#include <iostream>

using namespace std;

int main(){    
    string res;
    int i=1;
    int x=0;
    Trasportatore trasp(1, "BARTOLINI");
    int prodotto[] = {1,2,3,4,5,6};
    while(x<10){
        i = rand() % 6;
        res = trasp.consegnaProdotto(i);
        cout << res << endl;
        x++;
        cout << x << endl;
    }
    return 0;
}
