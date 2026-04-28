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

void printUserFinishedList(MatchedEvent *list, int count, const char *studentId) 
{
    const char *studentName = "Unknown";
    User userData;

    if (studentId != NULL && studentId[0] != '\0' && findUserById(studentId, &userData))
    {
        studentName = userData.studentName;
    }

    printf("\n--- FINISHED EVENTS HISTORY ---\n");
    printf("----------------------------------------------------------------------------------------------\n");
    printf("%-10s | %-30s | %-25s | %-12s\n", "Event ID", "Event Name", "Student Name", "Role");
    printf("----------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++)
    {
        const char *roleName;
        switch (list[i].studentRole) {
            case STAFF_LEADER: roleName = "Leader"; break;
            case STAFF_MEMBER: roleName = "Member"; break;
            case STAFF_SUPPORT: roleName = "Support"; break;
            default: roleName = "Unknown"; break;
        }
        printf("%-10s | %-30.30s | %-25.25s | %-12s\n",
               list[i].event.eventId,
               list[i].event.name,
               studentName,
               roleName);
    }
    printf("----------------------------------------------------------------------------------------------\n");
}

void cleanUserEventData(Event *event)
{
    event->eventId[sizeof(event->eventId) - 1] = '\0';
    event->eventId[strcspn(event->eventId, "\r\n ")] = '\0';
}

void printUserEventRowRole(const Event *event, StaffRole role, const char *studentId)
{
    const char *roleName;
    const char *studentName = "Unknown";
    User userData;
    switch (role)
    {
        case STAFF_LEADER:
            roleName = "Leader";
            break;
        case STAFF_MEMBER:
            roleName = "Member";
            break;
        case STAFF_SUPPORT:
            roleName = "Support";
            break;
        default:
            roleName = "Unknown";
            break;
    }


    if (studentId != NULL && studentId[0] != '\0' && findUserById(studentId, &userData))
    {
        studentName = userData.studentName;
    }

    printf("%-10s | %-25.25s | %-25.25s | %-12s | %s\n",
           event->eventId,
           event->name,
           studentName,
           roleName,          
           event->startDate);
}

void processChunk(Event *chunk, size_t eventsRead, const char *studentId, int *foundCount)
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

int findStaffInEventUser(const Event *event, const char *studentId, StaffRole *role)
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

void displayCurrentUserEventHistory(const Account *acc)
{   
    if (acc == NULL) {
        printf("[ERROR] Cannot find event history.\n");
        return;
    }
    int rawFoundCount = 0; //this is the variable to store the number of events that the user is involved in, including both ongoing and finished events. 
    
    MatchedEvent *rawList = getEventsByStudentId(acc->studentId, &rawFoundCount);

    /* *rawList will return a list of events that the user is involved in, including both ongoing and finished events. 
    The function will also set the rawFoundCount variable to the number of events found. */
   
    if (rawList == NULL || rawFoundCount == 0) {
        printf("No events found for this student.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    int finishedCount = 0;
    MatchedEvent *finishedList = (MatchedEvent *)malloc(rawFoundCount * sizeof(MatchedEvent));//allocate memory basse on the number of events found in rawList
    if (finishedList == NULL) {
        printf("Error: Out of memory. Cannot allocate finished list.\n");
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
        printf("No finished events found for this student.\n");
    }
    free(rawList); // free the memory allocated for rawList as it is no longer needed
    if(finishedList != NULL){
        free(finishedList);
    }
    printf("Press Enter to continue...");
    getchar();
}
int findUserById(const char *id, User *result)
{
    FILE *f = fopen("data/users.dat", "rb");
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

void searchUserByName(const char *name, User results[MAX_SEARCH_RESULTS], int *count)
{
    FILE *f = fopen("data/users.dat", "rb");
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

void searchUserById(const char *id, User results[MAX_SEARCH_RESULTS], int *count)
{
    FILE *f = fopen("data/users.dat", "rb");
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

void viewProfile(const Account *acc){
    User persona;
    int userFound = 0;
    FILE *f = fopen("data/users.dat", "rb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot open users.dat file\n");
        return;
    }
    printf("debug: looking for user profile with student ID: %s\n", acc->studentId);
    while (fread(&persona, sizeof(User), 1, f) == 1)
    {
        if (strcmp(persona.studentId, acc->studentId) == 0)
        {
            userFound = 1;
            break;
        }
        printf("debug: read user with student ID: %s\n", persona.studentId);
    }
    if (userFound == 0){
        printf("[ERROR] Cannot find user profile.\n");
        system("pause");
        fclose(f);
        return;
    }
    printf("================== PROFILE ==================\n");
    if(userFound ==1){
        printf("Student ID: %s\n", persona.studentId);
        printf("Name: %s\n", persona.studentName);
        printf("Email: %s\n", persona.email);
        printf("Phone Number: %s\n", persona.phoneNumber);
        printf("Position:%s\n", (acc->role == 1 ? "BCN" : "Member"));
        printf("Specialize: %s\n", persona.specialize);
    } else {
        printf("[ERROR] How did we get here?.\n");
    }
    printf("============================================\n");
    system("pause");
    fclose(f);
    clearScreen();
}
MatchedEvent* getCurrentEventsForUser(const Account *acc, int *count)
{
    FILE *f = fopen("data/events.dat", "rb");
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

void viewCurrentEvents(const Account *acc, int wait) // wait =1 will not print out "Press Enter to continue" to make the event details
                                                    //view smoother when user wants to view details of one of the events in the list
{
    int count;
    MatchedEvent *events = getCurrentEventsForUser(acc, &count);
    if (events == NULL && count == 0)
    {
        printf("\nNo events available in the system.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    printDivider("CURRENT EVENTS");
    printf("%-10s | %-25s | %-25s | %-12s | %s\n", "Event ID", "Event Name", "Student Name", "Your Role", "Start Date");
    printf("-----------------------------------------------------------------------------------------------------\n");

    if (count > 0)
    {
        printCurrentEvents(events, count, acc->studentId);
    }

    printf("-----------------------------------------------------------------------------------------------------\n");
    if (count == 0)
    {
        printf("You have no ongoing events.\n");
    }
    else
    {
        printf("Total: %d ongoing event(s).\n", count);
    }

    if (events != NULL)
    {
        free(events);
    }
    if (!wait) {
        printf("\nPress Enter to continue...");
        getchar();
    }
}
void viewUserEventDetails(const Account *acc, const char *eventId){
    viewCurrentEvents(acc,1);
    printf("\nEnter the Event ID to view details (or press Enter to go back): ");
    char inputEventId[EVENT_ID_LENGTH];
    fgets(inputEventId, sizeof(inputEventId), stdin);
    inputEventId[strcspn(inputEventId, "\r\n")] = '\0'; // Remove newline
    char lowerInputEventId[EVENT_ID_LENGTH];
    toLowerStr(lowerInputEventId, inputEventId); // ensure that the event ID inputed is casesentive
    if (strlen(lowerInputEventId) > EVENT_ID_LENGTH || lowerInputEventId[0] != 'e' || lowerInputEventId[1] != 'v') {
        printf("[ERROR] Invalid Event ID format. Please try again.\n");
        return;
    }
    if (strlen(lowerInputEventId) == 0) {
        return; // User chose to go back
    }
    userEventDetails(acc, inputEventId);
}

void userEventDetails(const Account *acc, const char *eventId)
{
    FILE *f = fopen("data/events.dat", "rb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot open events.dat file\n");
        return;
    }
    Event tempEvent;
    int found = 0;
    while (fread(&tempEvent, sizeof(Event), 1, f) == 1)
    {
        cleanUserEventData(&tempEvent); 
        char lowerTempEventId[EVENT_ID_LENGTH];
        toLowerStr(lowerTempEventId, tempEvent.eventId); // ensure that the event ID read from file is casesentive
        if (strcmp(lowerTempEventId, eventId) == 0)
        {
            StaffRole role;
            if (findStaffInEvent(&tempEvent, acc->studentId, &role))
            {
                printDivider("EVENT DETAILS");
                printf("Event ID: %s\n", tempEvent.eventId);
                printf("Name: %s\n", tempEvent.name);
                printf("Description: %s\n", tempEvent.description);
                printf("Location: %s\n", tempEvent.location);
                printf("Start Date: %s\n", tempEvent.startDate);
                printf("End Date: %s\n", tempEvent.endDate);
                printf("Status: %s\n", (tempEvent.status == STATUS_UPCOMING) ? "Upcoming" :(tempEvent.status == STATUS_ONGOING) ? "Ongoing" : "Finished");
                printf("Your Role: %s\n", (role == STAFF_LEADER) ? "Leader" : (role == STAFF_MEMBER) ? "Member" : "Support");
                found = 1;
                 printf("========================================\n");
                system("pause");
                break;
            }
        }
    }

    if (!found)
    {
        printf("[ERROR] You are not a staff member of this event or event not found.\n");
    }

    fclose(f);
}
