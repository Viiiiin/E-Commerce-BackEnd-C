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


char* ServerC::assegnaTrasportatore(){
    PGresult *res;
    char* result;
    char sqlcmd[1000]; 
    sprintf(sqlcmd, "SELECT t.id AS idTrasportatore FROM Trasportatore t WHERE t.id NOT IN (SELECT a.trasportatore FROM Acquisto a);");
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0 ) {
        result = PQgetvalue(res, 0, PQfnumber(res, "idTrasportatore"));
        PQclear(res);
        return result;
    }
    else {
        PQclear(res);
        sprintf(sqlcmd, "SELECT t.id idTrasportatore, count(*) numConsegne FROM Trasportatore t, Acquisto a WHERE t.id = a.trasportatore GROUP BY t.id ORDER BY numConsegne, t.nome");
        res = db.ExecSQLtuples(sqlcmd);
        if (res != NULL && PQntuples(res) > 0) {
            result = PQgetvalue(res, 0, PQfnumber(res, "idTrasportatore"));
            PQclear(res);
            return result;
        }
        else {
            PQclear(res);
            return NULL;
        }
    }
    return NULL;
}


int ServerC::acquistaProdotto( int block, redisReply *reply){
    int k,i;
    redisReply *ret;
    PGresult *res;
    bool exist = false;
    bool buy = false;

    char idProdotto[100];
    char idCostumer[100];
    char key[100];
    char sqlcmd[1000]; 
    char logmessage[1000];
    char dominio[10];
    char funzione[10];

    assertReply(this->c2r, reply);

    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    ReadStreamMsgVal(reply, k, i, 2, idProdotto);
    ReadStreamMsgVal(reply, k, i, 4, idCostumer);
    
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

    char* source;
    char idTrasportatore[100];
    source = assegnaTrasportatore();
    strcpy(idTrasportatore, source);
    if (idTrasportatore == NULL){
        return 3;
    }

    sprintf(sqlcmd, "INSERT INTO Acquisto (istante, costumer, prodotto, trasportatore, consegnato, istConsegna) VALUES (DEFAULT, \'%s\', \'%s\', \'%s\', 'false', NULL) ON CONFLICT DO NOTHING", idCostumer, idProdotto, idTrasportatore);
    db.ExecSQLcmd(sqlcmd);
    
    sprintf(logmessage,"Costumer %s ha acquistato il prodotto: %s",idCostumer, idProdotto);
    sprintf(dominio,"costumer");
    sprintf(funzione,"Acquisto");
    log2db(logmessage,1,db,dominio,funzione);

    sprintf(key,"Risultato");
    

    ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "Acquistato");
    assertReply(this->c2r, reply);
    freeReplyObject(ret);

    return 0;

}


