// includiamo le librerie necessarie presenti nella cartella lib
#include "../lib/funzioni.h"
#include "../lib/funzioni.c"
#include "../lib/cJSON.h"
#include <dirent.h>     // per la gestione delle directory
#include <stdio.h>

// definizioni delle costanti che saranno necessarire
#define ACCOUNT "../account"    // definiamo la cartella dove saranno salvati gli account

// protitipi delle funzioni che saranno utilizzate nel main


// funzione principale
// nel main prevere la possibilità di usare argomenti da riga di comando

int main(int argc, char *argv[]){

    // verifichiamo gli argomenti passati da riga di comando
    if(argc == 1){
        // se non ci sono argomenti passati da riga di comando
        // approviamo gli account presenti nella cartella
        printf("Non sono stati digitati argomenti\n");
    } else {
        // se l'argomento è uguale a -help
        if(strcmp(argv[1], "-help") == 0){
            printf("questa e' la guida per l'uso del programma\n");
            print_guida();
        }
        if(strcmp(argv[1], "-c") == 0){
            // approviamo gli account presenti nella cartella
            cJSON account = crea_account(ACCOUNT);
        }
    } 

    // creiamo un account
    return 0;

}

// definizione delle funzioni che saranno utilizzate nel main