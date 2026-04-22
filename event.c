#include <stdio.h>
#include <string.h>
#include "event.h"
#include "fileio.h"
#include "utils.h"

// Define format for row in case 5
#define EVENT_ROW_FORMAT "| %-10s | %-15.15s | %-11s | %-11s | %-15.15s | %-5d | %-10s |\n"
#define EVENT_HEADER_LINE "+------------+-----------------+-------------+-------------+-----------------+-------+------------+\n"

// Case 5: View all events

/*
    Table row format function for case 5
 */
void printEventTableHeader()
{
    printf(EVENT_HEADER_LINE);
    printf("| %-10s | %-15s | %-11s | %-11s | %-15s | %-5s | %-10s |\n",
           "ID", "Event Name", "Start Date", "End Date", "Location", "Staff", "Status");
    printf(EVENT_HEADER_LINE);
}

void printEventRow(Event e)
{
    char *statusText[] = {"Upcoming", "Ongoing", "Finished"};
    printf(EVENT_ROW_FORMAT,
           e.eventId,
           e.name,
           e.startDate,
           e.endDate,
           e.location,
           e.staffCount,
           statusText[e.status]);
}
int inputEventStatus()
{
    int choice;
    while (1)
    {
        printf("\n===== FILTER EVENTS BY STATUS =====\n");
        printf("0. View All\n");
        printf("1. Upcoming\n");
        printf("2. Ongoing\n");
        printf("3. Finished\n");
        printf("Enter your choice (0-3): ");

        // If user input something is not integer, these line helps clearing the buffer and continue the loop, avoiding wrong logic
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("[!] Invalid input. Please enter a number.\n");
            continue;
        }

        if (choice >= 0 && choice <= 3)
        {
            // remapping choice
            return choice - 1;
        }
        printf("[!] Out of range. Please choose 0 to 3.\n");
    }
}

void displayAllEvent(int filterStatus)
{
    int total = getNextEventIndex();
    Event event;
    int count = 0;

    printDivider("FULL EVENT LIST");

    printEventTableHeader();

    for (int i = 0; i < total; i++)
    {
        if (loadEventAt(i, &event))
        {
            // filterStatus == -1 is "View All"
            if (filterStatus == -1 || (int)event.status == filterStatus)
            {
                printEventRow(event);
                count++;
            }
        }
    }

    printf(EVENT_HEADER_LINE);
    if (count == 0)
    {
        printf(" [!] No events found matching your criteria.\n");
    }
    else
    {
        printf(" Total: %d event(s) listed.\n", count);
    }
    printf("\n");
}

// Search event with user input

void printEventResult() {
    clearScreen();
    char inputID[EVENT_ID_LENGTH];
    printf("Enter Event ID to search: ");
    inputString(inputID, sizeof(inputID));
    toUpperStr(inputID, inputID);
    int index = findEventIndexById(inputID);
    if (index == -1) {
        printf("Event not found.\n");
        printf("Press Enter to continue");
        getchar();
        clearScreen();
        return;
    }
    
    FILE *f = fopen("data/events.dat", "rb");
    if (f == NULL) {
        printf("No events found.\n");
        printf("Press Enter to continue");
        getchar();
        clearScreen();
        return;
    }
    
    Event temp;
    printf("\n");
    printDivider("SEARCH RESULTS");
    printf("%-12s | %-30s | %-15s | %s\n", "Event ID", "Name", "Status", "Date");
    printf("===============================================================================\n");
    fseek(f, index * sizeof(Event), SEEK_SET);
    if(fread(&temp, sizeof(Event), 1, f)) {
        printf("DEBUG: ID=%s | DATE=%s\n", temp.eventId, temp.startDate);
        char statusStr[20];
        switch (temp.status) {
            case STATUS_UPCOMING: strcpy(statusStr, "Upcoming"); break;
            case STATUS_ONGOING: strcpy(statusStr, "Ongoing"); break;
            case STATUS_FINISHED: strcpy(statusStr, "Completed"); break;
            default: strcpy(statusStr, "Unknown"); break;
        }
        printf("%-12s | %-30s | %-15s | %s\n", temp.eventId, temp.name, statusStr, temp.startDate);
    }
    fclose(f);
}
