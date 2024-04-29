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

    // Chiedo all'utente di inserire un numero per visualizzare una sezione del menu
    printf("Inserire un numero per visualizzare una sezione del menu.\n\n");
    do userInput = getch();
    while (userInput < 49 || userInput > 56);

    while (userInput != '0')
    {
        switch (userInput)
        {
        case '1':
            printf("ANTIPASTI\n");
            printf("- VEGETABLE SAMOSA € 5.00\n  - Paste triangolari ripiene di verdure\n");
            printf("- ONION BAJI € 5.00\n  - Frittelle di cipolla con farina di ceci\n");
            printf("- VEGETABLE PAKORA € 5.00\n  - Frittelle miste di verdure con farina di ceci\n");
            printf("- VEGETABLE CUTLET € 5.00\n  - Polpettine di verdure miste\n");
            break;

        case '2':
            printf("NAAN (PANE) € 1.50\n");
            printf("- PLAN NAAN\t pane al naturale\n");
            printf("- CHEESE NAAN\t pane al formaggio\n");
            printf("- ALOO NAAN\t pane alle patate\n");
            printf("- ONION NAAN\t pane alle cipolle\n\n");
            break;

        case '3':
            printf("BIRYANI (RISO BASMATI)\n");
            printf("- DRY FRUIT RICE € 6.00\n  - Riso basmati con frutta secca\n");
            printf("- CHICKEN BIRYANI € 7.00\n  - Riso basmati con bocconcini di pollo e spezie\n");
            printf("- VEGETABLE BIRYANI € 6.00\n  - Riso basmati con verdure e spezie\n");
            printf("- MIX BIRYANI € 7.00\n  - Riso basmati con misto di carni e verdure\n\n");
            break;

        case '4':
            printf("VERDURE € 6.00\n");
            printf("- DAL MASOOR\t Lenticchie rosse con erbette e spezie\n");
            printf("- CHANA MASALA\t Ceci in umido con spezie\n");
            printf("- BEGEN BARTHA\t Melanzane arrostite\n");
            printf("- VEGETABLE KORMA\t Verdure immerse in una salsa di frutta secca\n\n");
            break;

        case '5':
            printf("SPECIALITÀ TANDOOR € 10.00\n");
            printf("- MIX TANDOOR\t Grigliata mista di carne e pesce marinata con yogurt e spezie\n");
            printf("- TANDOOR CHICKEN\t Coscette di pollo marinate con yogurt e arrostite al tandoor\n");
            printf("- CHICKEN TIKKA\t Bocconcini di pollo marinati con yogurt e arrostiti al tandoor\n");
            printf("- PORK TIKKA\t Spiedini di maiale arrostiti\n\n");
            break;

        case '6':
            printf("GAMBERI € 10.00\n");
            printf("- MALAI PRAWN\t Gamberi con latte di cocco\n");
            printf("- PRAWN MASALA\t Gamberi piccanti saltati in padella\n");
            printf("- PRAWN MADRAS\t Gamberi al curry piccante\n");
            printf("- FISH MADRAS\t Bocconcini di pesce al curry piccante\n\n");
            break;

        case '7':
            printf("POLLO € 10.00\n");
            printf("- KORMA\t\t Bocconcini di pollo con salsa alla frutta secca\n");
            printf("- BUTTER\t\t Bocconcini di pollo con salsa al burro\n");
            printf("- TIKKA MASALA\t Bocconcini di pollo al tandoor saltati con verdure e spezie\n");
            printf("- VINDALOO\t Pollo con patate speziate piccanti\n\n");
            break;

        case '8':
            printf("AGNELLO € 10.00\n");
            printf("- MADRAS\t\t Agnello al curry piccante con Madras\n");
            printf("- KORMA\t\t Agnello con frutta secca\n");
            printf("- VINDALOO\t Agnello con patate speziate\n\n");
            break;
        }

        printf("Inserire un numero per visualizzare una sezione del menu\n");
        printf("\toppure premere 0 per uscire dal menu principale.\n\n");
        do userInput = getch();
        while (userInput < 48 || userInput > 56);
    }

    printf("DOLCI € 3.00\nIl presente menu sostituisce i precedenti ed è valido da Aprile 2020\n\n");
    printf("NB: In assenza di prodotti freschi potranno essere utilizzati prodotti surgelati\n\n");

    printf("Premere un tasto per continuare...\n\n");
    getch();

    printf("CESTINI MISTI\n\nVEGETARIANO € 10.00\nMISTO € 11.00\nGRIGLIATA TANDOOR € 11.00\n\n");

    printf("Premere un tasto per chiudere il menu.");
    getch();
}