#include "main.h"



void log2db(char* message, int id, Con2DB db,char* domainType, char* function) {
    PGresult *res;
    int rows;
    long int dbnanosec, nsafters;
    char datebuf[1000];
    char sqlcmd[1000];
    long nanosec = get_nanos();
  

    #if (DEBUG > 1000000)
    fprintf(stderr, "log2db(): vid = %d\n", vid);
    #endif

    
    sprintf(sqlcmd, "BEGIN"); 
    res = db.ExecSQLcmd(sqlcmd);
    PQclear(res);

        
    sprintf(sqlcmd, "INSERT INTO LogTable VALUES (%ld, %d, \'%s\', \'%s\',\'%s\') ON CONFLICT DO NOTHING",
        nanosec,
        id,
        domainType,
        function,
        message);

    res = db.ExecSQLcmd(sqlcmd);
        PQclear(res);
        
    sprintf(sqlcmd, "COMMIT"); 
        res = db.ExecSQLcmd(sqlcmd);
        PQclear(res);

        
    #if (DEBUG > 0)

        // fprintf(stderr, "log2db(): check insertion\n");
        
        sprintf(sqlcmd, "SELECT * FROM LogTable where (nanosec = %ld)", nanosec);

        res = db.ExecSQLtuples(sqlcmd);
        rows = PQntuples(res);

        dbnanosec = strtol(PQgetvalue(res, 0, PQfnumber(res, "nanosec")), NULL, 10);
        
        fprintf(stderr, "log2db(): inserted in LogTable (%ld, %d, \'%s\')\n",
            dbnanosec,
            atoi(PQgetvalue(res, 0, PQfnumber(res, "id"))),
            PQgetvalue(res, 0, PQfnumber(res, "loginfo"))
            );
        PQclear(res);

        nsafters = nanos2day(datebuf, dbnanosec);
        
        fprintf(stderr, "log2db(): ns = %ld = TIME_UTC = %s + %ld ns\n", dbnanosec, datebuf, nsafters);
    #endif

}


