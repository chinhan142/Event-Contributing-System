#ifndef STAFF_H
#define STAFF_H

#include "event.h"

// Task 2.5: logic + ui add staff to event
void addStaffToEvent();

// Helper: checking if a student is already assigned to an event
int isStaffInEvent(Event *e, const char *studentId);

#endif
