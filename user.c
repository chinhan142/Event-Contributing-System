#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "staff.h"
#include "auth.h"  
#include "event.h"  
#include "fileio.h" 
#include "utils.h"  
#define CHUNK_SIZE 1000
#include "paths.h"
#include "colors.h"

void printUserFinishedList(MatchedEvent *list, int count, const char *studentId)  // helper function to print the list of finished events for a user in a formatted table. It takes in a list of MatchedEvent, the count of events in the list, and the studentId to display the user's name in the table. The function will print the event ID, event name, student name, role, and start date for each finished event in the list.
{
    const char *studentName = "Unknown";
    User userData;

    if (studentId != NULL && studentId[0] != '\0' && findUserById(studentId, &userData))
    {
        studentName = userData.studentName;
    }

    printf("\n" YELLOW BOLD "--- FINISHED EVENTS HISTORY ---\n" RESET);
    printf(CYAN "+------------+--------------------------------+---------------------------+--------------+\n" RESET);
    printf(CYAN "| " BOLD "%-10s" RESET CYAN " | " BOLD "%-30s" RESET CYAN " | " BOLD "%-25s" RESET CYAN " | " BOLD "%-12s" RESET CYAN " |\n" RESET, "Event ID", "Event Name", "Student Name", "Role");
    printf(CYAN "+------------+--------------------------------+---------------------------+--------------+\n" RESET);
    
    for (int i = 0; i < count; i++)
    {
        const char *roleName;
        const char *roleColor = RESET;
        switch (list[i].studentRole) {
            case STAFF_LEADER: roleName = "Leader"; roleColor = RED; break;
            case STAFF_MEMBER: roleName = "Member"; roleColor = GREEN; break;
            case STAFF_SUPPORT: roleName = "Support"; roleColor = CYAN; break;
            default: roleName = "Unknown"; break;
        }
        printf(CYAN "|" RESET " %-10s " CYAN "|" RESET " %-30.30s " CYAN "|" RESET " %-25.25s " CYAN "|" RESET " %s%-12s" RESET CYAN " |\n" RESET,
               list[i].event.eventId,
               list[i].event.name,
               studentName,
               roleColor, roleName);
    }
    printf(CYAN "+------------+--------------------------------+---------------------------+--------------+\n" RESET);
}

void cleanUserEventData(Event *event)
{
    event->eventId[sizeof(event->eventId) - 1] = '\0';
    event->eventId[strcspn(event->eventId, "\r\n ")] = '\0';
}

void printUserEventRowRole(const Event *event, StaffRole role, const char *studentId) // helper function to print  row of event details with the user's role in that event. It will also print the event status with different colors for better visualization.
{
    const char *roleName;
    const char *roleColor = RESET;
    const char *studentName = "Unknown";
    User userData;
    switch (role)
    {
        case STAFF_LEADER:  roleName = "Leader";  roleColor = RED;    break;
        case STAFF_MEMBER:  roleName = "Member";  roleColor = GREEN;  break;
        case STAFF_SUPPORT: roleName = "Support"; roleColor = CYAN;   break;
        default:            roleName = "Unknown"; break;
    }

    if (studentId != NULL && studentId[0] != '\0' && findUserById(studentId, &userData))
    {
        studentName = userData.studentName;
    }

    printf(CYAN "|" RESET " %-10s " CYAN "|" RESET " %-25.25s " CYAN "|" RESET " %-25.25s " CYAN "|" RESET " %s%-12s" RESET CYAN " |" GREEN " %s\n" RESET,
           event->eventId,
           event->name,
           studentName,
           roleColor, roleName,          
           event->startDate);
}

void processChunk(Event *chunk, size_t eventsRead, const char *studentId, int *foundCount) // helper function to process a chunk of events read from file, check if the student is involved in the event and print out the event details if found. It also updates the found count for the total number of events found for this student.
{
    StaffRole role;
    for (size_t i = 0; i < eventsRead; i++)
    {
        cleanUserEventData(&chunk[i]);

        //if the student is found in the event, print out the event details
        if (findStaffInEvent(&chunk[i], studentId, &role))
        {
            printUserEventRowRole(&chunk[i], role, studentId);
            (*foundCount)++; // increase the found count
        }
    }
}

int findStaffInEventUser(const Event *event, const char *studentId, StaffRole *role) // helper function to check if the student is involved in the event and return the role of the student in that event. It returns 1 if found and 0 if not found.
{
    for (int i = 0; i < event->staffCount; i++)
    {
        if (strcmp(event->staffList[i].studentId, studentId) == 0)
        {
            *role = event->staffList[i].role;
            return 1; // Found
        }
    }
    return 0; // Not found
}

