#include "../lib/cJSON.h"
#include "../lib/cJSON.c"
#include "../lib/funzioni.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

// definizioni delle costanti che saranno necessarire
#define BUFFER_SIZE_JSON 1024                   // creiamo un buffer di 1024 caratteri per la gestione dei file json
#define CHIAVE 3                                // definiamo la chiave per la cifratura
// funzioni propedeutiche

// funzioni per la gestione dei file json
// funzione per caricare un file json
void salva_file_json(cJSON *root, char *nome_file){

    FILE *file;                                 // creiamo un puntatore di tipo FILE
    file = fopen(nome_file, "w");               // apriamo il file in scrittura
    fprintf(file, "%s", cJSON_Print(root));     // scriviamo il file json
    fclose(file);                               // chiudiamo il file
}

// funzione per caricare un file json
cJSON *carica_file_json(char *nome_file){

    FILE *file;                                 // creiamo un puntatore di tipo FILE
    file = fopen(nome_file, "r");               // apriamo il file in lettura
    char buffer[BUFFER_SIZE_JSON];              
    fread(buffer, BUFFER_SIZE_JSON, 1, file);   // leggiamo il file
    fclose(file);                               // chiudiamo il file
    cJSON *root = cJSON_Parse(buffer);          // creiamo un oggetto cJSON e lo popoliamo con i dati del file
    return root;                                // ritorniamo l'oggetto cJSON
}

// cifratura e decifratura
// funzione per cifrare una stringa
char *cifra(char *stringa, int chiave){
    // cifriamo la stringa con il cifrario di cesare
    for(int i = 0; i < strlen(stringa); i++){
        if(stringa[i] >= 'a' && stringa[i] <= 'z'){
            stringa[i] = ((stringa[i] - 'a' + chiave) % 26) + 'a';
        }else if(stringa[i] >= 'A' && stringa[i] <= 'Z'){
            stringa[i] = ((stringa[i] - 'A' + chiave) % 26) + 'A';
        }
    }
    /*for(int i = 0; stringa[i] != '\0'; i++){
        if(stringa[i] >= 'a' && stringa[i] <= 'z'){
            if(stringa[i] > 'x'){
                stringa[i] = stringa[i] - 26 + 3;
            }else{
                stringa[i] = stringa[i] + 3;
            }
        }else if(stringa[i] >= 'A' && stringa[i] <= 'Z'){
            if(stringa[i] > 'X'){
                stringa[i] = stringa[i] - 26 + 3;
            }else{
                stringa[i] = stringa[i] + 3;
            }
        }
    }*/
    return stringa;
}


// funzione per decifrare una stringa
char *decifra(char *stringa, int chiave){
    // decifriamo la stringa con il cifrario di cesare
    for(int i = 0; i < strlen(stringa); i++){
        if(stringa[i] >= 'a' && stringa[i] <= 'z'){
            stringa[i] = ((stringa[i] - 'a' - chiave) % 26) + 'a';
        }else if(stringa[i] >= 'A' && stringa[i] <= 'Z'){
            stringa[i] = ((stringa[i] - 'A' - chiave) % 26) + 'A';
        }
    }
    /*for(int i = 0; stringa[i] != '\0'; i++){
        if(stringa[i] >= 'a' && stringa[i] <= 'z'){
            if(stringa[i] < 'd'){
                stringa[i] = stringa[i] + 26 - 3;
            }else{
                stringa[i] = stringa[i] - 3;
            }
        }else if(stringa[i] >= 'A' && stringa[i] <= 'Z'){
            if(stringa[i] < 'D'){
                stringa[i] = stringa[i] + 26 - 3;
            }else{
                stringa[i] = stringa[i] - 3;
            }
        }
    }*/
    return stringa;
}

