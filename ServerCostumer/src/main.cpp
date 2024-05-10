#include "serverC.h"
#include <string>
#include <iostream>

using namespace std;

int main(){
    int block = 1000000000;
    Cmd_Reply cmd_reply;

    ServerC server("serverCost");
    cout << "Il Server dei Costumer sta ascoltando..." << endl;
    while(1){
        cmd_reply = server.readCommandRedis(block);
        // cout << cmd_reply.cmd;
        if (cmd_reply.cmd =="Acquista"){
            
            server.acquistaProdotto(block,cmd_reply.reply);
           
        }
    }

    return 0;
    

}