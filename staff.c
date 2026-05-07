#include "staff.h"
#include "event.h"
#include "fileio.h"
#include "user.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paths.h"
#include "colors.h"

// Check if a staff member with the given studentId is already in the event's staffList
int isStaffInEvent(Event *e, const char *studentId)
{
    for (int i = 0; i < e->staffCount; i++)
    {
        if (strcmp(e->staffList[i].studentId, studentId) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// 2.5: Implementation of adding staff
void addStaffToEvent()
{
    char eventId[EVENT_ID_LENGTH];
    printDivider("ADD STAFF TO EVENT");
    printf(BOLD "Enter Event ID (EVxxxxxx): " RESET);
    inputString(eventId, sizeof(eventId));

    int idx = findEventIndexById(eventId);
    if (idx == -1)
    {
        printf(RED BOLD "[ERROR] " RESET "Event not found!\n");
        return;
    }

    Event e;
    loadEventAt(idx, &e);

    // Validate Status: Cannot add if event is finished
    if (e.status == STATUS_FINISHED)
    {
        printf(RED BOLD "[ERROR] " RESET "Event has finished, cannot add staff!\n");
        return;
    }

    // Validate Capacity: Max staff limit reached
    if (e.staffCount >= MAX_STAFF_PER_EVENT)
    {
        printf(RED BOLD "[ERROR] " RESET "Event has reached maximum capacity (%d people)!\n", MAX_STAFF_PER_EVENT);
        return;
    }

    int searchMode;
    printf(YELLOW BOLD "Search by:\n" RESET);
    printf(GREEN "  1." RESET " Student ID\n");
    printf(GREEN "  2." RESET " Name\n");
    printf(BOLD "Choice: " RESET);
    if (scanf("%d", &searchMode) != 1)
    {
        // Preventing infinite loop when EOF
        if (feof(stdin)) return;
        searchMode = 0;
        clearInputBuffer();
    }
    else
    {
        clearInputBuffer();
    }

    char query[NAME_LENGTH];
    if (searchMode == 1)
    {
        printf(BOLD "Enter Student ID: " RESET);
    }
    else if (searchMode == 2)
    {
        printf(BOLD "Enter Name: " RESET);
    }
    else
    {
        printf(RED BOLD "[ERROR] " RESET "Invalid search mode!\n");
        return;
    }
    inputString(query, sizeof(query));
    if (feof(stdin)) return;

    User results[MAX_SEARCH_RESULTS];
    int count = 0;
    if (searchMode == 1)
    {
        searchUserById(query, results, &count);
    }
    else
    {
        searchUserByName(query, results, &count);
    }

    if (count == 0)
    {
        printf(RED BOLD "[ERROR] " RESET "No matching members found!\n");
        return;
    }

    // Display search results
    printf("\n" YELLOW BOLD "Search Results:\n" RESET);
    for (int i = 0; i < count; i++)
    {
        printf(GREEN "  %d." RESET " [%s] %s\n", i + 1, results[i].studentId, results[i].studentName);
    }

    int choice;
    printf(BOLD "\nSelect member (1-%d) or 0 to cancel: " RESET, count);
    if (scanf("%d", &choice) != 1)
    {
        // Preventing infinite loop when EOF
        if (feof(stdin)) return;
        choice = 0;
        clearInputBuffer();
    }
    else
    {
        clearInputBuffer();
    }

    if (choice < 1 || choice > count)
    {
        return;
    }

    User selected = results[choice - 1];

    // Validate Duplicate: Cannot add the same MSSV twice
    if (isStaffInEvent(&e, selected.studentId))
    {
        printf(RED BOLD "[ERROR] " RESET "This member is already in the event's staff list!\n");
        return;
    }

    StaffEntry entry;
    strcpy(entry.studentId, selected.studentId);

    // Input role
    int tempRole;
    printf(BOLD "Enter Role (%d: Leader, %d: Member, %d: Support): " RESET, STAFF_LEADER, STAFF_MEMBER, STAFF_SUPPORT);
    if (scanf("%d", &tempRole) != 1)
    {
        // Preventing infinite loop when EOF
        if (feof(stdin)) return;
        tempRole = STAFF_MEMBER;
        clearInputBuffer();
    }
    else
    {
        clearInputBuffer();
    }

    // Input description/mission
    printf(BOLD "Enter Mission Description: " RESET);
    inputString(entry.description, sizeof(entry.description));
    if (feof(stdin)) return;

    // Append to staff list
    e.staffList[e.staffCount] = entry;
    e.staffCount++;

    // Save back to file
    if (saveEventAt(idx, &e))
    {
        printf(GREEN BOLD "[SUCCESS] " RESET "Staff added to event successfully!\n");
    }
    else
    {
        printf(RED BOLD "[ERROR] " RESET "Could not save event data!\n");
    }
}

// Helper: get role name as string
static const char *getRoleName(StaffRole role)
{
    switch (role)
    {
    case STAFF_LEADER:
        return "Leader";
    case STAFF_MEMBER:
        return "Member";
    case STAFF_SUPPORT:
        return "Support";
    default:
        return "Unknown";
    }
}

// 2.6: Implementation of editing staff
void editStaffInEvent()
{
    char eventId[EVENT_ID_LENGTH];
    printDivider("EDIT STAFF ROLE/MISSION");
    printf(BOLD "Enter Event ID (EVxxxxxx): " RESET);
    inputString(eventId, sizeof(eventId));

    int idx = findEventIndexById(eventId);
    if (idx == -1)
    {
        printf(RED BOLD "[ERROR] " RESET "Event not found!\n");
        return;
    }

    Event e;
    loadEventAt(idx, &e);

    if (!canModifyStaff(&e))
    {
        printf(RED BOLD "[ERROR] " RESET "Only allowed to edit when event is UPCOMING!\n");
        return;
    }

    if (e.staffCount == 0)
    {
        printf(YELLOW BOLD "[INFO] " RESET "No staff members in this event.\n");
        return;
    }

    // Display current staff
    printf("\n" YELLOW BOLD "Current Staff List:\n" RESET);
    for (int i = 0; i < e.staffCount; i++)
    {
        User u;
        char name[NAME_LENGTH] = "Unknown";
        if (findUserById(e.staffList[i].studentId, &u))
        {
            strcpy(name, u.studentName);
        }
        printf(GREEN "  %d." RESET " [%s] %-20s | Role: " CYAN "%-8s" RESET " | Mission: %s\n", i + 1, e.staffList[i].studentId, name, getRoleName(e.staffList[i].role), e.staffList[i].description);
    }

    printf(YELLOW "Enter the staff number to edit from the list above.\n" RESET);
    int choice;
    printf(BOLD "\nSelect staff member to edit (1-%d) or 0 to cancel: " RESET, e.staffCount);
    if (scanf("%d", &choice) != 1)
    {
        choice = 0;
    }
    getchar();

    if (choice < 1 || choice > e.staffCount)
    {
        return;
    }

    StaffEntry *entry = &e.staffList[choice - 1];

    // Update Role
    int tempRole;
    printf(BOLD "New Role (%d: Leader, %d: Member, %d: Support) [%s]: " RESET, STAFF_LEADER, STAFF_MEMBER, STAFF_SUPPORT, getRoleName(entry->role));
    if (scanf("%d", &tempRole) == 1)
    {
        entry->role = (StaffRole)tempRole;
    }
    getchar();

    // Update Mission
    printf(BOLD "New Mission Description [%s]: " RESET, entry->description);
    char newDesc[DESC_LENGTH];
    inputString(newDesc, sizeof(newDesc));
    if (strlen(newDesc) > 0)
    {
        strcpy(entry->description, newDesc);
    }

    if (saveEventAt(idx, &e))
    {
        printf(GREEN BOLD "[SUCCESS] " RESET "Staff information updated successfully!\n");
    }
    else
    {
        printf(RED BOLD "[ERROR] " RESET "Could not save event data!\n");
    }
}

// 2.7: Implementation of deleting staff
void deleteStaffFromEvent()
{
    char eventId[EVENT_ID_LENGTH];
    printDivider("DELETE STAFF FROM EVENT");
    printf(BOLD "Enter Event ID (EVxxxxxx): " RESET);
    inputString(eventId, sizeof(eventId));

    int idx = findEventIndexById(eventId);
    if (idx == -1)
    {
        printf(BOLD RED "[ERROR] Event not found!" RESET "\n");
        return;
    }

    Event e;
    loadEventAt(idx, &e);

    if (!canModifyStaff(&e))
    {
        printf(RED BOLD "[ERROR] " RESET "Only allowed to delete when event is UPCOMING!\n");
        return;
    }

    if (e.staffCount == 0)
    {
        printf(YELLOW BOLD "[INFO] " RESET "No staff members in this event.\n");
        return;
    }

    // Display current staff
    printf("\n" YELLOW BOLD "Current Staff List:\n" RESET);
    for (int i = 0; i < e.staffCount; i++)
    {
        User u;
        char name[NAME_LENGTH] = "Unknown";
        if (findUserById(e.staffList[i].studentId, &u))
        {
            strcpy(name, u.studentName);
        }
        printf(GREEN "  %d." RESET " [%s] %-20s | Role: " CYAN "%-8s" RESET " | Mission: %s\n", i + 1, e.staffList[i].studentId, name, getRoleName(e.staffList[i].role), e.staffList[i].description);
    }

    printf(YELLOW "Enter the staff number to remove from the list above.\n" RESET);
    int choice;
    printf(BOLD "\nSelect staff member to delete (1-%d) or 0 to cancel: " RESET, e.staffCount);
    if (scanf("%d", &choice) != 1)
    {
        choice = 0;
    }
    getchar();

    if (choice < 1 || choice > e.staffCount)
    {
        return;
    }

    char msg[100];
    sprintf(msg, "Are you sure you want to remove staff member %s?", e.staffList[choice - 1].studentId);
    if (!confirmAction(msg))
    {
        printf(YELLOW BOLD "[INFO] " RESET "Deletion cancelled.\n");
        return;
    }

    // Shifting array to delete element at choice-1
    for (int i = choice - 1; i < e.staffCount - 1; i++)
    {
        e.staffList[i] = e.staffList[i + 1];
    }
    e.staffCount--;

    if (saveEventAt(idx, &e))
    {
        printf(GREEN BOLD "[SUCCESS] " RESET "Staff removed from event successfully!\n");
    }
    else
    {
        printf(RED BOLD "[ERROR] " RESET "Could not save event data!\n");
    }
}

// Helper: check if staff in event can be modified (only if status is UPCOMING)
int canModifyStaff(Event *e)
{
    return (e->status == STATUS_UPCOMING);
}
void cleanEventData(Event *event)
{
    event->eventId[sizeof(event->eventId) - 1] = '\0';
    event->eventId[strcspn(event->eventId, "\r\n ")] = '\0';
}

void printEventRowRole(const Event *event, StaffRole role, const char *studentName)
{
    const char *roleName;
    const char *roleColor = RESET;
    switch (role)
    {
        case STAFF_LEADER: roleName = "Leader"; roleColor = CYAN; break;
        case STAFF_MEMBER: roleName = "Member"; roleColor = GREEN; break;
        case STAFF_SUPPORT: roleName = "Support"; roleColor = YELLOW; break;
        default: roleName = "Unknown"; break;
    }

    const char *statusName;
    const char *statusColor = RESET;
    switch (event->status) 
    {
        case 0: statusName = "Upcoming"; statusColor = YELLOW; break; 
        case 1: statusName = "Ongoing"; statusColor = GREEN; break;
        case 2: statusName = "Finished"; statusColor = RED; break; 
        default: statusName = "Unknown"; break;
    }

    printf(CYAN "|" RESET " %-10s " CYAN "|" RESET " %-30.30s " CYAN "|" RESET " %-25.25s " CYAN "|" RESET " %s%-10s" RESET CYAN " | " RESET "%s%-12s" RESET CYAN "|\n" RESET,
           event->eventId,
           event->name,
           studentName,
           roleColor, roleName,
           statusColor, statusName 
          );
}


int findStaffInEvent(const Event *event, const char *studentId, StaffRole *role)
{
    if (event == NULL || studentId == NULL || role == NULL)
    {
        return 0;
    }

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

#include <stdlib.h>
#include <string.h>

char *StudentIDInput() {
    char studentId[ID_LENGTH];
    printf(BOLD "Enter Student ID (or press Enter to skip): " RESET);
    
    
    inputString(studentId, sizeof(studentId));

    if (strlen(studentId) == 0) {
        return NULL;
    }

    // auto allocate memory 
    char *copy = (char *)malloc((strlen(studentId) + 1) * sizeof(char));
    if (copy == NULL) {
        printf(RED BOLD "[ERROR] " RESET "Memory allocation failed!\n");
        return NULL;
    }
    
    strcpy(copy, studentId);
    return copy;
}
void printEventList(MatchedEvent *list, int count, const char *studentId) 
{
    const char *studentName = "Unknown";
    User userData;
    if (studentId != NULL && studentId[0] != '\0' && findUserById(studentId, &userData))
    {
        studentName = userData.studentName;
    }

    printf("\n" YELLOW BOLD "Event History for Member ID: %s\n" RESET, studentId);
    printf(CYAN "+------------+--------------------------------+---------------------------+------------+--------------+\n" RESET);
    printf(CYAN "| " BOLD "%-10s" RESET CYAN " | " BOLD "%-30s" RESET CYAN " | " BOLD "%-25s" RESET CYAN " | " BOLD "%-10s" RESET CYAN " | " BOLD "%-12s" RESET CYAN " |\n" RESET, "Event ID", "Event Name", "Student Name", "Role", "Status");
    printf(CYAN "+------------+--------------------------------+---------------------------+------------+--------------+\n" RESET);
    
    for (int i = 0; i < count; i++)
    {
        printEventRowRole(&list[i].event, list[i].studentRole, studentName);
    }
    printf(CYAN "+------------+--------------------------------+---------------------------+------------+--------------+\n" RESET);
    printf(CYAN BOLD " Total: %d event(s) found.\n" RESET, count);
}
void displayEventHistory(const char *studentId) {
    if (studentId == NULL || studentId[0] == '\0')
    {
        printf(RED BOLD "[ERROR] " RESET "Invalid Student ID.\n");
        return;
    }

    int count = 0;
    
   //get events by studentId
    MatchedEvent *historyList = getEventsByStudentId(studentId, &count);
    
    // print results or message if not found
    if (count > 0 && historyList != NULL) {
        printEventList(historyList, count, studentId);
    } else {
        printf(YELLOW BOLD "[INFO] " RESET "No events found for this student.\n");
    }
    
    //release memory
    if (historyList != NULL) {
        free(historyList);
    }
}
MatchedEvent* getEventsByStudentId(const char *studentId, int *outFoundCount) 
{
    if (outFoundCount == NULL)
    {
        return NULL;
    }

    *outFoundCount = 0; // Initialize output count to 0

    if (studentId == NULL || studentId[0] == '\0')
    {
        return NULL;
    }

    // open file for reading
    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (f == NULL) {
        return NULL; //check if file opened successfully
    }

    // allocate memory for chunk
    Event *eventChunk = (Event *)malloc(CHUNK_SIZE * sizeof(Event));
    if (eventChunk == NULL) {
        fclose(f);
        return NULL; //check if memory allocated successfully
    }

   //allocate memory for result array
    int capacity = 10;
    int foundCount = 0; 
    MatchedEvent *matchedList = (MatchedEvent *)malloc(capacity * sizeof(MatchedEvent));
    if (matchedList == NULL) {
        printf(YELLOW BOLD "[INFO] " RESET "No events found.\n");
        fclose(f);
        return NULL;
    }

    size_t eventsRead;
    StaffRole role;

    // read file in chunks and process
    while ((eventsRead = fread(eventChunk, sizeof(Event), CHUNK_SIZE, f)) > 0)
    {
        for (size_t i = 0; i < eventsRead; i++)
        {
            cleanEventData(&eventChunk[i]); // remove garbage characters from eventId and other string fields
            
            //if there is a joined student
            if (findStaffInEvent(&eventChunk[i], studentId, &role))
            {
                // auto resize array if needed
                if (foundCount >= capacity) {
                    capacity *= 2;
                    MatchedEvent *temp = (MatchedEvent *)realloc(matchedList, capacity * sizeof(MatchedEvent));
                    if (temp == NULL) {
                        // If realloc fails, we should free existing resources and return NULL
                        printf(YELLOW BOLD "[WARNING] " RESET "Memory limit reached. Results may be incomplete.\n");
                        goto finish_reading; 
                        /*
                        Reason I used goto here instead of break or return:
                        - exit the loop immediately without trying to continue processing more events, which could lead to more realloc attempts and more memory issues.
                        - jump to the cleanup section to free allocated memory and close the file before returning, ensuring we don't leak resources even in this error case.
                        - make sure the memory absolutely safe, avoid forget to free the resource when the program get error                        
                        */                      
                    }
                    matchedList = temp;
                }
                
                // add to result array
                matchedList[foundCount].event = eventChunk[i];
                matchedList[foundCount].studentRole = role;
                foundCount++;
            } 
        }
    }finish_reading:

    //release resources 
    free(eventChunk);
    fclose(f);

    // record found count to output parameter
    *outFoundCount = foundCount;

    //if no events found, free matchedList and return NULL
    if (foundCount == 0) {
        free(matchedList);
        return NULL;
    }

    return matchedList; //return pointer to array of matched events (caller is responsible for freeing this memory)
}

