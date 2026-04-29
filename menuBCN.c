#include <stdio.h>
#include <stdlib.h>
#include "menuBCN.h"
#include "utils.h"
#include "fileio.h"
#include "staff.h"
#include "user.h"
#include "auth.h"
int bcnMenu(Account *acc)
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
            printf("Create new event\n");
            createEvent();
            printf("Enter to continue ");
            getchar();
            break;
        case 2:
            printf("Update event details\n");
            updateEventDetails();
            printf("Enter to continue ");
            getchar();
            break;
        case 3:
            printf("Delete event\n");
            deleteEvent();
            printf("Enter to continue ");
            getchar();
            break;
        case 4:
            printf("Manual update event status\n");
            manualUpdateEventStatus();
            printf("Enter to continue ");
            getchar();
            break;
        case 5:
            printf("View all events\n");
            int filterStatus = inputEventStatus();
            displayAllEvent(filterStatus);
            printf("Enter to continue ");
            getchar();
            break;
        case 6:
            printf("View event detail\n");
            viewEventDetails();
            printf("Enter to continue ");
            getchar();
            break;
        case 7:
            printf("Search event\n");
            printEventResult();
            printf("Enter to continue ");
            getchar();
            break;
        case 8:
            printf("Add staff to event\n");
            addStaffToEvent();
            printf("Enter to continue ");
            getchar();
            break;
        case 9:
            printf("Edit staff in event\n");
            editStaffInEvent();
            printf("Enter to continue ");
            getchar();
            break;
        case 10:
            printf("Delete staff from event\n");
            deleteStaffFromEvent();
            printf("Enter to continue ");
            getchar();
            break;
        case 11:
            printf("Staff history\n");
            char *studentId = StudentIDInput();
            displayEventHistory(studentId);
            free(studentId);
            printf("Enter to continue ");
            getchar();
            break;
        case 12:
            printf("[TODO] Statistics\n");
            break;
        case 13:
            viewProfile(acc);
            break;
        case 14:
            printf("Change password\n");
            changeOwnPassword(acc);
            break;
        case 15:
            printf("Reset password\n");
            resetMemberPassword();
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
