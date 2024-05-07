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
    printf("TAJ MAHAL - INDIAN CUISINE\n");
    printf("ORARI:\t12:00 - 15:00 / 18:30 - 22:30\n\n");

    printf("Premere un qualsiasi tasto per visualizzare le varie sezioni del menu.\n");
    getch();

    // "Cancello" la stampa "Premere un qualsiasi tasto per visualizzare le varie sezioni del menu."
    printf("\033[4A");
    for (int i = 0; i < 4; i++)
    {
        printf("                                                                           ");
        printf("                                                                           \n");
    }
    printf("\033[4A");

    // Lista delle sezioni del menu
    printf("1 - ANTIPASTI\n2 - NAAN (PANE)\n3 - BIRYANI (RISO BASMATI)\n4 - VERDURE\n");
    printf("5 - SPECIALITA' TANDOOR\n6 - GAMBERI\n7 - POLLO\n8 - AGNELLO\n9 - DOLCI\n\n");

    printf("Inserire un numero per visualizzare il menu\n");
    printf("\toppure premere 0 per chiudere il menu.\n\n");

    // Variabile per l'inserimento da parte dell'utente
    char userInput;

    // Variabile per evitare che la stampa precedente non venga sovrascritta dalla stampa del primo ciclo
    int primoCicloEseguito = 0;

    // Chiedo all'utente di inserire un numero per visualizzare il menu
    for (; ;)
    {
        if (primoCicloEseguito == 0)
        {
            do userInput = getch();
            while (userInput < '1' || userInput > '9');
        }
        else
        {
            do userInput = getch();
            while (userInput < '0' || userInput > '9');
        }

        if (userInput == '0') break;

        // Sposto il cursore sopra di 5 righe per sovrascrivere la sezione precedente del menu con una nuova
        if (primoCicloEseguito == 1)
        {
            printf("\033[6A");
            for (int i = 0; i < 5; i++)
            {
                printf("                                                                           ");
                printf("                                                                           \n");
            }
            printf("\033[5A");
        }

        switch (userInput)
        {
        case '1':
            printf("ANTIPASTI\t\t€ 5.00\n");
            printf(" 1 - VEGETABLE SAMOSA\tPaste triangolari ripiene di verdure\n");
            printf(" 2 - ONION BAJI\t\tFrittelle di cipolla con farina di ceci\n");
            printf(" 3 - VEGETABLE PAKORA\tFrittelle miste di verdure con farina di ceci\n");
            printf(" 4 - VEGETABLE CUTLET\tPolpettine di verdure miste\n\n");
            break;

        case '2':
            printf("NAAN (PANE)\t\t€ 1.50\n");
            printf(" 5 - PLAN NAAN\t\tPane al naturale\n");
            printf(" 6 - CHEESE NAAN\tPane al formaggio\n");
            printf(" 7 - ALOO NAAN\t\tPane alle patate\n");
            printf(" 8 - ONION NAAN\t\tPane alle cipolle\n\n");
            break;

        case '3':
            printf("BIRYANI (RISO BASMATI)\t€ 7.00\n");
            printf(" 9 - DRY FRUIT RICE\tRiso basmati con frutta secca\n");
            printf("10 - CHICKEN BIRYANI\tRiso basmati con bocconcini di pollo e spezie\n");
            printf("11 - VEGETABLE BIRYANI\tRiso basmati con verdure e spezie\n");
            printf("12 - MIX BIRYANI\tRiso basmati con misto di carni e verdure\n\n");
            break;

        case '4':
            printf("VERDURE\t\t\t€ 6.00\n");
            printf("13 - DAL MASOOR\t\tLenticchie rosse con erbette e spezie\n");
            printf("14 - CHANA MASALA\tCeci in umido con spezie\n");
            printf("15 - BEGEN BARTHA\tMelanzane arrostite\n");
            printf("16 - VEGETABLE KORMA\tVerdure immerse in una salsa di frutta secca\n\n");
            break;

        case '5':
            printf("SPECIALITA' TANDOOR\t€ 10.00\n");
            printf("17 - MIX TANDOOR\tGrigliata mista di carne e pesce marinata con yogurt e spezie\n");
            printf("18 - TANDOOR CHICKEN\tCoscette di pollo marinate con yogurt e arrostite al tandoor\n");
            printf("19 - CHICKEN TIKKA\tBocconcini di pollo marinati con yogurt e arrostiti al tandoor\n");
            printf("20 - PORK TIKKA\t\tSpiedini di maiale arrostiti\n\n");
            break;

        case '6':
            printf("GAMBERI\t\t\t€ 10.00\n");
            printf("21 - MALAI PRAWN\tGamberi con latte di cocco\n");
            printf("22 - PRAWN MASALA\tGamberi piccanti saltati in padella\n");
            printf("31 - PRAWN MADRAS\tGamberi al curry piccante\n");
            printf("24 - FISH MADRAS\tBocconcini di pesce al curry piccante\n\n");
            break;

        case '7':
            printf("POLLO\t\t\t€ 10.00\n");
            printf("25 - KORMA\t\tBocconcini di pollo con salsa alla frutta secca\n");
            printf("26 - BUTTER\t\tBocconcini di pollo con salsa al burro\n");
            printf("27 - TIKKA MASALA\tBocconcini di pollo al tandoor saltati con verdure e spezie\n");
            printf("28 - VINDALOO\t\tPollo con patate speziate piccanti\n\n");
            break;

        case '8':
            printf("AGNELLO\t\t\t€ 10.00\n");
            printf("29 - MADRAS\t\tAgnello al curry piccante con Madras\n");
            printf("30 - KORMA\t\tAgnello con frutta secca\n");
            printf("31 - VINDALOO\t\tAgnello con patate speziate\n");
            printf("32 - ROGAN JOSH\t\tAgnello tagliato a pezzi con yogurt, pomodori, zenzero, aglio, ghee, e spezie indiane\n\n");
            break;

        case '9':
            printf("DOLCI\t\t\t€ 3.00\n");
            printf("33 - GULAB JAMUN\tDolci palline di latte in polvere fritte, immerse in sciroppo di zucchero al cardamomo\n");
            printf("34 - JALEBI\t\tAnelli di pastella di farina fritti e poi immersi in sciroppo di zucchero con zafferano\n");
            printf("35 - RASGULLA\t\tMorbide palline di formaggio fresco cotte in uno sciroppo di zucchero profumato con acqua di rose\n");
            printf("36 - KULFI\t\tGelato indiano a base di latte condensato, panna, zucchero e cardamomo, con aggiunta di mandorle e pistacchi\n\n");
            break;
        }

        if (primoCicloEseguito == 0) primoCicloEseguito = 1;
    }

    // Cancello tutta la stampa
    printf("\033[19A");
    for (int i = 0; i < 18; i++)
    {
        printf("                                                                           ");
        printf("                                                                           \n");
    }
    printf("\033[18A");

    /* // Invito l'utente a terminare la funzione del menu
    printf("Premere un qualsiasi tasto per chiudere il menu.");
    getch(); */

    //printf("\n");
}