#include "trasportatore.h"



Trasportatore:: Trasportatore (
    const int id,
    const char *ragioneSociale   
)

{ 
    this->READ_STREAM = "stream_trasportatore_1";
    this->WRITE_STREAM = "stream_trasportatore_2";
    int read_counter = 0;
    int send_counter = 0;
    this->c2r = redisConnect("localhost", 6379);
    
    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);  
};

