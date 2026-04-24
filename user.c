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

void printUserFinishedList(MatchedEvent *list, int count) 
{
    printf("\n--- FINISHED EVENTS HISTORY ---\n");
    printf("----------------------------------------------------------\n");
    printf("%-35s | %-15s\n", "Event Name", "Role");
    printf("----------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++)
    {
        const char *roleName;
        switch (list[i].studentRole) {
            case STAFF_LEADER: roleName = "Leader"; break;
            case STAFF_MEMBER: roleName = "Member"; break;
            case STAFF_SUPPORT: roleName = "Support"; break;
            default: roleName = "Unknown"; break;
        }
        printf("%-35.35s | %-15s\n", list[i].event.name, roleName);
    }
    printf("----------------------------------------------------------\n");
}

void cleanUserEventData(Event *event)
{
    event->eventId[sizeof(event->eventId) - 1] = '\0';
    event->eventId[strcspn(event->eventId, "\r\n ")] = '\0';
}

void printUserEventRowRole(const Event *event, StaffRole role)
{

    const char *roleName;
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


    printf("%-10s | %-25.25s | %-12s | %s\n",
           event->eventId,
           event->name,
           roleName,          
           event->startDate);
}

void processChunk(Event *chunk, size_t eventsRead, const char *studentId, int *foundCount)
{
    StaffRole role;
    for (size_t i = 0; i < eventsRead; i++)
    {
        cleanUserEventData(&chunk[i]);

        // Nếu tìm thấy sinh viên trong event này
        if (findStaffInEvent(&chunk[i], studentId, &role))
        {
            printUserEventRowRole(&chunk[i], role);
            (*foundCount)++; // Tăng biến đếm số lượng tìm thấy
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
    int rawFoundCount = 0;
    MatchedEvent *rawList = getEventsByStudentId(acc->studentId, &rawFoundCount);
    if (rawList == NULL || rawFoundCount == 0) {
        printf("No events found for this student.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }
    int finishedCount = 0;
    MatchedEvent *finishedList = (MatchedEvent *)malloc(rawFoundCount * sizeof(MatchedEvent));
    if (finishedList == NULL) {
        printf("Error: Out of memory. Cannot allocate finished list.\n");
        free(rawList);
        return;
    }
    for(int i = 0; i<rawFoundCount;i++){
        if (rawList[i].event.status == STATUS_FINISHED){
            finishedList[finishedCount] = rawList[i];
            finishedCount++;
        }
    }
    if (finishedCount > 1) {
        quicksortByDate(finishedList, 0, finishedCount - 1);
    }
    if(finishedCount > 0){
        printUserFinishedList(finishedList, finishedCount);
    }else{
        printf("No finished events found for this student.\n");
    }
    free(rawList);
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
    while (fread(&temp, sizeof(User), 1, f) && *count < MAX_SEARCH_RESULTS)
    {
        if (strstr(temp.studentName, name) != NULL)
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
void viewCurrentEvents(const Account *acc)
{   
    //open file
    FILE *f = fopen("data/events.dat", "rb");
    if (f == NULL)
    {
        printf("\nNo events available in the system.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    // allocate memory for chunk
    Event *eventChunk = (Event *)malloc(CHUNK_SIZE * sizeof(Event));
    if (eventChunk == NULL)
    {
        printf("Error: Out of memory. Cannot allocate chunk.\n");
        fclose(f);
        return;
    }
// array for get the total results
    int capacity = 10;
    int foundCount = 0;
    MatchedEvent *matchedList = (MatchedEvent *)malloc(capacity * sizeof(MatchedEvent));
    size_t eventsRead;
    StaffRole role;
    while ((eventsRead = fread(eventChunk, sizeof(Event), CHUNK_SIZE, f)) > 0)
    {
        for (size_t i = 0; i < eventsRead; i++)
        {
            cleanUserEventData(&eventChunk[i]); // remove garbage 
            
            // if there is a joined student and event is ongoing
            if (findStaffInEvent(&eventChunk[i], acc->studentId, &role) && eventChunk[i].status == STATUS_ONGOING)
            {
                // auto resize array if needed
                if (foundCount >= capacity) {
                    capacity *= 2;
                    matchedList = (MatchedEvent *)realloc(matchedList, capacity * sizeof(MatchedEvent));
                }
                
                // insert to result array 
                matchedList[foundCount].event = eventChunk[i];
                matchedList[foundCount].studentRole = role;
                foundCount++;
            }
        }
    }
    free(eventChunk);
    fclose(f);
    printDivider("CURRENT EVENTS");
    printf("%-10s | %-25s | %-12s | %s\n", "Event ID", "Event Name", "Your Role", "Start Date");
    printf("-------------------------------------------------------------------------------\n");

    // sort and print results if found
    if (foundCount > 0)
    {
        // call sort function
        quicksortByDate(matchedList, 0, foundCount - 1);

        // print sorted results
        for (int i = 0; i < foundCount; i++)
        {
            printUserEventRowRole(&matchedList[i].event, matchedList[i].studentRole);
        }
    }

    // release matched list memory after use
    free(matchedList);

    // print footer
    printf("-------------------------------------------------------------------------------\n");
    if (foundCount == 0)
    {
        printf("You have no ongoing events.\n");
    }
    else
    {
        printf("Total: %d ongoing event(s).\n", foundCount);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}