#include "../lib/cJSON.h"
#include "../lib/cJSON.c"
#include "../lib/funzioni.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

// funzioni per la gestione dei file
// funzione per svuotare una cartella
void svuota_cartella(char *path){

    // apriamo la cartella
    DIR *dir = opendir(path);
    struct dirent *entry;
    char file_path[50];
    // scorriamo tutti i file presenti nella cartella
    while((entry = readdir(dir)) != NULL){
        // creiamo il path del file se diverso da . e ..
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
            sprintf(file_path, "%s/%s", path, entry->d_name);
            // eliminiamo il file
            remove(file_path);
        }
    }
    // chiudiamo la cartella
    closedir(dir);
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
    return stringa;
}

// FUNZIONI EFFETTIVE LATO CLIENT
// funzione per stampare la guida
void print_guida(char *path){
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

// funzione per controllare lo stato del server
bool status_server(char *temp_path){

    // creiamo un oggetto cJSON per contenere lo status del server
    cJSON *status = cJSON_CreateObject();
    cJSON_AddBoolToObject(status, "status", 0);

    // salviamo lo status in un file json con il nome status_server.json
    char save_path[50];
    sprintf(save_path, "%s/%s.json", temp_path, "status_server");        // la funzione sprintf permette di concatenare stringhe
    salva_file_json(status, save_path);

    // attendiamo la risposta del server con un ciclo che controlla se lo status è uguale a 0
    cJSON *risposta = carica_file_json(save_path);
    int count = 0;
    while(cJSON_IsFalse(cJSON_GetObjectItem(risposta, "status")) && count < 10){
        //printf("In attesa di risposta\n");
        risposta = carica_file_json(save_path);
        Sleep(1000);
        count++;
    }
    bool check = cJSON_IsTrue(cJSON_GetObjectItem(risposta, "status"));
    // eliminiamo il file status_server.json
    remove(save_path);
    // ritorniamo lo status del server
    return check;
}


// funzione per creare un account
cJSON crea_account(char *path_account){

    // creiamo un oggetto cJSON per contenere i dati dell'account
    cJSON *account = cJSON_CreateObject();

    // nell'account inseriremo: username, password, email, nome, cognome ed assegneremo un codice univoco
    char username[50], password[50], email[50], nome[50], cognome[50];

    // chiediamo all'utente di inserire i dati
    printf("Inserisci il tuo nome: ");
    scanf("%s", nome);
    printf("Inserisci il tuo cognome: ");
    scanf("%s", cognome);

    // inserimento dello username e controllo se l'username è già presente
    int flag = 0;
    while(flag == 0){
        printf("Inserisci il tuo username: ");
        scanf("%s", username);
        // controlliamo se l'username è già presente
        char save_path[50];
        sprintf(save_path, "%s/%s.json", path_account, username);
        FILE *file = fopen(save_path, "r");
        if(file != NULL){
            printf("Username già presente riprova\n");
        } else {
            flag = 1;
        }
    }

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
    sprintf(save_path, "%s/%s.json", path_account, username);        // la funzione sprintf permette di concatenare stringhe
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

// funzione per effettuare il login
void login(char *username, char *path_account, char *login_path){

    // carichiamo l'account
    char save_path[50];
    sprintf(save_path, "%s/%s.json", path_account, username);
    
    // controlliamo che il file esista
    FILE *file = fopen(save_path, "r");
    if(file == NULL){

        printf("Account non esistente\n");
        return;

    } else {
        // chiediamo di inserire la password
        // carichiamo il file json
        cJSON *account = carica_file_json(save_path);
        // verifichiamo se l'account è già loggato
        if(cJSON_IsTrue(cJSON_GetObjectItem(account, "login"))){
            printf("Account gia' loggato\n");
            fclose(file);
            return;
        } else {
            fclose(file);
            char password[50];
            printf("Inserisci la password: ");
            scanf("%s", password);
            // cifriamo la password
            char *password_cifrata = cifra(password, CHIAVE);
            // creiamo un file json che contenga username e password cifrata e una chiave di status per indicare se l'utente è loggato o meno
            // inizialmente questa chiave sarà espressa in "pending"
            cJSON *login = cJSON_CreateObject();
            cJSON_AddStringToObject(login, "username", username);
            cJSON_AddStringToObject(login, "password", password_cifrata);
            cJSON_AddStringToObject(login, "status", "pending");

            // salviamo il file json nella cartella temp
            char save_login[50];
            sprintf(save_login, "%s/%s", login_path, "/login.json");
            salva_file_json(login, save_login);

            // attendiamo la risposta del server con un ciclo che controlla se lo status è uguale a "pending"
            cJSON *risposta = carica_file_json(save_login);
            while(strcmp(cJSON_GetObjectItem(risposta, "status")->valuestring, "pending") == 0){
                printf("In attesa di risposta\n");
                risposta = carica_file_json(save_login);
                Sleep(1000);
            }
            // stampiamo la risposta del server
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            printf("%d-%02d-%02d %02d:%02d:%02d: Account %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, cJSON_GetObjectItem(risposta, "status")->valuestring);

            // se la risposta del server è "connesso" allora modifichiamo il valore del campo login in 1 in account
            if (strcmp(cJSON_GetObjectItem(risposta, "status")->valuestring, "connesso") == 0){
                // modifichiamo il valore del campo login in 1
                cJSON_ReplaceItemInObject(account, "login", cJSON_CreateBool(1));
                // salviamo l'account
                salva_file_json(account, save_path);
            } else {
                printf("Ripeti l'operazione di login\n");
            }
            // eliminiamo il file temp.json
            remove(save_login);
        }
    }
}

// funzione per effettuare il logout
void logout(char *username, char *path_account, char *login_path){

    // carichiamo l'account
    char save_path[50];
    sprintf(save_path, "%s/%s.json", path_account, username);
    
    // controlliamo che il file esista
    FILE *file = fopen(save_path, "r");
    if(file == NULL){

        printf("Account non esistente\n");
        return;

    } else {
        // chiediamo di inserire la password
        // carichiamo il file json
        cJSON *account = carica_file_json(save_path);
        // verifichiamo se l'account è già loggato
        if(cJSON_IsFalse(cJSON_GetObjectItem(account, "login"))){
            printf("Account gia' sloggato\n");
            fclose(file);
            return;
        } else {
            fclose(file);
            char password[50];
            printf("Inserisci la password: ");
            scanf("%s", password);
            // cifriamo la password
            char *password_cifrata = cifra(password, CHIAVE);
            // creiamo un file json che contenga username e password cifrata e una chiave di status per indicare se l'utente è loggato o meno
            // inizialmente questa chiave sarà espressa in "pending"
            cJSON *logout = cJSON_CreateObject();
            cJSON_AddStringToObject(logout, "username", username);
            cJSON_AddStringToObject(logout, "password", password_cifrata);
            cJSON_AddStringToObject(logout, "status", "pending");

            // salviamo il file json nella cartella temp
            char save_logout[50];
            sprintf(save_logout, "%s/%s", login_path, "/logout.json");
            salva_file_json(logout, save_logout);

            // attendiamo la risposta del server con un ciclo che controlla se lo status è uguale a "pending"
            cJSON *risposta = carica_file_json(save_logout);
            while(strcmp(cJSON_GetObjectItem(risposta, "status")->valuestring, "pending") == 0){
                printf("In attesa di risposta\n");
                risposta = carica_file_json(save_logout);
                Sleep(1000);
            }
            // stampiamo la risposta del server
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            printf("%d-%02d-%02d %02d:%02d:%02d: Account %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, cJSON_GetObjectItem(risposta, "status")->valuestring);

            // se la risposta del server è "connesso" allora modifichiamo il valore del campo login in 1 in account
            if (strcmp(cJSON_GetObjectItem(risposta, "status")->valuestring, "sconnesso") == 0){
                // modifichiamo il valore del campo login in 0
                cJSON_ReplaceItemInObject(account, "login", cJSON_CreateBool(0));
                // salviamo l'account
                salva_file_json(account, save_path);
            } else {
                printf("Ripeti l'operazione di logout\n");
            }
            // eliminiamo il file temp.json
            remove(save_logout);
        }
    }
}

// funzione per creare una sala
void crea_sala(char *path_sala, int n_tavoli){
    
    // creiamo un oggetto cJSON per contenere i dati della sala
    cJSON *sala = cJSON_CreateObject();

    // nel file json inseriamo un array di oggetti che rappresentano i tavoli, come una array di array
    // ogni array avrà 3 elementi: 0/1 che indica se è occupato, il numero di posti liberi e il numero di posti occupati
    cJSON *tavoli = cJSON_CreateArray();
    for (int i = 0; i < n_tavoli; i++){
        cJSON *tavolo = cJSON_CreateObject();
        cJSON_AddBoolToObject(tavolo, "occupato", 0);
        cJSON_AddNumberToObject(tavolo, "posti_liberi", 4);
        cJSON_AddNumberToObject(tavolo, "posti_occupati", 0);
        cJSON_AddItemToArray(tavoli, tavolo);
    }

    // inseriamo l'array di oggetti nell'oggetto sala
    cJSON_AddItemToObject(sala, "tavoli", tavoli);

    // aggiungiamo uno status per dire se la sala è piena
    cJSON_AddBoolToObject(sala, "piena", 0);

    // salviamo la sala in un file json con il nome template.json nella cartella sala
    char save_path[50];
    sprintf(save_path, "%s/template.json", path_sala);        // la funzione sprintf permette di concatenare stringhe
    salva_file_json(sala, save_path);

}


// funzione per visualizzare la disposizione della sala
void visualizza_sala(char *data){


    // carichiamo il file json
    cJSON *sala = carica_file_json(data);

    // analizziamo il contenuto del file json e visualizziamo la disposizione della sala
    // visualizziamo se la sala è piena o meno
    if(cJSON_IsTrue(cJSON_GetObjectItem(sala, "piena"))){
        printf("La sala e' piena\n");
    } else {
        printf("La sala non e' piena\n");
        // visualizziamo il numero di tavoli
        int n_tavoli = cJSON_GetArraySize(cJSON_GetObjectItem(sala, "tavoli"));
        printf("Il numero di tavoli e': %d\n", n_tavoli);

        // calcoliamo quanti posti liberi totali ci sono nella sala
        int posti_liberi_totali = 0;
        for (int i = 0; i < n_tavoli; i++){
            cJSON *tavolo = cJSON_GetArrayItem(cJSON_GetObjectItem(sala, "tavoli"), i);
            if (cJSON_IsFalse(cJSON_GetObjectItem(tavolo, "occupato"))){
                posti_liberi_totali += cJSON_GetObjectItem(tavolo, "posti_liberi")->valueint;
            }
        }
        printf("Il numero di posti liberi totali e': %d\n", posti_liberi_totali);

        // visualizziamo il numero di posti liberi e occupati per ogni tavolo
        for (int i = 0; i < n_tavoli; i++){
            cJSON *tavolo = cJSON_GetArrayItem(cJSON_GetObjectItem(sala, "tavoli"), i);
            printf("Tavolo %d: \n", i + 1);
            int posti_liberi = cJSON_GetObjectItem(tavolo, "posti_liberi")->valueint;
            int posti_occupati = cJSON_GetObjectItem(tavolo, "posti_occupati")->valueint;
            //printf("Posti liberi: %d", posti_liberi);
            //printf(" Posti occupati: %d\n", posti_occupati);

            // diegnamo un tavolo con i posti liberi e occupati
            int max_posti = posti_liberi + posti_occupati;
            int max_altezza = 3;

            // contorno superiore
            for (int j = 0; j < (max_posti + 2); ++j) {
                printf("\x1b[33m+ ");
            }
            printf("\n");

            // lato sinistro
            printf("\x1b[33m+ ");

            // posti liberi e occupati
            // se la chiave occupato del tavolo è true allora stampiamo tutti @ magenta
            // altrimenti stampiamo i posti liberi in verde e i posti occupati in rosso
            if (cJSON_IsTrue(cJSON_GetObjectItem(tavolo, "occupato"))){
                for (int k = 0; k < max_posti; k++) {
                    printf("\x1b[35m@ ");
                }
            } else {
                for (int k = 0; k < posti_occupati; k++) {
                    printf("\x1b[31m@ ");
                }
                for (int k = posti_occupati; k < max_posti; k++) {
                    printf("\x1b[32m@ ");
                }
            }
            // lato destro
            printf("\x1b[33m+ ");
            printf("\n");

            // contorno inferiore
            for (int j = 0; j < (max_posti + 2); ++j) {
                printf("\x1b[33m+ ");
            }            
            printf("\x1b[0m");
            printf("\n\n");
        }
    }
}

// FUNZIONI EFFETTIVE LATO SERVER
// funzione per stampare la guida
void print_guida_server(){
    // apriamo il file txt in lettura
    FILE *file = fopen("guida_server.txt", "r");
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

// funzione per ritornare lo stato del server
void return_status(char *temp_path){

    // carichiamo il file json con lo status del server
    cJSON *status = carica_file_json(temp_path);
    
    // modifichiamo il valore del campo status in attivo
    cJSON_ReplaceItemInObject(status, "status", cJSON_CreateBool(1));
    
    // printiamo un messaggio di server attivo con la data e l'ora
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d: conferma server online\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    // salviamo il nuovo status del server
    salva_file_json(status, temp_path);
}

// funzione per approvare un account
void approva_account(char *nome_file, char *path_account){

    // carichiamo l'account
    char save_path[50];
    sprintf(save_path, "%s/%s", path_account, nome_file);
    cJSON *account = carica_file_json(save_path);

    // modifichiamo il valore del campo status in attivo
    cJSON_ReplaceItemInObject(account, "status", cJSON_CreateBool(1));

    // salviamo l'account
    salva_file_json(account, save_path);
}

// funzione per controllare gli account presenti nella cartella
void controlla_account(char *path_account){

    // creiamo un array di stringhe per contenere i nomi dei file presenti nella cartella
    char *files[100];
    int count = 0;
    // opendir returns NULL if couldn't open directory
    DIR *dr = opendir(path_account);
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
            sprintf(save_path, "%s/%s", path_account, files[count]);
            cJSON *json = carica_file_json(save_path);                          // carichiamo il file json
            // se lo status dell'account è uguale a 0 allora approviamo l'account
            // https://stackoverflow.com/questions/50979077/how-to-test-for-a-boolean-being-true-false-in-cjson

            // stampa il contenuto del file json
            // printf("%s\n", cJSON_Print(json));

            cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");   // prendiamo il campo status
            // printf("Status: %d\n", cJSON_IsFalse(status));                   // stampiamo il valore del campo status per controllo
            if(cJSON_IsFalse(status) == 1 && strcmp(files[count], ".") != 0 && strcmp(files[count], "..") != 0){
                approva_account(files[count], path_account);
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                printf("%d-%02d-%02d %02d:%02d:%02d: Account %s approvato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, files[count]);
            }
            count++;
        }
        closedir(dr);
    }
}

// funzione per controllare il login
void login_check(char *temp_path, char *path){

    // carichiamo il file json
    cJSON *login = carica_file_json(temp_path);

    // visualizziamo il contenuto del file json
    //printf("%s\n", cJSON_Print(login));

    // estraiamo la stringa username dal file json
    cJSON *username = cJSON_GetObjectItemCaseSensitive(login, "username");
    //printf("Username: %s\n", username->valuestring);

    // carichiamo l'account
    char save_path[50];
    // salviamo lo username in una variabile
    
    sprintf(save_path, "%s/%s.json", path, username->valuestring);
    cJSON *account = carica_file_json(save_path);

    // controlliamo se la password è corretta
    // imposto il contatore del tempo
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if(strcmp(cJSON_GetObjectItem(login, "password")->valuestring, cJSON_GetObjectItem(account, "password")->valuestring) == 0){
        // modifichiamo il valore del campo status in connesso
        cJSON_ReplaceItemInObject(login, "status", cJSON_CreateString("connesso"));
        printf("%d-%02d-%02d %02d:%02d:%02d: Account %s connesso\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, username->valuestring);
        // salviamo il file json
        salva_file_json(login, temp_path);
    } else {
        // modifichiamo il valore del campo status in rifiutato
        cJSON_ReplaceItemInObject(login, "status", cJSON_CreateString("rifiutato"));
        printf("%d-%02d-%02d %02d:%02d:%02d: Account %s rifiutato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, username->valuestring);
        // salviamo il file json
        salva_file_json(login, temp_path);
    }
}

// funzione per controllare il logout
void logout_check(char *temp_path, char *path){

    // carichiamo il file json
    cJSON *logout = carica_file_json(temp_path);

    // visualizziamo il contenuto del file json
    //printf("%s\n", cJSON_Print(login));

    // estraiamo la stringa username dal file json
    cJSON *username = cJSON_GetObjectItemCaseSensitive(logout, "username");
    //printf("Username: %s\n", username->valuestring);

    // carichiamo l'account
    char save_path[50];
    // salviamo lo username in una variabile
    
    sprintf(save_path, "%s/%s.json", path, username->valuestring);
    cJSON *account = carica_file_json(save_path);

    // controlliamo se la password è corretta
    // imposto il contatore del tempo
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if(strcmp(cJSON_GetObjectItem(logout, "password")->valuestring, cJSON_GetObjectItem(account, "password")->valuestring) == 0){
        // modifichiamo il valore del campo status in connesso
        cJSON_ReplaceItemInObject(logout, "status", cJSON_CreateString("sconnesso"));
        printf("%d-%02d-%02d %02d:%02d:%02d: Account %s sconnesso\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, username->valuestring);
        // salviamo il file json
        salva_file_json(logout, temp_path);
    } else {
        // modifichiamo il valore del campo status in rifiutato
        cJSON_ReplaceItemInObject(logout, "status", cJSON_CreateString("rifiutato"));
        printf("%d-%02d-%02d %02d:%02d:%02d: Account %s rifiutato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, username->valuestring);
        // salviamo il file json
        salva_file_json(logout, temp_path);
    }
}

            