#include "../../con2db/pgsql.h"

#include <string>
#include <iostream>
#include <iomanip>
#include "../../tools/src/main.h"

using namespace std;

int main(){
    Con2DB db("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    char sqlcmd[1000]; 
    long nanoSecNow = get_nanos();
    long value;
    PGresult *res;
    long thirtyMinutes = 30LL * 60 * 1000000000; // 1.800.000.000.000 nanosecondi

    sprintf(sqlcmd, "SELECT nanosec FROM LogTable ORDER BY nanosec DESC LIMIT 1;");

    // Esecuzione della query e controllo del risultato
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        value = strtol(PQgetvalue(res, 0, PQfnumber(res, "nanosec")), nullptr, 10);
        long difference = nanoSecNow - value;
        if (difference > thirtyMinutes) {
            if (difference > thirtyMinutes*2) {
                long nanoSecondsInOneHour = 60LL * 60 * 1000000000;
                double differenceInHours = static_cast<double>(difference) / nanoSecondsInOneHour;
                cout << "Non viene eseguita una transazione da ore: " << differenceInHours << endl;
            }
            else{
                long nanoSecondsInOneMinute = 60LL * 1000000000;
                double differenceInMinutes = static_cast<double>(difference) / nanoSecondsInOneMinute;
                cout << "Non viene eseguita una transazione da minuti: " << differenceInMinutes << endl;
            }
        }
        long nanoSecondsInOneMinute = 60LL * 1000000000;
        double differenceInMinutes = static_cast<double>(difference) / nanoSecondsInOneMinute;
        cout << "Non viene eseguita una transazione da minuti: " << differenceInMinutes << endl;
        PQclear(res);

    }
}