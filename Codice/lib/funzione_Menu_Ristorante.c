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

    // "Cancello" la stampa "Premere un qualsiasi tasto per visualizzare le varie sezioni del menù."
    printf("\033[4A");
    for (int i = 0; i < 4; i++)
    {
        printf("                                                                           ");
        printf("                                                                           \n");
    }
    printf("\033[4A");

    // Lista delle sezioni del menù
    printf("1 - ANTIPASTI\n2 - NAAN (PANE)\n3 - BIRYANI (RISO BASMATI)\n4 - VERDURE\n");
    printf("5 - SPECIALITÀ TANDOOR\n6 - GAMBERI\n7 - POLLO\n8 - AGNELLO\n9 - DOLCI\n\n");

    printf("Inserire un numero per visualizzare il menu\n");
    printf("\toppure premere 0 per chiudere il menu.\n\n");

    // Variabile per l'inserimento da parte dell'utente
    char userInput;

    // Variabile per evitare che la stampa precedente non venga sovrascritta dalla stampa del primo ciclo
    int primoCicloEseguito = 0;

    // Chiedo all'utente di inserire un numero per visualizzare il menù
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

        // Sposto il cursore sopra di 5 righe per sovrascrivere la sezione precedente del menù con una nuova
        if (primoCicloEseguito == 1)
        {
            printf("\033[7A");
            for (int i = 0; i < 12; i++)
            {
                printf("                                                                           ");
                printf("                                                                           \n");
            }
            printf("\033[11A");
        }

        switch (userInput)
        {
        case '1':
            printf("ANTIPASTI\n");
            printf(" 1 - VEGETABLE SAMOSA\t€ 5.00\tPaste triangolari ripiene di verdure\n");
            printf(" 2 - ONION BAJI\t\t€ 5.00\tFrittelle di cipolla con farina di ceci\n");
            printf(" 3 - VEGETABLE PAKORA\t€ 5.00\tFrittelle miste di verdure con farina di ceci\n");
            printf(" 4 - VEGETABLE CUTLET\t€ 5.00\tPolpettine di verdure miste\n\n");
            break;

        case '2':
            printf("NAAN (PANE)\t\t€ 1.50\n");
            printf(" 5 - PLAN NAAN\t\t\tPane al naturale\n");
            printf(" 6 - CHEESE NAAN\t\tPane al formaggio\n");
            printf(" 7 - ALOO NAAN\t\t\tPane alle patate\n");
            printf(" 8 - ONION NAAN\t\t\tPane alle cipolle\n\n");
            break;

        case '3':
            printf("BIRYANI (RISO BASMATI)\n");
            printf(" 9 - DRY FRUIT RICE\t€ 6.00\tRiso basmati con frutta secca\n");
            printf("10 - CHICKEN BIRYANI\t€ 7.00\tRiso basmati con bocconcini di pollo e spezie\n");
            printf("11 - VEGETABLE BIRYANI\t€ 6.00\tRiso basmati con verdure e spezie\n");
            printf("12 - MIX BIRYANI\t€ 7.00\tRiso basmati con misto di carni e verdure\n\n");
            break;

        case '4':
            printf("VERDURE\t\t\t€ 6.00\n");
            printf("13 - DAL MASOOR\t\t\tLenticchie rosse con erbette e spezie\n");
            printf("14 - CHANA MASALA\t\tCeci in umido con spezie\n");
            printf("15 - BEGEN BARTHA\t\tMelanzane arrostite\n");
            printf("16 - VEGETABLE KORMA\t\tVerdure immerse in una salsa di frutta secca\n\n");
            break;

        case '5':
            printf("SPECIALITÀ TANDOOR\t€ 10.00\n");
            printf("17 - MIX TANDOOR\t\tGrigliata mista di carne e pesce marinata con yogurt e spezie\n");
            printf("18 - TANDOOR CHICKEN\t\tCoscette di pollo marinate con yogurt e arrostite al tandoor\n");
            printf("19 - CHICKEN TIKKA\t\tBocconcini di pollo marinati con yogurt e arrostiti al tandoor\n");
            printf("20 - PORK TIKKA\t\t\tSpiedini di maiale arrostiti\n\n");
            break;

        case '6':
            printf("GAMBERI\t\t\t€ 10.00\n");
            printf("21 - MALAI PRAWN\t\tGamberi con latte di cocco\n");
            printf("22 - PRAWN MASALA\t\tGamberi piccanti saltati in padella\n");
            printf("31 - PRAWN MADRAS\t\tGamberi al curry piccante\n");
            printf("24 - FISH MADRAS\t\tBocconcini di pesce al curry piccante\n\n");
            break;

        case '7':
            printf("POLLO\t\t\t€ 10.00\n");
            printf("31 - KORMA\t\t\tBocconcini di pollo con salsa alla frutta secca\n");
            printf("26 - BUTTER\t\t\tBocconcini di pollo con salsa al burro\n");
            printf("27 - TIKKA MASALA\t\tBocconcini di pollo al tandoor saltati con verdure e spezie\n");
            printf("28 - VINDALOO\t\t\tPollo con patate speziate piccanti\n\n");
            break;

        case '8':
            printf("AGNELLO\t\t\t€ 10.00\n");
            printf("29 - MADRAS\t\t\tAgnello al curry piccante con Madras\n");
            printf("30 - KORMA\t\t\tAgnello con frutta secca\n");
            printf("31 - VINDALOO\t\t\tAgnello con patate speziate\n");
            printf("32 - ROGAN JOSH\t\t\tAgnello tagliato a pezzi con yogurt, pomodori, zenzero, aglio, ghee, e spezie indiane\n\n");
            break;

        case '9':
            printf("DOLCI\t\t\t€ 3.00\n");
            printf("33 - GULAB JAMUN\t\tDolci palline di latte in polvere fritte, immerse in sciroppo di zucchero al cardamomo\n");
            printf("34 - JALEBI\t\t\tAnelli di pastella di farina fritti e poi immersi in sciroppo di zucchero con zafferano\n");
            printf("35 - RASGULLA\t\t\tMorbide palline di formaggio fresco cotte in uno sciroppo di zucchero profumato con acqua di rose\n");
            printf("36 - KULFI\t\t\tGelato indiano a base di latte condensato, panna, zucchero e cardamomo, con aggiunta di mandorle e pistacchi\n\n");
            break;
        }

        if (primoCicloEseguito == 0) primoCicloEseguito = 1;
    }

    // Cancello tutta la stampa
    printf("\033[19A");
    for (int i = 0; i < 19; i++)
    {
        printf("                                                                           ");
        printf("                                                                           \n");
    }
    printf("\033[19A");

    /* // Invito l'utente a terminare la funzione del menù
    printf("Premere un qualsiasi tasto per chiudere il menù.");
    getch(); */

    //printf("\n");
}