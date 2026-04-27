#ifndef STAFF_H
#define STAFF_H
#include "user.h"
#include "event.h"
#include <stddef.h>
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

void cleanEventData(Event *event);

void printEventRowRole(const Event *event, StaffRole role, const char *studentName);

void processChunk(Event *chunk, size_t eventsRead, const char *studentId, int *foundCount);

int findStaffInEvent(const Event *event, const char *studentId, StaffRole *role);

char *StudentIDInput();

void printEventList(MatchedEvent *list, int count, const char *studentId);

void displayEventHistory(const char *studentId);
MatchedEvent* getEventsByStudentId(const char *studentId, int *outFoundCount) ;
#endif
