// Request the server to add a product
#include "costumer.h"

// Constructor to initialize a Costumer object with an ID
Costumer::Costumer(int id)
{
    this->id = id;
    this->READ_STREAM = "stream_costumer_1";   // Name of the read stream
    this->WRITE_STREAM = "stream_costumer_2";  // Name of the write stream
    this->c2r = redisConnect("localhost", 6379); // Establish connection with Redis server

    /* 
    Uncomment this section if you want to delete the streams if they exist
    */
    /*
    redisReply *reply;

    // delete stream if it exists
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);
    */

    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);   // Initialize the read stream
    initStreams(this->c2r, this->WRITE_STREAM);  // Initialize the write stream
}

// Method to simulate the purchase of a product by a costumer
string Costumer::acquistaProdotto(int prodotto)
{
    char key0[100];
    char key1[100];
    char key2[100];
    int block = 10000000;
    int k,i;
    char result[100];
    redisReply *reply;

    // Formulate keys for the command
    sprintf(key0,"comando");
    sprintf(key1, "prodotto");
    sprintf(key2, "costumer");

    // Send a command to add a product to the write stream
    reply = RedisCommand(this->c2r, "XADD %s * %s %s %d %s %d %s", this->WRITE_STREAM, key1, "Acquista", prodotto, key0, this->id, key2);

    // Ensure that the reply type is a string
    assertReplyType(this->c2r,reply,REDIS_REPLY_STRING);

    freeReplyObject(reply);

    // Read the reply from the read stream
    reply = RedisCommand(this->c2r, "XREADGROUP GROUP diameter %d BLOCK %d COUNT 1 NOACK STREAMS %s >", this->id, block, this->READ_STREAM);
    
    // Ensure that the reply is received successfully
    assertReply(this->c2r, reply);

    k = ReadNumStreams(reply) -1; // Take the last sent stream
    
    i = ReadStreamNumMsg(reply, k) -1; // Last message of the last stream

    // Read the value of the message
    ReadStreamMsgVal(reply, k, i, 1, result);
    freeReplyObject(reply);

    return result;
}
