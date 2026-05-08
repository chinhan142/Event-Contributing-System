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
// static int firstIdxLE_ToDateDesc(const MatchedEvent *arr, int n, const char *toDate){
//     int l = 0, r = n;
//     while(l < r){
//         int m = l + (r - l)/2;
//         if (strcmp(arr[m].event.startDate, toDate) > 0)
//             l = m + 1;        
//         else
//             r = m;       
//     }
//     return l;
// }
/* first index has date < fromDate */
// static int firstIdxLT_FromDateDesc(const MatchedEvent *arr, int n, const char *fromDate){
//     int l = 0, r = n;
//     while(l < r){
//         int m = l + (r - l)/2;
//         if (strcmp(arr[m].event.startDate, fromDate) >= 0)
//             l = m + 1;
//         else
//             r = m;
//     }
//     return l;
// }

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
time_t toTimestamp(Event *event){
    int Y = stoi(event->startDate, 0, 3);
    int M = stoi(event->startDate, 5, 6);
    int D = stoi(event->startDate, 8, 9);
    struct tm s = {0};
    s.tm_year = Y - 1900;
    s.tm_mon = M - 1;
    s.tm_mday = D;
    s.tm_isdst = -1;
    return mktime(&s);
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

int checkSemester(time_t timestampCheck) {
    struct tm *t = localtime(&timestampCheck);
    int month = t->tm_mon + 1; // Tháng từ 1-12


    if (month >= 1 && month <= 4) return 1; // Spring


    if (month >= 5 && month <= 8) return 2; // Summer


    return 3; // Fall
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
        printf(GREEN BOLD "[SUCCESS] " RESET "Event created successfully with ID: %s\n", newEvent.eventId);
    }
    else
    {
        printf("\033[1;31m[ERROR] Could not save event data!\033[0m (enter to continue)");
        getchar();
    }
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

        int res = scanf("%d", &choice);
        clearInputBuffer();
        if (res != 1)
        {
            printf(RED BOLD "[!] " RESET "Invalid input. Please enter a number.\n");
            continue;
        }

        if (choice >= 0 && choice <= 3)
        {
            return choice - 1;
        }
        printf("[!] Out of range. Please choose 0 to 3.\n");
    }
}

void displayAllEvent(int filterStatus)
{
    char *line = CYAN "+------------+---------------------------+-------------+-------------+-----------------+-------+------------+\n" RESET;
    char *headerFmt = CYAN "| " BOLD "%-10s" RESET CYAN " | " BOLD "%-25s" RESET CYAN " | " BOLD "%-11s" RESET CYAN " | " BOLD "%-11s" RESET CYAN " | " BOLD "%-15s" RESET CYAN " | " BOLD "%-5s" RESET CYAN " | " BOLD "%-10s" RESET CYAN " |\n" RESET;
    char *rowFmt = CYAN "|" RESET " %-10s " CYAN "|" RESET " %-25.25s " CYAN "|" RESET " %-11s " CYAN "|" RESET " %-11s " CYAN "|" RESET " %-15.15s " CYAN "|" RESET " %-5d " CYAN "|" RESET " %s%-10s" RESET CYAN " |\n" RESET;
    char *statusNames[] = {"Upcoming", "Ongoing", "Finished"};
    char *statusColors[] = {YELLOW, GREEN, BLUE};

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
        printf(CYAN BOLD " Total: " RESET "%d event(s) listed.\n", count);
    }
}

// Search event with user input

