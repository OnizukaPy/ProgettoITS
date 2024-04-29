// includiamo le librerie necessarie presenti nella cartella lib
#include "../lib/funzioni.h"
#include "../lib/funzioni.c"
#include "../lib/cJSON.h"
#include <dirent.h>     // per la gestione delle directory
#include <stdio.h>

// definizioni delle costanti che saranno necessarire
#define ACCOUNT "../account"    // definiamo la cartella dove saranno salvati gli account
#define TEMP    "../temp"       // definiamo la cartella temporanea di comunicazione client server
#define SALA    "../sala"       // definiamo la cartella dove saranno salvati i file della sala

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
        if(strcmp(argv[1], "-login") == 0){
            // controlliamo se dopo -login c'è un argomento
            if(argc == 2){
                printf("Non è stato digitato l'username riprova\n");
            } else {
                // effettuiamo il login
                login(argv[2], ACCOUNT, TEMP);
            }
        }
        if (strcmp(argv[1], "-sala") == 0){
            // visualizziamo la sala
            char sala[100];
            sprintf(sala, "%s/%s.json", SALA, argv[2]);
            printf("Il tavolo selezionato è: %s\n", sala);
            visualizza_sala(sala);
        }
    } 

    // creiamo un account
    return 0;
}

// definizione delle funzioni che saranno utilizzate nel main