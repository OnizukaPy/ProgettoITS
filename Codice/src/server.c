// includiamo le librerie necessarie presenti nella cartella lib
#include "../lib/funzioni.h"
#include "../lib/funzioni.c"
#include "../lib/cJSON.h"
#include <dirent.h>     // per la gestione delle directory
#include <stdio.h>
#include <windows.h>


// definizioni delle costanti che saranno necessarire
#define ACCOUNT       "../account"                      // definiamo la cartella dove saranno salvati gli account
#define TEMP_LOGIN    "../temp/login.json"              // definiamo il file temporaneo di scambio informazioni
#define TEMP_LOGOUT   "../temp/logout.json"             // definiamo il file temporaneo di scambio informazioni
#define TEMP_STATUS   "../temp/status_server.json"      // definiamo il file temporaneo di scambio informazioni
#define SALA          "../sala"                         // definiamo la cartella dove saranno salvati i file della sala

// protitipi delle funzioni che saranno utilizzate nel main


// funzione principale
// nel main prevere la possibilità di usare argomenti da riga di comando

int main(int argc, char *argv[]){

    // approviamo gli account presenti nella cartella
    // creiamo un ciclo che controlla tutti gli account presenti nella cartella e termina con exit da tastiera

    // se l'argomento è uguale a -start
    if (argc == 1){
        printf("Il server è stato compilato o avviato senza istruzioni\n");
    } else {
        if(strcmp(argv[1], "-help") == 0){
            printf("questa e' la guida per l'uso del server\n");
            //print_guida_server();
        }
        if (strcmp(argv[1], "-start") == 0){
            // controlliamo la cartella temp per vedere se ci sono dei file e se ci sono li eliminiamo
            printf("Controlo se ci sono file temporanei da eliminare\n");
            DIR *dr = opendir("../temp");
            struct dirent *de;
            while ((de = readdir(dr)) != NULL){
                // se il file è diverso da . e da .. lo cancelliamo
                if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0){
                    char file[100];
                    sprintf(file, "%s/%s", "../temp", de->d_name);
                    remove(file);
                }
            }
            closedir(dr);
            printf("I file temporanei sono stati eliminati\n");

            // avviamo l'attività di monitoraggio attività
            printf("Il server e' stato avviato ed e' in attesa..\n");
            while(1){
                // controlla se c'è un file di status server in attesa e se c'è lo controlla
                if(access(TEMP_STATUS, F_OK) != -1){
                    return_status(TEMP_STATUS);
                }

                // controlla i nuovi account
                controlla_account(ACCOUNT);

                // controlla i login se c'è un file di login in attesa
                if(access(TEMP_LOGIN, F_OK) != -1){
                    login_check(TEMP_LOGIN, ACCOUNT);
                }

                // controlla i logout se c'è un file di logout in attesa
                if(access(TEMP_LOGOUT, F_OK) != -1){
                    logout_check(TEMP_LOGOUT, ACCOUNT);
                }

                // tempo di attesa
                Sleep(2000);
            }
        }
        if (strcmp(argv[1], "-c_sala") == 0){
            // creiamo una sala
            cJSON sala = crea_sala(SALA, 12);
        }
    }



    //controlla_account(ACCOUNT);

}

// definizione delle funzioni che saranno utilizzate nel main

