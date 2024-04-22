#include "serverC.h"


ServerC:: ServerC (
    const char *nome
):db("localhost", "5432", "ecommerce", "47002", "db_ecommerce")
{
    this->nome = nome;
    this->READ_STREAM = "stream_costumer_2";
    this->WRITE_STREAM = "stream_costumer_1";
    this->c2r = redisConnect("localhost", 6379);
    redisReply *reply;

    // delete stream if it exists
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);   
}


Cmd_Reply ServerC::readCommandRedis(int block){
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


int ServerC::acquistaProdotto( int block, redisReply *reply){
    int k,i;
    redisReply *ret;
    PGresult *res;
    bool exist = false;
    bool buy = false;

    char idProdotto[100];
    char key[100];
    char sqlcmd[1000]; 
    char logmessage[200];
    char dominio[10];
    char funzione[10];


    assertReply(this->c2r, reply);

    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    ReadStreamMsgVal(reply, k, i, 2, idProdotto);
    
    freeReplyObject(reply);

    // Costruzione della query SQL
    sprintf(sqlcmd, "SELECT * FROM Prodotto WHERE id = '%s';", idProdotto);

    // Esecuzione della query e controllo del risultato
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        exist = true; // Se la query restituisce almeno una riga, il prodotto esiste
        PQclear(res);
    }

    if (!exist){
        sprintf(key,"Risultato");
        ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "Il prodotto NON esiste... ");
        assertReply(this->c2r, reply);
        freeReplyObject(ret);
        return 1;
    }

    // Costruzione della query SQL
    sprintf(sqlcmd, "SELECT * FROM Acquisto WHERE prodotto = '%s';", idProdotto);

    // Esecuzione della query e controllo del risultato
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        buy = true; // Se la query restituisce almeno una riga, il prodotto esiste
        PQclear(res);
    }

    if (buy){
        sprintf(key,"Risultato");
        ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "Il prodotto e' stato gia' acquistato... ");
        assertReply(this->c2r, reply);
        freeReplyObject(ret);
        return 2;
    }

    sprintf(sqlcmd, "INSERT INTO Acquisto (istante, costumer, prodotto, trasportatore, consegnato, istConsegna) VALUES (DEFAULT, '1', \'%s\', '1', 'false', NULL) ON CONFLICT DO NOTHING", idProdotto);
    db.ExecSQLcmd(sqlcmd);
    
    sprintf(logmessage,"Costumer %d ha acquistato il prodotto: %s",1,idProdotto);
    sprintf(dominio,"costumer");
    sprintf(funzione,"Acquisto");
    log2db(logmessage,1,db,dominio,funzione);

    sprintf(key,"Risultato");
    

    ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "Acquistato");
    assertReply(this->c2r, reply);
    freeReplyObject(ret);

    return 0;

}