void displayCurrentUserEventHistory(const Account *acc) // display the history of events that the user has participated in, including both ongoing and finished events. The function will first get all events that the user is involved in, then filter out the finished events and sort them by date before printing. If there are no finished events, it will print a message saying that there are no finished events for this student.
{   
    if (acc == NULL) {
        printf(RED BOLD "[ERROR] " RESET "Cannot find event history.\n");
        return;
    }
    int rawFoundCount = 0; //this is the variable to store the number of events that the user is involved in, including both ongoing and finished events. 
    
    MatchedEvent *rawList = getEventsByStudentId(acc->studentId, &rawFoundCount);

    /* *rawList will return a list of events that the user is involved in, including both ongoing and finished events. 
    The function will also set the rawFoundCount variable to the number of events found. */
   
    if (rawList == NULL || rawFoundCount == 0) {
        printf(YELLOW BOLD "[INFO] " RESET "No events found for this student.\n");
        return;
    }
    int finishedCount = 0;
    MatchedEvent *finishedList = (MatchedEvent *)malloc(rawFoundCount * sizeof(MatchedEvent));//allocate memory basse on the number of events found in rawList
    if (finishedList == NULL) {
        printf(RED BOLD "[ERROR] " RESET "Out of memory. Cannot allocate finished list.\n");
        free(rawList);
        return;
    }
    for(int i = 0; i<rawFoundCount;i++){ // loop through the raw list to filter out the finished events and store them in finishedList
        if (rawList[i].event.status == STATUS_FINISHED){
            finishedList[finishedCount] = rawList[i];
            finishedCount++;
        }
    }
    if (finishedCount > 1) {// if there are more than 1 finished events, sort them by date before printing
        quicksortByDate(finishedList, 0, finishedCount - 1);
    }
    if(finishedCount > 0){// if there are finished events, print them out. Otherwise, print a message saying that there are no finished events for this student.
        printUserFinishedList(finishedList, finishedCount, acc->studentId);
    }else{
        printf(YELLOW BOLD "[INFO] " RESET "No finished events found for this student.\n");
    }
    free(rawList); // free the memory allocated for rawList as it is no longer needed
    if(finishedList != NULL){
        free(finishedList);
    }
}

int findUserById(const char *id, User *result)
{
    FILE *f = fopen(USER_DATA_PATH, "rb");
    if (f == NULL)
    {
        return 0;
    }

    User temp;
    while (fread(&temp, sizeof(User), 1, f))
    {
        if (strcmp(temp.studentId, id) == 0)
        {
            *result = temp;
            fclose(f);
            return 1; // Found
        }
    }
    fclose(f);
    return 0; // Not found
}

int findUserIndex(const char *id)
{
    FILE *f = fopen(USER_DATA_PATH, "rb");
    if (f == NULL)
    {
        return 0;
    }

    User temp;
    int index = 0;
    while (fread(&temp, sizeof(User), 1, f))
    {
        if (strcmp(temp.studentId, id) == 0)
        {
            fclose(f);
            return index; 
        }
        index++;
    }
    fclose(f);
    return -1;
} 

// search for user via namne
void searchUserByName(const char *name, User results[MAX_SEARCH_RESULTS], int *count)
{
    FILE *f = fopen(USER_DATA_PATH, "rb");
    if (f == NULL)
    {
        *count = 0;
        return;
    }

    *count = 0;
    User temp;
    char queryLower[NAME_LENGTH];
    toLowerStr(queryLower, name);
    while (fread(&temp, sizeof(User), 1, f) && *count < MAX_SEARCH_RESULTS)
    {
        char nameLower[NAME_LENGTH];
        toLowerStr(nameLower, temp.studentName);
        if (strstr(nameLower, queryLower) != NULL)
        {
            results[*count] = temp;
            (*count)++;
        }
    }
    fclose(f);
}

// search for user via Ids
void searchUserById(const char *id, User results[MAX_SEARCH_RESULTS], int *count)
{
    FILE *f = fopen(USER_DATA_PATH, "rb");
    if (f == NULL)
    {
        *count = 0;
        return;
    }

    *count = 0;
    User temp;
    char queryLower[ID_LENGTH];
    toLowerStr(queryLower, id);
    while (fread(&temp, sizeof(User), 1, f) && *count < MAX_SEARCH_RESULTS)
    {
        char idLower[ID_LENGTH];
        toLowerStr(idLower, temp.studentId);
        if (strcmp(idLower, queryLower) == 0)
        {
            results[*count] = temp;
            (*count)++;
        }
    }
    fclose(f);

}

