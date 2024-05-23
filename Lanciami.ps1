## scrivere uno script per personalizzare la shell di powershell,
## per accorciare il prompt e per visualizzare il nome utente e il nome del computer
function prompt {
    ## scriviamo Client@LaGangDelBosco.it> , dove Client è in verde, @LaGangDelBosco.it è in giallo
    Write-Host -NoNewline -ForegroundColor White "[ "
    Write-Host -NoNewline -ForegroundColor Green "Client"
    Write-Host -NoNewline -ForegroundColor White "@"
    Write-Host -NoNewline -ForegroundColor Red "LaGangDelBosco.it"
    Write-Host -NoNewline -ForegroundColor White " ]: "
}


## spostiamoci nella cartella del client
cd "C:\Users\lenov\Documents\Ivan Catalano\Documenti\1.13\ProgettoITS\Codice\src"

## lanciare un clear
clear

## eseguire il server in una nuova finestra e rinominiamo la scheda aperta in "Server"
##start-process -FilePath "powershell" -ArgumentList "-noexit -command &{. .\server.ps1}" -WindowStyle Maximized -Title "Server"
start-process -FilePath "server.exe" -ArgumentList "-start" 

## attendere 2 secondi
Start-Sleep -Seconds 2

## avviamo il client
. .\client.exe 