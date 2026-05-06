#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "fileio.h"
#include "staff.h"
#include "user.h"
#include "menuStaff.h"
#include "auth.h"
int staffMenu(Account *acc)
{
    int choice;
    do
    {
        printDivider("STAFF MENU");
        printf("  [My Events]\n");
        printf("  1. View current events\n");
        printf("  2. View event history\n");
        printf("\n");
        printf("  [Account]\n");
        printf("  3. View my profile\n");
        printf("  4. Change password\n");
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
            printf("View event menu\n");
            currentEventsMenu(acc);
            break;
        case 2:
            printf("View Event history\n");
            displayCurrentUserEventHistory(acc);
            break;
        case 3:
            printf("View profile\n");
            viewProfile(acc);
            break;
        case 4:
            printf("Change password\n");
            changeOwnPassword(acc);
            break;
        case 0:
            if (logoutMain())
                return 1;
            break;
        default:
            printf("[!] Invalid choice!\n");
            break;
        }
    } while (choice != 0);
    return 0;
}