void printEventResult()
{
    
    char inputID[EVENT_ID_LENGTH];
    printf("Enter Event ID to search (or press Enter to skip): ");
    inputString(inputID, sizeof(inputID));
    if (inputID[0] == '\0')
    {
        printf("[INFO] Search cancelled.\n");
        return;
    }
    toUpperStr(inputID, inputID);
    int index = findEventIndexById(inputID);
    if (index == -1)
    {
        printf("Event not found.\n");
        return;
    }

    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (f == NULL)
    {
        printf("No events found.\n");
        printf("Press Enter to continue");
        getchar();
        return;
    }

    Event temp;
    
    
    printf("\n");
    printDivider("SEARCH RESULTS");
    printf("%-12s | %-25s | %-12s | %-12s | %-20s | %s\n", "Event ID", "Name", "Status", "Date", "User Name", "Role");
    printf("=========================================================================================================\n");
    
    if (loadEventWithFile(f, index, &temp))
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
    printf(GREEN "Please enter new name: " RESET);
    inputString(newName, sizeof(newName));

    if (confirmAction("\033[31mAre you sure you want to change the name ?\033[0m"))
    {
        strcpy(event->name, newName);
        printf(GREEN BOLD "[SUCCESS]" RESET);
        printf(" Name changed successfully\n");
    }
}
void updateDescription(Event *event)
{
    char newDescription[DESC_LENGTH];
    printf(GREEN "Please enter new description: "  RESET);
    inputString(newDescription, sizeof(newDescription));

    if (confirmAction("\033[31mAre you sure you want to change the description ?\033[0m"))
    {
        strcpy(event->description, newDescription);
        printf(GREEN BOLD "[SUCCESS]" RESET);
        printf(" Description changed successfully\n");
    }
}

void updateLocation(Event *event)
{
    char newLocation[DESC_LENGTH];
    printf(GREEN "Please enter new location: " RESET);
    inputString(newLocation, sizeof(newLocation));

    if (confirmAction("\033[31mAre you sure you want to change the description ?\033[0m"))
    {
        strcpy(event->location, newLocation);
        printf(GREEN BOLD "[SUCCESS]" RESET);
        printf(" Location changed successfully\n");
    }
}
void updateStartDate(Event *event)
{
    char newStartDate[DESC_LENGTH];
    printf(GREEN "Please enter new start date (YYYY-MM-DD): " RESET);
    do
    {
        inputString(newStartDate, sizeof(newStartDate));
    } while (!isValidDate(newStartDate) || !isChronological(newStartDate, event->endDate));
    Event temp = *event;
    strcpy(temp.startDate, newStartDate);
    updateStatus(&temp);
    char message[1000];
    if (event->status > temp.status)
    {
        printf(RED "Date change failed: This would move the event back from 'Ongoing' to 'Upcoming'! Enter to continue " RESET);
        getchar();
        return;
    }
    if (temp.status != event->status)
    {
        strcpy(message, YELLOW "This action will change the status. Are you sure you want to proceed?" RESET);
    }
    else
        strcpy(message, "\033[0mAre you sure you want to change the start date ?\033[0m");

    if (confirmAction(message))
    {
        strcpy(event->startDate, newStartDate);
        updateStatus(event);
        printf(GREEN BOLD "[SUCCESS]" RESET);
        printf(" Start date changed successfully\n");
    }
}

