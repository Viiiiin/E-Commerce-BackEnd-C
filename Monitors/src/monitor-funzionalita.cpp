#include "main.h"

using namespace std;

void monitorFunzionalita(){
    Con2DB db("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    char sqlcmd[1000]; 
    long nanoSecNow = get_nanos(); // Get the current time in nanoseconds
    long value;
    PGresult *res;
    long thirtyMinutes = 30LL * 60 * 1000000000; // 1,800,000,000,000 nanoseconds (30 minutes)

    sprintf(sqlcmd, "SELECT nanosec FROM LogTable ORDER BY nanosec DESC LIMIT 1;");

    // Execute the query and check the result
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {
        value = strtol(PQgetvalue(res, 0, PQfnumber(res, "nanosec")), nullptr, 10);
        long difference = nanoSecNow - value;
        
        // Check if the difference is greater than thirty minutes
        if (difference > thirtyMinutes) {
            if (difference > thirtyMinutes * 2) {
                long nanoSecondsInOneHour = 60LL * 60 * 1000000000;
                double differenceInHours = static_cast<double>(difference) / nanoSecondsInOneHour;
                cout << "No transactions have been executed for hours: " << differenceInHours << endl;
            } else {
                long nanoSecondsInOneMinute = 60LL * 1000000000;
                double differenceInMinutes = static_cast<double>(difference) / nanoSecondsInOneMinute;
                cout << "No transactions have been executed for minutes: " << differenceInMinutes << endl;
            }
        }

        long nanoSecondsInOneMinute = 60LL * 1000000000;
        double differenceInMinutes = static_cast<double>(difference) / nanoSecondsInOneMinute;
        cout << "No transactions have been executed for minutes: " << differenceInMinutes << endl;

        PQclear(res);

    }
}
