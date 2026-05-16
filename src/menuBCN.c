#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menuBCN.h"
#include "utils.h"
#include "fileio.h"
#include "staff.h"
#include "user.h"
#include "auth.h"
#include "ExtendMenu.h"
#include "report.h"
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
        printf(GREEN "  13." RESET " Export Events to Text File\n");
        printf("\n");

        printf(YELLOW BOLD "  [ ACCOUNT SETTINGS ]\n" RESET);
        printf(GREEN "  14." RESET " View My Profile\n");
        printf(GREEN "  15." RESET " Change My Password\n");
        printf(GREEN "  16." RESET " Reset Member Password\n");
        printf("\n");

        printf(RED BOLD "  0. Logout System\n" RESET);
        printf(CYAN "==================================================\n" RESET);
        printf(BOLD "Your Selection >> " RESET);
        int res = scanf("%d", &choice);
        clearInputBuffer();
        if (res != 1) {
            choice = -1;
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
        case 13: // Export events
            exportEventsMenu();
            break;
        case 14: // View profile
            viewProfile(acc);
            break;
        case 15: // Change password
            changeOwnPassword(acc);
            break;
        case 16: // Reset password
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

        if (choice != 0 && choice != 7) {
            pressEnterToContinue();
        }

    } while (choice != 0);
    return 0;
}

// Display event list for user selection
void displayEventListForSelection()
{
    int totalEvents = getNextEventIndex();
    if (totalEvents <= 0)
    {
        printf("No events available\n");
        return;
    }

    printf(CYAN "=== SELECT AN EVENT ===\n" RESET);
    printf(CYAN "%-15s | %-30s | %-25s\n" RESET, "Event ID", "Event Name", "Location");
    printf(CYAN "%s\n" RESET, "====================================================================");

    for (int i = 0; i < totalEvents; i++)
    {
        Event event;
        if (loadEventAt(i, &event))
        {
            // Truncate long strings for display
            char name[32] = {0};
            char location[27] = {0};
            strncpy(name, event.name, 30);
            strncpy(location, event.location, 25);
            if (strlen(event.name) > 30) strcat(name, "..");
            if (strlen(event.location) > 25) strcat(location, "..");
            
            printf("%-15s | %-30s | %-25s\n", event.eventId, name, location);
        }
    }
    printf(CYAN "%s\n" RESET, "====================================================================");
}

// Export events menu
void exportEventsMenu()
{
    int choice;
    do
    {
        clearScreen();
        printDivider("EXPORT EVENTS");

        printf(YELLOW BOLD "Export Options:\n" RESET);
        printf(GREEN "1." RESET " Export a Single Event\n");
        printf(GREEN "2." RESET " Export All Events\n");
        printf(RED "0." RESET " Back to Menu\n");
        printf(CYAN "==================================================\n" RESET);
        printf(BOLD "Your Selection >> " RESET);

        int res = scanf("%d", &choice);
        clearInputBuffer();
        if (res != 1)
        {
            choice = -1;
        }

        switch (choice)
        {
        case 1: { // Export single event
            clearScreen();
            displayEventListForSelection();
            char eventId[EVENT_ID_LENGTH];
            printf("\n" BOLD "Enter Event ID to export: " RESET);
            scanf("%s", eventId);
            clearInputBuffer();
            exportSingleEvent(eventId);
            break;
        }
        case 2: { // Export all events
            printf(BOLD "Exporting all events...\n" RESET);
            exportAllEvents();
            break;
        }
        case 0: // Back
            return;
        default:
            printf(RED BOLD "[!] " RESET "Invalid choice!\n");
            break;
        }

        if (choice != 0)
        {
            pressEnterToContinue();
        }

    } while (choice != 0);
}
