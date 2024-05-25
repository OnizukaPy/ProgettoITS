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

//# Analisi dei requisiti

//## Funzionamento del client/server

# Generali

Ogni comando si accede tramite ./Client.exe --help

# Creazione Account

La creazione dell'account avviene tramite il client con il comando ./client.exe -c .
L'applicazione chiederà le generalità e la password.
Una volta creato l'account viene salvato in un file json con nome file uguale allo username dell'account a cui viene aggiunto status approvazione e login che cambiano true/false in base a conferma del server.

Il jSON ha questo formato:

{
	"nome":	"ivan",
	"cognome":	"catalano",
	"username":	"ivan",
	"password":	"sdvvzrug",
	"email":	"ivan@ivan.it",
	"status":	false,
        "login":    false
}

La password viene criptata con il cifrario di cesare con chiave 3.

# Login/Logout

Una volta creato l'account per poter accedere alla visualizzazione, prenotazione o inserimento di recensione occorre effettuare login tramite il comando
./client.exe -login, la cui operazione di logout corrispondente sarà -logout, in entrambi i casi verrà chiesta la password di conferma. Questa operazione come menzionato in precedenza andrà a modificare la status true/false all'interno del username.jSON.

# Gestione Sala/ Menu

Previo login si può visualizzare la sala in base alla data e il menu, per iniziare la prenotazione però, bisognerà prima eseguire la procedura di login descritta sopra. Una volta effettuato il login, si potrà procedere con la prenotazione. Una volta selezionato il tavolo e quanti posti a sedere verranno occupati, il server genera una risposta di confermata prenotazione che andrà a scrivere il file "prenotazioni.csv" previa comunicazione temporanea sul FILE TEMP. Eventualmente anche per disdire una prenotazione effettuata in precedenza la procedura è la medesima.

# Gestione Ordine

Una volta effettuato il login, e visualizzato la disponibilità della sala e del menu, si può procedere alla creazione di un ordine attraverso il comando ..... , prima di confermare l ordine verranno chiesti i dati di pagamento. Una volta che l'inoltro dell ordine è completato avviene la conferma del server tramite scrittura del file temporaneo, una volta che viene confermato lo stato dell' ordine esso viene scritto al file "ordini.csv" che è non volatile. Inoltre viene emessa una ricevuta in formato "ricevuta.txt". Se si dovesse eliminare un ordine, lo si può fare in egual modo a seguito del login, digitando il comando .... seguito dal codice dell'ordine, anche qui il client comunica con il server tramite il file temporaneo, una volta che l' ordine è stato cancellato con successo viene aggiornato il file "ordini.csv"

# Gestione Account

Come citato in precedenza sia il login che il logout richiedono l inserimento della password. L'autenticazione o la conferma in caso di logout vengono confermate dal server tramite scrittura del FILE TEMP. Le modifiche di stato vengono scritte sul file "utente.json" descritto in precedenza.
Una volta effettuata la procedura di login, si potrà visualizzare il profilo e gli ordini/ prenotazioni con i comandi....., inoltre si potrà eliminare l'account con il comando....
Se l'utente non avesse ancora un profilo, esso si può creare con il comando .... , qui vengono chieste le generalità, il server approva tramite FILE TEMP, e la conferma di creazione account viene scritta sul file rigido "utente.json" al quale vengono associati n°prenotazioni e ordini.

# Gestione Feedback

Per poter inserire una recensione bisogna disporre di un numero di prenotazione, l' approvazione della recensione avviene tramite il server che comunica con il client tramite FILE TEMP. Il sistema farà riferimento al file "prenotazioni.csv" citato in precedenza, a questo punto la recensione viene scritta sul file "recensioni.csv" visualizzabile da qualsiasi utente loggato con il comando ...
