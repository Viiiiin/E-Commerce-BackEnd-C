#include "main.h"

int main(int argc,char *argv[]){
	
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " --monitor <a|f|p|s|c>" << endl;
        return 1;
    }

    string flag = argv[1];

    if (flag == "--monitor=a") {
        monitorAttivita();
    } else if (flag == "--monitor=f") {
        monitorFunzionalita();
    } else if (flag == "--monitor=p") {
        monitorPrestazioni();
    } else if (flag == "--monitor=s") {
        monitorScalabilita();
    } else if (flag == "--monitor=c") {
        monitorConsegne();
    }else {
        cerr << "Flag non riconosciuta. Usa --funzione <a|f|p|s>" << endl;
        return 1;
    }

    return 0;
}
