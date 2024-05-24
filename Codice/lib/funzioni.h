// in questo file saranno elencate tutti i prototipi delle funzioni che saranno utilizzate nei file client.c e server.c
#include "../lib/cJSON.h"
#include <dirent.h>  
#include <stdbool.h>

// STRUTTURE
// definiamo la struttura portata
#ifndef FUNZIONI_H
#define FUNZIONI_H

typedef struct Portata{
    int codice;
    char categoria[50];
    char nome[50];
    char descrizione[100];
    float prezzo;
} Portata;

typedef struct Prenotazione{
    int codice;
    char username[50];
    char data[50];
    int tavolo;
    int posti;
} Prenotazione;

typedef struct OrdineSingolo{
    Portata portata;
    int quantita;
} OrdineSingolo;

typedef struct OrdineCompleto{
    int n_prenotazione;
    int n_ordine;
    char username[50];
    char data[50];
    int tavolo;
    OrdineSingolo *OrdineSingolo;
    double conto_totale;
    char stato[50];
} OrdineCompleto;

typedef struct Recensione{
    int codice;
    char username[50];
    char data[50];
    char testo[100];
    int stelle;
} Recensione;

#endif
// FUNZIONI PROPEDEUTICHE

// funzioni per la gestione dei file json
void salva_file_json(cJSON *root, char *nome_file);                 // funzione per salvare un file json
cJSON *carica_file_json(char *nome_file);                           // funzione per caricare un file json

// funzioni per la gestione dei file
void svuota_cartella(char *path);                                   // funzione per svuotare una cartella
int conta_righe(char *path);                                        // funzione per contare le righe di un file
char* conversione_data(char *data);                                 // funzione per convertire la data in un formato leggibile
bool check_formato_data(char *data);                                // funzione per controllare il formato della data
bool se_esiste(char *path, char *nome_file, char* tipo_file);    // funzione per controllare se un file esiste
Prenotazione carica_prenotazioni(char *riga);                     // funzione per caricare le prenotazioni
OrdineCompleto carica_ordine(char *riga);                         // funzione per caricare una riga di ordine

// Gestione password, cifratura e decifratura
char *get_pass(char *pword, int size);                              // funzione per ottenere la password copeerta da asterischi
char *cifra(char *stringa, int chiave);                             // funzione per cifrare una stringa
char *decifra(char *stringa, int chiave);                           // funzione per decifrare una stringa

// FUNZIONI EFFETTIVE LATO CLIENT
void print_guida(char *path);                                       // funzione per stampare la guida
bool status_server(char *temp_path);                                // funzione per controllare lo stato del server
cJSON crea_account(char *path_account);                             // funzione per creare un account
void visualizza_account(char *path_account, char *path_sala, char *path_ordini);       // funzione per visualizzare gli account
void login(char *username, char *path_account, char *login_path);   // funzione per effettuare il login
bool seLoggato(char *username, char *path_account);                 // funzione per controllare se un utente è loggato
void logout(char *username, char *path_account, char *login_path);  // funzione per effettuare il logout
void crea_sala(char *path_sala, int n_tavoli);                      // funzione per creare una sala
void visualizza_sala(char *data);                                   // funzione per la disposizione della sala
int status_sala(cJSON *sala);                                       // funzione per controllare lo stato della sala
Portata *carica_menu(char *path);                                   // funzione per caricare il menu del ristorante
void visualizza_menu(Portata *portate, char *path);                              // funzione per visualizzare il menu
void prenota_tavolo(char *username, char *data, cJSON *sala, char *path_temp);   // funzione per prenotare un tavolo
void elimina_tavolo(int prenotazione, char *path_sala, char *path_temp);   // funzione per eliminare la prenotazione di un tavolo
void elimina_prenotazioni(char *username, char *path_sala, char *path_temp);   // funzione per eliminare tutte le prenotazioni di un utente
void visualizza_prenotazioni(char* username, char *path);   // funzione per visualizzare le prenotazioni di un utente
bool check_prenotazione_c(int n, char *path);   // funzione per controllare se una prenotazione è in elenco
bool check_prenotazione_u(char *username, char *path);   // funzione per controllare se una prenotazione è in elenco
OrdineSingolo crea_ordine(Portata *menu, int num_portate, OrdineCompleto ordine);                              // funzione per creare un ordine
cJSON *crea_ordine_completo_json(OrdineCompleto ordine, int n_ordini);                      // funzione per creare un ordine in formato json
void salva_ordine(cJSON *ordine, char *path_ordine, char *path_sala);                      // funzione per salvare l'ordine
void visualizza_ordini(char* username, char *path);                     // funzione per visualizzare l'ordine
void elimina_ordine(int n_ordine, char *path_sala, char *path_temp);                     // funzione per eliminare un ordine
void elimina_ordinazioni_utente(char* username, char *path_sala, char *path_temp);                     // funzione per eliminare tutti gli ordini di un utente
bool check_ordini_utente(char* username, char *path_sala);                     // funzione per controllare se un utente ha ordini

// funzioni su cui lavorare
void inserisci_recensione(char *username, char *path_sala, char *path_temp);   // funzione per inserire una recensione
Recensione caricare_recensione(char *riga);   // funzione per caricare una recensione
void visualizza_recensioni(char *path_recensioni);   // funzione per visualizzare le recensioni


// FUNZIONI EFFETTIVE LATO SERVER
void print_guida_server();                                          // funzione per stampare la guida
void return_status(char *temp_path);                                // funzione per ritornare lo stato del server
void approva_account(char *nome_file, char *path_account);          // funzione per approvare un account
void logout_all(char *nome_file, char *path_account);               // funzione per sloggare tutti gli utenti
void controlla_account(char *path_account, char *tipo_controllo);   // funzione per controllare un account
void login_check(char *temp_path, char *path_account);              // funzione per controllare il login
void logout_check(char *temp_path, char *path_account);             // funzione per controllare il logout
//void controlla_sala(char *path_sala);                               // funzione per controllare la sala
void conferma_prenotazione(char *temp_path, char *path_sala);       // funzione per confermare la prenotazione
void elimina_prenotazione(char *temp_path, char *path_sala);       // funzione per eliminare la prenotazione di un tavolo
void conferma_ordine(char *temp_path, char *path_sala);             // funzione per confermare l'ordine
void crea_ricevuta(cJSON *ordine, char *path_ricevuta);                      // funzione per creare la ricevuta
void elimina_ordinazione(char *temp_path, char *path_sala);              // funzione per eliminare un ordine

// funzioni su cui lavorare
void approva_recensione(char *temp_path, char *path_recensioni);   // funzione per approvare una recensione
