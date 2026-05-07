#include "event.h"
#include "colors.h"
#include "utils.h"
#include "fileio.h"
#include "user.h"
#include "auth.h"
#include "paths.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
/* first index has date <= toDate */
static int firstIdxLE_ToDateDesc(const MatchedEvent *arr, int n, const char *toDate){
    int l = 0, r = n;
    while(l < r){
        int m = l + (r - l)/2;
        if (strcmp(arr[m].event.startDate, toDate) > 0)
            l = m + 1;        
        else
            r = m;       
    }
    return l;
}
/* first index has date < fromDate */
static int firstIdxLT_FromDateDesc(const MatchedEvent *arr, int n, const char *fromDate){
    int l = 0, r = n;
    while(l < r){
        int m = l + (r - l)/2;
        if (strcmp(arr[m].event.startDate, fromDate) >= 0)
            l = m + 1;
        else
            r = m;
    }
    return l;
}

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
                printf(RED "Invalid date, Please try again (Chronological error) !\n" RESET);
                return 0;
            }
        }
        else
        {
            printf(RED "Invalid date, Please try again (Chronological error) !\n" RESET);
            return 0;
        }
    }
    printf(RED "Invalid date, Please try again (Chronological error) !\n" RESET);
    return 0;
}
// Collects information to initialize a new Event and save it.
void createEvent()
{
    Event newEvent;

    printf(BOLD "Enter event's name: " RESET);
    inputString(newEvent.name, sizeof(newEvent.name));

    printf(BOLD "Enter event's description: " RESET);
    inputString(newEvent.description, sizeof(newEvent.description));
    do
    {
        do
        {
            printf(BOLD "Enter event's start date (YYYY-MM-DD): " RESET);
            inputString(newEvent.startDate, sizeof(newEvent.startDate));
            // Preventing infinite loop when EOF
        if (feof(stdin)) return;
        } while (!isValidDate(newEvent.startDate));

        do
        {
            printf(BOLD "Enter event's end date (YYYY-MM-DD): " RESET);
            inputString(newEvent.endDate, sizeof(newEvent.endDate));
            // Preventing infinite loop when EOF
        if (feof(stdin)) return;
        } while (!isValidDate(newEvent.endDate));

    } while (!isChronological(newEvent.startDate, newEvent.endDate));

    printf(BOLD "Enter event's location: " RESET);
    inputString(newEvent.location, sizeof(newEvent.location));

    newEvent.staffCount = 0;
    updateStatus(&newEvent);

    int index = getNextEventIndex();
    Event temp;
    loadEventAt(index - 1, &temp);
    int actualIndex = stoi(temp.eventId, 2, 7);
    int tempID = actualIndex + 1;

    strcpy(newEvent.eventId, "EV000000");

    int pos = 7;
    while (tempID > 0 && pos > 1)
    {
        newEvent.eventId[pos--] = (tempID % 10) + '0';
        tempID /= 10;
    }

    if (saveEventAt(index, &newEvent))
    {
        printf(GREEN BOLD "[SUCCESS] Event created successfully with ID: %s\n" RESET, newEvent.eventId);
    }
    else
    {
        printf(RED BOLD "[ERROR] " RESET "Could not save event data!\n");
    }
}

int inputEventStatus()
{
    int choice;
    while (1)
    {
        printf(YELLOW BOLD "\n===== FILTER EVENTS BY STATUS =====\n" RESET);
        printf(GREEN "0." RESET " View All\n");
        printf(GREEN "1." RESET " Upcoming\n");
        printf(GREEN "2." RESET " Ongoing\n");
        printf(GREEN "3." RESET " Finished\n");
        printf(BOLD "Enter your choice (0-3): " RESET);

        if (scanf("%d", &choice) != 1)
        {
            // Preventing infinite loop when EOF
            if (feof(stdin)) return -1;
            clearInputBuffer();
            printf(RED BOLD "[!] " RESET "Invalid input. Please enter a number.\n");
            continue;
        }

        if (choice >= 0 && choice <= 3)
        {
            getchar();
            return choice - 1;
        }
        printf(RED BOLD "[!] " RESET "Out of range. Please choose 0 to 3.\n");
    }
}

