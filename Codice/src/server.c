// includiamo le librerie necessarie presenti nella cartella lib
#include "../lib/funzioni.h"
#include "../lib/funzioni.c"
#include "../lib/cJSON.h"
#include <dirent.h>     // per la gestione delle directory
#include <stdio.h>
#include <windows.h>


// definizioni delle costanti che saranno necessarire
#define ACCOUNT       "../account"            // definiamo la cartella dove saranno salvati gli account
#define TEMP_LOGIN    "../temp/login.json"    // definiamo il file temporaneo di scambio informazioni
#define SALA          "../sala"               // definiamo la cartella dove saranno salvati i file della sala

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
            printf("Il server è stato avviato ed è in attesa..\n");
            while(1){
                // controlla i nuovi account
                controlla_account(ACCOUNT);
                // controlla i login se c'è un file di login in attesa
                if(access(TEMP_LOGIN, F_OK) != -1){
                    login_check(TEMP_LOGIN, ACCOUNT);
                }
                Sleep(5000);
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

/*

    // scriviamo un codice per avere una lista di file dentro una cartella
    // https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/

    struct dirent *de;  // Pointer for directory entry
    
    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(ACCOUNT);
    // creiamo un array di stringhe per contenere i nomi dei file presenti nella cartella
    char *files[100];
    int count = 0;
    // opendir returns NULL if couldn't open directory
    if (dr == NULL){
        printf("Could not open current directory" );
        return 0;
    } else {
        
        while ((de = readdir(dr)) != NULL){
            // salva il nome del file nell'array
            files[count] = de->d_name;
            // se il nome del file è uguale a ivan.json stampa il nome del file
            if(strcmp(files[count], "ivan.json") == 0){
                printf("%s\n", files[count]);
                approva_account(files[count], ACCOUNT);
            }
            count++;
        }
        closedir(dr);
    }

*/