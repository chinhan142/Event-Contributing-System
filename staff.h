#ifndef STAFF_H
#define STAFF_H

#include "event.h"

// Task 2.5: logic + ui add staff to event
void addStaffToEvent();

// Task 2.6: edit staff role/mission
void editStaffInEvent();

// Task 2.7: delete staff from event
void deleteStaffFromEvent();

// Helper: checking if a student is already assigned to an event
int isStaffInEvent(Event *e, const char *studentId);

// Helper: check if staff in event can be modified (only if status is UPCOMING)
int canModifyStaff(Event *e);

#endif
