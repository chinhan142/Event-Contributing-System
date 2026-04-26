#include "staff.h"
#include "event.h"
#include "fileio.h"
#include "user.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    printf("Enter Event ID (EVxxxxxx): ");
    inputString(eventId, sizeof(eventId));

    int idx = findEventIndexById(eventId);
    if (idx == -1)
    {
        printf("\033[1;31m[ERROR] Event not found!\033[0m\n");
        return;
    }

    Event e;
    loadEventAt(idx, &e);

    // Validate Status: Cannot add if event is finished
    if (e.status == STATUS_FINISHED)
    {
        printf("\033[1;31m[ERROR] Event has finished, cannot add staff!\033[0m\n");
        return;
    }

    // Validate Capacity: Max staff limit reached
    if (e.staffCount >= MAX_STAFF_PER_EVENT)
    {
        printf("\033[1;31m[ERROR] Event has reached maximum capacity (%d people)!\033[0m\n", MAX_STAFF_PER_EVENT);
        return;
    }

    int searchMode;
    printf("Search by:\n");
    printf("  1. Student ID\n");
    printf("  2. Name\n");
    printf("Choice: ");
    if (scanf("%d", &searchMode) != 1)
    {
        searchMode = 0;
    }
    getchar();

    char query[NAME_LENGTH];
    if (searchMode == 1)
    {
        printf("Enter Student ID: ");
    }
    else if (searchMode == 2)
    {
        printf("Enter Name: ");
    }
    else
    {
        printf("[ERROR] Invalid search mode!\n");
        return;
    }
    inputString(query, sizeof(query));

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
        printf("\033[1;31m[ERROR] No matching members found!\033[0m\n");
        return;
    }

    // Display search results
    printf("\nSearch Results:\n");
    for (int i = 0; i < count; i++)
    {
        printf("%d. [%s] %s\n", i + 1, results[i].studentId, results[i].studentName);
    }

    int choice;
    printf("\nSelect member (1-%d) or 0 to cancel: ", count);
    if (scanf("%d", &choice) != 1)
    {
        choice = 0;
    }
    getchar(); // Clear newline character from buffer

    if (choice < 1 || choice > count)
    {
        return;
    }

    User selected = results[choice - 1];

    // Validate Duplicate: Cannot add the same MSSV twice
    if (isStaffInEvent(&e, selected.studentId))
    {
        printf("\033[1;31m[ERROR] This member is already in the event's staff list!\033[0m\n");
        return;
    }

    StaffEntry entry;
    strcpy(entry.studentId, selected.studentId);

    // Input role
    int tempRole;
    printf("Enter Role (%d: Leader, %d: Member, %d: Support): ", STAFF_LEADER, STAFF_MEMBER, STAFF_SUPPORT);
    if (scanf("%d", &tempRole) != 1)
    {
        tempRole = STAFF_MEMBER;
    }
    entry.role = (StaffRole)tempRole;
    getchar();

    // Input description/mission
    printf("Enter Mission Description: ");
    inputString(entry.description, sizeof(entry.description));

    // Append to staff list
    e.staffList[e.staffCount] = entry;
    e.staffCount++;

    // Save back to file
    if (saveEventAt(idx, &e))
    {
        printf("\033[1;32m[SUCCESS] Staff added to event successfully!\033[0m\n");
    }
    else
    {
        printf("\033[1;31m[ERROR] Could not save event data!\033[0m\n");
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
    printf("Enter Event ID (EVxxxxxx): ");
    inputString(eventId, sizeof(eventId));

    int idx = findEventIndexById(eventId);
    if (idx == -1)
    {
        printf("\033[1;31m[ERROR] Event not found!\033[0m\n");
        return;
    }

    Event e;
    loadEventAt(idx, &e);

    if (!canModifyStaff(&e))
    {
        printf("\033[1;31m[ERROR] Only allowed to edit when event is UPCOMING!\033[0m\n");
        return;
    }

    if (e.staffCount == 0)
    {
        printf("\033[1;33m[INFO] No staff members in this event.\033[0m\n");
        return;
    }

    // Display current staff
    printf("\nCurrent Staff List:\n");
    for (int i = 0; i < e.staffCount; i++)
    {
        User u;
        char name[NAME_LENGTH] = "Unknown";
        if (findUserById(e.staffList[i].studentId, &u))
        {
            strcpy(name, u.studentName);
        }
        printf("%d. [%s] %-20s | Role: %-8s | Mission: %s\n", i + 1, e.staffList[i].studentId, name, getRoleName(e.staffList[i].role), e.staffList[i].description);
    }

    printf("Enter the staff number to edit from the list above.\n");
    int choice;
    printf("\nSelect staff member to edit (1-%d) or 0 to cancel: ", e.staffCount);
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
    printf("New Role (%d: Leader, %d: Member, %d: Support) [%s]: ", STAFF_LEADER, STAFF_MEMBER, STAFF_SUPPORT, getRoleName(entry->role));
    if (scanf("%d", &tempRole) == 1)
    {
        entry->role = (StaffRole)tempRole;
    }
    getchar();

    // Update Mission
    printf("New Mission Description [%s]: ", entry->description);
    char newDesc[DESC_LENGTH];
    inputString(newDesc, sizeof(newDesc));
    if (strlen(newDesc) > 0)
    {
        strcpy(entry->description, newDesc);
    }

    if (saveEventAt(idx, &e))
    {
        printf("\033[1;32m[SUCCESS] Staff information updated successfully!\033[0m\n");
    }
    else
    {
        printf("\033[1;31m[ERROR] Could not save event data!\033[0m\n");
    }
}

