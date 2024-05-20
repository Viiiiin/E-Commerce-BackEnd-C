#include "main.h"

using namespace std;

void monitorConsegne(){
    Con2DB db("localhost", "5432", "ecommerce", "47002", "db_ecommerce");
    char sqlcmd[1000]; 
    PGresult *res;

    sprintf(sqlcmd, "SELECT prodotto FROM Acquisto WHERE consegnato = false AND istante < CURRENT_TIMESTAMP - INTERVAL '2 days';");

    // Esecuzione della query e controllo del risultato
    res = db.ExecSQLtuples(sqlcmd);
    if (res != NULL && PQntuples(res) > 0) {        
        int numProdotti = PQntuples(res);
        long *listaProdotti = new long[numProdotti]; // Array dinamico per memorizzare i prodotti

        // Itera sui risultati e memorizza i prodotti nell'array
        for (int i = 0; i < numProdotti; ++i) {
            listaProdotti[i] = strtol(PQgetvalue(res, i, PQfnumber(res, "prodotto")), nullptr, 10);
        }

        // Stampa i prodotti memorizzati nell'array
        cout << "I seguenti prodotti devono ancora essere consegnati:" << endl;
        for (int i = 0; i < numProdotti; ++i) {
            cout << listaProdotti[i] << endl;
        }

        delete[] listaProdotti; // Liberare la memoria allocata per l'array

        PQclear(res);

    } else {
        cout << "Nessuna acquisto rimasto non consegnato da piu di 2 giorni..." << endl; 
    }
}