void displayAllEvent(int filterStatus)
{
    char *line = CYAN "+------------+---------------------------+-------------+-------------+-----------------+-------+------------+\n" RESET;
    char *headerFmt = CYAN "| " BOLD "%-10s" RESET CYAN " | " BOLD "%-25s" RESET CYAN " | " BOLD "%-11s" RESET CYAN " | " BOLD "%-11s" RESET CYAN " | " BOLD "%-15s" RESET CYAN " | " BOLD "%-5s" RESET CYAN " | " BOLD "%-10s" RESET CYAN " |\n" RESET;
    char *rowFmt = CYAN "|" RESET " %-10s " CYAN "|" RESET " %-25.25s " CYAN "|" RESET " %-11s " CYAN "|" RESET " %-11s " CYAN "|" RESET " %-15.15s " CYAN "|" RESET " %-5d " CYAN "|" RESET " %s%-10s" RESET CYAN " |\n" RESET;
    char *statusNames[] = {"Upcoming", "Ongoing", "Finished"};
    char *statusColors[] = {YELLOW, GREEN, RED};

    int total = getNextEventIndex();
    Event event;
    int count = 0;

    printDivider("FULL EVENT LIST");

    printf("%s", line);
    printf(headerFmt, "ID", "Event Name", "Start", "End", "Location", "Staff", "Status");
    printf("%s", line);

    for (int i = 0; i < total; i++)
    {
        if (loadEventAt(i, &event))
        {
            if (filterStatus == -1 || (int)event.status == filterStatus)
            {
                printf(rowFmt,
                       event.eventId,
                       event.name,
                       event.startDate,
                       event.endDate,
                       event.location,
                       event.staffCount,
                       statusColors[event.status],
                       statusNames[event.status]);
                count++;
            }
        }
    }

    printf("%s", line);

    if (count == 0)
    {
        printf(YELLOW BOLD "[INFO] " RESET "No events found matching your criteria.\n");
    }
    else
    {
        printf(" Total: %d event(s) listed.\n", count);
    }
}

// Search event with user input

void printEventResult()
{
    
    char inputID[EVENT_ID_LENGTH];
    printf(BOLD "Enter Event ID to search (or press Enter to skip): " RESET);
    inputString(inputID, sizeof(inputID));
    if (inputID[0] == '\0')
    {
        printf(YELLOW BOLD "[INFO] " RESET "Search cancelled.\n");
        return;
    }
    toUpperStr(inputID, inputID);
    int index = findEventIndexById(inputID);
    if (index == -1)
    {
        printf(RED BOLD "[ERROR] " RESET "Event not found.\n");
        clearScreen();
        return;
    }

    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (f == NULL)
    {
        printf(YELLOW BOLD "[INFO] " RESET "No events found.\n");
        return;
    }

    Event temp;
    
    
    printf("\n");
    printDivider("SEARCH RESULTS");
    printf(CYAN BOLD "%-12s | %-25s | %-12s | %-12s | %-20s | %s\n" RESET, "Event ID", "Name", "Status", "Date", "User Name", "Role");
    printf(CYAN "=========================================================================================================\n" RESET);
    fseek(f, index * sizeof(Event), SEEK_SET);
    if (fread(&temp, sizeof(Event), 1, f))
    {
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
        User currentUser;
        char userName[50] = "Unknown";
        char userRole[30] = "Unknown";

        // Avoid out-of-bounds access when the event has no assigned staff.
        if (temp.staffCount > 0)
        {
            int foundUser = findUserById(temp.staffList[0].studentId, &currentUser);
            if (foundUser)
            {
                strcpy(userName, currentUser.studentName);
            }

            switch (temp.staffList[0].role)
            {
            case STAFF_LEADER:
                strcpy(userRole, "Leader");
                break;
            case STAFF_MEMBER:
                strcpy(userRole, "Member");
                break;
            case STAFF_SUPPORT:
                strcpy(userRole, "Support");
                break;
            default:
                strcpy(userRole, "Unknown");
                break;
            }
        }
        printf("%-12s | %-25s | %-12s | %-12s | %-20s | %s\n", 
               temp.eventId, temp.name, statusStr, temp.startDate, userName, userRole);
               printf("=========================================================================================================\n");
    }
    fclose(f);
}
void updateName(Event *event)
{
    char newName[NAME_LENGTH];
    printf(GREEN "Please enter new name: ");
    inputString(newName, sizeof(newName));

    if (confirmAction(RED "Are you sure you want to change the name ?" RESET))
    {
        strcpy(event->name, newName);
        printf(GREEN BOLD "[SUCCESS]" RESET " Name changed successfully\n");
    }
}
void updateDescription(Event *event)
{
    char newDescription[DESC_LENGTH];
    printf(GREEN "Please enter new description: ");
    inputString(newDescription, sizeof(newDescription));

    if (confirmAction(RED "Are you sure you want to change the description ?" RESET))
    {
        strcpy(event->description, newDescription);
        printf(GREEN BOLD "[SUCCESS]" RESET " Description changed successfully\n");
    }
}

