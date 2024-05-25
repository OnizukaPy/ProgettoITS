# ProgettoITS: La Gang del Bosco

Membri del team:

Ivan Catalano
Alessio Canessa
Francesco Bindi
Ruben Uremassi
Mirco Càstino

# Definizione delle caratteristiche del progetto

Il progetto consiste nel progettare, implementare, documentare e testare un'applicazione per la gestione di un ristorante. Questa app deve avere le seguenti funzionalità:

    Gestione utenti,
    Prenotazione Tavoli,
    Gestione del Menu,
    Gestione Ordini,
    Feedback dei Clienti.

Per ogni entità saranno previste le operazioni di:

    inserimento,
    modifica,
    cancellazione,
    visualizzazione.

I dati dovranno essere salvati in modo da poter essere utilizzati dopo un riavvio dell’applicazione. (usare preferibilmente un database)
L’applicazione dovrebbe essere composta da un modulo client e da un modulo server. (requisito opzionale)

# Analisi dei requisiti

La progettazione dell'applicazione è avvenuta tramite il metodo Scrum, l'utilizzo dei diagrammi Use Case e Sequence e la realizzazione del diagramma di Gantt.
La prima operazione svolta è stata l'analisi dei requisiti e la pesatura delle card, mediante il metodo del poker.

Abbiamo deciso di optare per una architettura client/server con la comunicazione mediante file temporanei e archiviazione su file tipo jSON, csv e txt. Il linguaggio di programmazione scelto è C, con l'uso di una libreria esterna dell'autore DaveGamble, cJSON (https://github.com/DaveGamble/cJSON).

# Guida d'uso

## Avvio del Server

La sequenza di avvio dell'app prevede l'avvio del server per il tramite del comando:

 	./server.exe -start

Questo comandlo lancia il server il quale controllerà l'esistenza di tutte le cartelle funzionali al funzionamento del client, creandole in assenza. La seconda operazione che fa è una cancellazione di tutti i file temporanei. Successivamente si mette in ascolto delle richieste che arrivano dal client:

- Richiesta di status (attivo/inattivo) del server
- Approvazione/modifica di account appena creati/modificati
- Login/Logout utenti
- Richieste di prenotazione di tavoli
- Richieste di ordinazioni delle portate dal menu (con pagamento ed emissione della ricevuta)
- Rilascio di recensioni

## Uso del Client:

### Gestione dell'account

Per comprendere le operazioni eseguibili con il client, si può leggere la guida digitando _./Client.exe --help_. I comandi a disposizione sono:

	./client.exe [PARAM] [OPTION/S]

PARAM sono i parametri utilizzabili e sono:

	Creazione account:		-c, senza options. 
 
Consente la creazione di un account. Verranno richiesti i dati utenti e la password verrà cifrata e resa non visibile

 	Visualizzazione account:	-v, seguito da "username".

Consente di visualizzare i dati dell'account, comprese le prenotazioni e le ordinazioni effettuate.

	Modificare account:		-m, seguito da "username".

Consente ad un account loggato, di modificare i campi di nome, cognome, mail e password. Lo username non è modificabile.  

	Eliminazione account:		-e, seguito da "username".

Consente ad un account loggato, di eliminare l'account, comprese le prenotazioni effettuate. GLi ordini e le recensioni non vengono rimosse, in quanto storico del ristorante. Anche le ricevute emesse non vengono cancellate.

  	Login e Logout:			-login, -logout, seguiti da "username".

Consentono il login e il logout dell'utente. Verrà richiesta la digitazione della password per entrambe le operazioni.  

La gestione dell'account avviene tramite l'uso di un file jSON, il cui nome sarà: username.json. Il contenuto del file sarà il seguente:

Il jSON ha questo formato:
	
	{
		"nome":		"ivan",
		"cognome":	"catalano",
		"username":	"ivan",
		"password":	"sdvvzrug",
		"email":	"pippo@pippo.it",
		"status":	false,
	    	"login":    	false
	}

La password viene criptata con il cifrario di cesare con chiave 3. I valori di "login" e "status" sono quei valori del file che verranno modificati, rispettivamente nelle operazioni di login e logout dell'utente e nelle operazioni di creazione/modifica account.

Le operazioni di dialogo tra client e server verranno gestite tramite un file temporaneo dedicato.

### Gestione Sala/Menu

	Visualizzazione del menu:	-menu, senza opzioni.

Il menu del ristorante è editato esternalmente su un file di tipo csv. Questo file viene soltanto letto dal client quando richiesto per la visualizzazione dello stesso dalle varie funzionalità. L'editing è quindi fisico da parte dell'amministratore del sistema o da utente delegato dal ristorante.

	Visualizzazione della sala:	-sala, "data[gg/mm/aaa]".

La visualizzazione della sala prevede il passaggio di una sola option che è la data in formato gg/mm/aaaa. _Queste due operazioni sono effettuabili senza essere loggati_.



# Gestione Ordine

Una volta effettuato il login, e visualizzato la disponibilità della sala e del menu, si può procedere alla creazione di un ordine attraverso il comando ..... , prima di confermare l ordine verranno chiesti i dati di pagamento. Una volta che l'inoltro dell ordine è completato avviene la conferma del server tramite scrittura del file temporaneo, una volta che viene confermato lo stato dell' ordine esso viene scritto al file "ordini.csv" che è non volatile. Inoltre viene emessa una ricevuta in formato "ricevuta.txt". Se si dovesse eliminare un ordine, lo si può fare in egual modo a seguito del login, digitando il comando .... seguito dal codice dell'ordine, anche qui il client comunica con il server tramite il file temporaneo, una volta che l' ordine è stato cancellato con successo viene aggiornato il file "ordini.csv"

# Gestione Account
Come citato in precedenza sia il login che il logout richiedono l inserimento della password. L'autenticazione o la conferma in caso di logout vengono confermate dal server tramite scrittura del FILE TEMP. Le modifiche di stato vengono scritte sul file "utente.json" descritto in precedenza.
Una volta effettuata la procedura di login, si potrà visualizzare il profilo e gli ordini/ prenotazioni con i comandi....., inoltre si potrà eliminare l'account con il comando....
Se l'utente non avesse ancora un profilo, esso si può creare con il comando .... , qui vengono chieste le generalità, il server approva tramite FILE TEMP, e la conferma di creazione account viene scritta sul file rigido "utente.json" al quale vengono associati n°prenotazioni e ordini.

# Gestione Feedback

Per poter inserire una recensione bisogna disporre di un numero di prenotazione, l' approvazione della recensione avviene tramite il server che comunica con il client tramite FILE TEMP. Il sistema farà riferimento al file "prenotazioni.csv" citato in precedenza, a questo punto la recensione viene scritta sul file "recensioni.csv" visualizzabile da qualsiasi utente loggato con il comando ...
