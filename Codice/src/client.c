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
void visualizza_logo(char *path);

// funzione principale
// nel main prevere la possibilità di usare argomenti da riga di comando

int main(int argc, char *argv[]){



    // verifichiamo gli argomenti passati da riga di comando
    if(argc == 1){
        // carichiamo dal file ../sala/Logo.txt il logo del ristorante e lo visualizziamo. Il file contiene un testo ASCII
        char logo[100];
        sprintf(logo, "%s/Logo.txt", SALA);
        // visualizziamo il logo
        visualizza_logo(logo);
        // controlliamo lo stato del server
        printf("Controllo lo stato del server\n");
        if(status_server(TEMP) == true){
            printf("Il server è attivo\n\n");
        } else {
            printf("Il server è inattivo\n\n");
        }
    } else {
        // se l'argomento è uguale a -help
        if(strcmp(argv[1], "-help") == 0){
            print_guida("guida.txt");
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
                // controlliamo che il server sia attivo
                if(status_server(TEMP) == false){
                    printf("Il server è inattivo\n");
                } else {
                // effettuiamo il login
                    login(argv[2], ACCOUNT, TEMP);
                }
            }
        }
        if(strcmp(argv[1], "-logout") == 0){
            // controlliamo se dopo -logout c'è un argomento
            if(argc == 2){
                printf("Non è stato digitato l'username riprova\n");
            } else {
                // controlliamo che il server sia attivo
                if(status_server(TEMP) == false){
                    printf("Il server è inattivo\n");
                } else {
                    // effettuiamo il logout
                    logout(argv[2], ACCOUNT, TEMP);
                }
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
void visualizza_logo(char *path){
    // apriamo il file txt in lettura
    FILE *file = fopen(path, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
        return;
    }
    // leggiamo il file e lo visualizziamo
    char c;
    while((c = fgetc(file)) != EOF){
        printf("%c", c);
    }
}