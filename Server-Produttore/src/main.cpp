#include "serverP.h"
#include <string>
#include <iostream>
using namespace std;

int main(){
   
    int block = 1000000000;
    Cmd_Reply cmd_reply;

    ServerP server("serverProd");
    cout << "Il Server dei Produttori sta ascoltando..." << endl;
    while(1){
        cmd_reply = server.readCommandRedis(block);
        // cout << cmd_reply.cmd;
        cout<< cmd_reply.cmd;
        if (cmd_reply.cmd =="Inserisci"){
            
            server.inserisciProdotto(block,cmd_reply.reply);
           
        }
         if (cmd_reply.cmd =="Rimuovi"){
            server.rimuoviProdotto(block,cmd_reply.reply);
        
        }
        
    }

    return 0;
    

}