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
            printf("Il server e' attivo\n\n");
        } else {
            printf("Il server e' inattivo\n\n");
        }
    } else {
        // se l'argomento e' uguale a -help
        if(strcmp(argv[1], "-help") == 0){
            if (argc == 2){
                print_guida("guida.txt");
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }
        // se l'argomento e' uguale a -c
        if(strcmp(argv[1], "-c") == 0){
            // approviamo gli account presenti nella cartella
            if (argc == 2) {
            cJSON account = crea_account(ACCOUNT);
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }
        if(strcmp(argv[1], "-login") == 0){
            // controlliamo se dopo -login c'e' un argomento
            if(argc == 2){
                printf("Non e' stato digitato l'username riprova\n");
            } else if (argc == 3){
                // controlliamo che il server sia attivo
                if(status_server(TEMP) == false){
                    printf("Il server e' inattivo\n");
                } else {
                // effettuiamo il login
                    login(argv[2], ACCOUNT, TEMP);
                }
                // svuotiamo la cartella temp per evitare problemi
                svuota_cartella(TEMP);
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }
        if(strcmp(argv[1], "-logout") == 0){
            // controlliamo se dopo -logout c'e' un argomento
            if(argc == 2){
                printf("Non e' stato digitato l'username riprova\n");
            } else if (argc == 3){
                // controlliamo che il server sia attivo
                if(status_server(TEMP) == false){
                    printf("Il server e' inattivo\n");
                } else {
                    // effettuiamo il logout
                    logout(argv[2], ACCOUNT, TEMP);
                }
                // svuotiamo la cartella temp per evitare problemi
                svuota_cartella(TEMP);
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }
        if (strcmp(argv[1], "-sala") == 0){
            // visualizziamo la sala
                // se data è uguale a NULL allora visualizziamo la sala template
            if(argc == 2){
                char sala[100];
                sprintf(sala, "%s/%s.json", SALA, "template");
                printf("Il tavolo selezionato e': %s\n", sala);
                visualizza_sala(sala);
            } else if (argc == 3){
                char sala[100];
                sprintf(sala, "%s/%s.json", SALA, argv[2]);
                printf("Il tavolo selezionato e': %s\n", sala);
                visualizza_sala(sala);
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }
    } 
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