// look through datas and return the user personal profile, if the user is not found, print error message and return
void viewProfile(const Account *acc){
    User persona;
    int userFound = 0;
    FILE *f = fopen(USER_DATA_PATH, "rb");
    if (f == NULL)
    {
        printf(RED BOLD "[ERROR] " RESET "Cannot open users.dat file\n");
        return;
    }
    while (fread(&persona, sizeof(User), 1, f) == 1)
    {
        if (strcmp(persona.studentId, acc->studentId) == 0) // compare the user Id with the student Id to find profile data, if the id matches, raise found flag to 1 and break or else loop till found or end of file
        {
            userFound = 1;
            break;
        }
    }
    fclose(f);

    if (userFound == 0){
        printf(RED BOLD "[ERROR] " RESET "Cannot find user profile.\n");
        pressEnterToContinue();
        return;
    }

    printf("\n" YELLOW BOLD "================ PERSONAL PROFILE ================\n" RESET);
    printf(BOLD "  %-15s : " RESET YELLOW "%s" RESET "\n", "Student ID", persona.studentId);
    printf(BOLD "  %-15s : " RESET CYAN "%s" RESET "\n", "Full Name", persona.studentName);
    printf(BOLD "  %-15s : " RESET "%s\n", "Email", persona.email);
    printf(BOLD "  %-15s : " RESET "%s\n", "Phone Number", persona.phoneNumber);
    printf(BOLD "  %-15s : " RESET GREEN BOLD "%s" RESET "\n", "Position", (acc->role == 1 ? "BCN" : "Member"));
    printf(BOLD "  %-15s : " RESET MAGENTA "%s" RESET "\n", "Specialize", persona.specialize);
    printf(YELLOW BOLD "==================================================\n" RESET);
}

MatchedEvent* getCurrentEventsForUser(const Account *acc, int *count)
{
    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (f == NULL)
    {
        *count = 0;
        return NULL;
    }

    Event *eventChunk = (Event *)malloc(CHUNK_SIZE * sizeof(Event));
    if (eventChunk == NULL)
    {
        fclose(f);
        *count = 0;
        return NULL;
    }

    int capacity = 10;
    int foundCount = 0;
    MatchedEvent *matchedList = (MatchedEvent *)malloc(capacity * sizeof(MatchedEvent));
    if (matchedList == NULL)
    {
        free(eventChunk);
        fclose(f);
        *count = 0;
        return NULL;
    }

    size_t eventsRead;
    StaffRole role;
    while ((eventsRead = fread(eventChunk, sizeof(Event), CHUNK_SIZE, f)) > 0)
    {
        for (size_t i = 0; i < eventsRead; i++)
        {
            cleanUserEventData(&eventChunk[i]);
            if (findStaffInEvent(&eventChunk[i], acc->studentId, &role) && eventChunk[i].status == STATUS_ONGOING)
            {
                if (foundCount >= capacity)
                {
                    capacity *= 2;
                    matchedList = (MatchedEvent *)realloc(matchedList, capacity * sizeof(MatchedEvent));
                    if (matchedList == NULL)
                    {
                        free(eventChunk);
                        fclose(f);
                        *count = 0;
                        return NULL;
                    }
                }
                matchedList[foundCount].event = eventChunk[i];
                matchedList[foundCount].studentRole = role;
                foundCount++;
            }
        }
    }
    free(eventChunk);
    fclose(f);

    if (foundCount > 1)
    {
        quicksortByDate(matchedList, 0, foundCount - 1);
    }

    *count = foundCount;
    return matchedList;
}

void printCurrentEvents(MatchedEvent *events, int count, const char *studentId)
{
    for (int i = 0; i < count; i++)
    {
        printUserEventRowRole(&events[i].event, events[i].studentRole, studentId);
    }
}

