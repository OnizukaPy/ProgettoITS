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
    // Avvio senza argomenti
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
            printf("Il server e' inattivo. Attendere che sia attivo.\n\n");
        }
    // avvio con argomenti
    } else {
        // se l'argomento e' uguale a -help (guida)
        if(strcmp(argv[1], "-help") == 0){
            if (argc == 2){
                print_guida("guida.txt");
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }
        // se l'argomento e' uguale a -c (crea account)
        if(strcmp(argv[1], "-c") == 0){
            // approviamo gli account presenti nella cartella
            if (argc == 2) {
                if(status_server(TEMP) == false){
                    printf("Il server e' inattivo\n");
                } else {
                    cJSON account = crea_account(ACCOUNT);
                }
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }
        // se l'argomento e' uguale a -login (effettua il login)
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
        // se l'argomento e' uguale a -logout (effettua il logout)
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
        // se l'argomento e' uguale a -sala (visualizza la sala)
        if (strcmp(argv[1], "-sala") == 0){
            // visualizziamo la sala
            // se data è uguale a NULL allora visualizziamo la sala template
            if(argc == 2){
                char sala[100];
                sprintf(sala, "%s/%s.json", SALA, "template");
                printf("Il tavolo selezionato e': %s\n", sala);
                visualizza_sala(sala);
            // se data è uguale a un nome di un file json allora visualizziamo la sala selezionata
            } else if (argc == 3){
                char sala[100];
                /* 
                 * parte di codice dove trasformiamo il valore della variabile argv[2] espresso in formato data gg/mm/aaaa 
                 * in un formato di stringa che possa essere utilizzato per la ricerca del file json del tipo gg_mm_aaaa                
                 */
                sprintf(sala, "%s/%s.json", SALA, argv[2]);
                printf("La sala selezionata e': %s\n", sala);
                visualizza_sala(sala);
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }
        // se l'argomento e' uguale a -menu (visualizza il menu)
        if (strcmp(argv[1], "-menu") == 0){
            // visualizziamo il menu
            if(argc == 2){
                char menu[100];
                sprintf(menu, "%s/%s.csv", SALA, "menu");
                printf("Il menu selezionato e': %s\n", menu);
                visualizza_menu(menu);
            // se aggiungiamo -crea allora creiamo il menu ma questa funzione è nascosta
            } /*else if (argc == 3 && strcmp(argv[2], "-crea") == 0){
                // creiamo il menu
                crea_menu(SALA);
            }*/ else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }
        // se l'argomento è -p_tavolo (prenota il tavolo) seguita da username e da una data gg/mm/aaaa
        if (strcmp(argv[1], "-p_tavolo") == 0){
            // prenotiamo il tavolo
            if(argc == 4){
                // controlliamo che il server sia attivo, se non è attivo non possiamo procedere
                if(status_server(TEMP) == false){
                    printf("Il server e' inattivo\n");
                    return 0;
                } else {
                    // verifichiamo se l'account è loggato, controllando l'esistenza del file dell'account e il valore del campo loggato
                    printf("Controllo se l'utente e' loggato\n");
                    if(se_esiste(ACCOUNT, argv[2], "json") == false){
                        printf("L'account non esiste. Controlla di aver scritto giusto lo username.\n");
                        return 0;
                    } else {
                        printf("L'account esiste. Procediamo con la verifica del Login\n");
                        char save_path[100];
                        sprintf(save_path, "%s/%s.json", ACCOUNT, argv[2]);
                        cJSON *account = carica_file_json(save_path);
                        //printf("Account caricato: %s\n", save_path);
                        cJSON *loggato = cJSON_GetObjectItem(account, "login");
                        //printf("Valore del campo loggato: %s\n", loggato->valueint == 1 ? "true" : "false");
                        if(loggato->valueint == 1){
                            printf("L'utente e' loggato, puoi procedere alla prenotazione.\n");
                        } else {
                            printf("L'utente non e' loggato. Effettua il Login.\n");
                            // terminiamo il programma
                            return 0;
                        }
                    }

                    // fare qui il controllo della sala
                    char *data_json = conversione_data(argv[3]);
                    char save_sala[100];
                    if(se_esiste(SALA, data_json, "json") == false){
                        printf("Non ci sono prenotazioni in data %s, la sala e' libera\n", argv[3]);
                        // carichiamo la sala template
                        sprintf(save_sala, "%s/template.json", SALA);
                    } else {
                        printf("Esistono gia' delle prenotazioni.\n");
                        // carichiamo la sala
                        sprintf(save_sala, "%s/%s.json", SALA, data_json);
                        
                    }
                    // carichiamo la sala
                    cJSON *sala = carica_file_json(save_sala);
                    // visualizziamo la sala
                    printf("La disposizione della sala e' la seguente:\n");
                    visualizza_sala(save_sala);
                    char risposta[5];
                    do{
                        // chiediamo se si vuole prenotare un tavolo
                        printf("Vuoi prenotare un tavolo? (S/N): ");
                        scanf("%s", risposta);
                        if(strcmp(risposta, "S") == 0 || strcmp(risposta, "s") == 0){
                            break;
                        } else if(strcmp(risposta, "N") == 0 || strcmp(risposta, "n") == 0){
                            return 0;
                        } else {
                            printf("Risposta non valida. Riprova\n");
                        }
                    } while(1);

                    // avviamo la funzione di prenotazione
                    prenota_tavolo(argv[2], data_json, sala, TEMP);
                    
                }
                // svuotiamo la cartella temp per evitare problemi
                svuota_cartella(TEMP);
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }

        // se l'argomento e' uguale a -crea_sala (crea la sala) ma questa funzione è nascosta
        /*if (strcmp(argv[1], "-crea_sala") == 0){
            if(argc == 2){
                crea_sala(SALA, 4);
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }*/
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