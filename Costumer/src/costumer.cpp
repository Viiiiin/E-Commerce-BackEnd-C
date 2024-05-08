// Effettua richiesta al server di aggiungere un prodotto
#include "costumer.h"

Costumer::Costumer(int id)
{
    this->id = id;
    this->READ_STREAM = "stream_costumer_1";
    this->WRITE_STREAM = "stream_costumer_2";
    this->c2r = redisConnect("localhost", 6379);

    /*

    redisReply *reply;

    // delete stream if it exists
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    */

    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);

}


string Costumer::acquistaProdotto(int prodotto){
    char key0[100];
    char key1[100];
    char key2[100];
    int block = 10000000;
    int k,i;
    char result[100];
    redisReply *reply;

    sprintf(key0,"comando");
    sprintf(key1, "prodotto");
    sprintf(key2, "costumer");

    reply = RedisCommand(this->c2r, "XADD %s * %s %s %d %s %d %s", this->WRITE_STREAM, key1, "Acquista", prodotto, key0, this->id, key2);

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