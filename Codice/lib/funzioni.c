#include "../lib/cJSON.h"
#include "../lib/cJSON.c"
#include "../lib/funzioni.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h> 
#include <ctype.h> 

// definizioni delle costanti che saranno necessarire
#define BUFFER_SIZE_JSON 1024                   // creiamo un buffer di 1024 caratteri per la gestione dei file json
#define CHIAVE 3                                // definiamo la chiave per la cifratura
#define MAX 50                                  // definiamo la lunghezza massima di una stringa   
#define MAX_PORTATE 100                         // definiamo il numero massimo di portate
#define MAX_LUNG_PORTATA 1000                   // definiamo la lunghezza massima di una portata
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

// funzione per contare le righe di un file
int conta_righe(char *path){
    FILE *file = fopen(path, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
        return 0;
    }
    // salviamo le righe del file
    char riga[MAX_LUNG_PORTATA];
    int i = 0;
    // leggiamo il file e salviamo le righe in un array di stringhe
    while(fgets(riga, MAX_PORTATE, file) != NULL){
        i++;
    }

    return i;
}

// conversione data da gg/mm/aaaa in gg_mm_aaaa
char* conversione_data(char *data){
    char static data_conv[50];
    strcpy(data_conv, data);
    for (int i = 0; i < strlen(data_conv); i++){
        if (data_conv[i] == '/'){
            data_conv[i] = '_';
        }
    }
    return data_conv;
}

// funzione per controllare il formato della data
bool check_formato_data(char *data){
    // controlliamo che la data sia nel formato gg/mm/aaaa
    if(strlen(data) != 10){
        return false;
    }
    if(data[2] != '/' || data[5] != '/'){
        return false;
    }
    for (int i = 0; i < strlen(data); i++){
        if(i != 2 && i != 5){
            if(!isdigit(data[i])){
                return false;
            }
        }
    }
    return true;
}

bool se_esiste(char *path, char *nome_file, char* tipo_file){
    char save_path[50];
    sprintf(save_path, "%s/%s.%s", path, nome_file, tipo_file);
    //printf("%s\n", save_path);
    FILE *file = fopen(save_path, "r");
    if(file == NULL){
        //printf("il file non c'e'\n");
        fclose(file);
        return false;
    } else {
        //printf("il file c'e'\n");
        fclose(file);
        return true;
    }
}

// funzione per caricare le prenotazioni
Prenotazione carica_prenotazioni(char *riga){
        
    Prenotazione static pren;
    char *parole[5];
    char *token = strtok(riga, ",");
    int k = 0;
    while(token != NULL){
        parole[k] = token;
        token = strtok(NULL, ",");
        k++;
    }
    // salviamo le parole in una struttura convertendo i numeri in interi
    pren.codice = atoi(parole[0]);
    strcpy(pren.username, parole[1]);
    strcpy(pren.data, parole[2]);
    pren.tavolo = atoi(parole[3]);
    pren.posti = atoi(parole[4]);
    // se il codice della prenotazione è uguale a quello passato come argomento allora eliminiamo la prenotazione

    return pren;
}



// Gestione password, cifratura e decifratura
// definiamo la funzione get_pass che restituisce la password coperta da asterischi
// https://m.pierotofy.it/p/forum/2/1046480-risoltonascondere_input_da_tastiera_console_con_degli_asterischi/
char *get_pass(char *pword, int size) {

    int ch;
    int i = 0;

    while ((ch = getch()) != EOF 

              && ch != '\n' 
              && ch != '\r' 
              && i < size - 1) {

                    if (ch == '\b' && i > 0) {

                        printf("\b \b");
                        fflush(stdout);
                        i--;
                        pword[i] = (char)ch;
                    } else if (isalnum(ch)){

                        putchar('*');
                        pword[i++] = (char)ch;
                    }
    }

    pword[i] = '\0';
    return pword;
}

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
    char username[MAX], password[MAX], email[MAX], nome[MAX], cognome[MAX];

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
    // chiediamo all'utente di inserire la password che abbia almeno 8 caratteri
    do {
        printf("Inserisci la tua password (almeno 8 caratteri): ");
        get_pass(password, MAX);
        if(strlen(password) < 8){
            printf("La password deve avere almeno 8 caratteri\n");
        } else {
            printf("Password inserita\n");
        }
    } while(strlen(password) < 8);
    /* printf("Inserisci la tua password: ");
    get_pass(password, MAX); */
    printf("\n");
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

// funzione per visualizzare gli account
void visualizza_account(char *path_account, char* path_sala, char *path_ordini){
    
        // apriamo il file json dell'accout in lettura
        cJSON *account = carica_file_json(path_account);

        // controlliamo se l'account è loggato
        if(cJSON_IsTrue(cJSON_GetObjectItem(account, "login"))){
            printf("Account loggato\n");
            // visualizziamo i dati dell'account
            printf("-----------------------------\n");
            printf("Dati dell'account: \n");
            printf("Nome: %s\n", cJSON_GetObjectItem(account, "nome")->valuestring);
            printf("Cognome: %s\n", cJSON_GetObjectItem(account, "cognome")->valuestring);
            printf("Username: %s\n", cJSON_GetObjectItem(account, "username")->valuestring);
            printf("Email: %s\n", cJSON_GetObjectItem(account, "email")->valuestring);
            // al posto della password visualizziamo una stringa di asterischi
            printf("Password: ");
            for (int i = 0; i < strlen(cJSON_GetObjectItem(account, "password")->valuestring); i++){
                printf("*");
            }
            printf("\n");
            // visualizziamo lo status dell'account
            if(cJSON_IsTrue(cJSON_GetObjectItem(account, "status"))){
                printf("Account approvato\n");
            } else {
                printf("Account non approvato\n");
            }
            printf("-----------------------------\n");
            char *username = cJSON_GetObjectItem(account, "username")->valuestring;
            // visualizziamo le prenotazioni effettuate
            printf("Prenotazioni effettuate: \n\n");
            visualizza_prenotazioni(username, path_sala);
            printf("-----------------------------\n");
            // visualizziamo gli ordini effettuati
            printf("Ordini effettuati: \n\n");
            visualizza_ordini(username, path_ordini);
            printf("-----------------------------\n");
        } /* else {
            printf("Account non loggato\n");
        } */

}

