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



int Produttore::inserisciProdotto(const char *nome,const char *descrizione, Prezzo prezzo){
    char key0[100];
    char key1[100];
    char key2[100];
    char key3[100]; 
    char key4[100];
    char result[100];
    int block = 10000000;
    int k,i;
    redisReply *reply;

    sprintf(key0,"comando");
    sprintf(key1, "nome");
    sprintf(key2,"descrizione");
    sprintf(key3,"cifra");
    sprintf(key4,"valuta");

   

    reply= RedisCommand(this->c2r, "XADD %s * %s %s %s %s %s %s %s %d %s %s %s %d",this->WRITE_STREAM, key0, "Inserisci", key1, nome, key2, descrizione,"produttore",this->id,"valuta",prezzo.valuta ,"prezzo",prezzo.prezzo);

    assertReplyType(this->c2r,reply,REDIS_REPLY_STRING);

    freeReplyObject(reply);

    reply = RedisCommand(this->c2r, "XREADGROUP GROUP diameter %d BLOCK %d COUNT 1 NOACK STREAMS %s >", this->id, block, this->READ_STREAM);
    
    assertReply(this->c2r, reply);

    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    ReadStreamMsgVal(reply, k, i, 1, result);
    freeReplyObject(reply);

    if (strcmp(result, "Aggiunto") != 0){ 
        return 1;
    }
    return 0;
}