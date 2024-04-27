#include "serverT.h"


ServerT:: ServerT (
    const char *nome)
:db("localhost", "5432", "ecommerce", "47002", "db_ecommerce")
{
    this->nome = nome;
    this->READ_STREAM = "stream_trasportatore_2";
    this->WRITE_STREAM = "stream_trasportatore_1";
    this->c2r = redisConnect("localhost", 6379);
    //redisReply *reply;

    
    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);   
}


Cmd_Reply ServerT::readCommandRedis(int block){
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


void ServerT::consegnaProdotto(int block,redisReply *reply){    
    int k,i;
    redisReply *ret;
    PGresult *res;

    char trasportatore[100];
    char idAcquisto[100];
    char key[100];
    char sqlcmd[1000];
    char logmessage[1000];
    char dominio[100];
    char funzione[100];
    bool exist=false;
    bool isTrasportatore=false;
    


    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    
    ReadStreamMsgVal(reply, k, i, 3, trasportatore);
    ReadStreamMsgVal(reply, k, i, 5, idAcquisto);
    
    cout<<"trasportatore";
    freeReplyObject(reply);

    sprintf(sqlcmd, "SELECT 1 FROM Acquisto WHERE id = '%s';", idAcquisto);

    // Esecuzione della query e controllo del risultato
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        exist = true; // Se la query restituisce almeno una riga, il prodotto esiste
        PQclear(res);
    }

    if (!exist){
        sprintf(key,"Risultato");
        ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "ERRORE: Non esiste acquisto con questo id");
        assertReply(this->c2r, reply);
        freeReplyObject(ret);
        return;
    }

    
    sprintf(sqlcmd, "SELECT * FROM Acquisto WHERE id = '%s' and trasportatore='%s';", idAcquisto,trasportatore);

    // Esecuzione della query e controllo del risultato
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        if (trasportatore==PQgetvalue(res, 0, PQfnumber(res, "trasportatore"))){
            isTrasportatore = true; 
        }
        PQclear(res);
    }

    if (!isTrasportatore){
        sprintf(key,"Risultato");
        ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "ERRORE: Il trasportatore incaricato è diverso ");
        assertReply(this->c2r, reply);
        freeReplyObject(ret);
        return;
    }

    sprintf(sqlcmd, "UPDATE Acquisto SET consegnato = true, istConsegna = CURRENT_TIMESTAMP WHERE id ='%s'",idAcquisto);
    
    this->db.ExecSQLcmd(sqlcmd);
    
    
    sprintf(key,"Risultato");

    sprintf(logmessage,"Trasportatore %s ha effettuato la consegna dell' acquisto: %s",trasportatore,idAcquisto);
    sprintf(dominio,"trasportatore");
    sprintf(funzione,"Consegna");
    log2db(logmessage,trasportatore,db,dominio,funzione);
    
    ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "Consegnato");
    assertReply(this->c2r, ret);
    freeReplyObject(ret);

}
