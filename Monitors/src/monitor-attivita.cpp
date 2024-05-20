#include "main.h"

using namespace std;

void monitorAttivita(){
Con2DB db("localhost", "5432", "ecommerce", "47002", "db_ecommerce");

char sqlcmd[1000];
PGresult *res;
int countConsegna;
int countAcquisto;
int countVendita;
int countRimozione;

#if (DEBUG > 0)
  setvbuf(stdout, (char*) NULL, _IONBF, 0);
  setvbuf(stderr, (char*) NULL, _IONBF, 0);
#endif
while(1){
    // Ottengo il numero di Consegne effettuate 
    sprintf(sqlcmd,"SELECT COUNT(*) AS Consegna  FROM LogTable WHERE fun='Consegna';");
    res = db.ExecSQLtuples(sqlcmd);
    countConsegna=  atoi(PQgetvalue(res, 0, PQfnumber(res, "Consegna")));
    PQclear(res);

    // Ottengo il numero di Acquisti effettuati
    sprintf(sqlcmd,"SELECT COUNT(*) AS Acquisto  FROM LogTable WHERE fun='Acquisto';");
    res = db.ExecSQLtuples(sqlcmd);
    countAcquisto=  atoi(PQgetvalue(res, 0, PQfnumber(res, "Acquisto")));
    PQclear(res);

    // Ottengo il numero di Vendite effettuate
    sprintf(sqlcmd,"SELECT COUNT(*) AS Vendita  FROM LogTable WHERE fun='Vendita';");
    res = db.ExecSQLtuples(sqlcmd);
    countVendita=  atoi(PQgetvalue(res, 0, PQfnumber(res, "Vendita")));
    PQclear(res);

    // Ottengo il numero di Rimozioni effettuate
    sprintf(sqlcmd,"SELECT COUNT(*) AS Rimozione  FROM LogTable WHERE fun='Rimozione';");
    res = db.ExecSQLtuples(sqlcmd);
    countRimozione=  atoi(PQgetvalue(res, 0, PQfnumber(res, "Rimozione")));
    PQclear(res);



    int totalCount = countAcquisto + countConsegna + countRimozione + countVendita;
    
    // Calcolo delle percentuali
    double percentAcquisto = (static_cast<double>(countAcquisto) / totalCount) * 100;
    double percentConsegna = (static_cast<double>(countConsegna) / totalCount) * 100;
    double percentRimozione = (static_cast<double>(countRimozione) / totalCount) * 100;
    double percentVendita = (static_cast<double>(countVendita) / totalCount) * 100;

    // Output delle percentuali
    cout << "Totale Operazioni: " << totalCount << endl;
    cout << "Percentuale Acquisto: " << std::fixed << std::setprecision(2) << percentAcquisto << "%" << endl;
    cout << "Percentuale Consegna: " << std::fixed << std::setprecision(2) << percentConsegna << "%" << endl;
    cout << "Percentuale Rimozione: " << std::fixed << std::setprecision(2) << percentRimozione << "%" << endl;
    cout << "Percentuale Vendita: " << std::fixed << std::setprecision(2) << percentVendita << "%" << endl;
    
    // Aspetta un minuto
    msleep(60000);
    }
}