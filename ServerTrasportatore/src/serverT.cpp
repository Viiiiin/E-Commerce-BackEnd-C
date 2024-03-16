#include "serverT.h"


ServerT:: ServerT (
    const char *nome)
{
    this->nome = nome;
    this->READ_STREAM = "stream_trasportatore_2";
    this->WRITE_STREAM = "stream_trasportatore_1";
    this->c2r = redisConnect("localhost", 6379);
    Con2DB db("localhost", "5432", "ecommerce", "47002", "db_ecommerce");

    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);   
}

char *ServerT::readCommandRedis(int block){
    char* cmd = new char[100];
    int i=0;
    int k=0;
    redisReply *reply;

    reply = RedisCommand(this->c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT 1 NOACK STREAMS %s >", 
			  this->nome, block, READ_STREAM);

    assertReply(this->c2r, reply);

    ReadStreamMsgVal(reply, k, i, 4, cmd);

    return cmd;
}
