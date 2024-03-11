#ifndef serverT.h
#define serverT.h
#include "../../con2db/pgsql.h"

#define READ_STREAM = "stream2"
#define WRITE_STREAM = "stream1"

class ServerT {
    public: 
        ServerT( const char *nome );
    private: 
        redisContext *c2r;
        redisReply *reply;  
        Con2DB db;
}



#endif