void updateLocation(Event *event)
{
    char newLocation[DESC_LENGTH];
    printf(GREEN "Please enter new location: ");
    inputString(newLocation, sizeof(newLocation));

    if (confirmAction(RED "Are you sure you want to change the location ?" RESET))
    {
        strcpy(event->location, newLocation);
        printf(GREEN BOLD "[SUCCESS]" RESET " Location changed successfully\n");
    }
}
void updateStartDate(Event *event)
{
    char newStartDate[DESC_LENGTH];
    printf(GREEN "Please enter new start date (YYYY-MM-DD): ");
    do
    {
        inputString(newStartDate, sizeof(newStartDate));
        // Preventing infinite loop when EOF
        if (feof(stdin)) return;

    } while (!isValidDate(newStartDate) || !isChronological(newStartDate, event->endDate));
    Event temp = *event;
    strcpy(temp.startDate, newStartDate);
    updateStatus(&temp);
    char message[1000];
    if (event->status > temp.status)
    {
        printf(RED "Date change failed: This would move the event back from 'Ongoing' to 'Upcoming'!\n" RESET);
        return;
    }
    if (temp.status != event->status)
    {
        strcpy(message, ORANGE "This action will change the status. Are you sure you want to proceed?" RESET);
    }
    else
        strcpy(message, RESET "Are you sure you want to change the start date ?" RESET);

    if (confirmAction(message))
    {
        strcpy(event->startDate, newStartDate);
        updateStatus(event);
        printf(GREEN BOLD "[SUCCESS]" RESET " Start date changed successfully\n");
    }
}

