#include <stdio.h>
#include "event.h"
#include "ExtendMenu.h"
#include "utils.h"
#include "colors.h"
void searchEventMenuBCN()
{
    int choice = -1;

    while (choice != 0)
    {
        clearScreen();
        printDivider("EVENT SEARCH");
        
        printf(YELLOW BOLD "  [ SEARCH OPTIONS ]\n" RESET);
        printf(GREEN "  1." RESET " Search by Unique Event ID\n");
        printf(GREEN "  2." RESET " Search by Start Date Range\n");
        printf("\n");
        
        printf(RED BOLD "  0. Return to Dashboard\n" RESET);
        printf(CYAN "==================================================\n" RESET);
        printf(BOLD "Your Selection >> " RESET);

        if (scanf("%d", &choice) != 1)
        {
            if (feof(stdin)) return;
            clearInputBuffer();
            printf(RED BOLD "[!] " RESET "Invalid input. Please enter a number.\n");
            continue;
        }
        getchar(); /* clear '\n' */

        switch (choice)
        {
        case 1:
            // printf("Search event\n");
            printEventResult();
            break;
        case 2:
            searchEventsByStartDateRange();
            break;
        case 0:
            break;
        default:
            printf(RED BOLD "[!] " RESET "Invalid choice!\n");
            break;
        }

        if (choice == 1 || choice == 2)
        {
            printf(BOLD "Enter to continue..." RESET);
            int c = getchar();
            if (c == EOF) return;
        }
    }
}
