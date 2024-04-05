#include "serverC.h"
#include <string>
#include <iostream>
#include "logger.h"

using namespace std;

int main(){
    //int i=0;
    //int j=0;
    int block = 1000000000;
    int read_counter=0;
    //int send_counter=0;
    Cmd_Reply cmd_reply;

    ServerC server("serverCost");
    cout << "Il Server dei Costumer sta ascoltando..." << endl;
    while(1){
        cmd_reply = server.readCommandRedis(block);
        cout << cmd_reply.cmd << endl;
        if (cmd_reply.cmd =="Acquista"){
            cout<<"Acquistando il prodotto..."<< endl;
            server.acquistaProdotto(block,cmd_reply.reply);
            cout<<"Prodotto acquistato con successo"<< endl;
        }
        read_counter++;

        log("Comando letto", -1);
    }

    return 0;
    

}