#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "auth.h"  
#include "event.h"  
#include "fileio.h" 
#include "utils.h"  
#define CHUNK_SIZE 1000



void cleanEventData(Event *event)
{
    event->eventId[sizeof(event->eventId) - 1] = '\0';
    event->eventId[strcspn(event->eventId, "\r\n ")] = '\0';
}

void printEventRowRole(const Event *event, StaffRole role)
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
        cleanEventData(&chunk[i]);

        // Nếu tìm thấy sinh viên trong event này
        if (findStaffInEvent(&chunk[i], studentId, &role))
        {
            printEventRowRole(&chunk[i], role);
            (*foundCount)++; // Tăng biến đếm số lượng tìm thấy
        }
    }
}

int findStaffInEvent(const Event *event, const char *studentId, StaffRole *role)
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

void displayEventHistory(const Account *acc)
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
            cleanEventData(&eventChunk[i]); // remove garbage 
            
            // if there is a joined student
            if (findStaffInEvent(&eventChunk[i], acc->studentId, &role))
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
    printDivider("EVENT HISTORY");
    printf("%-10s | %-25s | %-12s | %s\n", "Event ID", "Event Name", "Your Role", "Start Date");
    printf("-------------------------------------------------------------------------------\n");

    // 5. SẮP XẾP VÀ IN RA (Đã gom đủ rồi thì mới in)
    if (foundCount > 0)
    {
        // Gọi hàm Sort
        quicksortByDate(matchedList, 0, foundCount - 1);

        // In lần lượt từ mảng đã sort
        for (int i = 0; i < foundCount; i++)
        {
            printEventRowRole(&matchedList[i].event, matchedList[i].studentRole);
        }
    }

    // Giải phóng mảng gom sau khi xài xong
    free(matchedList);

    // 6. IN FOOTER
    printf("-------------------------------------------------------------------------------\n");
    if (foundCount == 0)
    {
        printf("You have not participated in any events yet.\n");
    }
    else
    {
        printf("Total: %d event(s) found in your history.\n", foundCount);
    }
    
    printf("\nPress Enter to continue...");
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
            cleanEventData(&eventChunk[i]); // remove garbage 
            
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

    // 5. SẮP XẾP VÀ IN RA (Đã gom đủ rồi thì mới in)
    if (foundCount > 0)
    {
        // Gọi hàm Sort
        quicksortByDate(matchedList, 0, foundCount - 1);

        // In lần lượt từ mảng đã sort
        for (int i = 0; i < foundCount; i++)
        {
            printEventRowRole(&matchedList[i].event, matchedList[i].studentRole);
        }
    }

    // Giải phóng mảng gom sau khi xài xong
    free(matchedList);

    // 6. IN FOOTER
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