// print out current events for user
void viewCurrentEvents(const Account *acc) {
    int count;
    MatchedEvent *events = getCurrentEventsForUser(acc, &count);
    if (events == NULL && count == 0)
    {
        printf("\n" YELLOW BOLD "[INFO] " RESET "No events available in the system.\n");
        return;
    }
    printDivider("CURRENT ONGOING EVENTS");
    printf(CYAN "| " BOLD "%-10s" RESET CYAN " | " BOLD "%-25s" RESET CYAN " | " BOLD "%-25s" RESET CYAN " | " BOLD "%-12s" RESET CYAN " | " BOLD "%-10s" RESET CYAN " |\n" RESET, "Event ID", "Event Name", "Student Name", "Your Role", "Start Date");
    printf(CYAN "+------------+---------------------------+---------------------------+--------------+------------+\n" RESET);

    if (count > 0)
    {
        printCurrentEvents(events, count, acc->studentId);
    }

    printf(CYAN "+------------+---------------------------+---------------------------+--------------+------------+\n" RESET);
    if (count == 0)
    {
        printf(YELLOW BOLD "[INFO] " RESET "You have no ongoing events.\n");
    }
    else
    {
        printf(CYAN BOLD " Total: " RESET "%d ongoing event(s) found.\n", count);
    }

    if (events != NULL)
    {
        free(events);
    }
}

// Event menu for user to do operations related to their current events
void currentEventsMenu(const Account *acc) { 
    while (1) {
        clearScreen();
        viewCurrentEvents(acc);
        printf("\n" YELLOW BOLD "Options:\n" RESET);
        printf(GREEN "  1." RESET " View Event Details\n");
        printf(GREEN "  2." RESET " Sort Events\n");
        printf(GREEN "  3." RESET " Refresh List\n");
        printf(GREEN "  4." RESET " Back to Main Menu\n");
        printf(BOLD "Enter your choice: " RESET);
        
        char choice[10];
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\r\n")] = '\0'; // Remove newline

        if (strcmp(choice, "1") == 0) {  //compare the input with options and call the corresponding function
            if (acc == NULL) {
                printf(RED BOLD "[ERROR] " RESET "Cannot find event details.\n");
                pressEnterToContinue();
                continue;
            }
            viewUserEventDetails(acc, NULL);
            pressEnterToContinue();
        } else if (strcmp(choice, "2") == 0) {
            printf("How would you like to sort the events?\n");
            printf("1. By Name\n");
            printf("2. By Date\n");
            printf("3. By Event ID\n");
            printf(BOLD "Enter your choice: " RESET);
            
            char sortChoice[10];
            fgets(sortChoice, sizeof(sortChoice), stdin);
            sortChoice[strcspn(sortChoice, "\r\n")] = '\0'; 

            int count;
            MatchedEvent *events = getCurrentEventsForUser(acc, &count);
            if (events != NULL) {
                if (strcmp(sortChoice, "1") == 0) {                            
                    sortUserEventsByName(events, count);
                } else if (strcmp(sortChoice, "2") == 0) {
                    printf("How would you like to sort by Date?\n");
                    printf("1. Ascending\n");  
                    printf("2. Descending\n");
                    printf(BOLD "Enter your choice: " RESET);
                    char dateSortChoice[10];
                    fgets(dateSortChoice, sizeof(dateSortChoice), stdin);
                    dateSortChoice[strcspn(dateSortChoice, "\r\n")] = '\0';
                    if (strcmp(dateSortChoice, "1") == 0) {
                        sortUserEventsByDateAsc(events, count);
                    } else if (strcmp(dateSortChoice, "2") == 0) {
                        sortUserEventsByDate(events, count);
                    } else {
                        printf(RED BOLD "[ERROR] " RESET "Invalid sort choice.\n");
                        free(events);
                        continue;
                    }
                } else if (strcmp(sortChoice, "3") == 0) {
                    printf("How would you like to sort by Event ID?\n");
                    printf("1. Ascending\n");
                    printf("2. Descending\n");
                    printf(BOLD "Enter your choice: " RESET);

                    char idSortChoice[10];
                    fgets(idSortChoice, sizeof(idSortChoice), stdin);
                    idSortChoice[strcspn(idSortChoice, "\r\n")] = '\0';

                    if (strcmp(idSortChoice, "1") == 0) {
                        sortUserEventsByIdAsc(events, count);
                    } else if (strcmp(idSortChoice, "2") == 0) {
                        sortUserEventsByIdDesc(events, count);
                    } else {
                        printf(RED BOLD "[ERROR] " RESET "Invalid sort choice.\n");
                        free(events);
                        continue;
                    }
                } else {
                    printf(RED BOLD "[ERROR] " RESET "Invalid sort choice.\n");
                    free(events);
                    continue;
                }
                printCurrentEvents(events, count, acc->studentId);
                free(events);
                pressEnterToContinue();
            }
        } else if (strcmp(choice, "3") == 0) {
            continue; // Just refresh the list
            clearScreen();
        } else if (strcmp(choice, "4") == 0) {
            break; // Exit to main menu
            clearScreen();
        } else {
            printf(RED BOLD "[ERROR] " RESET "Invalid choice. Please try again.\n");
        }
    }
}

