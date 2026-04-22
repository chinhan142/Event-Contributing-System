#include "staff.h"
#include "event.h"
#include "fileio.h"
#include "user.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Check if a staff member with the given studentId is already in the event's
// staffList
int isStaffInEvent(Event *e, const char *studentId) {
  for (int i = 0; i < e->staffCount; i++) {
    if (strcmp(e->staffList[i].studentId, studentId) == 0) {
      return 1;
    }
  }
  return 0;
}

// 2.5: Implementation of adding staff
void addStaffToEvent() {
  char eventId[EVENT_ID_LENGTH];
  printDivider("ADD STAFF TO EVENT");
  printf("Enter Event ID (EVxxxxxx): ");
  inputString(eventId, sizeof(eventId));

  int idx = findEventIndexById(eventId);
  if (idx == -1) {
    printf("\033[1;31m[ERROR] Event not found!\033[0m\n");
    return;
  }

  Event e;
  loadEventAt(idx, &e);

  // Validate Status: Cannot add if event is finished
  if (e.status == STATUS_FINISHED) {
    printf("\033[1;31m[ERROR] Event has finished, cannot add staff!\033[0m\n");
    return;
  }

  // Validate Capacity: Max staff limit reached
  if (e.staffCount >= MAX_STAFF_PER_EVENT) {
    printf("\033[1;31m[ERROR] Event has reached maximum capacity (%d "
           "people)!\033[0m\n",
           MAX_STAFF_PER_EVENT);
    return;
  }

  char query[NAME_LENGTH];
  printf("Search Member (Name or ID): ");
  inputString(query, sizeof(query));

  User results[MAX_SEARCH_RESULTS];
  int count = 0;
  searchUserByName(query, results, &count);

  if (count == 0) {
    printf("\033[1;31m[ERROR] No matching members found!\033[0m\n");
    return;
  }

  // Display search results
  printf("\nSearch Results:\n");
  for (int i = 0; i < count; i++) {
    printf("%d. [%s] %s\n", i + 1, results[i].studentId,
           results[i].studentName);
  }

  int choice;
  printf("Select member (1-%d) or 0 to cancel: ", count);
  if (scanf("%d", &choice) != 1) {
    choice = 0;
  }
  getchar(); // Clear newline character from buffer

  if (choice < 1 || choice > count) {
    return;
  }

  User selected = results[choice - 1];

  // Validate Duplicate: Cannot add the same MSSV twice
  if (isStaffInEvent(&e, selected.studentId)) {
    printf("\033[1;31m[ERROR] This member is already in the event's staff "
           "list!\033[0m\n");
    return;
  }

  StaffEntry entry;
  strcpy(entry.studentId, selected.studentId);

  // Input role
  int tempRole;
  printf("Enter Role (%d: Leader, %d: Member, %d: Support): ", STAFF_LEADER,
         STAFF_MEMBER, STAFF_SUPPORT);
  if (scanf("%d", &tempRole) != 1) {
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
  if (saveEventAt(idx, &e)) {
    printf("\033[1;32m[SUCCESS] Staff added to event successfully!\033[0m\n");
  } else {
    printf("\033[1;31m[ERROR] Could not save event data!\033[0m\n");
  }
}
