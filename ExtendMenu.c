#include <stdio.h>
#include "event.h"
#include "ExtendMenu.h"
#include "utils.h"
void searchEventMenuBCN()
{
    int choice = -1;

    while (choice != 0)
    {
        printDivider("SEARCH EVENT");
        printf("  1. Search by Event ID\n");
        printf("  2. Search by Start Date Range\n");
        printf("  0. Back\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n') {}
            printf("[!] Invalid input.\n");
            continue;
        }
        getchar(); /* clear '\n' */

        switch (choice)
        {
        case 1:
            printf("Search event\n");
            printEventResult();
            break;
        case 2:
            searchEventsByStartDateRange();
            break;
        case 0:
            break;
        default:
            printf("[!] Invalid choice!\n");
            break;
        }

        if (choice == 1 || choice == 2)
        {
            printf("Enter to continue...");
            getchar();
        }
    }
}
