// includiamo le librerie necessarie presenti nella cartella lib
#include "../lib/funzioni.h"
#include "../lib/funzioni.c"
#include "../lib/cJSON.h"
#include <dirent.h>     // per la gestione delle directory
#include <stdio.h>
#include <windows.h>


// definizioni delle costanti che saranno necessarire
#define ACCOUNT       "../account"                      // definiamo la cartella dove saranno salvati gli account
#define TEMP          "../temp"                         // definiamo la cartella temporanea di comunicazione client server
#define TEMP_LOGIN    "../temp/login.json"              // definiamo il file temporaneo di scambio informazioni
#define TEMP_LOGOUT   "../temp/logout.json"             // definiamo il file temporaneo di scambio informazioni
#define TEMP_STATUS   "../temp/status_server.json"      // definiamo il file temporaneo di scambio informazioni
#define TEMP_PRENOTAZIONE "../temp/prenotazione.json"   // definiamo il file temporaneo di scambio informazioni
#define TEMP_PRENOTAZIONE_CANC "../temp/prenotazione_canc.json"   // definiamo il file temporaneo di scambio informazioni
#define TEMP_ORDINE   "../temp/ordine.json"             // definiamo il file temporaneo di scambio informazioni
#define TEMP_ORDINE_CANC   "../temp/ordine_canc.json"   // definiamo il file temporaneo di scambio informazioni
#define TEMP_RECENSIONE "../temp/recensione.json"       // definiamo il file temporaneo di scambio informazioni
#define SALA          "../sala"                         // definiamo la cartella dove saranno salvati i file della sala

// protitipi delle funzioni che saranno utilizzate nel main


// funzione principale
// nel main prevere la possibilità di usare argomenti da riga di comando

int main(int argc, char *argv[]){

    // controlliamo che ci siano le cartelle necessarie al funzionamento del server
    if(access(ACCOUNT, F_OK) == -1){
        printf("La cartella account non esiste\n");
        // creiamo la cartella account
        if(mkdir(ACCOUNT) == -1){
            printf("Errore nella creazione della cartella account\n");
            return 0;
        } 
    } else {
            printf("La cartella account esiste\n");
            //Fare un controllo per sloggare tutti gli utenti presenti nella cartella e sono loggati
            controlla_account(ACCOUNT, "logout_all");
    }

    if(access(TEMP, F_OK) == -1){
        printf("La cartella temp non esiste\n");
        // creiamo la cartella temp
        if(mkdir(TEMP) == -1){
            printf("Errore nella creazione della cartella temp\n");
            return 0;
        } else {
            printf("La cartella temp e' stata creata con successo\n");
        }
    }
    if(access(SALA, F_OK) == -1){
        printf("La cartella sala non esiste\n");
        // creiamo la cartella sala
        if(mkdir(SALA) == -1){
            printf("Errore nella creazione della cartella sala\n");
            return 0;
        } else {
            printf("La cartella sala e' stata creata con successo\n");
        }
    }

    printf("Tutte le cartelle necessarie sono presenti.\n");
    // se l'argomento è uguale a -start
    if (argc == 1){
        printf("Il server è stato compilato o avviato senza istruzioni\n");
    } else {
        if(strcmp(argv[1], "-help") == 0){
            printf("questa e' la guida per l'uso del server\n");
            print_guida_server();
        }
        if (strcmp(argv[1], "-start") == 0){
            // controlliamo la cartella temp per vedere se ci sono dei file e se ci sono li eliminiamo
            printf("Controlo se ci sono file temporanei da eliminare\n");
            DIR *dr = opendir(TEMP);
            // controlliamo se la cartella esiste
            if (dr == NULL){
                printf("La cartella temp non esiste\n");
                return 0;
            } else {
                //printf("La cartella temp esiste\n");
                svuota_cartella(TEMP);
                printf("I file temporanei sono stati eliminati\n");
            }

            // avviamo l'attività di monitoraggio attività
            printf("Il server e' stato avviato ed e' in attesa..\n");
            while(1){
                // controlla se c'è un file di status server in attesa e se c'è lo controlla
                if(access(TEMP_STATUS, F_OK) != -1){
                    return_status(TEMP_STATUS);
                }

                // controlla i nuovi account
                controlla_account(ACCOUNT, "approva");

                // controlla i login se c'è un file di login in attesa
                if(access(TEMP_LOGIN, F_OK) != -1){
                    login_check(TEMP_LOGIN, ACCOUNT);
                }

                // controlla i logout se c'è un file di logout in attesa
                if(access(TEMP_LOGOUT, F_OK) != -1){
                    logout_check(TEMP_LOGOUT, ACCOUNT);
                }

                /* // controlliamo nella cartella sala se ci sono sale da controllare
                // leggiamo il conentuto della cartella sala e se ci sono file .json che hanno il nome diverso da template 
                // li controlliamo
                DIR *dr = opendir(SALA);
                struct dirent *de;
                while ((de = readdir(dr)) != NULL){
                    if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0 && strcmp(de->d_name, "template.json") != 0 && strstr(de->d_name, ".json") != NULL){
                        char path[100];
                        sprintf(path, "%s/%s", path, de->d_name);
                        controlla_sala(path);
                    }
                }
                closedir(dr); */

                // controlla le prenotazioni se c'è un file di prenotazione in attesa
                if(access(TEMP_PRENOTAZIONE, F_OK) != -1){
                    conferma_prenotazione(TEMP, SALA);
                }

                // controlliamo se ci sono prenotazioni da eliminare
                if(access(TEMP_PRENOTAZIONE_CANC, F_OK) != -1){
                    elimina_prenotazione(TEMP, SALA);
                }

                // controlliamo se ci sono ordini da confermare
                if (access(TEMP_ORDINE, F_OK) != -1){
                    conferma_ordine(TEMP, SALA);
                }

                // controlliamo se ci sono ordini da eliminare
                //printf("Controllo se ci sono ordini da eliminare\n");
                if (access(TEMP_ORDINE_CANC, F_OK) != -1){
                    elimina_ordinazione(TEMP, SALA);
                }

                // controlliamo se ci sono recensioni da approvare
                if(access(TEMP_RECENSIONE, F_OK) != -1){
                    approva_recensione(TEMP, SALA);
                }


                // tempo di attesa
                Sleep(2000);
            }
        }
        /* if (strcmp(argv[1], "-c_sala") == 0){
            // creiamo una sala
            crea_sala(SALA, 12);
        } */
    }
}

// definizione delle funzioni che saranno utilizzate nel main

