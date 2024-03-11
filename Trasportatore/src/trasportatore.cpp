#include "trasportatore.h"


Trasportatore:: Trasportatore (
    const int id,
    const char *ragioneSociale,
    const Indirizzo indirizzo     
)

{ 
    this->READ_STREAM = READ_STREAM;
    this->WRITE_STREAM = WRITE_STREAM;
    int read_counter = 0;
    int send_counter = 0;
    this->c2r = redisConnect("localhost", 6379);
    
    /* Create streams/groups */
    initStreams(this->c2r, this->READ_STREAM);
    initStreams(this->c2r, this->WRITE_STREAM);   
}

