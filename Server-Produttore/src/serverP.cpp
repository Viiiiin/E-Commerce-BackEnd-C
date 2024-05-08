
/*If you have a stream and multiple clients, and you want the stream to be partitioned or sharded across your clients, 
so that each client will get a sub set of the messages arriving in a stream, you need a consumer group.*/
#include "serverP.h"
using namespace std;

ServerP:: ServerP ( const char *nome
)
    :db("localhost", "5432", "ecommerce", "47002", "db_ecommerce")

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

    char produttore[100];
    char nome[100];
    char descrizione[100];
    char key[100];
    char sqlcmd[1000]; 
    char valuta[4];
    char prezzo[100];
    char logmessage[1000];
    char dominio[100];
    char funzione[100];

    


    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    ReadStreamMsgVal(reply, k, i, 3, nome);
    ReadStreamMsgVal(reply, k, i, 5, descrizione);
    ReadStreamMsgVal(reply, k, i, 7, produttore);
    ReadStreamMsgVal(reply, k, i, 9, valuta);
    ReadStreamMsgVal(reply, k, i, 11, prezzo);
    
    freeReplyObject(reply);
    

    sprintf(sqlcmd,  "INSERT INTO Prodotto VALUES (DEFAULT,\'%s\' ,\'%s\', \'%s\',ROW(\'%s\',\'%s\')) ON CONFLICT DO NOTHING;",produttore,nome,descrizione,valuta,prezzo);
    // cout << sqlcmd << endl;
    
    this->db.ExecSQLcmd(sqlcmd);
    
    sprintf(logmessage,"Produttore %s ha aggiunto un prodotto",produttore);
    sprintf(dominio,"produttore");
    sprintf(funzione,"Vendita");
    int id_produttore = atoi(produttore);
    log2db(logmessage,id_produttore,db,dominio,funzione);
    sprintf(key,"Risultato");

    
    ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "Aggiunto");
    assertReply(this->c2r, ret);
    freeReplyObject(ret);
    


}
void ServerP::rimuoviProdotto( int block, redisReply *reply){
    int k,i;
    redisReply *ret;
    PGresult *res;

    char produttore[100];
    char idProdotto[100];
    char key[100];
    char sqlcmd[1000];
    char logmessage[1000];
    char dominio[100];
    char funzione[100];
    bool exist=false;
    bool buy=false;
    bool isProduttore=true;
    


    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    
    ReadStreamMsgVal(reply, k, i, 3, produttore);
    ReadStreamMsgVal(reply, k, i, 5, idProdotto);
    
    
    freeReplyObject(reply);

    sprintf(sqlcmd, "SELECT 1 FROM Prodotto WHERE id = '%s';", idProdotto);

    // Esecuzione della query e controllo del risultato
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        exist = true; // Se la query restituisce almeno una riga, il prodotto esiste
        PQclear(res);
    }

    if (!exist){
        sprintf(key,"Risultato");
        ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "ERRORE: Il prodotto non esiste");
        assertReply(this->c2r, reply);
        freeReplyObject(ret);
        return;
    }
    sprintf(sqlcmd, "SELECT * FROM Acquisto WHERE prodotto = '%s';", idProdotto);

    // Esecuzione della query e controllo del risultato
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        buy = true; // Se la query restituisce almeno una riga, il prodotto è stato acquistato
        PQclear(res);
    }

    if (buy){
        sprintf(key,"Risultato");
        ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "ERRORE: Il prodotto è stato gia' acquistato");
        assertReply(this->c2r, reply);
        freeReplyObject(ret);
        return;
    }

    sprintf(sqlcmd, "SELECT * FROM Prodotto p WHERE  p.produttore=\'%s\' and p.id=\'%s\';",produttore,idProdotto);

    // Esecuzione della query e controllo del risultato
    res = db.ExecSQLtuples(sqlcmd); 

    if (res != NULL && PQntuples(res) > 0) {   
        if (produttore==PQgetvalue(res, 0, PQfnumber(res, "produttore"))){
            isProduttore = false; 
        }
        PQclear(res);
    }

    if (!isProduttore){
        sprintf(key,"Risultato");
        ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "ERRORE: Non puoi eliminare prodotti altrui");
        assertReply(this->c2r, reply);
        freeReplyObject(ret);
        return;
    }


    sprintf(sqlcmd,  "DELETE FROM Prodotto p WHERE  p.id=\'%s\';",idProdotto);
    // cout << sqlcmd << endl;
    
    db.ExecSQLcmd(sqlcmd);
    
    sprintf(logmessage,"Produttore %s ha rimosso prodotto: %s",produttore,idProdotto);
    sprintf(dominio,"produttore");
    sprintf(funzione,"Rimozione");
    int id_produttore = atoi(produttore);
    log2db(logmessage,id_produttore,db,dominio,funzione);


    sprintf(key,"Risultato");

    
    ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "Prodotto rimosso");
    assertReply(this->c2r, ret);
    freeReplyObject(ret);
}