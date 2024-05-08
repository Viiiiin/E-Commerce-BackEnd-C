#include "trasportatore.h"

Trasportatore:: Trasportatore (
    const int id,
    const char *ragioneSociale   
)

{ 
    this->ragioneSociale= ragioneSociale;
    this->id=id;
    this->READ_STREAM = "stream_trasportatore_1";
    this->WRITE_STREAM = "stream_trasportatore_2";
    this->c2r = redisConnect("localhost", 6379);
 
   
    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);
};

string Trasportatore::consegnaProdotto(const int idAcquisto){
    char key0[100];
    char key1[100];
    char key2[100];
    char result[100];
    int block = 10000000;
    int k,i;
    redisReply *reply;

    sprintf(key0,"comando");
    sprintf(key1, "nome");
    sprintf(key2,"Acquisto");

   

    reply= RedisCommand(this->c2r, "XADD %s * %s %s %s %d %s %d",this->WRITE_STREAM, key0, "Consegna", key1, this->id, key2,idAcquisto);

    assertReplyType(this->c2r,reply,REDIS_REPLY_STRING);

    freeReplyObject(reply);

    reply = RedisCommand(this->c2r, "XREADGROUP GROUP diameter %d BLOCK %d COUNT 1 NOACK STREAMS %s >", this->id, block, this->READ_STREAM);
    
    assertReply(this->c2r, reply);

    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    ReadStreamMsgVal(reply, k, i, 1, result);
    freeReplyObject(reply);
    return result;
   
}

/*
int* Trasportatore::getNonConsegnati(){
    char key0[100];
    char key1[100];
    int prodottiNonConsegnati[100] = {-1};
    int block = 10000000;
    int k,i;
    redisReply *reply;

    sprintf(key0,"comando");
    sprintf(key1, "nome");  

    reply = RedisCommand(this->c2r, "XADD %s * %s %s %s %d",this->WRITE_STREAM, key0, "GetNonConsegnati", key1, this->id);

    assertReplyType(this->c2r,reply,REDIS_REPLY_STRING);

    freeReplyObject(reply);

    reply = RedisCommand(this->c2r, "XREADGROUP GROUP diameter %d BLOCK %d COUNT 1 NOACK STREAMS %s >", this->id, block, this->READ_STREAM);
    
    assertReply(this->c2r, reply);

    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    ReadStreamMsgValInt(reply, k, i, 1, prodottiNonConsegnati);
    freeReplyObject(reply);
    return prodottiNonConsegnati;
   
}
*/

