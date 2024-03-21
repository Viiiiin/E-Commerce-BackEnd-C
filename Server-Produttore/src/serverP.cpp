
/*If you have a stream and multiple clients, and you want the stream to be partitioned or sharded across your clients, 
so that each client will get a sub set of the messages arriving in a stream, you need a consumer group.*/
#include "serverP.h"


ServerP:: ServerP ( const char *nome
)

{ 
    this->nome = nome;
    this->READ_STREAM = "stream2_produttore_2";
    this->WRITE_STREAM = "stream_produttore_1";
    this->c2r = redisConnect("localhost", 6379);
    Con2DB db("localhost", "5432", "ecommerce", "47002", "db_ecommerce");


    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);   
}
 
char* ServerP::readCommandRedis(int block){
    int i,k;
    char *cmd = new char[100];
    redisReply *reply;

    reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT 2 NOACK STREAMS %s >", this->nome, block, this->READ_STREAM);

    assertReply(c2r, reply);

    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT 1 NOACK STREAMS %s >", 
			  this->nome, block, this->READ_STREAM);

    assertReply(this->c2r, reply);

    ReadStreamMsgVal(reply, k, i, 4, cmd);

    return cmd;
}

// Legge le caratteristiche del prodotto da redis e salva prodotto nel database
void ServerP::inserisciProdotto( int block){
    int k,i;
    redisReply *reply;

    PGresult *res;

    char nome[100];
    char descrizione[100];
    char prezzo[100];
    char result[100];
    char key[100];
    char errorMsg[100];
    char sqlcmd[1000]; 

    reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT 2 NOACK STREAMS %s >", this->nome, block, this->READ_STREAM);

    assertReply(c2r, reply);

    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    ReadStreamMsgVal(reply, k, i, 4, nome);
    ReadStreamMsgVal(reply, k, i, 6, descrizione);
    ReadStreamMsgVal(reply, k, i, 8, prezzo);
    
    freeReplyObject(reply);

    sprintf(sqlcmd,  "INSERT INTO Prodotto VALUES (DEFAULT, \'%s\', \'%s\', \'%s') ON CONFLICT DO NOTHING",nome,descrizione,prezzo);
    
    res = this->db->ExecSQLcmd(sqlcmd);
    
    
    redisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, res);
    assertReplyType(this->c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);
    
    PQclear(res);



}