#ifndef trasportatore.h
#define trasportatore.h

#define READ_STREAM = "stream1"
#define WRITE_STREAM = "stream2"

class Trasportatore {
    public: 
        Trasportatore(
            const int id,
            const char *ragioneSociale,
            const Indirizzo indirizzo 
        ); 
    private: 
        redisContext *c2r;
        redisReply *reply; 
}



#endif
