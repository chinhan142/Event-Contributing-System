#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "fileio.h"
#include "staff.h"
#include "user.h"
#include "menuStaff.h"
#include "auth.h"
#include "colors.h"
int staffMenu(Account *acc)
{
    int choice;
    do
    {
        clearScreen();
        printDivider("MEMBER DASHBOARD");
        
        printf(YELLOW BOLD "  [ MY ACTIVITIES ]\n" RESET);
        printf(GREEN "  1." RESET " View Ongoing Events\n");
        printf(GREEN "  2." RESET " View My Participation History\n");
        printf("\n");

        printf(YELLOW BOLD "  [ ACCOUNT SETTINGS ]\n" RESET);
        printf(GREEN "  3." RESET " View My Profile\n");
        printf(GREEN "  4." RESET " Change My Password\n");
        printf("\n");

        printf(RED BOLD "  0. Logout System\n" RESET);
        printf(CYAN "=========================================================================================================\n" RESET);
        printf(BOLD "Your Selection >> " RESET);
        if (scanf("%d", &choice) != 1) {
            // Preventing infinite loop when EOF
            if (feof(stdin)) return 0;
            choice = -1;
            clearInputBuffer();
        } else {
            getchar();
        }

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
            printf(RED BOLD "[!] " RESET "Invalid choice!\n");
            break;
        }
    } while (choice != 0);
    return 0;
}