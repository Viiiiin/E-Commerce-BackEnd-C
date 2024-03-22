#include "serverP.h"
#include <string>
#include <iostream>
using namespace std;

int main(){
    int i=0;
    int j=0;
    int block = 1000000000;
    int read_counter=0;
    int send_counter=0;
    Cmd_Reply cmd_reply;

    ServerP server("serverProd");
    cout << "Il Server dei Produttori sta ascoltando..." << endl;
    while(1){

        cmd_reply = server.readCommandRedis(block);
        cout << cmd_reply.cmd << endl;
        if (cmd_reply.cmd =="Inserisci"){
            cout<<"Inserendo il prodotto nel database..."<< endl;
            server.inserisciProdotto(block,cmd_reply.reply);
            cout<<"Prodotto inserito con successo"<< endl;
        }
        read_counter++;
    }

    return 0;
    

}