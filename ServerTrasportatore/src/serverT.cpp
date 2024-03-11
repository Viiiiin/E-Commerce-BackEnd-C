#include "serverT.h"


ServerT:: ServerT (
    const char *nome;
)

{ 
    this->nome = nome;
    this->READ_STREAM = READ_STREAM;
    this->WRITE_STREAM = WRITE_STREAM;
    int read_counter = 0;
    int send_counter = 0;
    this->c2r = redisConnect("localhost", 6379);
    this->db = Con2DB db("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    
    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);   
}