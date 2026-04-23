#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "auth.h"  
#include "event.h"  
#include "fileio.h" 
#include "utils.h"  

// global cache
CacheEvent *eventCache = NULL;
int cacheSize = 0;
int cacheCapacity = 0;
int cacheInitialized = 0;

void initializeEventCache()
{
    // lấy event trước
    if (cacheInitialized)
        return;                          // Chỉ load 1 lần
    cacheCapacity = getNextEventIndex(); // Số lượng event hiện có
    if (cacheCapacity == 0)
    {
        return; // No events to load
    }
    // cấp memory cho cache

    eventCache = (CacheEvent *)malloc(sizeof(CacheEvent) * cacheCapacity);
    if (eventCache == NULL)
    {
        printf("Failed to allocate memory for event cache because out of memory.\n");
        return; // Failed to allocate
    }

    // Mở file và đọc
    FILE *f = fopen("data/events.dat", "rb");
    if (f == NULL)
        return;

    Event event;
    while (fread(&event, sizeof(Event), 1, f))
    {
        // dọn rác bộ nhớ tránh in ký tự rác
        event.eventId[sizeof(event.eventId) - 1] = '\0';
        event.eventId[strcspn(event.eventId, "\r\n ")] = '\0';
        // Lưu event vào cache
        eventCache[cacheSize].event = event;
        eventCache[cacheSize].studentRole = STAFF_MEMBER; // Default
        cacheSize++;
    }

    fclose(f);
    cacheInitialized = 1; // load marked as done
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
void displayEventHistory()
{
    char StudentId[20];
    printf("Enter your Student ID: ");
    inputString(StudentId, sizeof(StudentId));
    initializeEventCache(); // call this once to load cache
    if (cacheSize == 0)
    {
        printf("\nNo events available in the system.\n");
        printf("Press Enter to continue");
        getchar();
        return;
    }
    int foundCount = 0;
    StaffRole role; // this variable will be used by findStaffInEvent to return the role of the user in the event
    printDivider("MY EVENT HISTORY");
    printf("%-10s | %-25s | %-12s | %s\n", "Event ID", "Event Name", "Your Role", "Start Date");
    printf("-------------------------------------------------------------------------------\n");
    for (int i = 0; i < cacheSize; i++)
    {
        if (findStaffInEvent(&eventCache[i].event, StudentId, &role))
        {
            printf("%-10s | %-25.25s | %-12s | %s\n",
                   eventCache[i].event.eventId,
                   eventCache[i].event.name,
                   (role == STAFF_LEADER) ? "Leader" : (role == STAFF_MEMBER) ? "Member"
                                                                              : "Support",
                   eventCache[i].event.startDate);
            foundCount++;
        }
    }
    printf("-------------------------------------------------------------------------------\n");
    if (foundCount == 0)
    {
        printf("You have not participated in any events yet.\n");
    }
    else
    {
        printf("Total: %d event(s) found in your history.\n", foundCount);
    }
    printf("\nPress Enter to continue");
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
        system("pause");
    } else {
        printf("[ERROR] How did we get here?.\n");
        system("pause");
    }
    printf("============================================\n");
   
    fclose(f);
    clearScreen();
}
