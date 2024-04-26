// in questo file saranno elencate tutti i prototipi delle funzioni che saranno utilizzate nei file client.c e server.c
#include "../lib/cJSON.h"
#include <dirent.h>  

// funzioni propedeutiche

// funzioni per la gestione dei file json
void salva_file_json(cJSON *root, char *nome_file);     // funzione per salvare un file json
cJSON *carica_file_json(char *nome_file);               // funzione per caricare un file json

// funzioni per la gestione dei file


// cifratura e decifratura
char *cifra(char *stringa, int chiave);                         // funzione per cifrare una stringa
char *decifra(char *stringa, int chiave);                       // funzione per decifrare una stringa

// funzioni effettive per il client
void print_guida();                                               // funzione per stampare la guida
cJSON crea_account(char *path);                                   // funzione per creare un account

// funzioni effettive per il server
void approva_account(char *nome_file, char *path);                // funzione per approvare un account
void controlla_account(char *path);                               // funzione per controllare un account