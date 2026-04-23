#ifndef STAFF_H
#define STAFF_H

#include "event.h"

// Case 8: logic + ui add staff to event
void addStaffToEvent();

// Case 9: logic + ui edit staff role/mission
void editStaffInEvent();

// Case 10: logic + ui delete staff from event
void deleteStaffFromEvent();

// Helper: checking if a student is already assigned to an event
int isStaffInEvent(Event *e, const char *studentId);

// Helper: check if staff in event can be modified (only if status is UPCOMING)
int canModifyStaff(Event *e);

void viewMemberHistory();
#endif
