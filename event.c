#include "event.h"
#include "colors.h"
#include "utils.h"
#include "fileio.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// Returns the difference in seconds between now and the specified date (start date and end date)
int checkTime(int year, int mon, int day)
{
    time_t now = time(NULL);
    struct tm info = {0};
    info.tm_year = year - 1900;
    info.tm_mon = mon - 1;
    info.tm_mday = day;
    info.tm_isdst = -1;
    return now - mktime(&info);
}
void updateStatus(Event *event)
{

    int startY = stoi(event->startDate, 0, 3);
    int startM = stoi(event->startDate, 5, 6);
    int startD = stoi(event->startDate, 8, 9);
    int endY = stoi(event->endDate, 0, 3);
    int endM = stoi(event->endDate, 5, 6);
    int endD = stoi(event->endDate, 8, 9);
    int start = checkTime(startY, startM, startD);
    int end = checkTime(endY, endM, endD);
    if (start < 0)
    {
        event->status = STATUS_UPCOMING;

    }
    else if (start >= 0 && end <= 0)
    {
        event->status = STATUS_ONGOING;

    }
    else
    {
        event->status = STATUS_FINISHED;

    }
}
// Checks if two dates follow the YYYY-MM-DD format ensure the date is valid
int isValidDate(char *date)
{
    if (date[4] != '-' || date[7] != '-' || date[10] != '\0')
    {
        printf(RED "Input does not match the required format (YYYY-MM-DD) !\n" RESET);
        return 0;
    }

    int checkY = stoi(date, 0, 3);
    int checkM = stoi(date, 5, 6);
    int checkD = stoi(date, 8, 9);
    if (checkM > 12 || checkM < 1)
    {
        printf(RED "Invalid date, Please try again !\n" RESET);
        return 0;
    }
    else if (checkM == 4 || checkM == 6 || checkM == 9 || checkM == 11)
    {
        if (1 <= checkD && checkD <= 30)
        {
            return 1;
        }
        else
        {
            printf(RED "Invalid date, Please try again !\n" RESET);
            return 0;
        }
    }
    else if (checkM == 2)
    {
        if (1 <= checkD && checkD <= (checkY % 4 == 0 && checkY % 100 != 0 ? 29 : 28))
        {
            return 1;
        }
        else
        {
            printf(RED "Invalid date, Please try again !\n" RESET);
            return 0;
        }
    }
    else
    {
        if (1 <= checkD && checkD <= 31)
        {
            return 1;
        }
        else
        {
            printf(RED "Invalid date, Please try again !\n" RESET);
            return 0;
        }
    }
}
// Check if  Start Date ≤ End Date.
int isChronological(char *start, char *end)
{
    int year = strncmp(start, end, 4);
    int month = strncmp(start + 5, end + 5, 2);
    int day = strncmp(start + 8, end + 8, 2);

    if (year < 0)
        return 1;
    else if (year <= 0)
    {
        if (month < 0)
            return 1;
        else if (month == 0)
        {
            if (day <= 0)
                return 1;
            else
            {
                printf(RED "Invalid date, Please try again !\n" RESET);
                return 0;
            }
        }
        else
            return 0;
    }
    return 0;
}
// Collects information to initialize a new Event and save it.
void createEvent()
{
    Event newEvent;

    printf("Enter event's name: ");
    inputString(newEvent.name, sizeof(newEvent.name));

    printf("Enter event's decription: ");
    inputString(newEvent.description, sizeof(newEvent.description));
    do
    {
        do
        {
            printf("Enter event's start date (YYYY-MM-DD): ");
            inputString(newEvent.startDate, sizeof(newEvent.startDate));
        } while (!isValidDate(newEvent.startDate));

        do
        {
            printf("Enter event's end date (YYYY-MM-DD): ");
            inputString(newEvent.endDate, sizeof(newEvent.endDate));
        } while (!isValidDate(newEvent.endDate));

    } while (!isChronological(newEvent.startDate, newEvent.endDate));

    printf("Enter event's location: ");
    inputString(newEvent.location, sizeof(newEvent.location));

    newEvent.staffCount = 0;
    updateStatus(&newEvent);

    int actualIndex = getNextEventIndex();
    int tempID = actualIndex + 1;

    strcpy(newEvent.eventId, "EV000000");

    int pos = 7;
    while (tempID > 0 && pos > 1)
    {
        newEvent.eventId[pos--] = (tempID % 10) + '0';
        tempID /= 10;
    }

    if (saveEventAt(actualIndex, &newEvent))
    {
        printf("\033[1;32m[SUCCESS] Event created successfully with ID: %s\033[0m\n", newEvent.eventId);
    }
    else
    {
        printf("\033[1;31m[ERROR] Could not save event data!\033[0m\n");
    }
}
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

void printEventResult()
{
    clearScreen();
    char inputID[EVENT_ID_LENGTH];
    printf("Enter Event ID to search: ");
    inputString(inputID, sizeof(inputID));
    toUpperStr(inputID, inputID);
    int index = findEventIndexById(inputID);
    if (index == -1)
    {
        printf("Event not found.\n");
        printf("Press Enter to continue");
        getchar();
        clearScreen();
        return;
    }

    FILE *f = fopen("data/events.dat", "rb");
    if (f == NULL)
    {
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
    if (fread(&temp, sizeof(Event), 1, f))
    {
        printf("DEBUG: ID=%s | DATE=%s\n", temp.eventId, temp.startDate);
        char statusStr[20];
        switch (temp.status)
        {
        case STATUS_UPCOMING:
            strcpy(statusStr, "Upcoming");
            break;
        case STATUS_ONGOING:
            strcpy(statusStr, "Ongoing");
            break;
        case STATUS_FINISHED:
            strcpy(statusStr, "Completed");
            break;
        default:
            strcpy(statusStr, "Unknown");
            break;
        }
        printf("%-12s | %-30s | %-15s | %s\n", temp.eventId, temp.name, statusStr, temp.startDate);
    }
    fclose(f);
}
