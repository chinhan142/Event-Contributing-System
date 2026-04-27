#include "event.h"
#include "colors.h"
#include "utils.h"
#include "fileio.h"
#include "user.h"
#include "auth.h"
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
        printf("\033[1;32m[SUCCESS] Event created successfully with ID: %s\033[0m (enter to continue) ", newEvent.eventId);
        getchar();
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
            getchar();
            return choice - 1;
        }
        printf("[!] Out of range. Please choose 0 to 3.\n");
    }
}

void displayAllEvent(int filterStatus)
{
    char *line = "+------------+---------------------------+-------------+-------------+-----------------+-------+------------+\n";
    char *headerFmt = "| %-10s | %-25s | %-11s | %-11s | %-15s | %-5s | %-10s |\n";
    char *rowFmt = "| %-10s | %-25.25s | %-11s | %-11s | %-15.15s | %-5d | %-10s |\n";
    char *statusNames[] = {"Upcoming", "Ongoing", "Finished"};

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
                       statusNames[event.status]);
                count++;
            }
        }
    }

    printf("%s", line);

    if (count == 0)
    {
        printf(" [!] No events found matching your criteria.\n");
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
        return;
    }

    Event temp;
    
    
    printf("\n");
    printDivider("SEARCH RESULTS");
    printf("%-12s | %-25s | %-12s | %-12s | %-20s | %s\n", "Event ID", "Name", "Status", "Date", "User Name", "Role");
    printf("=========================================================================================================\n");
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
        int foundUser = findUserById(temp.staffList[0].studentId, &currentUser);
        if (foundUser) {
            strcpy(userName, currentUser.studentName);

            switch (temp.staffList[0].role) { 
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
    printf(GREEN "Please enter new description: ");
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
    printf(GREEN "Please enter new location: ");
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
    printf(GREEN "Please enter new start date (YYYY-MM-DD): ");
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
        strcpy(message, "\033[38;2;255;165;0mThis action will change the status. Are you sure you want to proceed?\033[0m");
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
        printf(GREEN "Please enter new end date (YYYY-MM-DD): ");
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
        strcpy(message, "\033[38;2;255;165;0mThis action will change the status. Are you sure you want to proceed?\033[0m");
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
        displayAllEvent(-1);
        do
        {
            printf("Enter the ID of the event you want to update (enter to exit): ");
            inputString(eventID, sizeof(eventID));
            if (eventID[0] == '\0')
                return;
            index = findEventIndexById(eventID);
            if (index == -1)
            {
                printf("\033[38;2;255;165;0mID not found, please try again!\n\033[0m");
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
        printf("===== Choose attribute =====\n");
        printf(GREEN "0: Event's name\n1: Event's description\n2: Event's location\n3: Event's start date\n4: Event's end date\n" RESET);
        int choice;
        while (1)
        {

            // If user input something is not integer, these line helps clearing the buffer and continue the loop, avoiding wrong logic
            if (scanf("%d", &choice) != 1)
            {
                while (getchar() != '\n')
                    ;
                printf("[!] Invalid input. Please enter a number.\n");
                continue;
            }

            if (choice >= 0 && choice <= 4)
            {
                // remapping choice
                getchar();
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
            printf("Enter event's id you want to delete (enter to exit): ");
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
            printf(ORANGE "You cannot delete an ongoing event\033[0m! (Enter to continue)");
            getchar();
            continue;
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
                printf(GREEN "[SUCCESS]" RESET " The event \033[1m%s\033[0m has been successfully deleted (enter to continue)", name);
                getchar();
            }
        }
    }
}

void viewEventDetails()
{
    char eventId[EVENT_ID_LENGTH];
    printf("ENTER EVENT ID TO VIEW: ");
    inputString(eventId, sizeof(eventId));

    Event event;
    int eventIndex = findEventIndexById(eventId);

    if (loadEventAt(eventIndex, &event))
    {
        printDivider("EVENT DETAILS");
        char *stStr[] = {"Upcoming", "Ongoing", "Finished"};
        printf("ID          : %s\n", event.eventId);
        printf("Name        : %s\n", event.name);
        printf("Location    : %s\n", event.location);
        printf("Status      : %s\n", stStr[event.status]);
        printf("Time        : %s to %s\n", event.startDate, event.endDate);
        printf("Description : %s\n", event.description);

        printf("\n--- ASSIGNED STAFF LIST ---\n");
        char *sLine = "+--------------+----------------------+------------+--------------------------------+\n";
        char *sHead = "| %-12s | %-20s | %-10s | %-30s |\n";
        char *sRow = "| %-12s | %-20.20s | %-10s | %-30.30s |\n";
        char *roleNames[] = {"Leader", "Member", "Support"};

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
                       roleNames[event.staffList[i].role],
                       event.staffList[i].description);
            }
        }
        printf("%s", sLine);
    }
    else
    {
        printf("\033[31m[ERROR] Event not found!\033[0m\n");
    }
    printf("\nPress Enter to continue...");
    getchar();
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
    printf("Current status: \033[1;33m%s\033[0m\n", stNames[e.status]);

    if (e.status == STATUS_FINISHED)
    {
        printf("\033[33m[INFO] Event is already Finished. No further changes allowed.\033[0m\n");
        return;
    }

    printf("Change status to:\n");
    if (e.status == STATUS_UPCOMING)
    {
        printf("  1. Ongoing\n  2. Finished\n");
    }
    else if (e.status == STATUS_ONGOING)
    {
        printf("  2. Finished\n");
    }
    printf("  0. Cancel\nChoice: ");

    int choice;
    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n')
            ;
        return;
    }
    getchar();

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