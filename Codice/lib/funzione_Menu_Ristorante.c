#include <stdio.h>
#include <conio.h>

void Menu_Ristorante();

int main()
{
    Menu_Ristorante();

    return 0;
}

void Menu_Ristorante()
{
    // Variabile per l'inserimento da parte dell'utente
    char userInput;

    printf("TAJ MAHAL - INDIAN CUISINE\n");
    printf("ORARI:\t12.00 - 03.00 / 18.30 - 22-30\n\n");

    printf("Premere INVIO per visualizzare le varie sezioni del menu.");
    getch();
    printf("\n\n");

    // Lista delle sezioni del menu
    printf("1 - ANTIPASTI\n2 - NAAN (PANE)\n3 - BIRYANI (RISO BASMATI)\n4 - VERDURE\n");
    printf("5 - SPECIALITÀ TANDOOR\n6 - GAMBERI\n7 - POLLO\n8 - AGNELLO\n\n");

    int cont = 0;
    // Chiedo all'utente di inserire un numero per visualizzare una sezione del menu
    while (userInput != '0')
    {
        printf("Inserire un numero per visualizzare una sezione del menu\n");
        printf("\toppure premere 0 per uscire dal menu principale.\n\n");
        do userInput = getch();
        while (userInput < 48 || userInput > 56);

        if (userInput == '0') break;

        // Sposto il cursore sopra di 5 righe per sovrascrivere la sezione precedente del menu con una nuova
        if (cont == 1)
        {
            printf("\033[9A");
            for (int i = 0; i < 12; i++)
            {
                printf("                                                  ");
                printf("                                                  \n");
            }
            printf("\033[12A");
        }

        switch (userInput)
        {
        case '1':
            printf("ANTIPASTI\n");
            printf("- VEGETABLE SAMOSA € 5.00\tPaste triangolari ripiene di verdure\n");
            printf("- ONION BAJI € 5.00\t\tFrittelle di cipolla con farina di ceci\n");
            printf("- VEGETABLE PAKORA € 5.00\tFrittelle miste di verdure con farina di ceci\n");
            printf("- VEGETABLE CUTLET € 5.00\tPolpettine di verdure miste\n\n");
            break;

        case '2':
            printf("NAAN (PANE) € 1.50\n");
            printf("- PLAN NAAN\tPane al naturale\n");
            printf("- CHEESE NAAN\tPane al formaggio\n");
            printf("- ALOO NAAN\tPane alle patate\n");
            printf("- ONION NAAN\tPane alle cipolle\n\n");
            break;

        case '3':
            printf("BIRYANI (RISO BASMATI)\n");
            printf("- DRY FRUIT RICE € 6.00\t\tRiso basmati con frutta secca\n");
            printf("- CHICKEN BIRYANI € 7.00\tRiso basmati con bocconcini di pollo e spezie\n");
            printf("- VEGETABLE BIRYANI € 6.00\tRiso basmati con verdure e spezie\n");
            printf("- MIX BIRYANI € 7.00\t\tRiso basmati con misto di carni e verdure\n\n");
            break;

        case '4':
            printf("VERDURE € 6.00\n");
            printf("- DAL MASOOR\t\tLenticchie rosse con erbette e spezie\n");
            printf("- CHANA MASALA\t\tCeci in umido con spezie\n");
            printf("- BEGEN BARTHA\t\tMelanzane arrostite\n");
            printf("- VEGETABLE KORMA\tVerdure immerse in una salsa di frutta secca\n\n");
            break;

        case '5':
            printf("SPECIALITÀ TANDOOR € 10.00\n");
            printf("- MIX TANDOOR\t\tGrigliata mista di carne e pesce marinata con yogurt e spezie\n");
            printf("- TANDOOR CHICKEN\tCoscette di pollo marinate con yogurt e arrostite al tandoor\n");
            printf("- CHICKEN TIKKA\t\tBocconcini di pollo marinati con yogurt e arrostiti al tandoor\n");
            printf("- PORK TIKKA\t\tSpiedini di maiale arrostiti\n\n");
            break;

        case '6':
            printf("GAMBERI € 10.00\n");
            printf("- MALAI PRAWN\tGamberi con latte di cocco\n");
            printf("- PRAWN MASALA\tGamberi piccanti saltati in padella\n");
            printf("- PRAWN MADRAS\tGamberi al curry piccante\n");
            printf("- FISH MADRAS\tBocconcini di pesce al curry piccante\n\n");
            break;

        case '7':
            printf("POLLO € 10.00\n");
            printf("- KORMA\t\tBocconcini di pollo con salsa alla frutta secca\n");
            printf("- BUTTER\tBocconcini di pollo con salsa al burro\n");
            printf("- TIKKA MASALA\tBocconcini di pollo al tandoor saltati con verdure e spezie\n");
            printf("- VINDALOO\tPollo con patate speziate piccanti\n\n");
            break;

        case '8':
            printf("\nAGNELLO € 10.00\n");
            printf("- MADRAS\tAgnello al curry piccante con Madras\n");
            printf("- KORMA\t\tAgnello con frutta secca\n");
            printf("- VINDALOO\tAgnello con patate speziate\n\n");
            break;
        }

        if (cont == 0) cont = 1;
    }

    printf("\033[23A");
    for (int i = 0; i < 23; i++)
    {
        printf("                                                  ");
        printf("                                                  \n");
    }
    printf("\033[23A");

    printf("DOLCI € 3.00\n");
    printf("// AGGIUNGERE DEI DOLCI\n\n"); // AGGIUNGERE DEI DOLCI
    printf("NB: In assenza di prodotti freschi potranno essere utilizzati prodotti surgelati\n\n");

    printf("Premere un tasto per continuare...\n\n");
    getch();

    printf("CESTINI MISTI\n\nVEGETARIANO € 10.00\nMISTO € 11.00\nGRIGLIATA TANDOOR € 11.00\n\n");

    printf("Premere un tasto per chiudere il menu.");
    getch();
}