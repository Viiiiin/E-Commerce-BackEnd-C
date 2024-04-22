#include "serverC.h"
#include <string>
#include <iostream>

using namespace std;

int main(){
    //int i=0;
    //int j=0;
    int block = 1000000000;
    int read_counter=0;
    int res = 0;
    //int send_counter=0;
    Cmd_Reply cmd_reply;

    ServerC server("serverCost");
    cout << "Il Server dei Costumer sta ascoltando..." << endl;
    while(1){
        cmd_reply = server.readCommandRedis(block);
        // cout << cmd_reply.cmd << endl;
        if (cmd_reply.cmd =="Acquista"){
            cout<<"Acquistando il prodotto..."<< endl;
            res = server.acquistaProdotto(block,cmd_reply.reply);
            if (res==0){
                cout << "Prodotto acquistato con successo "<< endl;
            }
            else if (res == 1) {
                cout << "Il prodotto da acquistare non esiste "<< endl;
            }
            else if (res == 2) {
                cout << "Il prodotto da acquistare e' stato gia' venduto "<< endl;
            }
            
            cout << "---------------------------------------------------------------"<< endl;
        }
        read_counter++;

    }

    return 0;
    

}