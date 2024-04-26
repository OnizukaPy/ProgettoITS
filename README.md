# ProgettoITS: La Gang del Bosco

Membri:

Ivan Catalano
Alessio Canessa
Francesco Bindi
Ruben Uremassi
Mirco Càstino

# Definizione delle caratteristiche del progetto

Progettazione, implementazione, documentazione e test di un'applicazione per la gestione di un ristorante.
Dettagli del progetto:

Creare una applicazione per la gestione di un ristorante. 
Deve essere possibile gestire: 

    Prenotazione Tavoli, 
    Gestione del Menu, 
    Gestione Ordini, 
    Feedback dei Clienti. 

Per ogni entità devono essere previste le operazioni di:

    inserimento, 
    modifica, 
    visualizzazione, 
    cancellazione.

I dati devono essere salvati in modo da poter essere utilizzati dopo un riavvio dell’applicazione. (usare preferibilmente un database)
L’applicazione dovrebbe essere composta da un modulo client e da un modulo server. (requisito opzionale)


# Analisi dei requisiti

## Funzionamento del client/server

# Creazione Account

La creazione dell'account avviene tramite il client. Una volta creato l'account viene salvato in un file json con nome file uguale allo username dell'account. Il jSON ha questo formato:

{
	"nome":	"ivan",
	"cognome":	"catalano",
	"username":	"ivan",
	"password":	"sdvvzrug",
	"email":	"ivan@ivan.it",
	"status":	false,
    "login":    false
}

La password viene criptata con il cifrario di cesare con chiave 3. Lo status indica se l'account è connesso o meno e viene aggiornato dal server. Il campo login viene usato invece per determinare una richiesta di connessione