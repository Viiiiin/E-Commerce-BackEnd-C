
#include "produttore.h"

int main(){

    Produttore prod(4);
    char *nome = "Computer";
    char *descrizione = "Un computer";
    prod.inserisciProdotto(nome,descrizione);

    return 0;
}