// in questo file saranno elencate tutti i prototipi delle funzioni che saranno utilizzate nei file client.c e server.c
#include "../lib/cJSON.h"
#include <dirent.h>  
#include <stdbool.h>

// FUNZIONI PROPEDEUTICHE

// funzioni per la gestione dei file json
void salva_file_json(cJSON *root, char *nome_file);     // funzione per salvare un file json
cJSON *carica_file_json(char *nome_file);               // funzione per caricare un file json

// funzioni per la gestione dei file

// cifratura e decifratura
char *cifra(char *stringa, int chiave);                         // funzione per cifrare una stringa
char *decifra(char *stringa, int chiave);                       // funzione per decifrare una stringa

// FUNZIONI EFFETTIVE LATO CLIENT
void print_guida(char *path);                                     // funzione per stampare la guida
bool status_server(char *temp_path);                                  // funzione per controllare lo stato del server
cJSON crea_account(char *path_account);                           // funzione per creare un account
void login(char *username, char *path_account, char *login_path);                   // funzione per effettuare il login
void logout(char *username, char *path_account, char *login_path);                                    // funzione per effettuare il logout
void visualizza_sala(char *data);                                      // funzione per la disposizione della sala
cJSON crea_sala(char *path_sala, int n_tavoli);                                      // funzione per creare una sala

// FUNZIONI EFFETTIVE LATO SERVER
void return_status(char *temp_path);                              // funzione per ritornare lo stato del server
void approva_account(char *nome_file, char *path_account);        // funzione per approvare un account
void controlla_account(char *path_account);                       // funzione per controllare un account
void login_check(char *temp_path, char *path_account);            // funzione per controllare il login
void logout_check(char *temp_path, char *path_account);           // funzione per controllare il logout