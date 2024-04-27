#include "serverT.h"
#include <string>
#include <iostream>
using namespace std;

int main(){
   
    int block = 1000000000;
    int count=0;
    Cmd_Reply cmd_reply;

    ServerT server("serverTrasportatori");
    cout << "Il Server dei Trasportatori sta ascoltando..." << endl;
    while(1){
        cmd_reply = server.readCommandRedis(block);
        // cout << cmd_reply.cmd;
        if (cmd_reply.cmd =="Consegna"){
            
            server.consegnaProdotto(block,cmd_reply.reply);
            cout<<"Operazione completata: ";
        }
        count++;
        cout<<count<<endl;
    }

    return 0;
}