// NOTA: InserisciProdotto come Rimuovi contengono parti di codice che è possibile rendere Funzione, inoltre dovrebbero restituire una stringa(messaggio ricevuto dal server)
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



string Produttore::inserisciProdotto(const char *nome,const char *descrizione, Prezzo prezzo){
    char key0[100];
    char key1[100];
    char key2[100];
    char key3[100]; 
    char key4[100];
    char key5[100];
    char result[100];
    int block = 10000000;
    int k,i;
    redisReply *reply;

    sprintf(key0,"comando");
    sprintf(key1, "nome");
    sprintf(key2,"descrizione");
    sprintf(key3,"cifra");
    sprintf(key4,"valuta");
    sprintf(key5,"produttore");

   

    reply= RedisCommand(this->c2r, "XADD %s * %s %s %s %s %s %s %s %d %s %s %s %d",this->WRITE_STREAM, key0, "Inserisci", key1, nome, key2, descrizione,key5,this->id,key4,prezzo.valuta ,key3,prezzo.prezzo);

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
string Produttore::rimuoviProdotto(const int idProdotto){
    char key0[100];
    char key1[100];
    char key2[100];
    char result[100];
    int block = 10000000;
    int k,i;
    redisReply *reply;

    sprintf(key0,"comando");
    sprintf(key1, "nome");
    sprintf(key2,"prodotto");

   

    reply= RedisCommand(this->c2r, "XADD %s * %s %s %s %d %s %d",this->WRITE_STREAM, key0, "Rimuovi", key1, this->id, key2,idProdotto);

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
