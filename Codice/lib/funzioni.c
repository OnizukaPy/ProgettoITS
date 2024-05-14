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

    printf("Inserisci la tua password: ");
    get_pass(password, MAX);
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
void visualizza_account(char *path_account, char* path_sala){
    
        // apriamo il file json dell'accout in lettura
        cJSON *account = carica_file_json(path_account);

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
        // visualizziamo le prenotazioni effettuate
        printf("Prenotazioni effettuate: \n\n");

        // leggiamo il file delle prenotazioni.csv nella cartella path_sala
        char save_path[50];
        sprintf(save_path, "%s/prenotazioni.csv", path_sala);
        FILE *file = fopen(save_path, "r");
        if(file == NULL){
            printf("Errore nell'apertura del file\n");
            return;
        } else {
            // leggiamo il file e lo visualizziamo
            char riga[MAX_LUNG_PORTATA];
            while(fgets(riga, MAX_PORTATE, file) != NULL){
                if (strstr(riga, cJSON_GetObjectItem(account, "username")->valuestring) != NULL){
                    printf("%s", riga);
                }
            }
        }
        printf("-----------------------------\n");
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
void visualizza_menu(char* path){
    // apriamo il file csv in lettura
    
    int n_portate = conta_righe(path);
    Portata *portate = carica_menu(path);

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
            