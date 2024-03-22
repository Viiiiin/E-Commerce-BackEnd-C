
/*If you have a stream and multiple clients, and you want the stream to be partitioned or sharded across your clients, 
so that each client will get a sub set of the messages arriving in a stream, you need a consumer group.*/
#include "serverP.h"
using namespace std;

ServerP:: ServerP ( const char *nome
)

{ 
    this->nome = nome;
    this->READ_STREAM = "stream_produttore_2";
    this->WRITE_STREAM = "stream_produttore_1";
    this->c2r = redisConnect("localhost", 6379);


    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);   
}
 
Cmd_Reply ServerP::readCommandRedis(int block){
    int i,k;
    char *cmd = new char[100];
    redisReply *reply;
    Cmd_Reply cmd_reply;

    reply = RedisCommand(this->c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT 1 NOACK STREAMS %s >", this->nome, block, this->READ_STREAM);

    cout << "SEGFAULT" << endl;
    assertReply(this->c2r, reply);

    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    ReadStreamMsgVal(reply, k, i, 1, cmd);
    cmd_reply.cmd = cmd;
    cmd_reply.reply = reply;


    return cmd_reply;
}

// Legge le caratteristiche del prodotto da redis e salva prodotto nel database
void ServerP::inserisciProdotto( int block, redisReply *reply){
    int k,i;
    redisReply *ret;

    Con2DB db("localhost", "5432", "ecommerce", "47002", "db_ecommerce");

    char nome[100];
    char descrizione[100];
    char key[100];
    char sqlcmd[1000]; 

    cout << "ENTRA" << endl;

    assertReply(this->c2r, reply);

    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    ReadStreamMsgVal(reply, k, i, 3, nome);
    ReadStreamMsgVal(reply, k, i, 5, descrizione);

    
    freeReplyObject(reply);

    sprintf(sqlcmd,  "INSERT INTO Prodotto VALUES (DEFAULT, \'%s\', \'%s\',ROW('EUR',1000)) ON CONFLICT DO NOTHING",nome,descrizione);
    cout << sqlcmd << endl;
    
    db.ExecSQLcmd(sqlcmd);
    cout << "QUI" << endl;
    
    sprintf(key,"Risultato");
    cout << "QUI" << endl;
    
    ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "Aggiunto");
    assertReplyType(this->c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(ret);
    


}