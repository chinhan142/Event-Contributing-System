#include <stdio.h>
#include <stdlib.h>
#include "menuBCN.h"
#include "utils.h"
#include "fileio.h"
#include "staff.h"
#include "user.h"
#include "auth.h"
#include "ExtendMenu.h"
#include "colors.h"
int bcnMenu(Account *acc)
{
    int choice;
    do
    {
        clearScreen();
        printDivider("BAN CHU NHIEM (BCN) DASHBOARD");
        
        printf(YELLOW BOLD "  [ EVENT MANAGEMENT ]\n" RESET);
        printf(GREEN "  1." RESET " Create New Event\n");
        printf(GREEN "  2." RESET " Edit Event Information\n");
        printf(GREEN "  3." RESET " Delete Event\n");
        printf(GREEN "  4." RESET " Update Event Status\n");
        printf(GREEN "  5." RESET " View Full Event List\n");
        printf(GREEN "  6." RESET " View Event Detailed View\n");
        printf(GREEN "  7." RESET " Event Search\n");
        printf("\n");

        printf(YELLOW BOLD "  [ STAFF MANAGEMENT ]\n" RESET);
        printf(GREEN "  8." RESET "  Add Staff to Event\n");
        printf(GREEN "  9." RESET "  Edit Staff Role/Task\n");
        printf(GREEN "  10." RESET " Remove Staff from Event\n");
        printf(GREEN "  11." RESET " View Staff Participation History\n");
        printf("\n");

        printf(YELLOW BOLD "  [ REPORTS & ANALYTICS ]\n" RESET);
        printf(GREEN "  12." RESET " Generate Staff Statistics\n");
        printf("\n");

        printf(YELLOW BOLD "  [ ACCOUNT SETTINGS ]\n" RESET);
        printf(GREEN "  13." RESET " View My Profile\n");
        printf(GREEN "  14." RESET " Change My Password\n");
        printf(GREEN "  15." RESET " Reset Member Password\n");
        printf("\n");

        printf(RED BOLD "  0. Logout System\n" RESET);
        printf(CYAN "==================================================\n" RESET);
        printf(BOLD "Your Selection >> " RESET);
        if (scanf("%d", &choice) != 1) {
            // Preventing infinite loop when EOF

            choice = -1;
            clearInputBuffer();
        } else {
            getchar(); // clear newline
        }

        switch (choice)
        {
            // calling function here
        case 1: // Create new event
            createEvent();
            break;
        case 2: // Update event details
            updateEventDetails();
            break;
        case 3: // Delete event
            deleteEvent();
            break;
        case 4: // Manual update event status
            manualUpdateEventStatus();
            break;
        case 5: { // View all events
            int filterStatus = inputEventStatus();
            displayAllEvent(filterStatus);
            break;
        }
        case 6: // View event detail
            viewEventDetails();
            break;
        case 7: // Search event
            searchEventMenuBCN();
            break;
        case 8: // Add staff to event
            addStaffToEvent();
            break;
        case 9: // Edit staff in event
            editStaffInEvent();
            break;
        case 10: // Delete staff from event
            deleteStaffFromEvent();
            break;
        case 11: { // Staff history
            char *studentId = StudentIDInput();
            displayEventHistory(studentId);
            free(studentId);
            break;
        }
        case 12: // Generate staff statistics
            generateStaffStatistics(); 
            break;
        case 13: // View profile
            viewProfile(acc);
            break;
        case 14: // Change password
            changeOwnPassword(acc);
            break;
        case 15: // Reset password
            resetMemberPassword();
            break;
        case 0: // Logout
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
