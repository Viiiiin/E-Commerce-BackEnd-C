#include "serverC.h"
#include "logger.h"


ServerC:: ServerC (
    const char *nome
):db("localhost", "5432", "ecommerce", "47002", "db_ecommerce")
{
    this->nome = nome;
    this->READ_STREAM = "stream_costumer_2";
    this->WRITE_STREAM = "stream_costumer_1";
    this->c2r = redisConnect("localhost", 6379);

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

    cout << "SEGFAULT" << endl;
    assertReply(this->c2r, reply);

    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    ReadStreamMsgVal(reply, k, i, 1, cmd);
    cmd_reply.cmd = cmd;
    cmd_reply.reply = reply;

    log("Comando letto", -1);

    return cmd_reply;
}


void ServerC::acquistaProdotto( int block, redisReply *reply){
    int k,i;
    redisReply *ret;

    char idProdotto[100];
    char key[100];
    char sqlcmd[1000]; 

    cout << "ENTRA" << endl;

    assertReply(this->c2r, reply);

    k = ReadNumStreams(reply) -1; // prendo l'ultima stream inviata
    
    i = ReadStreamNumMsg(reply, k) -1; // ultimo messaggio dell'ultima stream

    ReadStreamMsgVal(reply, k, i, 2, idProdotto);
    
    freeReplyObject(reply);

    sprintf(sqlcmd, "INSERT INTO Acquisto (istante, costumer, prodotto, trasportatore, consegnato, istConsegna) VALUES (DEFAULT, '1', \'%s\', '1', 'false', NULL) ON CONFLICT DO NOTHING", idProdotto);
    cout << sqlcmd << endl;
    
    db.ExecSQLcmd(sqlcmd);
    cout << "QUI" << endl;
    
    sprintf(key,"Risultato");
    cout << "QUI" << endl;
    
    ret = RedisCommand(this->c2r,"XADD %s * %s %s",this->WRITE_STREAM, key, "Aggiunto");
    assertReply(this->c2r, reply);
    freeReplyObject(ret);

    log("Prodotto acquistato", -1);
}