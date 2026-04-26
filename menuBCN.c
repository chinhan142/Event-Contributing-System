#include <stdio.h>
#include <stdlib.h>
#include "menuBCN.h"
#include "utils.h"
#include "fileio.h"
#include "staff.h"
#include "user.h"
#include "auth.h"
void bcnMenu(Account *acc)
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
            createEvent();
            break;
        case 2:
            updateEventDetails();
            break;
        case 3:
            deleteEvent();
            break;
        case 4:
            manualUpdateEventStatus();
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
            break;
        case 7:
            printf("Search event\n");
            printEventResult(); // còn tình trạng WET
            break;
        case 8:
            addStaffToEvent();
            break;
        case 9:
            editStaffInEvent();
            break;
        case 10:
            deleteStaffFromEvent();
            break;
        case 11:
            printf("Staff history\n");
            char *studentId = StudentIDInput();
            displayEventHistory(studentId);
            free(studentId);
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
            printf("Logging out...\n");
            break;
        default:
            printf("[!] Invalid choice!\n");
            break;
        }
    } while (choice != 0);
}