void updateEndDate(Event *event)
{
    char newEndDate[DESC_LENGTH];
    do
    {
        printf(GREEN "Please enter new end date (YYYY-MM-DD): " RESET);
        inputString(newEndDate, sizeof(newEndDate));
    } while (!isValidDate(newEndDate) || !isChronological(event->startDate, newEndDate));
    Event temp = *event;
    strcpy(temp.endDate, newEndDate);
    updateStatus(&temp);
    char message[1000];
    if (event->status > temp.status)
    {
        printf(RED "Date change failed: This would move the event back from 'Ongoing' to 'Upcoming'! Enter to continue " RESET);
        getchar();
        return;
    }
    if (temp.status != event->status)
    {
        strcpy(message, YELLOW "This action will change the status. Are you sure you want to proceed?" RESET);
    }
    else
        strcpy(message, "\033[31mAre you sure you want to change the end date ?\033[0m");

    if (confirmAction(message))
    {
        strcpy(event->endDate, newEndDate);
        updateStatus(event);
        printf(GREEN BOLD "[SUCCESS]" RESET);
        printf(" End date changed successfully\n");
    }
}
void updateEventDetails()
{

    char eventID[ID_LENGTH];
    int index;
    Event event;
    while (1)
    {
        do
        {
            printf("Enter the ID of the event you want to update (enter to exit): ");
            inputString(eventID, sizeof(eventID));
            if (eventID[0] == '\0')
                return;
            index = findEventIndexById(eventID);
            if (index == -1)
            {
                printf(RED BOLD "[ERROR] " RESET "ID not found, please try again!\n");
            }
        } while (index == -1);

        loadEventAt(index, &event);
        if (event.status == 2)
        {
            printf(RED "You cannot edit a finished event. Enter to continue" RESET);
            getchar();
            continue;
        }
        else if (event.status == 1)
        {
            if (!confirmAction("\033[31mThis event is ongoing. Are you sure you want to edit it?\033[0m"))
            {
                continue;
            }
        }
        printf(YELLOW BOLD "\n===== CHOOSE ATTRIBUTE TO UPDATE =====\n" RESET);
        printf(GREEN "0." RESET " Event's Name\n");
        printf(GREEN "1." RESET " Event's Description\n");
        printf(GREEN "2." RESET " Event's Location\n");
        printf(GREEN "3." RESET " Event's Start Date\n");
        printf(GREEN "4." RESET " Event's End Date\n");
        printf(BOLD "Enter your choice (0-4): " RESET);
        int choice;
        while (1)
        {

            // If user input something is not integer, these line helps clearing the buffer and continue the loop, avoiding wrong logic
            if (scanf("%d", &choice) != 1)
            {
                clearInputBuffer();
                printf(RED BOLD "[!] " RESET "Invalid input. Please enter a number.\n");
                continue;
            }

            if (choice >= 0 && choice <= 4)
            {
                // remapping choice
                clearInputBuffer();
                break;
            }
            printf("[!] Out of range. Please choose 0 to 4.\n");
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
    printf(CYAN "+");
    for (int i = 0; i < len + 2; i++)
        printf("-");
    printf("+--------------+\n" RESET);
}
void deleteEvent()
{

    char id[ID_LENGTH];
    int index;
    while (1)
    {
        do
        {
            printf("Enter event's id you want to delete (enter to exit): ");
            inputString(id, sizeof(id));
            if (id[0] == '\0')
                return;

            index = findEventIndexById(id);
            if (index == -1)
            {
                printf(RED BOLD "[ERROR] " RESET "Cannot find event with id %s, please try again!\n", id);
            }
        } while (index == -1);
        Event event;

        loadEventAt(index, &event);

        if (event.status == STATUS_ONGOING)
        {
            printf(RED BOLD "[ERROR] " RESET "You cannot delete an ongoing event!\n");
            return;
        }
        char *statusNames[] = {"Upcoming", "Ongoing", "Finished"};
        char *statusColors[] = {YELLOW, GREEN, BLUE};

        printDivider("CONFIRM DELETION");
        printf(BOLD "  Event Name   : " RESET CYAN "%-30s" RESET "\n", event.name);
        printf(BOLD "  Staff Count  : " RESET YELLOW "%-15d" RESET "\n", event.staffCount);
        printf(BOLD "  Status       : " RESET "%s%-15s" RESET "\n\n", statusColors[event.status], statusNames[event.status]);
        
        if (confirmAction("Do you want to delete this event?"))
        {
            if (confirmAction(RED "Are you sure? This event will be permanently deleted" RESET))
            {
                deleteEventById(event.eventId);
                printf(GREEN BOLD "[SUCCESS] " RESET "The event " BOLD "%s" RESET " has been successfully deleted!\n", event.name);
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
        char *stColors[] = {YELLOW, GREEN, BLUE};
        printf(BOLD "  Status       : " RESET "%s%s" RESET "\n", stColors[event.status], stStr[event.status]);
        
        printf(BOLD "  Time         : " RESET GREEN "%s" RESET " to " RED "%s" RESET "\n", event.startDate, event.endDate);
        printf(BOLD "  Description  : " RESET "%s" RESET "\n", event.description);

        printf("\n" YELLOW BOLD "--- ASSIGNED STAFF LIST ---\n" RESET);
        char *sLine = CYAN "+--------------+----------------------+------------+--------------------------------+\n" RESET;
        char *sHead = CYAN "| " BOLD "%-12s" RESET CYAN " | " BOLD "%-20s" RESET CYAN " | " BOLD "%-10s" RESET CYAN " | " BOLD "%-30s" RESET CYAN " |\n" RESET;
        char *sRow = CYAN "|" RESET " %-12s " CYAN "|" RESET " %-20.20s " CYAN "|" RESET " %s%-10s" RESET CYAN " | " RESET "%-30.30s " CYAN "|\n" RESET;
        char *roleNames[] = {"Leader", "Member", "Support"};
    char *roleColors[] = {RED, GREEN, CYAN};

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
    printf("ENTER EVENT ID TO UPDATE STATUS: ");
    inputString(eventId, sizeof(eventId));

    int idx = findEventIndexById(eventId);
    if (idx == -1)
    {
        printf("\033[31m[ERROR] Event not found!\033[0m\n");
        return;
    }

    Event e;
    loadEventAt(idx, &e);

    char *stNames[] = {"Upcoming", "Ongoing", "Finished"};
    printf(BOLD "  Current Status : " RESET YELLOW BOLD "%s\n" RESET, stNames[e.status]);

    if (e.status == STATUS_FINISHED)
    {
        printf("\033[33m[INFO] Event is already Finished. No further changes allowed.\033[0m\n");
        return;
    }

    printf("Change status to:\n");
    if (e.status == STATUS_UPCOMING)
    {
        printf(GREEN "  1." RESET " Ongoing\n" GREEN "  2." RESET " Finished\n");
    }
    else if (e.status == STATUS_ONGOING)
    {
        printf(GREEN "  2." RESET " Finished\n");
    }
    printf(RED BOLD "  0." RESET BOLD " Cancel\n" BOLD "Your Selection >> " RESET);

    int choice;
    if (scanf("%d", &choice) != 1)
    {
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    if (choice == 0)
        return;

    EventStatus targetStatus = (choice == 1) ? STATUS_ONGOING : STATUS_FINISHED;

    if (targetStatus <= e.status)
    {
        printf("\033[31m[ERROR] Rule violation: Status can only move forward!\033[0m\n");
        return;
    }

    if (targetStatus == STATUS_ONGOING && e.staffCount == 0)
    {
        printf("\033[1;31m[WARNING] This event has NO staff members assigned yet!\033[0m\n");
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
            printf("\033[32m[SUCCESS] Event status updated to %s!\033[0m\n", stNames[targetStatus]);
        }
    }
}
void searchEventsByStartDateRange()
{
    char fromDate[DATE_LENGTH], toDate[DATE_LENGTH];
    char *statusNames[] = {"Upcoming", "Ongoing", "Finished"};
    char *statusColors[] = {YELLOW, GREEN, BLUE};
    char *line = CYAN "+------------+---------------------------+-------------+-------------+-----------------+-------+------------+\n" RESET;
    char *headerFmt = CYAN "| " BOLD "%-10s" RESET CYAN " | " BOLD "%-25s" RESET CYAN " | " BOLD "%-11s" RESET CYAN " | " BOLD "%-11s" RESET CYAN " | " BOLD "%-15s" RESET CYAN " | " BOLD "%-5s" RESET CYAN " | " BOLD "%-10s" RESET CYAN " |\n" RESET;
    char *rowFmt = CYAN "|" RESET " %-10s " CYAN "|" RESET " %-25.25s " CYAN "|" RESET " %-11s " CYAN "|" RESET " %-11s " CYAN "|" RESET " %-15.15s " CYAN "|" RESET " %-5d " CYAN "|" RESET " %s%-10s" RESET CYAN " |\n" RESET;
    do
    {
        do
        {
            printf(BOLD "Enter from date (YYYY-MM-DD): " RESET);
            inputString(fromDate,sizeof(fromDate));
        } while (!isValidDate(fromDate));
        do
        {
            printf(BOLD "Enter to date (YYYY-MM-DD): " RESET);
            inputString(toDate,sizeof(toDate));
        } while (!isValidDate(toDate));
        
    } while (!isChronological(fromDate, toDate));
    int filterStatus = inputEventStatus();
    int total = getNextEventIndex();
    if (total <= 0){
        printf(YELLOW BOLD "[INFO] " RESET "No events found.\n");
        return;
    }
    MatchedEvent *arr = (MatchedEvent *)calloc((size_t)total, sizeof(MatchedEvent));
    if (!arr) {
        printf(RED BOLD "[ERROR] " RESET "Out of memory.\n");
        return;
    }

    int n = 0;
    Event e;

    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (f) {
        while (fread(&e, sizeof(Event), 1, f) == 1) {
            if (filterStatus != -1 && (int)e.status != filterStatus) {
                continue; 
            }

            if (strcmp(e.startDate, fromDate) >= 0 && strcmp(e.startDate, toDate) <= 0) {
                arr[n].event = e;
                arr[n].studentRole = STAFF_MEMBER;
                n++; // Chỉ đưa vào mảng những event đúng khoảng thời gian
            }
        }
        fclose(f);

    } else {
        printf(RED BOLD "[ERROR] " RESET "Cannot open file.\n");
        free(arr);
        return;
    }

    if (n == 0) {
        free(arr);
        printf(YELLOW BOLD "[INFO] " RESET "No events found with current filters.\n");
        return;
    }

    if (n > 1) {
            quicksortByDate(arr, 0, n - 1); 
        }

printDivider("EVENTS IN DATE RANGE");
    printf("%s", line);
    printf(headerFmt, "ID", "Event Name", "Start", "End", "Location", "Staff", "Status");
    printf("%s", line);
    
    for (int i = 0; i < n; i++)
    {
        Event *ev = &arr[i].event;
        const char *st = "Unknown";
        const char *stColor = RESET;
        if ((int)ev->status >= STATUS_UPCOMING && (int)ev->status <= STATUS_FINISHED) {
            st = statusNames[ev->status];
            stColor = statusColors[ev->status];
        }
        printf(rowFmt, ev->eventId, ev->name, ev->startDate, ev->endDate, ev->location, ev->staffCount, stColor, st);
    }

    printf("%s", line);
    printf(CYAN BOLD "Total: %d event(s) found.\n" RESET, n);

    free(arr);
}
        

void printEventByName()
{
    char name[NAME_LENGTH];
    printf("Enter event name to search (or press Enter to skip): ");
    inputString(name, sizeof(name));
    if (name[0] == '\0')
    {
        printf("[INFO] Search cancelled.\n");
        return;
    }
    // transfer input to uppercase for case-insensitive search
    toUpperStr(name, name);

    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (!f) {
        printf("[ERROR] Cannot open file.\n");
        return;
    }

    printDivider("SEARCH RESULTS BY NAME");
    char *line = "+------------+---------------------------+-------------+-------------+-----------------+-------+------------+\n";
    char *headerFmt = "| %-10s | %-25s | %-11s | %-11s | %-15s | %-5s | %-10s |\n";
    char *rowFmt = "| %-10s | %-25.25s | %-11s | %-11s | %-15.15s | %-5d | %-10s |\n";
    char *statusNames[] = {"Upcoming", "Ongoing", "Finished"};

    printf("%s", line);
    printf(headerFmt, "ID", "Event Name", "Start", "End", "Location", "Staff", "Status");
    printf("%s", line);

    int count = 0;
    Event ev;

   
    while (fread(&ev, sizeof(Event), 1, f) == 1)
    {
        char evNameUpper[NAME_LENGTH];
        strcpy(evNameUpper, ev.name);
        toUpperStr(evNameUpper, evNameUpper);

        if (strstr(evNameUpper, name) != NULL)
        {
            const char *st = "Unknown";
            if ((int)ev.status >= STATUS_UPCOMING && (int)ev.status <= STATUS_FINISHED) {
                st = statusNames[ev.status];
            }

            printf(rowFmt, ev.eventId, ev.name, ev.startDate, ev.endDate, ev.location, ev.staffCount, st);
            count++;
        }
    }
    
    fclose(f); 

    printf("%s", line);
    if (count == 0) {
        printf("[!] No events found with current filters.\n");
    } else {
        printf("Total: %d event(s) found.\n", count);
    }
}