// funzione per modificare un account
void mofica_account(char *username, char *path_account){

        // apriamo il file json dell'accout in lettura
        cJSON *account = carica_file_json(path_account);

        // controlliamo se l'account è loggato
        if(cJSON_IsTrue(cJSON_GetObjectItem(account, "login"))){
            
            // chiediamo che campo modificare
            char campo[MAX];
            do{
                printf("Quale campo vuoi modificare? (nome, cognome, email, password): ");
                scanf("%s", campo);
                if(strcmp(campo, "nome") != 0 && strcmp(campo, "cognome") != 0 && strcmp(campo, "email") != 0 && strcmp(campo, "password") != 0){
                    printf("Campo non valido\n");
                } else {
                    break;
                }
            } while (1);
            
            // modifichiamo il campo scelto
            if(strcmp(campo, "nome") == 0){
                printf("Inserisci il nuovo nome: ");
                char nome[MAX];
                scanf("%s", nome);
                cJSON_ReplaceItemInObject(account, "nome", cJSON_CreateString(nome));
            } else if(strcmp(campo, "cognome") == 0){
                printf("Inserisci il nuovo cognome: ");
                char cognome[MAX];
                scanf("%s", cognome);
                cJSON_ReplaceItemInObject(account, "cognome", cJSON_CreateString(cognome));
            } else if(strcmp(campo, "email") == 0){
                printf("Inserisci la nuova email: ");
                char email[MAX];
                scanf("%s", email);
                cJSON_ReplaceItemInObject(account, "email", cJSON_CreateString(email));
            } else if(strcmp(campo, "password") == 0){
                // chiediamo all'utente di inserire la password che abbia almeno 8 caratteri
                char password[MAX];
                do {
                    printf("Inserisci la tua password (almeno 8 caratteri): ");
                    get_pass(password, MAX);
                    if(strlen(password) < 8){
                        printf("La password deve avere almeno 8 caratteri\n");
                    } else {
                        printf("Password inserita\n");
                    }
                } while(strlen(password) < 8);
                // cifriamo la password
                char *password_cifrata = cifra(password, CHIAVE);
                cJSON_ReplaceItemInObject(account, "password", cJSON_CreateString(password_cifrata));
            }

            // modifichiamo lo status dell'account in 0
            cJSON_ReplaceItemInObject(account, "status", cJSON_CreateBool(0));

            // salviamo l'account
            salva_file_json(account, path_account);

            // creiamo un ciclo che controllo se l'account è stato approvato dal server fino a che non sarà approvato l'account non potrà fare il login
            // il ciclo si ferma quando status è uguale a 1
            // riapriamo il file json per controllare lo status dell'account
            cJSON *json = carica_file_json(path_account);

            //cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");
            while(cJSON_GetObjectItem(json, "status")->valueint == 0){
                printf("Account in attesa di approvazione\n");
                json = carica_file_json(path_account);
                // attendiamo 5 secondi
                Sleep(1000);
            }
            // stampiamo un messaggio di account approvato con la data e l'ora
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            printf("%d-%02d-%02d %02d:%02d:%02d: Account approvato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            

        } else {
            printf("Account non loggato\n");
        }

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
            // proteggiamo la password mostrando degli asterischi al posto dei caratteri
            get_pass(password, MAX);
            printf("\n");
            
            //scanf("%s", password);
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

// funzione per controllare se un utente è loggato
bool seLoggato(char *username, char *path_account){

    // carichiamo l'account
    char save_path[50];
    sprintf(save_path, "%s/%s.json", path_account, username);
    
    // controlliamo che il file esista
    FILE *file = fopen(save_path, "r");
    if(file == NULL){

        printf("Account non esistente\n");
        return false;

    } else {
        // chiediamo di inserire la password
        // carichiamo il file json
        cJSON *account = carica_file_json(save_path);
        // verifichiamo se l'account è già loggato
        if(cJSON_IsTrue(cJSON_GetObjectItem(account, "login"))){
            //printf("Account gia' loggato\n");
            fclose(file);
            return true;
        } else {
            fclose(file);
            return false;
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
            char password[MAX];
            printf("Inserisci la password: ");
            get_pass(password, MAX);
            printf("\n");
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

        // elenchiamo i tavoli che sono liberi con i posti liberi e occupati
        for (int i = 0; i < n_tavoli; i++){
            cJSON *tavolo = cJSON_GetArrayItem(cJSON_GetObjectItem(sala, "tavoli"), i);
            // printiamo se il tavolo è coccupato o meno
            if (cJSON_IsTrue(cJSON_GetObjectItem(tavolo, "occupato"))){
                printf("Il tavolo %d e' occupato\n", i + 1);
            } else {
                printf("Il tavolo %d e' libero, e i posti disponibili sono: %d\n", i + 1, cJSON_GetObjectItem(tavolo, "posti_liberi")->valueint);
                // printiamo i posti liberi e occupati
                //printf(" Posti liberi: %d", cJSON_GetObjectItem(tavolo, "posti_liberi")->valueint);
                //printf(" Posti occupati: %d\n", cJSON_GetObjectItem(tavolo, "posti_occupati")->valueint);
            }
        }

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

// funzione per controllare lo status della sala
int status_sala(cJSON *sala){

    // facciamo un ciclo per chiedere che tavoli vuole prenotare e per quanti posti
    int n_tavoli = cJSON_GetArraySize(cJSON_GetObjectItem(sala, "tavoli"));

    // contiamo quanti tavoli sono liberi
    int tavoli_liberi = 0;
    for (int i = 0; i < n_tavoli; i++){
        cJSON *tavolo = cJSON_GetArrayItem(cJSON_GetObjectItem(sala, "tavoli"), i);
        if(cJSON_IsFalse(cJSON_GetObjectItem(tavolo, "occupato"))){
            tavoli_liberi++;
        }
    }
    return tavoli_liberi;
}

// funzione per caricare il menu del ristorante
Portata *carica_menu(char *path){

    // apriamo il file csv in lettura
    FILE *file = fopen(path, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
        return 0;
    }
    // salviamo le righe del file
    char riga[MAX_LUNG_PORTATA];
    char *righe[MAX_PORTATE];
    int i = 0;
    // leggiamo il file e salviamo le righe in un array di stringhe
    while(fgets(riga, MAX_PORTATE, file) != NULL){
        righe[i] = strdup(riga);
        i++;
    }
    // creiamo un array di strutture Portata di grandezza pari a i volte la grandezza di Portata
    Portata *portate = malloc(i * sizeof(Portata));

    // per ogni riga, dividiamo le parole separate dalla virgola
    for (int j = 0; j < i; j++){
        char *parole[5];
        char *token = strtok(righe[j], ",");
        int k = 0;
        while(token != NULL){
            parole[k] = token;
            //printf("%s\n", parole[k]);
            token = strtok(NULL, ",");
            k++;
        }
        // salviamo le parole in una struttura
        portate[j].codice = atoi(parole[0]);
        strcpy(portate[j].categoria, parole[1]);
        strcpy(portate[j].nome, parole[2]);
        strcpy(portate[j].descrizione, parole[3]);
        portate[j].prezzo = atof(parole[4]);
    }
    // ritorniamo l'array di strutture Portata
    return portate;
}

// definizione della visualizzazione del menu con lettura da file csv
// https://stdin.top/posts/csv-in-c/
void visualizza_menu(Portata *portate, char *path){
    // apriamo il file csv in lettura
    
    int n_portate = conta_righe(path);
    //Portata *portate = carica_menu(path);

    // printiamo a video tutte le parole separate dalla virgola in ogni riga
    for (int i = 0; i < n_portate; i++){
        // stampiamo le parole
        printf("%d) [%s] %s (%s) - %.2f\n", portate[i].codice, portate[i].categoria, portate[i].nome, portate[i].descrizione, portate[i].prezzo);       
    }
}

// funzione per prenotare un tavolo
void prenota_tavolo(char *username, char *data, cJSON* sala, char *path_temp){
    
    // contiamo quanti tavoli sono liberi
    int tavoli_liberi = status_sala(sala);
    
    // controlliamo se non ci sono tavoli liberi e usciamo dalla funzione
    printf("Ci sono %d tavoli liberi\n", tavoli_liberi);
    if(tavoli_liberi == 0){
        printf("Non ci sono tavoli liberi per questa data.\n");
    } else {

        do {
        
            int n_posti;
            int tavolo;
            printf("Che tavolo vuoi prenotare: ");
            scanf("%d", &tavolo);
            // controlliamo che il tavolo sia libero
            cJSON *tav = cJSON_GetArrayItem(cJSON_GetObjectItem(sala, "tavoli"), tavolo - 1);
            if(cJSON_IsTrue(cJSON_GetObjectItem(tav, "occupato"))){
                printf("Il tavolo e' occupato\n");
            } else {
            // totale di posti del tavolo
                printf("Il tavolo e' libero\n");
                int posti_liberi = cJSON_GetObjectItem(tav, "posti_liberi")->valueint;
                int posti_occupati = cJSON_GetObjectItem(tav, "posti_occupati")->valueint;
                // ciclo per chiedere quanti posti vuole prenotare maggioi di 0 e minore di posti liberi
                do {
                    printf("Il tavolo ha %d posti liberi\n", posti_liberi);
                    printf("Quanti posti vuoi prenotare (0-%d): ", posti_liberi);
                    scanf("%d", &n_posti);
                } while(n_posti <= 0 || n_posti > posti_liberi);
                // controlliamo che ci siano abbastanza posti liberi
                if(n_posti == posti_liberi){
                    // modifichiamo il tavolo
                    cJSON_ReplaceItemInObject(tav, "occupato", cJSON_CreateBool(1));
                    cJSON_ReplaceItemInObject(tav, "posti_liberi", cJSON_CreateNumber(cJSON_GetObjectItem(tav, "posti_liberi")->valueint - n_posti));
                    cJSON_ReplaceItemInObject(tav, "posti_occupati", cJSON_CreateNumber(cJSON_GetObjectItem(tav, "posti_occupati")->valueint + n_posti));
                } else if (n_posti < posti_liberi){
                    cJSON_ReplaceItemInObject(tav, "posti_liberi", cJSON_CreateNumber(cJSON_GetObjectItem(tav, "posti_liberi")->valueint - n_posti));
                    cJSON_ReplaceItemInObject(tav, "posti_occupati", cJSON_CreateNumber(cJSON_GetObjectItem(tav, "posti_occupati")->valueint + n_posti));
                } else {
                    printf("Non ci sono abbastanza posti liberi\n");
                }

                // aggiungiamo il tavolo e i posti prenotati a json per la prenotazione
                cJSON *tavolo_prenotato = cJSON_CreateNumber(tavolo);
                cJSON *posti_prenotati = cJSON_CreateNumber(n_posti);

                // salviamo la sala con i tavoli prenotati nella cartella temporanea
                char save_path[50];
                sprintf(save_path, "%s/%s", path_temp, data);
                salva_file_json(sala, save_path);

                // stampiamo un messaggio di conferma
                printf("Tavoli prenotati\n");

                // creiamo un file json per la prenotazione
                cJSON *prenotazione = cJSON_CreateObject();
                cJSON_AddStringToObject(prenotazione, "username", username);
                cJSON_AddStringToObject(prenotazione, "data", data);
                cJSON_AddItemToObject(prenotazione, "sala", sala);
                // inseriamo i tavoli e i posti prenotati nell'oggetto
                cJSON_AddItemToObject(prenotazione, "tavolo", tavolo_prenotato);
                cJSON_AddItemToObject(prenotazione, "posti", posti_prenotati);
                cJSON_AddBoolToObject(prenotazione, "confermata", 0);

                // scriviamo il file temporaneo per il server
                char save_path_server[50];
                sprintf(save_path_server, "%s/%s", path_temp, "prenotazione.json");
                salva_file_json(prenotazione, save_path_server);

                // attendiamo la risposta del server con un ciclo che controlla se "confermata" è uguale a 0
                cJSON *risposta = carica_file_json(save_path_server);
                while(cJSON_IsFalse(cJSON_GetObjectItem(risposta, "confermata"))){
                    printf("In attesa di risposta\n");
                    risposta = carica_file_json(save_path_server);
                    Sleep(1000);
                }
                // eliminiamo il file prenotazione.json e la sala temporanea
                remove(save_path_server);
                remove(save_path);

                // stampiamo la risposta del server
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                if (cJSON_IsTrue(cJSON_GetObjectItem(risposta, "confermata"))){
                    printf("%d-%02d-%02d %02d:%02d:%02d: Prenotazione confermata\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                } else {
                    printf("%d-%02d-%02d %02d:%02d:%02d: Prenotazione non confermata\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                }
                break;
            }
        } while(1);
    }
}

// funzione per eliminare la prenotazione di un tavolo
void elimina_tavolo(int prenotazione, char *path_sala, char *path_temp){

    // carichiamo il file csv delle prenotazioni nel path sala 
    char save_path[50];
    sprintf(save_path, "%s/prenotazioni.csv", path_sala);
    FILE *file = fopen(save_path, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file o file inesistente\n");
    } else {
        // leggiamo il file e carichiamo ogni riga in una struttura Prenotazione. 
        int num_pren = conta_righe(save_path);
        printf("Numero di prenotazioni: %d\n", num_pren);
        char riga[MAX_LUNG_PORTATA];
        // carichiamo le righe del file
        while(fgets(riga, MAX_PORTATE, file) != NULL){
            // dividiamo le parole separate dalla virgola
            Prenotazione pren = carica_prenotazioni(riga);

            // se il codice della prenotazione è uguale a quello passato come argomento allora eliminiamo la prenotazione
            if (pren.codice == prenotazione){
                // procediamo ad eliminare la prenotazione
                printf("Prenotazione %d in eliminazione\n", pren.codice);
                // creiamo un json per la prenotazione da eliminare
                cJSON *prenotazione = cJSON_CreateObject();
                cJSON_AddNumberToObject(prenotazione, "codice", pren.codice);
                cJSON_AddStringToObject(prenotazione, "data", pren.data);
                cJSON_AddNumberToObject(prenotazione, "tavolo", pren.tavolo);
                cJSON_AddNumberToObject(prenotazione, "posti", pren.posti);
                cJSON_AddBoolToObject(prenotazione, "confermata", 0);

                // creiamo un file temporaneo per salvare le prenotazioni da eliminare 
                char save_path_temp[50];
                sprintf(save_path_temp, "%s/prenotazione_canc.json", path_temp);
                salva_file_json(prenotazione, save_path_temp);

                // attendiamo la risposta del server con un ciclo che controlla se "confermata" è uguale a 0
                cJSON *risposta = carica_file_json(save_path_temp);
                while(cJSON_IsFalse(cJSON_GetObjectItem(risposta, "confermata"))){
                    printf("In attesa di risposta\n");
                    risposta = carica_file_json(save_path_temp);
                    Sleep(1000);
                }
                // eliminiamo il file prenotazione.json e la sala temporanea
                remove(save_path_temp);
                printf("Prenotazione eliminata\n");
                break;
            }

        }
        /* for (int i = 0; i < num_pren; i++){
            fscanf(file, "%d,%s,%s,%d,%d\n", &pren.codice, &pren.username, &pren.data, &pren.tavolo, &pren.posti);
            if (pren.codice == prenotazione){
                // printiamo la prenotazione
                printf("Prenotazione %d: %s %s %d %d\n", pren.codice, pren.username, pren.data, pren.tavolo, pren.posti);
                break;
            }
        } */
        fclose(file);
    }
}

// funzione per eliminare tutte le prenotazioni di un utente
void elimina_prenotazioni(char *username, char *path_sala, char *path_temp){

    // carichiamo il file csv delle prenotazioni nel path sala
    char save_path[50];
    sprintf(save_path, "%s/prenotazioni.csv", path_sala);
    
    // facciamo un ciclo per leggere le righe del file
    FILE *file = fopen(save_path, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
        return;
    } else {
        // leggiamo il file e carichiamo ogni riga in una struttura Prenotazione.
        char riga[MAX_LUNG_PORTATA];
        while(fgets(riga, MAX_PORTATE, file) != NULL){
            // carichiamo la riga come tipo Prenotazione
            Prenotazione pren = carica_prenotazioni(riga);
            // se l'username è uguale a quello passato eliminiamo la prenotazione
            if (strcmp(pren.username, username) == 0){
                elimina_tavolo(pren.codice, path_sala, path_temp);
            }
        }
    }
    fclose(file);
}

// funzione per visualizzare le prenotazioni di un utente
void visualizza_prenotazioni(char* username, char *path){

    // leggiamo il file delle prenotazioni.csv nella cartella path_sala
    // facciamo un ciclo per leggere le righe del file
    FILE *file = fopen(path, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
        return;
    } else {
        bool trovato = false;
        // leggiamo il file e carichiamo ogni riga in una struttura Prenotazione.
        char riga[MAX_LUNG_PORTATA];
        // stampiamo le colonne della prenotazione
        printf("            \tNum\tUser\tData\t\tTavolo\tPosti\n");
        while(fgets(riga, MAX_PORTATE, file) != NULL){
            // carichiamo la riga come tipo Prenotazione
            Prenotazione pren = carica_prenotazioni(riga);
            // se l'username è uguale a quello passato eliminiamo la prenotazione
            if (strcmp(pren.username, username) == 0){
                printf("Prenotazione \t%d\t%s\t%s\t%d\t%d\n", pren.codice, pren.username, pren.data, pren.tavolo, pren.posti);
                trovato = true;
            }
        }
        if (!trovato){
            printf("Nessuna prenotazione trovata\n");
        }
        fclose(file);
    }
}

// funzione per controllare se una prenotazione è in elenco dal codice prenotazione
bool check_prenotazione_c(int n, char *path){
    FILE *file = fopen(path, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
    } else {

        // leggiamo il file e carichiamo ogni riga in una struttura Prenotazione.
        char riga[MAX_LUNG_PORTATA];
        while(fgets(riga, MAX_PORTATE, file) != NULL){
            // carichiamo la riga come tipo Prenotazione
            Prenotazione pren = carica_prenotazioni(riga);
            // se il codice della prenotazione è uguale a quello passato come argomento allora eliminiamo la prenotazione
            if (pren.codice == n){
                return true;
            }
        }
    }
    return false;

}

// funzione per controllare se una prenotazione è in elenco dall'username
bool check_prenotazione_u(char *username, char *path){
    FILE *file = fopen(path, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
    } else {

        // leggiamo il file e carichiamo ogni riga in una struttura Prenotazione.
        char riga[MAX_LUNG_PORTATA];
        while(fgets(riga, MAX_PORTATE, file) != NULL){
            // carichiamo la riga come tipo Prenotazione
            Prenotazione pren = carica_prenotazioni(riga);
            // se il codice della prenotazione è uguale a quello passato come argomento allora eliminiamo la prenotazione
            if (strcmp(pren.username, username) == 0){
                return true;
            }
        }
    }
    return false;
}

// funzione per creare un ordine
OrdineSingolo crea_ordine(Portata *menu, int num_portate, OrdineCompleto ordine){
    // chiediamo la categoria del menu da visualizzare
    char categoria[50];
    do {
        
        printf("Cosa vuoi ordinare? (Antipasto, Primo, Secondo, Contorno, Dolce, Bevande): ");
        scanf("%s", categoria);
        // verifichiamo che la categoria sia corretta
        if (!(strcmp(categoria, "Antipasto") == 0 
                || strcmp(categoria, "Primo") == 0 
                || strcmp(categoria, "Secondo") == 0 
                || strcmp(categoria, "Contorno") == 0 
                || strcmp(categoria, "Dolce") == 0 
                || strcmp(categoria, "Bevande") == 0)){

                printf("Categoria non valida. Riprova\n");
            } else {
                break;
            }
    } while (1);

    // cicliamo le portate per visualizzare solo quelle della categoria scelta
    printf("\n-------------------------------\n");
    printf("    Menu %s\n", categoria);
    printf("-------------------------------\n");
    for (int i = 0; i < num_portate; i++){
        if(strcmp(menu[i].categoria, categoria) == 0){
            printf("%d - %s - %s - %s - %.2f\n", menu[i].codice, menu[i].categoria, menu[i].nome, menu[i].descrizione, menu[i].prezzo);
        }
    }
    printf("-------------------------------\n\n");
    // chiediamo cosa vuole ordinare
    OrdineSingolo static ordine_singolo;
    bool trovata = false;
    do{
        printf("Cosa vuoi ordinare? (Inserisci il codice della portata): ");
        int codice;
        scanf("%d", &codice);
        // controlliamo che il codice sia valido
            for (int i = 0; i < num_portate; i++){
                if(menu[i].codice == codice){
                    trovata = true;
                    printf("Portata trovata\n");
                    ordine_singolo.portata = menu[i];
                    // chiediamo la quantità
                    printf("Quanti ne vuoi ordinare?: ");
                    scanf("%d", &ordine_singolo.quantita);
                    break;
                }
            }
        } while(!trovata);
    
    return ordine_singolo;
}          

// funzione per creare un ordine in formato json
cJSON *crea_ordine_completo_json(OrdineCompleto ordine, int n_ordini){
    cJSON *ordine_json = cJSON_CreateObject();

    // replichiamo la struttura dell'ordine completo in un file json
    cJSON_AddNumberToObject(ordine_json, "n_prenotazione", ordine.n_prenotazione);
    cJSON_AddStringToObject(ordine_json, "username", ordine.username);
    cJSON_AddStringToObject(ordine_json, "data", ordine.data);
    cJSON_AddNumberToObject(ordine_json, "tavolo", ordine.tavolo);
    cJSON *ordine_singolo = cJSON_CreateArray();
    for(int i = 0; i < n_ordini; i++){
        cJSON *ordine_singolo_json = cJSON_CreateObject();
        cJSON_AddNumberToObject(ordine_singolo_json, "codice", ordine.OrdineSingolo[i].portata.codice);
        cJSON_AddStringToObject(ordine_singolo_json, "categoria", ordine.OrdineSingolo[i].portata.categoria);
        cJSON_AddStringToObject(ordine_singolo_json, "nome", ordine.OrdineSingolo[i].portata.nome);
        cJSON_AddStringToObject(ordine_singolo_json, "descrizione", ordine.OrdineSingolo[i].portata.descrizione);
        cJSON_AddNumberToObject(ordine_singolo_json, "prezzo", ordine.OrdineSingolo[i].portata.prezzo);
        cJSON_AddNumberToObject(ordine_singolo_json, "quantita", ordine.OrdineSingolo[i].quantita);
        cJSON_AddItemToArray(ordine_singolo, ordine_singolo_json);
    }
    // aggiungiamo l'array di oggetti ordine_singolo_json all'oggetto ordine_json
    cJSON_AddItemToObject(ordine_json, "ordini", ordine_singolo);
    cJSON_AddNumberToObject(ordine_json, "conto", ordine.conto_totale);

    return ordine_json;
}           

// funzione per salvare l'ordine                  
void salva_ordine(cJSON *ordine, char *path_ordine, char *path_sala){
    
    // creiamo un json per la conferma dell'ordine
    cJSON *conferma = cJSON_CreateObject();
    cJSON_AddItemToObject(conferma, "ordine", ordine);
    cJSON_AddStringToObject(conferma, "status", "pending");

    // chiediamo le informazioni per il pagamento
    char carta_di_credito[50];
    char scadenza[50];
    int cvv;
    // inseriamo la carta di credito composta da 16 numeri
    printf("\n");
    printf("\nInseirsci i dati di pagamento\n");
    do {
        printf("Inserisci il numero della carta di credito (16 cifre): ");
        scanf("%s", carta_di_credito);
    } while(strlen(carta_di_credito) != 16);

    // inseriamo la scadenza della carta di credito espressa come MM/YY
    do {
        printf("Inserisci la scadenza della carta di credito (MM/YY): ");
        scanf("%s", scadenza);
        // controlliamo che il mese e l'anno siano separati da uno slash
        if(strchr(scadenza, '/') == NULL){
            printf("Formato non valido. Riprova\n");
        } else {
            // verifichiamo che il mese sia compreso tra 1 e 12 e l'anno tra 24 e 99
            char *mese = strtok(scadenza, "/");
            char *anno = strtok(NULL, "/");
            if(atoi(mese) < 1 || atoi(mese) > 12){
                printf("Mese non valido deve essere compreso tra 1 e 12. Riprova\n");
            } else if (atoi(anno) < 24 || atoi(anno) > 99){
                printf("Anno non valido deve essere >= di 24 e minore di 99. Riprova\n");
            } else {
                break;
            }
        }
    } while(1);

    // inseriamo il cvv della carta di credito composto da 3 cifre
    do {
        printf("Inserisci il cvv della carta di credito (3 cifre) tra 100 e 999: ");
        scanf("%d", &cvv);
    } while(cvv < 100 || cvv > 999);
    printf("\n");

    // salviamo le informazioni della carta di credito nel file json
    cJSON_AddStringToObject(conferma, "carta_di_credito", carta_di_credito);
    /* cJSON_AddStringToObject(conferma, "scadenza", scadenza);
    cJSON_AddNumberToObject(conferma, "cvv", cvv); */
    // aggiungiamo un ok per il pagamento e la data e ora del pagamento
    cJSON_AddStringToObject(conferma, "pagamento", "ok");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char data[50];
    sprintf(data, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    cJSON_AddStringToObject(conferma, "data", data);

    // salviamo il file json nella cartella temp
    salva_file_json(conferma, path_ordine);

    // attendiamo la risposta del server con un ciclo che controlla se lo status è uguale a "pending"
    printf("\n");
    cJSON *risposta = carica_file_json(path_ordine);
    while(strcmp(cJSON_GetObjectItem(risposta, "status")->valuestring, "pending") == 0){
        printf("In attesa di risposta\n");
        risposta = carica_file_json(path_ordine);
        Sleep(1000);
    }
    
    // printiamo a video il path della ricevuta caricandola dalla risposta del server
    printf("\n");
    char path_ricevuta[50];
    strcpy(path_ricevuta, cJSON_GetObjectItem(risposta, "ricevuta")->valuestring);
    printf("Percorso ricevuta: %s\n", path_ricevuta);
    // stampiamo la risposta del server
    printf("\n");
    time_t t2 = time(NULL);
    struct tm tm2 = *localtime(&t);
    if (strcmp(cJSON_GetObjectItem(risposta, "status")->valuestring, "confermato") == 0){
        printf("%d-%02d-%02d %02d:%02d:%02d: Ordine confermato\n", tm2.tm_year + 1900, tm2.tm_mon + 1, tm2.tm_mday, tm2.tm_hour, tm2.tm_min, tm2.tm_sec);
        } else {
        printf("%d-%02d-%02d %02d:%02d:%02d: Ordine non confermato\n", tm2.tm_year + 1900, tm2.tm_mon + 1, tm2.tm_mday, tm2.tm_hour, tm2.tm_min, tm2.tm_sec);
    }

    // eliminiamo il file ordine.json
    remove(path_ordine);

    /* // creiamo la ricevuta della risposta del server
    printf("Creazione ricevuta\n");
    char path_ricevuta[50];
    cJSON *ordine_risp = cJSON_GetObjectItem(risposta, "ordine");
    char *username = cJSON_GetObjectItem(ordine_risp, "username")->valuestring;
    char data_ordine[50];
    sprintf(data_ordine, "%d-%02d-%02d_%02d_%02d_%02d", tm2.tm_year + 1900, tm2.tm_mon + 1, tm2.tm_mday, tm2.tm_hour, tm2.tm_min, tm2.tm_sec);
    sprintf(path_ricevuta, "%s/%s_%s.txt", path_sala, username, data_ordine);
    printf("Percorso ricevuta: %s\n", path_ricevuta);
    crea_ricevuta(risposta, path_ricevuta); */
}      

// funzione per caricare l'ordine                   
OrdineCompleto carica_ordine(char *riga){
    
    OrdineCompleto static ordine;
    char *parole[5];
    char *token = strtok(riga, ",");
    int k = 0;
    while(token != NULL){
        parole[k] = token;
        token = strtok(NULL, ",");
        k++;
    }
    // salviamo le parole in una struttura convertendo i numeri in interi
    // ordine,prenotazione,data,username,conto,pagato
    ordine.n_ordine = atoi(parole[0]);
    ordine.n_prenotazione = atoi(parole[1]);
    strcpy(ordine.data, parole[2]);
    strcpy(ordine.username, parole[3]);
    ordine.conto_totale = atof(parole[4]);
    // occorre eliminare dall'ultima parola il carattere \n
    parole[5][strlen(parole[5]) - 1] = '\0';
    strcpy(ordine.stato, parole[5]);
    // se il codice della prenotazione è uguale a quello passato come argomento allora eliminiamo la prenotazione

    return ordine;
}         

// funzione per visualizzare l'ordine               
void visualizza_ordini(char* username, char *path){

    // leggiamo il file delle prenotazioni.csv nella cartella path_sala
    // facciamo un ciclo per leggere le righe del file
    FILE *file = fopen(path, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
        return;
    } else {
        bool trovato = false;
        // leggiamo il file e carichiamo ogni riga in una struttura Prenotazione.
        char riga[MAX_LUNG_PORTATA];
        // stampiamo le colonne della prenotazione
        // ordine,prenotazione,data,username,conto,pagato
        printf("       \tCod\tPren\tData\t\tUser\tConto\tPagato\n");
        while(fgets(riga, MAX_PORTATE, file) != NULL){
            // carichiamo la riga come tipo Prenotazione
            OrdineCompleto ordine = carica_ordine(riga);
            // se l'username è uguale a quello passato eliminiamo la prenotazione
            if (strcmp(ordine.username, username) == 0){
                printf("Ordine \t%d\t%d\t%s\t%s\t%.2f\t%s\n", ordine.n_ordine, ordine.n_prenotazione, ordine.data, ordine.username, ordine.conto_totale, ordine.stato);
                trovato = true;
            }
        }
        if (!trovato){
            printf("Nessuna prenotazione trovata\n");
        }
        fclose(file);
    }
}                      

// funzione per eliminare un ordine
void elimina_ordine(int n_ordine, char *path_sala, char *path_temp){
    // carichiamo il file csv delle prenotazioni nel path sala 
    char save_path[50];
    sprintf(save_path, "%s/ordini.csv", path_sala);
    FILE *file = fopen(save_path, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file o file inesistente\n");
    } else {
        // leggiamo il file e carichiamo ogni riga in una struttura OrdineCompleto. 
        int num_ordine = conta_righe(save_path);
        printf("Numero di ordinazioni: %d\n", num_ordine);
        char riga[MAX_LUNG_PORTATA];
        // carichiamo le righe del file
        while(fgets(riga, MAX_PORTATE, file) != NULL){
            // dividiamo le parole separate dalla virgola
            OrdineCompleto ordine = carica_ordine(riga);

            // se il codice dell'ordine è uguale a quello passato come argomento allora eliminiamo l'ordine'
            if (ordine.n_ordine == n_ordine){
                // procediamo ad eliminare la prenotazione
                printf("Ordine %d in eliminazione\n", ordine.n_ordine);
                // creiamo un json per l'ordine da eliminare
                cJSON *ordine_json = cJSON_CreateObject();
                cJSON_AddNumberToObject(ordine_json, "n_ordine", ordine.n_ordine);
                cJSON_AddNumberToObject(ordine_json, "n_prenotazione", ordine.n_prenotazione);
                cJSON_AddStringToObject(ordine_json, "data", ordine.data);
                cJSON_AddStringToObject(ordine_json, "username", ordine.username);
                cJSON_AddNumberToObject(ordine_json, "conto", ordine.conto_totale);
                cJSON_AddStringToObject(ordine_json, "stato", ordine.stato);
                cJSON_AddBoolToObject(ordine_json, "confermata", 0);

                // creiamo un file temporaneo per salvare l'ordine da eliminare 
                char save_path_temp[50];
                sprintf(save_path_temp, "%s/ordine_canc.json", path_temp);
                salva_file_json(ordine_json, save_path_temp);

                // creiamo un ciclo per leggere la risposta del server. Se la risposta è confermata allora eliminiamo l'ordine
                // se dopo 1,5 secondi non abbiamo risposta allora usciamo dal ciclo
                while(1){
                    // attendiamo la risposta del server con un ciclo che controlla se "confermata" è uguale a 0
                    cJSON *risposta = carica_file_json(save_path_temp);
                    if(cJSON_IsTrue(cJSON_GetObjectItem(risposta, "confermata"))){
                        printf("Ordine eliminato\n");
                        // eliminiamo il file temporaneo
                        remove(save_path_temp);
                        break;
                    } else {
                        int i = 0;
                        while(cJSON_IsFalse(cJSON_GetObjectItem(risposta, "confermata")) && i < 3){
                            printf("In attesa di risposta\n");
                            risposta = carica_file_json(save_path_temp);
                            Sleep(500);
                            i++;
                            if(i == 3){
                                printf("L'ordine non è stato pagato, non e' possibile eliminarlo\n");
                            }
                        }
                        // eliminiamo il file temporaneo
                        remove(save_path_temp);
                        break;
                    }
                }

            }
        }
        fclose(file);
    }
}

// funzione per eliminare tutti gli ordini di un utente
void elimina_ordinazioni_utente(char* username, char *path_sala, char *path_temp){
    // carichiamo il file csv delle prenotazioni nel path sala
    char save_path[50];
    sprintf(save_path, "%s/ordini.csv", path_sala);
    
    // facciamo un ciclo per leggere le righe del file
    FILE *file = fopen(save_path, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
        return;
    } else {
        // leggiamo il file e carichiamo ogni riga in una struttura OrdineCompleto.
        char riga[MAX_LUNG_PORTATA];
        while(fgets(riga, MAX_PORTATE, file) != NULL){
            // carichiamo la riga come tipo OrdineCompleto
            OrdineCompleto ordine = carica_ordine(riga);
            // se l'username è uguale a quello passato eliminiamo la prenotazione
            if (strcmp(ordine.username, username) == 0){
                elimina_ordine(ordine.n_ordine, path_sala, path_temp);
            }
        }
    }
    fclose(file);
}

// funzione per controllare se un utente ha ordini
bool check_ordini_utente(char* username, char *path_sala){
    /* da implementare */
}   

// funzione per inserire una recensione
void inserisci_recensione(char *username, char *path_sala, char *path_temp){

    // visualizziamo le prenotazioni dell'username
    char path_prenotazioni[50];
    sprintf(path_prenotazioni, "%s/prenotazioni.csv", path_sala);
    visualizza_prenotazioni(username, path_prenotazioni);

    // se ci sono prenotazioni dell'utente allora procediamo con l'inserimento della recensione
    if(check_prenotazione_u(username, path_prenotazioni) == 0){
        printf("Non ci sono prenotazioni per l'utente %s\n", username);
    } else {
        // chiediamo il codice della prenotazione per la quale si vuole inserire la recensione
        int codice;
        do{
            printf("Inserisci il codice della prenotazione per la quale vuoi inserire la recensione: ");
            scanf("%d", &codice);
            if(check_prenotazione_c(codice, path_prenotazioni) == 0){
                printf("Prenotazione non trovata. Riprova\n");
            } else {
                break;
            }
        } while(1);

        // chiediamo all'utente di inserire la recensione
        char recensione[1000];
        // controlliamo che la recensione sia compresa tra 1 e 900 caratteri
        do {
            printf("Inserisci la tua recensione (max 900 caratteri): ");
            getchar();
            fgets(recensione, 1000, stdin);
            if(strlen(recensione) > 900){
                printf("Recensione non valida. Riprova\n");
            } else {
                break;
            }
        } while(1);

        // inseriamo il numero delle stelle da 1 a 5
        int stelle;
        do{
            printf("Inserisci il numero di stelle da 1 a 5: ");
            scanf("%d", &stelle);
            if(stelle < 1 || stelle > 5){
                printf("Numero di stelle non valido. Riprova\n");
            } else {
                break;
            }
        } while(1);

        // settiamo la data
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        // creiamo un json per la recensione
        cJSON *recensione_json = cJSON_CreateObject();
        cJSON_AddNumberToObject(recensione_json, "codice", codice);
        cJSON_AddStringToObject(recensione_json, "username", username);

        // rendiamo stringa la data
        char data[50];
        sprintf(data, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        cJSON_AddStringToObject(recensione_json, "data", data);

        // eliminiamo l'ultimo carattere \n dalla recensione
        recensione[strlen(recensione) - 1] = '\0';
        cJSON_AddStringToObject(recensione_json, "recensione", recensione);

        cJSON_AddNumberToObject(recensione_json, "stelle", stelle);
        cJSON_AddBoolToObject(recensione_json, "confermata", 0);

        // creiamo un file temporaneo per salvare la recensione
        char save_path_temp[50];
        sprintf(save_path_temp, "%s/recensione.json", path_temp);
        salva_file_json(recensione_json, save_path_temp);

        // attendiamo la risposta del server con un ciclo che controlla se "confermata" è uguale a 0
        cJSON *risposta = carica_file_json(save_path_temp);
        while(cJSON_IsFalse(cJSON_GetObjectItem(risposta, "confermata"))){
            printf("In attesa di risposta\n");
            risposta = carica_file_json(save_path_temp);
            Sleep(1000);
        }

        // eliminiamo il file temporaneo
        remove(save_path_temp);

        // eliminiamo la prenotazione
        printf("Eliminazione della prenotazione corrispondente alla recensione\n");
        elimina_tavolo(codice, path_sala, path_temp);

        // stampiamo la risposta del server
        if (cJSON_IsTrue(cJSON_GetObjectItem(risposta, "confermata"))){
            printf("%s: Recensione confermata\n", data);
        } else {
            printf("%s: Recensione non confermata\n", data);
        }
    }

}

// funzione per caricare la recensione
Recensione carica_recensione(char *riga){
    
    Recensione static recensione;
    char *parole[5];
    char *token = strtok(riga, ",");
    int k = 0;
    while(token != NULL){
        parole[k] = token;
        token = strtok(NULL, ",");
        k++;
    }
    // salviamo le parole in una struttura convertendo i numeri in interi
    // codice,username,data,recensione,stelle
    recensione.codice = atoi(parole[0]);
    strcpy(recensione.username, parole[1]);
    strcpy(recensione.data, parole[2]);
    strcpy(recensione.testo, parole[3]);
    recensione.stelle = atoi(parole[4]);

    return recensione;
}

// funzione per visualizzare le recensioni
void visualizza_recensioni(char *path_recensioni){

    // leggiamo il file delle recensioni.csv nella cartella path_sala
    // facciamo un ciclo per leggere le righe del file
    FILE *file = fopen(path_recensioni, "r");
    if(file == NULL){
        printf("Errore nell'apertura del file\n");
    } else {
        // leggiamo il file e carichiamo ogni riga in una struttura Recensione.
        char riga[MAX_LUNG_PORTATA];
        // stampiamo le colonne della prenotazione
        // codice,username,data,recensione,stelle
        printf("= O = O = O = O = O = O = O = O = O = O = O = O = O = O = O = O =\n");
        printf("\nElenco delle recensioni\n");
        printf("---------------------------------\n");
        int count = 0;
        while(fgets(riga, MAX_PORTATE, file) != NULL){
            if(count > 0){
                // carichiamo la riga come tipo Recensione
                Recensione recensione = carica_recensione(riga);
                // printiamo la recensione in un formato leggibile
                printf("Recensione n %d\n", recensione.codice);
                printf("Utente: %s\n", recensione.username);
                printf("Data: %s\n", recensione.data);
                // printiamo tanti simboli di stella gialla quanti sono le stelle
                printf("Stelle: ");
                for(int i = 0; i < recensione.stelle; i++){
                    printf("\x1b[33mX ");
                }
                printf("\x1b[0m\n");
                printf("Recensione: %s\n", recensione.testo);
                printf("---------------------------------\n");
            } else {
                count++;
            }
        }
        printf("= O = O = O = O = O = O = O = O = O = O = O = O = O = O = O = O =\n");
    }
    fclose(file);
}



// ############################################
// FUNZIONI EFFETTIVE LATO SERVER
// ############################################
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

    /* // salviamo lo username in un archivio.csv dove salviamo tutti gli username
    char path_archivio[50];
    char *username = cJSON_GetObjectItem(account, "username")->valuestring;
    sprintf(path_archivio, "%s/archivio.csv", path_account);
    if(se_esiste(path_account, "archivio", "csv") == 0){
        FILE *file = fopen(path_archivio, "a");
        fprintf(file, "%s,\n", username);
        fclose(file);
    } else {
        FILE *file = fopen(path_archivio, "w");
        fprintf(file, "Elenco Account,\n");
        fprintf(file, "%s,\n", username);
        fclose(file);
    } */
}

// funzione per sloggare tutti gli utenti
void logout_all(char *nome_file, char *path_account){

    // carichiamo l'account
    char save_path[50];
    sprintf(save_path, "%s/%s", path_account, nome_file);
    cJSON *account = carica_file_json(save_path);

    // modifichiamo il valore del campo login in 0
    cJSON_ReplaceItemInObject(account, "login", cJSON_CreateBool(0));

    // salviamo l'account
    salva_file_json(account, save_path);
}

// funzione per controllare gli account presenti nella cartella
void controlla_account(char *path_account, char *tipo_controllo){

    // creiamo un array di stringhe per contenere i nomi dei file presenti nella cartella
    char *files[100];
    int count = 0;
    // opendir returns NULL if couldn't open directory
    DIR *dr = opendir(path_account);
    if (dr == NULL){
        printf("Cartella Account non trovata" );
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
            // controlliamo che tipo di controllo dobbiamo fare, se tipo_controllo è uguale a "approva" allora approviamo l'account altrimenti rifiutiamo l'account
            //printf("Tipo controllo: %s\n", tipo_controllo);
            if (strcmp(tipo_controllo, "approva") == 0){
                cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "status");   // prendiamo il campo status
                // printf("Status: %d\n", cJSON_IsFalse(status));                   // stampiamo il valore del campo status per controllo
                if(cJSON_IsFalse(status) == 1 && strcmp(files[count], ".") != 0 && strcmp(files[count], "..") != 0){
                    approva_account(files[count], path_account);
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    printf("%d-%02d-%02d %02d:%02d:%02d: Account %s approvato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, files[count]);
                }
                count++;
            } else if (strcmp(tipo_controllo, "logout_all") == 0){
                //printf("allert: %s\n", tipo_controllo);
                cJSON *login = cJSON_GetObjectItemCaseSensitive(json, "login");   // prendiamo il campo status
                if(cJSON_IsFalse(login) == 0 && strcmp(files[count], ".") != 0 && strcmp(files[count], "..") != 0){
                    logout_all(files[count], path_account);
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    printf("%d-%02d-%02d %02d:%02d:%02d: Account %s sloggato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, files[count]);
                }
                count++;
            } else {
                printf("Tipo di controllo non riconosciuto\n");
            }
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
        printf("%d-%02d-%02d %02d:%02d:%02d: Login account %s rifiutato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, username->valuestring);
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
        printf("%d-%02d-%02d %02d:%02d:%02d: Logout account %s rifiutato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, username->valuestring);
        // salviamo il file json
        salva_file_json(logout, temp_path);
    }
}

// funzione per controllare la sala
/* void controlla_sala(char *path_sala){

    // carichiamo il file json e la voce piena
    cJSON *sala = carica_file_json(path_sala);
    cJSON *piena = cJSON_GetObjectItem(sala, "piena");

    // controlliamo se la sala è piena non facciamo niente, mentre se la sala non è piena allora controlliamo i tavoli
    // se tutti i tavoli sono occupati allora la sala è piena
    if(cJSON_IsFalse(piena)){
        int n_tavoli = cJSON_GetArraySize(cJSON_GetObjectItem(sala, "tavoli"));
        int tavoli_occupati = 0;
        for (int i = 0; i < n_tavoli; i++){
            cJSON *tavolo = cJSON_GetArrayItem(cJSON_GetObjectItem(sala, "tavoli"), i);
            if(cJSON_IsTrue(cJSON_GetObjectItem(tavolo, "occupato"))){
                tavoli_occupati++;
            }
        }
        if(tavoli_occupati == n_tavoli){
            cJSON_ReplaceItemInObject(sala, "piena", cJSON_CreateBool(1));
            // salviamo il file json
            salva_file_json(sala, path_sala);
            // stampiamo un messaggio di sala piena
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            printf("%d-%02d-%02d %02d:%02d:%02d: %s Sala piena\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, path_sala);
        }
    }
} */

// funzione per prenotare un tavolo
void conferma_prenotazione(char *temp_path, char *path_sala){

    // carichiamo il file prenotazione.json
    char save_path[50];
    sprintf(save_path, "%s/prenotazione.json", temp_path);
    cJSON *prenotazione = carica_file_json(save_path);

    // estraiamo la stringa username dal file json
    cJSON *username = cJSON_GetObjectItemCaseSensitive(prenotazione, "username");

    // estraiamo la data dal file json
    cJSON *data = cJSON_GetObjectItemCaseSensitive(prenotazione, "data");

    // estraiamo il cJSON sala dal file prenotazione
    cJSON *sala = cJSON_GetObjectItem(prenotazione, "sala");

    // estraiamo i tavoli e i posti prenotati dall'oggetto prenotazione
    cJSON *tavolo = cJSON_GetObjectItem(prenotazione, "tavolo");
    cJSON *posti = cJSON_GetObjectItem(prenotazione, "posti");

    // salviamo la sala con i tavoli prenotati nella cartella sala
    char save_path_sala[50];
    sprintf(save_path_sala, "%s/%s.json", path_sala, data->valuestring);
    salva_file_json(sala, save_path_sala);

    // creiamo un controllo per vedere se il file esiste
    int codice = 1;
    char path_prenotazione[50];
    sprintf(path_prenotazione, "%s/prenotazioni.csv", path_sala);
    if(!se_esiste(path_sala, "prenotazioni", "csv")){
        // se il file non esiste allora creiamo il file e scriviamo l'intestazione
        //printf("File non esistente\n");
        FILE* file = fopen(path_prenotazione, "w");
        //printf("File creato\n");
        fprintf(file, "codide,username,data,tavolo,posti\n");
        fprintf(file, "%d,%s,%s,%d,%d\n", codice, username->valuestring, data->valuestring, tavolo->valueint, posti->valueint);
        //printf("Prenotazione scritta\n");
        fclose(file);

    } else {
        // se il file esiste allora controlliamo l'ultimo codice
        //printf("File esistente\n");
        codice = conta_righe(path_prenotazione);
        FILE* file = fopen(path_prenotazione, "a");
        //printf("Ultimo codice: %d\n", codice);
        fprintf(file, "%d,%s,%s,%d,%d\n", codice, username->valuestring, data->valuestring, tavolo->valueint, posti->valueint);
        //printf("Prenotazione scritta\n");
        fclose(file);
    }

    // modifichiamo il valore del campo confermata in 1
    cJSON_ReplaceItemInObject(prenotazione, "confermata", cJSON_CreateBool(1));
    //printf("Valore di prenotazione in confermata\n");
    //cJSON *confermata = cJSON_GetObjectItem(prenotazione, "confermata");
    //printf("Confermata: %d\n", cJSON_IsTrue(confermata));

    // salviamo il file json della prenotazione
    salva_file_json(prenotazione, save_path);

    // stampiamo un messaggio di conferma
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d: Prenotazione confermata\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);    

}

// funzione per eliminare la prenotazione di un tavolo
void elimina_prenotazione(char *temp_path, char *path_sala){

    // carichiamo il file prenotazione_canc.json
    char save_path[50];
    sprintf(save_path, "%s/prenotazione_canc.json", temp_path);
    cJSON *prenotazione = carica_file_json(save_path);

    // carichiamo il file prenotazioni.csv
    char path_prenotazioni[50];
    sprintf(path_prenotazioni, "%s/prenotazioni.csv", path_sala);

    // estraiamo la data dal file json
    cJSON *data = cJSON_GetObjectItemCaseSensitive(prenotazione, "data");

    // estraiamo il tavolo e i posti prenotati dal file json
    int tavolo = cJSON_GetObjectItem(prenotazione, "tavolo")->valueint;
    int posti = cJSON_GetObjectItem(prenotazione, "posti")->valueint;

    // estraiamo il codice della prenotazione
    int codice = cJSON_GetObjectItem(prenotazione, "codice")->valueint;

    // carichiamo il file csv delle prenotazioni nel path sala
    char sala_path[50];
    sprintf(sala_path, "%s/%s.json", path_sala, data->valuestring);
    cJSON *sala = carica_file_json(sala_path);

    // aggiorniamo il tavolo e i posti prenotati nella sala
    cJSON *tav = cJSON_GetArrayItem(cJSON_GetObjectItem(sala, "tavoli"), tavolo - 1);
    // recupero il valore iniziale dei posti liberi e occupati
    int posti_liberi = cJSON_GetNumberValue(cJSON_GetObjectItem(tav, "posti_liberi"));
    int posti_occupati = cJSON_GetNumberValue(cJSON_GetObjectItem(tav, "posti_occupati"));
    // modifichiamo il tavolo
    cJSON_ReplaceItemInObject(tav, "posti_liberi", cJSON_CreateNumber(posti_liberi + posti));
    cJSON_ReplaceItemInObject(tav, "posti_occupati", cJSON_CreateNumber(posti_occupati - posti));

    // se il tavolo era occupato allora lo modifichiamo in libero
    if(cJSON_IsTrue(cJSON_GetObjectItem(tav, "occupato"))){
        cJSON_ReplaceItemInObject(tav, "occupato", cJSON_CreateBool(0));
    }

    // riscriviamo il file prenotazione.csv in path_sala senza la riga della prenotazione con il codice uguale a quello da eliminare
    // apriamo in lettura il file e salviamo tutte le righe in un array di stringhe
    FILE *file_o = fopen(path_prenotazioni, "r");
    char *righe[100];
    int i = 0;
    char riga[MAX_LUNG_PORTATA];
    while(fgets(riga, MAX_PORTATE, file_o) != NULL){
        righe[i] = strdup(riga);
        i++;
    }
    fclose(file_o);

    // apriamo il file in scrittura e riscriviamo tutte le righe tranne quella con il codice uguale a quello da eliminare
    FILE *file_w = fopen(path_prenotazioni, "w");
    for (int j = 0; j < i; j++){
        Prenotazione pren = carica_prenotazioni(righe[j]);
        if(pren.codice != codice){
            fprintf(file_w, "%d,%s,%s,%d,%d\n", pren.codice, pren.username, pren.data, pren.tavolo, pren.posti);
        }
    }
    fclose(file_w);

    // salviamo la sala con i tavoli prenotati nella cartella sala
    char save_path_sala[50];
    sprintf(save_path_sala, "%s/%s.json", path_sala, data->valuestring);
    salva_file_json(sala, save_path_sala);

    // aggiorniamo il file prenotazione_canc.json
    cJSON_ReplaceItemInObject(prenotazione, "confermata", cJSON_CreateBool(1));
    salva_file_json(prenotazione, save_path);

    // stampiamo un messaggio di conferma
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d: Prenotazione %d eliminata\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, codice);

}

// funzione per confermare l'ordine
void conferma_ordine(char *temp_path, char *path_sala){

    // carichiamo il file ordine.json
    char save_path[50];
    sprintf(save_path, "%s/ordine.json", temp_path);
    cJSON *temp = carica_file_json(save_path);

    // creiamo il file di archivio degli ordini in csv nella cartella sala
    char path_ordini[50];
    sprintf(path_ordini, "%s/ordini.csv", path_sala);

    // creiamo un controllo per vedere se il file esiste
    int codice = 1;
    if(!se_esiste(path_sala, "ordini", "csv")){
        // se il file non esiste allora creiamo il file e scriviamo l'intestazione
        FILE* file = fopen(path_ordini, "w");
        fprintf(file, "ordine,prenotazione,data,username,conto,pagato\n");
        fclose(file);
    } else {
        // se il file esiste allora controlliamo l'ultimo codice
        codice = conta_righe(path_ordini);
    }

    // estraiamo da temp la voce ordine
    cJSON *ordine = cJSON_GetObjectItem(temp, "ordine");

    // da ordine estraiamo il campo n_prenotazione, username, data e conto
    cJSON *n_prenotazione = cJSON_GetObjectItem(ordine, "n_prenotazione");
    cJSON *username = cJSON_GetObjectItem(ordine, "username");
    cJSON *data = cJSON_GetObjectItem(ordine, "data");
    cJSON *conto = cJSON_GetObjectItem(ordine, "conto");



    // scriviamo l'ordine nel file csv
    FILE *file = fopen(path_ordini, "a");
    fprintf(file, "%d,%d,%s,%s,%.2f,%s\n", codice, n_prenotazione->valueint, data->valuestring, username->valuestring, conto->valuedouble, "si");
    fclose(file);

    // modifichiamo il valore del campo status in confermato
    cJSON_ReplaceItemInObject(temp, "status", cJSON_CreateString("confermato"));

    // aggiungiamo il numero d'ordine
    cJSON_AddNumberToObject(temp, "n_ordine", codice);

    // creiamo una variabile temporanea per la data
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // creiamo la ricevuta
    //printf("Creazione ricevuta\n");
    char path_ricevuta[50];
    char data_ordine[50];
    sprintf(data_ordine, "%d-%02d-%02d_%02d_%02d_%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    sprintf(path_ricevuta, "%s/%s_%s.txt", path_sala, username->valuestring, data_ordine);
    //printf("Percorso ricevuta: %s\n", path_ricevuta);
    crea_ricevuta(temp, path_ricevuta);

    // aggiungiamo nel file json il percorso della ricevuta
    cJSON_AddItemToObject(temp, "ricevuta", cJSON_CreateString(path_ricevuta));

    // stampiamo un messaggio di conferma
    printf("%d-%02d-%02d %02d:%02d:%02d: Ordine %d confermato e ricevuta stampata\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, codice);

    // salviamo il file json
    salva_file_json(temp, save_path);

}

// funzione per creare la ricevuta dell'ordine
void crea_ricevuta(cJSON *risposta, char *path_ricevuta){
    // creiamo un file json per la ricevuta dell'ordine

    // creiamo un file txt per la ricevuta dell'ordine con i dati della risposta e salviamolo nel percorso path_ricevuta
    FILE *file = fopen(path_ricevuta, "w");

    // stampiamo la ricevuta a video
    fprintf(file, "Ricevuta ordine\n");
    fprintf(file, "Numero ordine: %d\n", cJSON_GetObjectItem(risposta, "n_ordine")->valueint);
    fprintf(file, "Descrizione ordine: %s\n", cJSON_Print(cJSON_GetObjectItem(risposta, "ordine")));
    fprintf(file, "Status: %s\n", cJSON_GetObjectItem(risposta, "status")->valuestring);
    fprintf(file, "Carta di credito: %s\n", cJSON_GetObjectItem(risposta, "carta_di_credito")->valuestring);
    // mettiamo il conto
    fprintf(file, "Conto: %.2f\n", cJSON_GetObjectItem(cJSON_GetObjectItem(risposta, "ordine"), "conto")->valuedouble);
    fprintf(file, "Pagamento: %s\n", cJSON_GetObjectItem(risposta, "pagamento")->valuestring);
    fprintf(file, "Data: %s\n", cJSON_GetObjectItem(risposta, "data")->valuestring);

    // chiudiamo il file
    fclose(file);
}

// funzione per eliminare l'ordine 
void elimina_ordinazione(char *temp_path, char *path_sala){

    // carichiamo il file prenotazione_canc.json
    //printf("Carihco il file temporaneo\n");
    char save_path[50];
    sprintf(save_path, "%s/ordine_canc.json", temp_path);
    cJSON *ordine_canc = carica_file_json(save_path);

    // carichiamo il file prenotazioni.csv
    //printf("Carico il file delle ordinazioni\n");
    char path_ordini[50];
    sprintf(path_ordini, "%s/ordini.csv", path_sala);

    // estraiamo il codide dell'ordine da cancellare
    int codice = cJSON_GetObjectItem(ordine_canc, "n_ordine")->valueint;
    //printf("Codice: %d\n", codice);

    // verifichiamo se lo stato è "si" allora procediamo con l'eliminazione 
    // estraiamo il campo stato dal file json
    char *stato = cJSON_GetObjectItem(ordine_canc, "stato")->valuestring;
    //printf("Stato: %s\n", stato);
    
    // se lo stato è uguale a "si" allora procediamo con l'eliminazione
    //printf("Controlliamo lo stato ed effettuiamo la cancellazione dell'ordine\n");
    if(strcmp(stato, "si") == 0){
        // apriamo il file in lettura e salviamo tutte le righe in un array di stringhe
        FILE *file = fopen(path_ordini, "r");
        char *righe[100];
        int i = 0;
        char riga[MAX_LUNG_PORTATA];
        while(fgets(riga, MAX_PORTATE, file) != NULL){
            righe[i] = strdup(riga);
            i++;
        }
        fclose(file);

        // apriamo il file in scrittura e riscriviamo tutte le righe tranne quella con il codice uguale a quello da eliminare
        FILE *file_w = fopen(path_ordini, "w");
        for (int j = 0; j < i; j++){
            OrdineCompleto ordine = carica_ordine(righe[j]);
            if(ordine.n_ordine != codice){
                fprintf(file_w, "%d,%d,%s,%s,%.2f,%s\n", ordine.n_ordine, ordine.n_prenotazione, ordine.data, ordine.username, ordine.conto_totale, ordine.stato);
            }
        }
        fclose(file_w);

        // aggiorniamo il file prenotazione_canc.json
        //printf("Aggiorniamo il file temporaneo\n");
        cJSON_ReplaceItemInObject(ordine_canc, "confermata", cJSON_CreateBool(1));
        //printf("Salviamo il file temporaneo\n");
        salva_file_json(ordine_canc, save_path);

        // stampiamo un messaggio di conferma
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("%d-%02d-%02d %02d:%02d:%02d: Ordine %d eliminato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, codice);
    } else {
        // stampiamo un messaggio di errore
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("%d-%02d-%02d %02d:%02d:%02d: Ordine %d non eliminato\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, codice);
    }
}

// funzione per approvare una recensione
void approva_recensione(char *temp_path, char *path_recensioni){

    // carichiamo il file recensione.json
    char save_path[50];
    sprintf(save_path, "%s/recensione.json", temp_path);
    cJSON *recensione = carica_file_json(save_path);

    // modifichiamo il valore del campo confermata in 1
    cJSON_ReplaceItemInObject(recensione, "confermata", cJSON_CreateBool(1));

    // salviamo il file json
    salva_file_json(recensione, save_path);

    // salviamo una copia della recensione nella cartella sala nel file recensioni.csv
    char path_recensioni_csv[50];
    sprintf(path_recensioni_csv, "%s/recensioni.csv", path_recensioni);

    // creiamo un controllo per vedere se il file esiste
    int codice = conta_righe(path_recensioni_csv);
    if(!se_esiste(path_recensioni, "recensioni", "csv")){
        // se il file non esiste allora creiamo il file e scriviamo l'intestazione
        FILE* file = fopen(path_recensioni_csv, "w");
        fprintf(file, "codice,username,data,recensione,stelle\n");
        fprintf(file, "%d,%s,%s,%s,%d\n", codice, cJSON_GetObjectItem(recensione, "username")->valuestring, cJSON_GetObjectItem(recensione, "data")->valuestring, cJSON_GetObjectItem(recensione, "recensione")->valuestring, cJSON_GetObjectItem(recensione, "stelle")->valueint);
        fclose(file);
    } else {
        // se il file esiste allora controlliamo l'ultimo codice
        // scriviamo la recensione nel file csv
        FILE *file = fopen(path_recensioni_csv, "a");
        fprintf(file, "%d,%s,%s,%s,%d\n", codice, cJSON_GetObjectItem(recensione, "username")->valuestring, cJSON_GetObjectItem(recensione, "data")->valuestring, cJSON_GetObjectItem(recensione, "recensione")->valuestring, cJSON_GetObjectItem(recensione, "stelle")->valueint);
        fclose(file);
    }

    // stampiamo un messaggio di conferma
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%d-%02d-%02d %02d:%02d:%02d: Recensione approvata e salvata\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}