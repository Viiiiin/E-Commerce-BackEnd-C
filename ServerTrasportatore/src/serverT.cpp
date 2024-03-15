#include "serverT.h"
#include "pgsql.h" // Assuming this is the header file for con2DB

ServerT::ServerT(const char *nome) : nome(nome), READ_STREAM("stream_trasportatore_2"), WRITE_STREAM("stream_trasportatore_1") {
    // Connect to Redis
    this->c2r = redisConnect("localhost", 6379);
    if (this->c2r == nullptr || this->c2r->err) {
        throw std::runtime_error("Failed to connect to Redis server");
    }

    // Connect to con2DB
    this->c2db = new Con2DB(); // Assuming Con2DB has a default constructor
    if (!this->c2db->connect("localhost", "user", "password", "database")) {
        throw std::runtime_error("Failed to connect to con2DB");
    }

    // Create streams/groups in Redis
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);
}

std::string ServerT::readCommandRedis(int block) {
    redisReply *reply = RedisCommand(this->c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT 1 NOACK STREAMS %s >", 
                                    this->nome.c_str(), block, this->READ_STREAM.c_str());

    if (reply == nullptr) {
        throw std::runtime_error("Failed to read from Redis");
    }

    std::string cmd;
    if (reply->elements >= 2 && reply->element[0]->elements >= 2 && reply->element[1]->elements >= 2) {
        cmd = reply->element[1]->element[1]->str;
    }

    freeReplyObject(reply);
    return cmd;
}

ServerT::~ServerT() {
    redisFree(this->c2r);
    delete this->c2db;
}
