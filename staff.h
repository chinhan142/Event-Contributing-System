#ifndef STAFF_H
#define STAFF_H
#include "user.h"
#include "event.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

// Helper: process a chunk of data read from file
void processChunk(Event *chunk, size_t eventsRead, const char *studentId, int *foundCount);


int findStaffInEvent(const Event *event, const char *studentId, StaffRole *role);

// Helper: input student ID with validation and option to skip
char *StudentIDInput();


void printEventList(MatchedEvent *list, int count, const char *studentId);

// Case 11: display event history for a student (both ongoing and finished events)
void displayEventHistory(const char *studentId);

// Helper: get all events that a student has participated in, along with their role in each event. It reads through the event data file in chunks, checks if the student is involved in each event, and if so, adds it to a dynamically allocated list of MatchedEvent. The function also handles memory allocation and returns the total count of matched events through an output parameter.
MatchedEvent* getEventsByStudentId(const char *studentId, int *outFoundCount) ;
#endif
