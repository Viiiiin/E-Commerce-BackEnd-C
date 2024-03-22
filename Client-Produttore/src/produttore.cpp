// Effettua richiesta al server di aggiungere un prodotto
#include "produttore.h"

Produttore::Produttore(int id)
{
    this->id = id; 
    this->READ_STREAM = "stream_produttore_1";
    this->WRITE_STREAM = "stream_produttore_2";
    this->c2r = redisConnect("localhost", 6379);
    
    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);  
}



void Produttore::inserisciProdotto(char *nome, char *descrizione){
    char key0[100];
    char key1[100];
    char key2[100];
    char key3[100]; 
    char key4[100];
    redisReply *reply;

    sprintf(key0,"comando");
    sprintf(key1, "nome");
    sprintf(key2,"descrizione");
    sprintf(key3,"cifra");
    sprintf(key4,"valuta");

   

    reply= RedisCommand(this->c2r, "XADD %s * %s %s %s %s %s %s",this->WRITE_STREAM, key0, "Inserisci", key1, nome, key2, descrizione);

    assertReplyType(this->c2r,reply,REDIS_REPLY_STRING);

    freeReplyObject(reply);

}