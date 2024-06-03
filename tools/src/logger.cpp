#include "main.h"

// Log a message to the database
void log2db(char* message, int id, Con2DB db, char* domainType, char* function) {
    PGresult *res;
    int rows;
    long int dbnanosec, nsafters;
    char datebuf[1000];
    char sqlcmd[1000];
    long nanosec = get_nanos(); 
    
    // Begin a transaction
    sprintf(sqlcmd, "BEGIN"); 
    res = db.ExecSQLcmd(sqlcmd);
    PQclear(res);

    // Insert the log message into the LogTable
    sprintf(sqlcmd, "INSERT INTO LogTable VALUES (%ld, %d, \'%s\', \'%s\',\'%s\') ON CONFLICT DO NOTHING",
        nanosec,
        id,
        domainType,
        function,
        message);
    res = db.ExecSQLcmd(sqlcmd);
    PQclear(res);

    // Commit the transaction
    sprintf(sqlcmd, "COMMIT"); 
    res = db.ExecSQLcmd(sqlcmd);
    PQclear(res);

    #if (DEBUG > 0)
    // Debugging information
    sprintf(sqlcmd, "SELECT * FROM LogTable where (nanosec = %ld)", nanosec);
    res = db.ExecSQLtuples(sqlcmd);
    rows = PQntuples(res);

    // Get nanosec value from database result
    dbnanosec = strtol(PQgetvalue(res, 0, PQfnumber(res, "nanosec")), NULL, 10);
    
    // Print inserted log message
    fprintf(stderr, "log2db(): inserted in LogTable (%ld, %d, \'%s\')\n",
        dbnanosec,
        atoi(PQgetvalue(res, 0, PQfnumber(res, "id"))),
        PQgetvalue(res, 0, PQfnumber(res, "loginfo"))
    );
    PQclear(res);

    // Calculate date from nanoseconds
    nsafters = nanos2day(datebuf, dbnanosec);
    
    // Print debugging information about nanoseconds
    fprintf(stderr, "log2db(): ns = %ld = TIME_UTC = %s + %ld ns\n", dbnanosec, datebuf, nsafters);
    #endif
}