// 2.7: Implementation of deleting staff
void deleteStaffFromEvent()
{
    char eventId[EVENT_ID_LENGTH];
    printDivider("DELETE STAFF FROM EVENT");
    printf("Enter Event ID (EVxxxxxx): ");
    inputString(eventId, sizeof(eventId));

    int idx = findEventIndexById(eventId);
    if (idx == -1)
    {
        printf("\033[1;31m[ERROR] Event not found!\033[0m\n");
        return;
    }

    Event e;
    loadEventAt(idx, &e);

    if (!canModifyStaff(&e))
    {
        printf("\033[1;31m[ERROR] Only allowed to delete when event is UPCOMING!\033[0m\n");
        return;
    }

    if (e.staffCount == 0)
    {
        printf("\033[1;33m[INFO] No staff members in this event.\033[0m\n");
        return;
    }

    // Display current staff
    printf("\nCurrent Staff List:\n");
    for (int i = 0; i < e.staffCount; i++)
    {
        User u;
        char name[NAME_LENGTH] = "Unknown";
        if (findUserById(e.staffList[i].studentId, &u))
        {
            strcpy(name, u.studentName);
        }
        printf("%d. [%s] %-20s | Role: %-8s | Mission: %s\n", i + 1, e.staffList[i].studentId, name, getRoleName(e.staffList[i].role), e.staffList[i].description);
    }

    printf("Enter the staff number to remove from the list above.\n");
    int choice;
    printf("\nSelect staff member to delete (1-%d) or 0 to cancel: ", e.staffCount);
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
        printf("[INFO] Deletion cancelled.\033[0m\n");
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
        printf("\033[1;32m[SUCCESS] Staff removed from event successfully!\033[0m\n");
    }
    else
    {
        printf("\033[1;31m[ERROR] Could not save event data!\033[0m\n");
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

void printEventRowRole(const Event *event, StaffRole role)
{
    // 1. Chuyển Role (từ số/enum) thành chuỗi
    const char *roleName;
    switch (role)
    {
        case STAFF_LEADER: roleName = "Leader"; break;
        case STAFF_MEMBER: roleName = "Member"; break;
        case STAFF_SUPPORT: roleName = "Support"; break;
        default: roleName = "Unknown"; break;
    }

    // 2. Chuyển Status (từ số/enum) thành chuỗi
    const char *statusName;
    switch (event->status) 
    {
        // LƯU Ý: Hãy thay 0, 1, 2 bằng đúng tên Enum/Số mà bạn định nghĩa trong struct Event
        case 0: statusName = "Upcoming"; break; 
        case 1: statusName = "Ongoing"; break;
        case 2: statusName = "Finished"; break; // Đây là trạng thái bạn đang cần lọc nè
        default: statusName = "Unknown"; break;
    }

    // 3. In ra màn hình (Dùng biến chuỗi statusName thay vì event->status)
    printf("%-30.30s | %-10s | %-12s\n",          
           event->name,
           roleName,
           statusName 
          );
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

#include <stdlib.h>
#include <string.h>

char *StudentIDInput() {
    char studentId[ID_LENGTH];
    printf("Enter Student ID (or press Enter to skip): ");
    
    // Giả sử inputString đã xử lý việc xóa ký tự '\n' ở cuối
    inputString(studentId, sizeof(studentId));

    if (strlen(studentId) == 0) {
        return NULL;
    }

    // Tự cấp phát và copy thay vì dùng strdup
    char *copy = (char *)malloc((strlen(studentId) + 1) * sizeof(char));
    if (copy == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    
    strcpy(copy, studentId);
    return copy;
}
void printEventList(MatchedEvent *list, int count, const char *studentId) 
{
    printf("\nEvent History for Member MSSV: %s\n", studentId);
    printf("----------------------------------------------------------\n");
    // total 58 chars
    printf("%-30s | %-10s | %-12s\n", "Event Name", "Role", "Status");
    printf("----------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++)
    {
        printEventRowRole(&list[i].event, list[i].studentRole);
    }
    printf("----------------------------------------------------------\n");
    printf("Total: %d event(s) found.\n", count);
}
void displayEventHistory(const char *studentId) {
    int count = 0;
    
   //get events by studentId
    MatchedEvent *historyList = getEventsByStudentId(studentId, &count);
    
    // print results or message if not found
    if (count > 0 && historyList != NULL) {
        printEventList(historyList, count, studentId);
    } else {
        printf("No events found for this student.\n");
    }
    
    //release memory
    if (historyList != NULL) {
        free(historyList);
    }
}
MatchedEvent* getEventsByStudentId(const char *studentId, int *outFoundCount) 
{
    *outFoundCount = 0; // Initialize output count to 0

    // Mở file
    FILE *f = fopen("data/events.dat", "rb");
    if (f == NULL) {
        return NULL; //check if file opened successfully
    }

    // allocate memory cho chunk
    Event *eventChunk = (Event *)malloc(CHUNK_SIZE * sizeof(Event));
    if (eventChunk == NULL) {
        fclose(f);
        return NULL; //check if memory allocated successfully
    }

   //allocate memory cho result array
    int capacity = 10;
    int foundCount = 0; 
    MatchedEvent *matchedList = (MatchedEvent *)malloc(capacity * sizeof(MatchedEvent));
    if (matchedList == NULL) {
        free(eventChunk);
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
                        printf("Warning: Memory limit reached. Results may be incomplete.\n");
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
