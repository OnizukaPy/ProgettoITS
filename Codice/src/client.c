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
        // se l'argomento e' uguale a -v (visualizza account)
        if(strcmp(argv[1], "-v") == 0){
            // visualizziamo gli account
            if(argc == 3){
                char path_account[50];
                sprintf(path_account, "%s/%s.json", ACCOUNT, argv[2]);
                //printf("L'account selezionato e': %s\n", path_account);
                // controlliamo se l'account esiste ed è loggato
                if(se_esiste(ACCOUNT, argv[2], "json") == false){
                    printf("L'account non esiste\n");
                    return 0;
                } else {
                    printf("Verifica se l'account e' loggato\n");
                    login(argv[2], ACCOUNT, TEMP);
                    printf("=====================================\n");
                    char path_prenotazioni[50], path_ordini[50];
                    sprintf(path_prenotazioni, "%s/prenotazioni.csv", SALA);
                    sprintf(path_ordini, "%s/ordini.csv", SALA);

                    visualizza_account(path_account, path_prenotazioni, path_ordini);
                    printf("=====================================\n");
                }
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }

        // se l'argomento e' uguale a -e (eliminazione account) seguito dall'username
        if(strcmp(argv[1], "-e") == 0){
            // visualizziamo gli account
            if(argc == 3){
                char path_account[50];
                sprintf(path_account, "%s/%s.json", ACCOUNT, argv[2]);
                //printf("L'account selezionato e': %s\n", path_account);
                // controlliamo se l'account esiste ed è loggato
                if(se_esiste(ACCOUNT, argv[2], "json") == false){
                    printf("L'account non esiste\n");
                    return 0;
                } else {
                    printf("L'account esiste\n");
                    printf("Verifica se l'account e' loggato\n");
                    login(argv[2], ACCOUNT, TEMP);
                    //eliminazione_account(path_account, argv[2], TEMP);
                    printf("Eliminazione account in corso\n");
                    // eliminiamo tutte le prenotazioni
                    printf("Eliminazione prenotazioni in corso\n");
                    elimina_prenotazioni(argv[2], SALA, TEMP);
                    // eliminiamo l'account
                    printf("Cancellazione account in corso\n");                    
                    remove(path_account);
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
                /* char sala[100];
                sprintf(sala, "%s/%s.json", SALA, "template");
                printf("Il tavolo selezionato e': %s\n", sala);
                visualizza_sala(sala); */
                printf("Questo comando non ti e' permesso\n");
                return 0;
            // se data è uguale a un nome di un file json allora visualizziamo la sala selezionata
            } else if (argc == 3){
                char sala[100];
                /* 
                 * parte di codice dove trasformiamo il valore della variabile argv[2] espresso in formato data gg/mm/aaaa 
                 * in un formato di stringa che possa essere utilizzato per la ricerca del file json del tipo gg_mm_aaaa                
                 */
                
                if(check_formato_data(argv[2]) == false){
                    printf("Formato data non valido. Inserire la data nel formato gg/mm/aaaa\n");
                    return 0;
                }

                char *data = conversione_data(argv[2]);
                // se la sala esiste visualizziamo la sala
                if(se_esiste(SALA, data, "json") == false){
                    printf("In data %s non ci sono prenotazioni.\n", argv[2]);
                } else {
                    sprintf(sala, "%s/%s.json", SALA, data);
                    printf("La sala selezionata e': %s\n", sala);
                    visualizza_sala(sala);
                }
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
                Portata *portate = carica_menu(menu);
                visualizza_menu(portate, menu);
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }
        // se l'argomento è -p_tavolo (prenota il tavolo) seguita da username e da una data gg/mm/aaaa
        if (strcmp(argv[1], "-p_tavolo") == 0){
            // prenotiamo il tavolo
            if(argc == 4){

                // controlliamo che la data sia nel formato gg/mm/aaaa
                if(check_formato_data(argv[3]) == false){
                    printf("Formato data non valido. Inserire la data nel formato gg/mm/aaaa\n");
                    return 0;
                }
                
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
                        /* char save_path[100];
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
                        } */
                        login(argv[2], ACCOUNT, TEMP);
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
                        // se la sala è piena non possiamo prenotare
                    }
                    // carichiamo la sala
                    cJSON *sala = carica_file_json(save_sala);
                    /* fare ciclo per controllare se tutti i tavoli sono occupati e assegnare il valore di sala in pieno*/
                    if(status_sala(sala) == 0){
                        printf("Non ci sono piu' tavoli liberi\n");
                        return 0;
                    }
                    // se la sala è piena non possiamo prenotare
                    /* cJSON *piena = cJSON_GetObjectItem(sala, "piena");
                    if(cJSON_IsTrue(piena)){
                        printf("La sala e' piena. Non e' possibile prenotare un tavolo.\n");
                        return 0;
                    }  */
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
                    //prenota_tavolo(argv[2], data_json, sala, TEMP);

                    // avviamo un ciclo per chiedere la prenotazione del tavolo
                    do {

                        // avviamo la funzione di prenotazione
                        prenota_tavolo(argv[2], data_json, sala, TEMP);
                        // chiediamo se vuole prenotare un altro tavolo se ci sono ancora tavoli liberi
                        if(status_sala(sala) == 0){
                            printf("Non ci sono piu' tavoli liberi\n");
                            break;
                        }
                        char risposta[5];
                        printf("Vuoi prenotare un altro tavolo (S/N)?: ");
                        scanf("%s", risposta);
                        if(strcmp(risposta, "N") == 0 || strcmp(risposta, "n") == 0){
                            break;
                        }
                        // riscriviamo la varibaile save_sala con la sala aggiornata
                        sprintf(save_sala, "%s/%s.json", SALA, data_json);
                        visualizza_sala(save_sala);

                    } while(1);
                    
                }
                // svuotiamo la cartella temp per evitare problemi
                svuota_cartella(TEMP);
            } else {
                printf("Errore nell'inserimento dei parametri\n");
            }
        }
        // se l'argomento è -e_tavolo (elimina la prenotazione del tavolo) seguita dal codice della prenotazione
        if (strcmp(argv[1], "-e_tavolo") == 0){
            if (argc == 3){
                // controlliamo che l'argomento sia un numero
                int prenotazione = atoi(argv[2]);
                // eliminiamo la prenotazione
                elimina_tavolo(prenotazione, SALA, TEMP);
            }
        }

        // se l'argomento è -ordina (effettua l'ordine) seguito da username
        if (strcmp(argv[1], "-ordina") == 0){

            // effettuiamo l'ordine
            if(argc == 3){

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
                        // effettuiamo il login
                        login(argv[2], ACCOUNT, TEMP);
                    }

                    // Visualizziamo le prenotazioni dell'utente
                    char path_prenotazioni[50];
                    sprintf(path_prenotazioni, "%s/prenotazioni.csv", SALA);
                    printf("--------------------------------\n");
                    if(check_prenotazione_u(argv[2], path_prenotazioni) == false){
                        printf("Non ci sono prenotazioni per l'utente %s\n", argv[2]);
                        printf("--------------------------------\n");
                        return 0;
                    }
                    visualizza_prenotazioni(argv[2], path_prenotazioni);
                    printf("--------------------------------\n");

                    // dichiariamo la struttura della prenotazione
                    Prenotazione prenotazione;

                    // chiediamo per quale prenotazione si vuole effettuare l'ordine
                    do {
                        printf("Inserisci il numero della prenotazione per la quale vuoi effettuare l'ordine: ");
                        int num_prenotazione;
                        scanf("%d", &num_prenotazione);
                        // controlliamo se la prenotazione esiste
                        if (check_prenotazione_c(num_prenotazione, path_prenotazioni)){
                            // cicliamo il file delle prenotazioni e carichiamo la prenotazione corrispondente
                            FILE *file = fopen(path_prenotazioni, "r");
                            if(file == NULL){
                                printf("Errore nell'apertura del file\n");
                                return 0;
                            }
                            char riga[100];
                            while(fgets(riga, 100, file) != NULL){
                                Prenotazione temp = carica_prenotazioni(riga);
                                if(temp.codice == num_prenotazione){
                                    printf("Prenotazione trovata\n");
                                    prenotazione = temp;
                                    break;
                                }
                            }
                            break;
                        }else {
                            printf("La prenotazione non esiste. Riprova\n");
                        }

                    } while(1);

                    // printiamo la prenotazione
                    printf("--------------------------------\n");
                    printf("Prenotazione selezionata:\n");
                    printf("Codice: %d\n", prenotazione.codice);
                    printf("Data: %s\n", prenotazione.data);
                    printf("Tavolo: %d\n", prenotazione.tavolo);
                    printf("Username: %s\n", prenotazione.username);
                    printf("--------------------------------\n");


                    // estraiamo la data, il tavolo e lo username della prenotazione
                    char *data = prenotazione.data;
                    int tavolo = prenotazione.tavolo;
                    char *username = prenotazione.username;

                    
                    // carichiamo le portate del menu
                    char path_menu[50];
                    sprintf(path_menu, "%s/menu.csv", SALA);
                    Portata *menu = carica_menu(path_menu);
                    int num_portate = conta_righe(path_menu);

                    // creiamo l'ordine completo
                    OrdineCompleto ordine;
                    ordine.n_prenotazione = prenotazione.codice;
                    strcpy(ordine.username, username);
                    strcpy(ordine.data, data);
                    ordine.tavolo = tavolo;
                    ordine.OrdineSingolo = (OrdineSingolo *)malloc(num_portate * sizeof(OrdineSingolo));    // allochiamo la memoria per l'ordine pari alle portate del menu
                    // inizializziamo il conteggio degli ordini che si incrementerà ad ogni ordine singolo effettuato
                    int n_ordini = 0;
                    // inizializziamo il conto totale
                    ordine.conto_totale = 0.0;

                    do {
                        // effettuiamo l'ordine
                        OrdineSingolo ordine_singolo = crea_ordine(menu, num_portate, ordine);

                        // salviamo l'ordine nell'ordine completo
                        ordine.OrdineSingolo[n_ordini] = ordine_singolo;
                        // incrementiamo il numero di ordini
                        n_ordini++;
                        // incrementiamo il conto totale
                        ordine.conto_totale += ordine_singolo.portata.prezzo * ordine_singolo.quantita;

                        // chiediamo se vuole ordinare altro
                        char risposta[5];
                        printf("Vuoi ordinare altro (S/N)?: ");
                        scanf("%s", risposta);
                        if(strcmp(risposta, "N") == 0 || strcmp(risposta, "n") == 0){
                            break;
                        }
                    } while(1);
                
                    // salviamo l'ordine completo in un file json
                    cJSON *ordine_json = crea_ordine_completo_json(ordine, n_ordini);

                    // salviamo un file temp json con la richiesta di ordine
                    char path_ordine[50];
                    sprintf(path_ordine, "%s/ordine.json", TEMP);

                    // creiamo il file json temporaneo
                    printf("Salviamo l'ordine in corso\n");
                    salva_ordine(ordine_json, path_ordine);
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