#include "serverP.h"
using namespace std;

ServerP::ServerP(const char *nome)
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
    int i, k;
    char *cmd = new char[100];
    redisReply *reply;
    Cmd_Reply cmd_reply;
    
    reply = RedisCommand(this->c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT 1 NOACK STREAMS %s >", this->nome, block, this->READ_STREAM);
    
    assertReply(this->c2r, reply);

    k = ReadNumStreams(reply) - 1; // Take the last sent stream
    
    i = ReadStreamNumMsg(reply, k) - 1; // Last message of the last stream

    ReadStreamMsgVal(reply, k, i, 1, cmd);
    
    cmd_reply.cmd = cmd;
    cmd_reply.reply = reply;

    return cmd_reply;
}

// Read the product characteristics from redis and save the product in the database
void ServerP::inserisciProdotto(int block, redisReply *reply){
    int k, i;
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

    k = ReadNumStreams(reply) - 1; // Take the last sent stream
    
    i = ReadStreamNumMsg(reply, k) - 1; // Last message of the last stream

    ReadStreamMsgVal(reply, k, i, 3, nome);
    ReadStreamMsgVal(reply, k, i, 5, descrizione);
    ReadStreamMsgVal(reply, k, i, 7, produttore);
    ReadStreamMsgVal(reply, k, i, 9, valuta);
    ReadStreamMsgVal(reply, k, i, 11, prezzo);
    
    freeReplyObject(reply);
    
    sprintf(sqlcmd,  "INSERT INTO Prodotto VALUES (DEFAULT, '%s', '%s', '%s', ROW('%s', '%s')) ON CONFLICT DO NOTHING;", produttore, nome, descrizione, valuta, prezzo);
   
    
    this->db.ExecSQLcmd(sqlcmd);
    
    sprintf(logmessage, "Produttore %s ha aggiunto un prodotto", produttore);
    sprintf(dominio, "produttore");
    sprintf(funzione, "Vendita");
    int id_produttore = atoi(produttore);
    log2db(logmessage, id_produttore, db, dominio, funzione);
    sprintf(key, "Risultato");
    
    ret = RedisCommand(this->c2r, "XADD %s * %s %s", this->WRITE_STREAM, key, "Aggiunto");
    assertReply(this->c2r, ret);
    freeReplyObject(ret);
}

int ServerP::monitor(char *idProdotto, char *produttore){
    bool exist = false;
    bool buy = false;
    bool isProduttore = true;
    char key[100];
    PGresult *res;
    redisReply *ret;
    char sqlcmd[1000];
    
    sprintf(sqlcmd, "SELECT 1 FROM Prodotto WHERE id = '%s';", idProdotto);

    // Execute the query and check the result
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        exist = true; // If the query returns at least one row, the product exists
        PQclear(res);
    }

    if (!exist) {
        sprintf(key, "Risultato");
        ret = RedisCommand(this->c2r, "XADD %s * %s %s", this->WRITE_STREAM, key, "ERRORE: Il prodotto non esiste");
        assertReply(this->c2r, ret);
        freeReplyObject(ret);
        return 1;
    }
    
    sprintf(sqlcmd, "SELECT * FROM Acquisto WHERE prodotto = '%s';", idProdotto);

    // Execute the query and check the result
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        buy = true; // If the query returns at least one row, the product has been purchased
        PQclear(res);
    }

    if (buy) {
        sprintf(key, "Risultato");
        ret = RedisCommand(this->c2r, "XADD %s * %s %s", this->WRITE_STREAM, key, "ERRORE: Il prodotto è stato gia' acquistato");
        assertReply(this->c2r, ret);
        freeReplyObject(ret);
        return 1;
    }

    sprintf(sqlcmd, "SELECT * FROM Prodotto p WHERE p.produttore = '%s' and p.id = '%s';", produttore, idProdotto);

    // Execute the query and check the result
    res = db.ExecSQLtuples(sqlcmd); 

    if (res != NULL && PQntuples(res) > 0) {   
        if (produttore == PQgetvalue(res, 0, PQfnumber(res, "produttore"))) {
            isProduttore = false; 
        }
        PQclear(res);
    }

    if (!isProduttore) {
        sprintf(key, "Risultato");
        ret = RedisCommand(this->c2r, "XADD %s * %s %s", this->WRITE_STREAM, key, "ERRORE: Non puoi eliminare prodotti altrui");
        assertReply(this->c2r, ret);
        freeReplyObject(ret);
        return 1;
    }
    return 0;
}

void ServerP::rimuoviProdotto(int block, redisReply *reply){
    int k, i;
    redisReply *ret;

    char produttore[100];
    char idProdotto[100];
    char key[100];
    char sqlcmd[1000];
    char logmessage[1000];
    char dominio[100];
    char funzione[100];

    k = ReadNumStreams(reply) - 1; // Take the last sent stream
    
    i = ReadStreamNumMsg(reply, k) - 1; // Last message of the last stream
    
    ReadStreamMsgVal(reply, k, i, 3, produttore);
    ReadStreamMsgVal(reply, k, i, 5, idProdotto);
    
    freeReplyObject(reply);

    if (monitor(idProdotto, produttore) == 1) {
        return;
    }

    sprintf(sqlcmd, "DELETE FROM Prodotto p WHERE p.id = '%s';", idProdotto);

    
    db.ExecSQLcmd(sqlcmd);
    
    sprintf(logmessage, "Produttore %s ha rimosso prodotto: %s", produttore, idProdotto);
    sprintf(dominio, "produttore");
    sprintf(funzione, "Rimozione");
    int id_produttore = atoi(produttore);
    log2db(logmessage, id_produttore, db, dominio, funzione);

    sprintf(key, "Risultato");
    
    ret = RedisCommand(this->c2r, "XADD %s * %s %s", this->WRITE_STREAM, key, "Prodotto rimosso");
    assertReply(this->c2r, ret);
    freeReplyObject(ret);
}