void updateEndDate(Event *event)
{
    char newEndDate[DESC_LENGTH];
    do
    {
        printf(GREEN "Please enter new end date (YYYY-MM-DD): ");
        inputString(newEndDate, sizeof(newEndDate));
        // Preventing infinite loop when EOF
        if (feof(stdin)) return;

    } while (!isValidDate(newEndDate) || !isChronological(event->startDate, newEndDate));
    Event temp = *event;
    strcpy(temp.endDate, newEndDate);
    updateStatus(&temp);
    char message[1000];
    if (event->status > temp.status)
    {
        printf(RED "Date change failed: This would move the event back from 'Ongoing' to 'Upcoming'!\n" RESET);
        return;
    }
    if (temp.status != event->status)
    {
        strcpy(message, ORANGE "This action will change the status. Are you sure you want to proceed?" RESET);
    }
    else
        strcpy(message, RED "Are you sure you want to change the end date ?" RESET);

    if (confirmAction(message))
    {
        strcpy(event->endDate, newEndDate);
        updateStatus(event);
        printf(GREEN BOLD "[SUCCESS]" RESET " End date changed successfully\n");
    }
}
void updateEventDetails()
{

    char eventID[ID_LENGTH];
    int index;
    Event event;
    while (1)
    {
        displayAllEvent(-1);
        do
        {
            printf(BOLD "Enter the ID of the event you want to update (enter to exit): " RESET);
            inputString(eventID, sizeof(eventID));
            if (eventID[0] == '\0')
                return;
            index = findEventIndexById(eventID);
            if (index == -1)
            {
                printf(ORANGE "ID not found, please try again!\n" RESET);
            }
        } while (index == -1);

        loadEventAt(index, &event);
        if (event.status == STATUS_FINISHED)
        {
            printf(RED "You cannot edit a finished event.\n" RESET);
            return;
        }
        if (event.status == STATUS_ONGOING)
        {
            if (!confirmAction(RED "This event is ongoing. Are you sure you want to edit it?" RESET))
            {
                return;
            }
        }
        printf("===== Choose attribute =====\n");
        printf(GREEN "0: Event's name\n1: Event's description\n2: Event's location\n3: Event's start date\n4: Event's end date\n" RESET);
        int choice;
        while (1)
        {

            // If user input something is not integer, these line helps clearing the buffer and continue the loop, avoiding wrong logic
            if (scanf("%d", &choice) != 1)
            {
                // Preventing infinite loop when EOF
        if (feof(stdin)) return;
                clearInputBuffer();
                printf(RED BOLD "[!] " RESET "Invalid input. Please enter a number.\n");
                continue;
            }

            if (choice >= 0 && choice <= 4)
            {
                // remapping choice
                getchar();
                break;
            }
            printf(RED BOLD "[!] " RESET "Out of range. Please choose 0 to 4.\n");
        }
        switch (choice)
        {
        case 0:
            updateName(&event);
            break;
        case 1:
            updateDescription(&event);
            break;
        case 2:
            updateLocation(&event);
            break;
        case 3:
            updateStartDate(&event);
            break;
        case 4:
            updateEndDate(&event);
            break;
        default:
            break;
        }
        saveEventAt(index, &event);
    }
}
void printTable(int len)
{

    printf("+");
    for (int i = 0; i < len + 2; i++)
        printf("-");
    printf("+--------------+\n");
}
void deleteEvent()
{

    char id[ID_LENGTH];
    int index;
    while (1)
    {
        displayAllEvent(-1);
        do
        {
            printf(BOLD "Enter event's id you want to delete (enter to exit): " RESET);
            inputString(id, sizeof(id));
            if (id[0] == '\0')
                return;

            index = findEventIndexById(id);
            if (index == -1)
            {
                printf(ORANGE "cannot find event with id %s, please try again!!\n" RESET, id);
            }
        } while (index == -1);
        Event event;

        loadEventAt(index, &event);

        if (event.status == STATUS_ONGOING)
        {
            printf(ORANGE "You cannot delete an ongoing event!\n" RESET);
            return;
        }
        int length = strlen(event.name);
        char name[NAME_LENGTH];
        strcpy(name, event.name);
        printf(BLUE "=============== Information ===============\n" RESET);
        printTable(length);
        printf("| %-*s | %-12s |\n", length, "Name", "Staff count");
        printTable(length);
        printf("| %-*s | %-12d |\n", length, event.name, event.staffCount);
        printTable(length);
        if (confirmAction("Do you want to delete this event ?"))
        {
            if (confirmAction(RED "Are you sure? This event will be permanently deleted" RESET))
            {
                deleteEventById(event.eventId);
                printf(GREEN BOLD "[SUCCESS] " RESET "The event " BOLD "%s" RESET " has been successfully deleted\n", name);
            }
        }
    }
}