// funzioni effettive per il client
// funzione per stampare la guida
void print_guida(){
    printf("-------------------------------------------------\n");
    printf("Per creare un account digitare -c\n");
    printf("Per visualizzare la guida digitare -help\n");
}
// funzione per creare un account
cJSON crea_account(char *path){

    // creiamo un oggetto cJSON per contenere i dati dell'account
    cJSON *account = cJSON_CreateObject();

    // nell'account inseriremo: username, password, email, nome, cognome ed assegneremo un codice univoco
    char username[50], password[50], email[50], nome[50], cognome[50];

    // chiediamo all'utente di inserire i dati
    printf("Inserisci il tuo nome: ");
    scanf("%s", nome);
    printf("Inserisci il tuo cognome: ");
    scanf("%s", cognome);
    printf("Inserisci il tuo username: ");
    scanf("%s", username);
    printf("Inserisci la tua password: ");
    scanf("%s", password);
    printf("Inserisci la tua email: ");
    scanf("%s", email);

    // cifriamo la password
    char *password_cifrata = cifra(password, CHIAVE);

    // inseriamo i dati nell'oggetto cJSON
    cJSON_AddStringToObject(account, "nome", nome);
    cJSON_AddStringToObject(account, "cognome", cognome);
    cJSON_AddStringToObject(account, "username", username);
    cJSON_AddStringToObject(account, "password", password_cifrata);
    cJSON_AddStringToObject(account, "email", email);
    // Inseriamo un campo per gestire la richiesta di login e logout. Quando il valore è 0 l'utente non ha fatto richiesta di login
    cJSON_AddBoolToObject(account, "login", 0);

    // creiamo la chiave json dello status dell'account come un boleano. Sarà 0 se l'account sarà disconnesso e 1 se l'account sarà connesso
    cJSON_AddBoolToObject(account, "status", 0);

    // salviamo l'account in un file json con il nome dell'username
    char save_path[50];
    sprintf(save_path, "%s/%s.json", path, username);        // la funzione sprintf permette di concatenare stringhe
    salva_file_json(account, save_path);

    // creiamo un ciclo che controllo se l'account è stato approvato dal server fino a che non sarà approvato l'account non potrà fare il login
    // il ciclo si ferma quando status è uguale a 1
    // riapriamo il file json per controllare lo status dell'account
    cJSON *json = carica_file_json(save_path);
    //cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");
    while(cJSON_GetObjectItem(json, "status")->valueint == 0){
        printf("Account in attesa di approvazione\n");
        json = carica_file_json(save_path);
        // attendiamo 5 secondi
        Sleep(1000);
    }
    // stampiamo un messaggio di account approvato con la data e l'ora
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d: Account approvato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    //printf("Account approvato\n");

    // ritorniamo lo username
    return *account;
}

// funzioni effettive per il server
// funzione per approvare un account
void approva_account(char *nome_file, char *path){

    // carichiamo l'account
    char save_path[50];
    sprintf(save_path, "%s/%s", path, nome_file);
    cJSON *account = carica_file_json(save_path);

    // modifichiamo il valore del campo status in attivo
    cJSON_ReplaceItemInObject(account, "status", cJSON_CreateBool(1));

    // salviamo l'account
    salva_file_json(account, save_path);
}

// funzione per controllare gli account presenti nella cartella
void controlla_account(char *path){

    // creiamo un array di stringhe per contenere i nomi dei file presenti nella cartella
    char *files[100];
    int count = 0;
    // opendir returns NULL if couldn't open directory
    DIR *dr = opendir(path);
    if (dr == NULL){
        printf("Could not open current directory" );
    } else {
        // scriviamo un codice per avere una lista di file dentro una cartella
        // https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/
        struct dirent *de;
        while ((de = readdir(dr)) != NULL){
            // salva il nome del file nell'array
            files[count] = de->d_name;
            // carichiamo il file json
            char save_path[50];
            sprintf(save_path, "%s/%s", path, files[count]);
            cJSON *json = carica_file_json(save_path);                          // carichiamo il file json
            // se lo status dell'account è uguale a 0 allora approviamo l'account
            // https://stackoverflow.com/questions/50979077/how-to-test-for-a-boolean-being-true-false-in-cjson

            // stampa il contenuto del file json
            // printf("%s\n", cJSON_Print(json));

            cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");   // prendiamo il campo status
            // printf("Status: %d\n", cJSON_IsFalse(status));                   // stampiamo il valore del campo status per controllo
            if(cJSON_IsFalse(status) == 1 && strcmp(files[count], ".") != 0 && strcmp(files[count], "..") != 0){
                approva_account(files[count], path);
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                printf("%d-%02d-%02d %02d:%02d:%02d: Account %s approvato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, files[count]);
            }
            count++;
        }
        closedir(dr);
    }
}