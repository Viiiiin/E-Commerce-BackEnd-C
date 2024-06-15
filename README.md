# BACK-END

## creazione database
1. Entrare nella cartella db-scripts
2. Eseguire lo script create.sh

## Compilazione
### Per ogni cartella 
1. Entrare nella directory src
2. Aprire terminale ed eseguire il comando make clean ed il comando make

## Esecuzione
### Esecuzione lato server
#### Per ogni componente 'server' (quindi le cartelle che iniziano con la scritta 'Server')
1. Entrare nella directory bin 
2. Mettere in ascolto il server aprendo il terminale ed eseguendo il comando ./main

### Esecuzione lato client Produttore
1. Entrare nella directory bin 
2. Aprire terminale ed eseguire il comando ./main -p numeroProduttori -t timeToSleepMillisec -n numeroOperazioni\n
Nota: il numero dei produttori può essere al massimo il numero di produttori inseriti nel DB\n
Esempio: ./main -p 3 -t 1 -n 20\n

### Esecuzione lato client Costumer
1. Entrare nella directory bin 
2. Aprire terminale ed eseguire il comando ./main -c numeroCostumer -t timeToSleepMillisec -n numeroOperazioni
Nota: il numero dei costumer può essere al massimo il numero di costumer inseriti nel DB\n
Esempio: ./main -c 3 -t 1 -n 20\n

### Esecuzione lato client Trasportatori
1. Entrare nella directory bin 
2. Aprire terminale edeseguire il comando ./main -r numeroTrasportatori -t timeToSleepMillisec -n numeroOperazioni
Nota: il numero dei Trasportatori può essere al massimo il numero di trasportatori inseriti nel DB\n
Esempio: ./main -r 3 -t 1 -n 20\n

### Esecuzione Monitor
1. Entrare nella directory Monitor
2. Aprire terminale ed eseguire il comando ./main -- monitor = <a|f|p|s|c>\n
   ./main --monitor=a esegue il monitor 'Attività'\n
   ./main --monitor=f esegue il monitor 'Funzionalità'\n
   ./main --monitor=p esegue il monitor 'Prestazioni'\n
   ./main --monitor=s esegue il monitor 'Scalabilità'\n
   ./main --monitor=c esegue il monitor 'Consegne'\n



