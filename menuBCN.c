#include <stdio.h>
#include "menuBCN.h"
#include "utils.h"
#include "event.h"

void bcnMenu()
{
    int choice;
    do
    {
        printDivider("BCN MENU");
        printf("  [Event Management]\n");
        printf("  1. Create new event\n");
        printf("  2. Edit event info\n");
        printf("  3. Delete event\n");
        printf("  4. Update event status\n");
        printf("  5. View all events\n");
        printf("  6. View event detail\n");
        printf("  7. Search event\n");
        printf("\n");
        printf("  [Staff Management]\n");
        printf("  8.  Add staff to event\n");
        printf("  9.  Edit staff role/task\n");
        printf("  10. Remove staff from event\n");
        printf("  11. View staff history\n");
        printf("\n");
        printf("  [Reports]\n");
        printf("  12. Staff statistics\n");
        printf("\n");
        printf("  [Account]\n");
        printf("  13. View my profile\n");
        printf("  14. Change password\n");
        printf("  15. Reset member password\n");
        printf("\n");
        printf("  0. Logout\n");
        printf("========================================\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline sau scanf

        switch (choice)
        {
            // calling function here
        case 1:
            printf("[TODO] Create event\n");
            break;
        case 2:
            printf("[TODO] Edit event\n");
            break;
        case 3:
            printf("[TODO] Delete event\n");
            break;
        case 4:
            printf("[TODO] Update status\n");
            break;
        case 5:
            printf("View all events\n");
            int filterStatus = inputEventStatus();
            displayAllEvent(filterStatus);
            break;
        case 6:
            printf("[TODO] View event detail\n");
            break;
        case 7:
            printf("[TODO] Search event\n");
            break;
        case 8:
            printf("[TODO] Add staff\n");
            break;
        case 9:
            printf("[TODO] Edit staff role\n");
            break;
        case 10:
            printf("[TODO] Remove staff\n");
            break;
        case 11:
            printf("[TODO] Staff history\n");
            break;
        case 12:
            printf("[TODO] Statistics\n");
            break;
        case 13:
            printf("[TODO] View profile\n");
            break;
        case 14:
            printf("[TODO] Change password\n");
            break;
        case 15:
            printf("[TODO] Reset password\n");
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