void viewEventDetails()
{
    char eventId[EVENT_ID_LENGTH];
    printf(BOLD "ENTER EVENT ID TO VIEW: " RESET);
    inputString(eventId, sizeof(eventId));

    Event event;
    int eventIndex = findEventIndexById(eventId);

    if (loadEventAt(eventIndex, &event))
    {
        printDivider("EVENT DETAILS");
        printf(BOLD "  Event ID     : " RESET YELLOW "%s" RESET "\n", event.eventId);
        printf(BOLD "  Name         : " RESET CYAN "%s" RESET "\n", event.name);
        printf(BOLD "  Location     : " RESET "%s" RESET "\n", event.location);
        
        char *stStr[] = {"Upcoming", "Ongoing", "Finished"};
        char *stColors[] = {YELLOW, GREEN, RED};
        printf(BOLD "  Status       : " RESET "%s%s" RESET "\n", stColors[event.status], stStr[event.status]);
        
        printf(BOLD "  Time         : " RESET GREEN "%s" RESET " to " RED "%s" RESET "\n", event.startDate, event.endDate);
        printf(BOLD "  Description  : " RESET "%s" RESET "\n", event.description);

        printf("\n" YELLOW BOLD "--- ASSIGNED STAFF LIST ---\n" RESET);
        char *sLine = CYAN "+--------------+----------------------+------------+--------------------------------+\n" RESET;
        char *sHead = CYAN "| " BOLD "%-12s" RESET CYAN " | " BOLD "%-20s" RESET CYAN " | " BOLD "%-10s" RESET CYAN " | " BOLD "%-30s" RESET CYAN " |\n" RESET;
        char *sRow = CYAN "|" RESET " %-12s " CYAN "|" RESET " %-20.20s " CYAN "|" RESET " %s%-10s" RESET CYAN " | " RESET "%-30.30s " CYAN "|\n" RESET;
        char *roleNames[] = {"Leader", "Member", "Support"};
        char *roleColors[] = {CYAN, GREEN, YELLOW};

        printf("%s", sLine);
        printf(sHead, "MSSV", "Full Name", "Role", "Mission");
        printf("%s", sLine);

        for (int i = 0; i < event.staffCount; i++)
        {
            User person;
            if (findUserById(event.staffList[i].studentId, &person))
            {
                printf(sRow,
                       event.staffList[i].studentId,
                       person.studentName,
                       roleColors[event.staffList[i].role],
                       roleNames[event.staffList[i].role],
                       event.staffList[i].description);
            }
        }
        printf("%s", sLine);
    }
    else
    {
        printf(RED BOLD "[ERROR] " RESET "Event not found!\n");
    }
}

