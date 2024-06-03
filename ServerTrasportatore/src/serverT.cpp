#include "serverT.h"

// Constructor
ServerT::ServerT(const char *nome)
    : db("localhost", "5432", "ecommerce", "47002", "db_ecommerce") {
    this->nome = nome;
    this->READ_STREAM = "stream_trasportatore_2";
    this->WRITE_STREAM = "stream_trasportatore_1";
    this->c2r = redisConnect("localhost", 6379);

    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);
}

// Read the last message sent by the client and extract the command
Cmd_Reply ServerT::readCommandRedis(int block) {
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

// Check if the transport is valid for the given purchase and deliver the product
void ServerT::consegnaProdotto(int block, redisReply *reply) {
    int k, i;
    redisReply *ret;

    char trasportatore[100];
    char idAcquisto[100];
    char key[100];
    char sqlcmd[1000];
    char logmessage[1000];
    char dominio[100];
    char funzione[100];

    k = ReadNumStreams(reply) - 1; // Take the last sent stream
    i = ReadStreamNumMsg(reply, k) - 1; // Last message of the last stream

    ReadStreamMsgVal(reply, k, i, 3, trasportatore);
    ReadStreamMsgVal(reply, k, i, 5, idAcquisto);

    freeReplyObject(reply);

    if (monitor(idAcquisto, trasportatore) == 1) {
        return;
    }

    sprintf(sqlcmd, "UPDATE Acquisto SET consegnato = true, istConsegna = CURRENT_TIMESTAMP WHERE id ='%s'", idAcquisto);

    this->db.ExecSQLcmd(sqlcmd);

    sprintf(key, "Risultato");

    sprintf(logmessage, "Trasportatore %s ha effettuato la consegna di acquisto: %s", trasportatore, idAcquisto);
    sprintf(dominio, "trasportatore");
    sprintf(funzione, "Consegna");
    int id_trasportatore = atoi(trasportatore);
    log2db(logmessage, id_trasportatore, db, dominio, funzione);

    ret = RedisCommand(this->c2r, "XADD %s * %s %s", this->WRITE_STREAM, key, "Consegnato");
    assertReply(this->c2r, ret);
    freeReplyObject(ret);
}

// Monitor the status of a purchase for delivery
int ServerT::monitor(char *idAcquisto, char *trasportatore) {
    bool exist = false;
    bool isTrasportatore = true;
    bool consegnato = false;
    char key[100];
    redisReply *ret;
    PGresult *res;

    sprintf(sqlcmd, "SELECT 1 FROM Acquisto WHERE id = '%s';", idAcquisto);

    // Execute the query and check the result
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        exist = true; // If the query returns at least one row, the product exists
        PQclear(res);
    }

    if (!exist) {
        sprintf(key, "Risultato");
        ret = RedisCommand(this->c2r, "XADD %s * %s %s", this->WRITE_STREAM, key, "ERRORE: Non esiste acquisto con questo id");
        assertReply(this->c2r, ret);
        freeReplyObject(ret);
        return 1;
    }

    sprintf(sqlcmd, "SELECT * FROM Acquisto WHERE id = '%s' and trasportatore='%s';", idAcquisto, trasportatore);

    // Execute the query and check the result
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        if (trasportatore == PQgetvalue(res, 0, PQfnumber(res, "trasportatore"))) {
            isTrasportatore = false;
        }
        PQclear(res);
    }

    if (!isTrasportatore) {
        sprintf(key, "Risultato");
        ret = RedisCommand(this->c2r, "XADD %s * %s %s", this->WRITE_STREAM, key, "ERRORE: Il trasportatore incaricato è diverso ");
        assertReply(this->c2r, ret);
        freeReplyObject(ret);
        return 1;
    }

    // Construct the SQL query
    sprintf(sqlcmd, "SELECT * FROM Acquisto WHERE id = '%s' and consegnato = true;", idAcquisto);

    // Execute the query and check the result
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        consegnato = true; // If the query returns at least one row, the product exists
        PQclear(res);
    }

    if (consegnato) {
        sprintf(key, "Risultato");
        ret = RedisCommand(this->c2r, "XADD %s * %s %s", this->WRITE_STREAM, key, "Il prodotto e' stato gia' consegnato... ");
        assertReply(this->c2r, ret);
        freeReplyObject(ret);
        return 1;
    }
    return 0;
}
