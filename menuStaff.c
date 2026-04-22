#include <stdio.h>
#include "user.h"
#include "auth.h"
#include "menuStaff.h"
#include "utils.h"


void staffMenu(Account *acc)
{
    int choice;
    do
    {
        printDivider("STAFF MENU");
        printf("  [My Events]\n");
        printf("  1. View current events\n");
        printf("  2. View event detail\n");
        printf("  3. View event history\n");
        printf("\n");
        printf("  [Account]\n");
        printf("  4. View my profile\n");
        printf("  5. Change password\n");
        printf("\n");
        printf("  0. Logout\n");
        printf("========================================\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            // calling function here
        case 1:
            printf("[TODO] Current events\n");
            break;
        case 2:
            printf("[TODO] Event detail\n");
            break;
        case 3:
            printf("[TODO] Event history\n");
            break;
        case 4:
            printf("View profile\n");
            viewProfile(acc);
            break;
        case 5:
            printf("[TODO] Change password\n");
            break;
        case 0:
            printf("Logging out...\n");
            break;
        default:
            printf("[!] Invalid choice!\n");
            break;
        }
    } while (choice != 0);
}