void manualUpdateEventStatus()
{
    char eventId[EVENT_ID_LENGTH];
    printf(BOLD "ENTER EVENT ID TO UPDATE STATUS: " RESET);
    inputString(eventId, sizeof(eventId));

    int idx = findEventIndexById(eventId);
    if (idx == -1)
    {
        printf(RED BOLD "[ERROR] " RESET "Event not found!\n");
        return;
    }

    Event e;
    loadEventAt(idx, &e);

    char *stNames[] = {"Upcoming", "Ongoing", "Finished"};
    printf("Current status: " YELLOW BOLD "%s\n" RESET, stNames[e.status]);

    if (e.status == STATUS_FINISHED)
    {
        printf(YELLOW BOLD "[INFO] " RESET "Event is already Finished. No further changes allowed.\n");
        return;
    }

    printf(YELLOW BOLD "Change status to:\n" RESET);
    if (e.status == STATUS_UPCOMING)
    {
        printf("  1. Ongoing\n  2. Finished\n");
    }
    else if (e.status == STATUS_ONGOING)
    {
        printf("  2. Finished\n");
    }
    printf(BOLD "  0. Cancel\nChoice: " RESET);

    int choice;
    if (scanf("%d", &choice) != 1)
    {
        // Preventing infinite loop when EOF
        if (feof(stdin)) return;
        clearInputBuffer();
        return;
    }
    getchar();

    if (choice == 0)
        return;

    EventStatus targetStatus = (choice == 1) ? STATUS_ONGOING : STATUS_FINISHED;

    if (targetStatus <= e.status)
    {
        printf(RED BOLD "[ERROR] " RESET "Rule violation: Status can only move forward!\n");
        return;
    }

    if (targetStatus == STATUS_ONGOING && e.staffCount == 0)
    {
        printf(RED BOLD "[WARNING] " RESET "This event has NO staff members assigned yet!\n");
        if (!confirmAction("Are you sure you want to start this event?"))
        {
            return;
        }
    }

    if (confirmAction("Confirm status update?"))
    {
        e.status = targetStatus;
        if (saveEventAt(idx, &e))
        {
            printf(GREEN "[SUCCESS]" RESET " Event status updated to %s!\n", stNames[targetStatus]);
        }
    }
}
void searchEventsByStartDateRange()
{
    char fromDate[DATE_LENGTH], toDate[DATE_LENGTH];
    char *statusNames[] = {"Upcoming", "Ongoing", "Finished"};
    char *statusColors[] = {YELLOW, GREEN, RED};
    char *line = CYAN "+------------+---------------------------+-------------+-------------+-----------------+-------+------------+\n" RESET;
    char *headerFmt = CYAN "| " BOLD "%-10s" RESET CYAN " | " BOLD "%-25s" RESET CYAN " | " BOLD "%-11s" RESET CYAN " | " BOLD "%-11s" RESET CYAN " | " BOLD "%-15s" RESET CYAN " | " BOLD "%-5s" RESET CYAN " | " BOLD "%-10s" RESET CYAN " |\n" RESET;
    char *rowFmt = CYAN "|" RESET " %-10s " CYAN "|" RESET " %-25.25s " CYAN "|" RESET " %-11s " CYAN "|" RESET " %-11s " CYAN "|" RESET " %-15.15s " CYAN "|" RESET " %-5d " CYAN "|" RESET " %s%-10s" RESET CYAN " |\n" RESET;
    do
    {
        do
        {
            printf(BOLD "Enter from date (YYYY-MM-DD): " RESET);
            inputString(fromDate,sizeof(fromDate));
            // Preventing infinite loop when EOF
        if (feof(stdin)) return;
        } while (!isValidDate(fromDate));
        do
        {
            printf(BOLD "Enter to date (YYYY-MM-DD): " RESET);
            inputString(toDate,sizeof(toDate));
            // Preventing infinite loop when EOF
        if (feof(stdin)) return;
        } while (!isValidDate(toDate));
        
    } while (!isChronological(fromDate, toDate));
    int filterStatus = inputEventStatus();
    int total = getNextEventIndex();
    if (total <= 0){
        printf(YELLOW BOLD "[INFO] " RESET "No events found.\n");
        getchar();
        return;
    }
    MatchedEvent *arr = (MatchedEvent *)calloc((size_t)total, sizeof(MatchedEvent));
    if (!arr) {
        printf(RED BOLD "[ERROR] " RESET "Out of memory.\n");
        return;
    }

    int n = 0;
    Event e;
    for (int i = 0; i < total; i++)
    {
        if (loadEventAt(i, &e))
        {
            arr[n].event = e;
            arr[n].studentRole = STAFF_MEMBER; /* placeholder */
            n++;
        }
    }
    if (n == 0) {
        free(arr);
        printf(YELLOW BOLD "[INFO] " RESET "No readable events.\n");
        return;
    }

    if (n > 1) quicksortByDate(arr, 0, n - 1); //reuse the quicksort function to sort events by start date in descending order

    int L = firstIdxLE_ToDateDesc(arr, n, toDate);
    int R = firstIdxLT_FromDateDesc(arr, n, fromDate);
    printDivider("EVENTS IN DATE RANGE (BINARY SEARCH + STATUS)");
    printf("%s", line);
    printf(headerFmt, "ID", "Event Name", "Start", "End", "Location", "Staff", "Status");
    printf("%s", line);
    int count = 0;
    for (int i = L; i < R; i++)
    {
        Event *ev = &arr[i].event; /* matchedEvent -> event */

        if (filterStatus != -1 && (int)ev->status != filterStatus) continue;

        const char *st = "Unknown";
        const char *stColor = RESET;
        if ((int)ev->status >= STATUS_UPCOMING && (int)ev->status <= STATUS_FINISHED) {
            st = statusNames[ev->status];
            stColor = statusColors[ev->status];
        }

        printf(rowFmt, ev->eventId, ev->name, ev->startDate, ev->endDate, ev->location, ev->staffCount, stColor, st);
        count++;
    }

    printf("%s", line);
    if (count == 0) printf(YELLOW BOLD "[INFO] " RESET "No events found with current filters.\n");
    else printf(CYAN BOLD "Total: %d event(s) found.\n" RESET, count);

    free(arr);
}
