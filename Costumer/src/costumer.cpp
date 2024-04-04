// Effettua richiesta al server di aggiungere un prodotto
#include "costumer.h"

Costumer::Costumer(int id, const char *nome, const char *cognome)
{
    this->id = id;
    this->nome = nome;
    this->cognome = cognome;
    this->READ_STREAM = "stream_costumer_1";
    this->WRITE_STREAM = "stream_costumer_2";
    this->c2r = redisConnect("localhost", 6379);

    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);
}


void Costumer::acquistaProdotto(int prodotto){
    char key0[100];
    char key1[100];
    redisReply *reply;

    sprintf(key0,"comando");
    sprintf(key1, "prodotto");

    reply = RedisCommand(this->c2r, "XADD %s * %s %s %d %s", this->WRITE_STREAM, key1, "Acquista", prodotto, key0);

    assertReplyType(this->c2r,reply,REDIS_REPLY_STRING);

    freeReplyObject(reply);

}