#include "serverP.h"
#include <string>
#include <iostream>
using namespace std;

int main(){
    int i=0;
    int j=0;
    int block = 1000000000;


    ServerP server("serverProd");
    cout << "Il Server dei Produttori sta ascoltando..." << endl;
    while(1){

        char *cmd = server.readCommandRedis(block);

        if (cmd=="Inserisci"){
                server.inserisciProdotto(block);
        }
    }

    return 0;
    

}