// function to display a list of events and then let user select an event to view details, if the user input is empty, return to previous menu. If the user input is invalid, print error message and return to previous menu.
void viewUserEventDetails(const Account *acc, const char *eventId){
    viewCurrentEvents(acc);

    printf(BOLD "Enter the Event ID to view details (or press Enter to go back): " RESET);

    char inputEventId[EVENT_ID_LENGTH];

    fgets(inputEventId, sizeof(inputEventId), stdin);
    inputEventId[strcspn(inputEventId, "\r\n")] = '\0'; // Remove newline to avoid issues with string comparison later
    
    char lowerInputEventId[EVENT_ID_LENGTH];

    toLowerStr(lowerInputEventId, inputEventId); // ensure that the event ID is still valid even if the user input in casesentive
    if (strlen(lowerInputEventId) == 0) {
        return; // User chose to go back
        clearScreen();
    }
    if (strlen(lowerInputEventId) > EVENT_ID_LENGTH || lowerInputEventId[0] != 'e' || lowerInputEventId[1] != 'v') {
        printf(RED BOLD "[ERROR] " RESET "Invalid Event ID format. Please try again.\n");
        return;
    }
    
    userEventDetails(acc, inputEventId);
    clearScreen();
}

// Function to display event details for a specific event that the user is involved in
void userEventDetails(const Account *acc, const char *eventId)
{
    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (f == NULL)
    {
        printf(RED BOLD "[ERROR] " RESET "Cannot open events.dat file\n");
        return;
    }
    Event tempEvent;
    int found = 0;
    while (fread(&tempEvent, sizeof(Event), 1, f) == 1)
    {
        cleanUserEventData(&tempEvent); 
        char lowerTempEventId[EVENT_ID_LENGTH];
        toLowerStr(lowerTempEventId, tempEvent.eventId); // ensure that the event ID can be found even if the user input in casesentive
        if (strcmp(lowerTempEventId, eventId) == 0)
        {
            StaffRole role;
            if (findStaffInEvent(&tempEvent, acc->studentId, &role))
            {
                printDivider("EVENT DETAILS");
                printf(BOLD "  %-15s : " RESET YELLOW "%s" RESET "\n", "Event ID", tempEvent.eventId);
                printf(BOLD "  %-15s : " RESET CYAN "%s" RESET "\n", "Name", tempEvent.name);
                printf(BOLD "  %-15s : " RESET "%s" RESET "\n", "Description", tempEvent.description);
                printf(BOLD "  %-15s : " RESET "%s" RESET "\n", "Location", tempEvent.location);
                printf(BOLD "  %-15s : " RESET GREEN "%s" RESET "\n", "Start Date", tempEvent.startDate);
                printf(BOLD "  %-15s : " RESET RED "%s" RESET "\n", "End Date", tempEvent.endDate);
                
                const char *stName = "Unknown";
                const char *stColor = RESET;
                if (tempEvent.status == STATUS_UPCOMING) { stName = "Upcoming"; stColor = YELLOW; }
                else if (tempEvent.status == STATUS_ONGOING) { stName = "Ongoing"; stColor = GREEN; }
                else if (tempEvent.status == STATUS_FINISHED) { stName = "Finished"; stColor = BLUE; }
                
                printf(BOLD "  %-15s : " RESET "%s%s" RESET "\n", "Status", stColor, stName);
                
                const char *rName = "Unknown";
                const char *rColor = RESET;
                if (role == STAFF_LEADER) { rName = "Leader"; rColor = RED; }
                else if (role == STAFF_MEMBER) { rName = "Member"; rColor = GREEN; }
                else if (role == STAFF_SUPPORT) { rName = "Support"; rColor = CYAN; }
                
                printf(BOLD "  %-15s : " RESET "%s%s" RESET "\n", "Your Role", rColor, rName);
                found = 1;
                printf(YELLOW BOLD "==================================================\n" RESET);
                pressEnterToContinue();
                break;
            }
        }
    }

    if (!found)
    {
        printf(RED BOLD "[ERROR] " RESET "You are not a staff member of this event or event not found.\n");
        pressEnterToContinue();
    }

    fclose(f);
}
