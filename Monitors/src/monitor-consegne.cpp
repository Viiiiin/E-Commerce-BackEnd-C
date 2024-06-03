#include "main.h"

using namespace std;

void monitorConsegne(){
    Con2DB db("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    char sqlcmd[1000]; 
    PGresult *res;

    sprintf(sqlcmd, "SELECT prodotto FROM Acquisto WHERE consegnato = false AND istante < CURRENT_TIMESTAMP - INTERVAL '2 days';");

    // Execute the query and check the result
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {        
        int numProdotti = PQntuples(res);
        long *listaProdotti = new long[numProdotti]; // Dynamic array to store products

        // Iterate over the results and store the products in the array
        for (int i = 0; i < numProdotti; ++i) {
            listaProdotti[i] = strtol(PQgetvalue(res, i, PQfnumber(res, "prodotto")), nullptr, 10);
        }

        // Print the products stored in the array
        cout << "The following products have yet to be delivered:" << endl;
        for (int i = 0; i < numProdotti; ++i) {
            cout << listaProdotti[i] << endl;
        }

        delete[] listaProdotti; // Free the memory allocated for the array

        PQclear(res);

    } else {
        cout << "No purchases left undelivered for more than 2 days..." << endl; 
    }
}
