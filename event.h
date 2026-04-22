#ifndef EVENT_H
#define EVENT_H

#include "user.h"

#define MAX_STAFF_PER_EVENT 30
#define EVENT_ID_LENGTH 10
#define DESC_LENGTH 255
#define LOCATION_LENGTH 255
#define DATE_LENGTH 11

typedef enum
{
    STATUS_UPCOMING = 0,
    STATUS_ONGOING = 1,
    STATUS_FINISHED = 2
} EventStatus;

typedef enum
{
    STAFF_LEADER = 0,
    STAFF_MEMBER = 1,
    STAFF_SUPPORT = 2
} StaffRole;

typedef struct
{
    char studentId[ID_LENGTH];
    StaffRole role; // Using StaffRole
    char description[DESC_LENGTH];
} StaffEntry;
typedef struct
{
    char eventId[EVENT_ID_LENGTH];
    char name[NAME_LENGTH];
    char description[DESC_LENGTH];
    char location[LOCATION_LENGTH];
    char startDate[DATE_LENGTH];
    char endDate[DATE_LENGTH];
    EventStatus status; // Using EventStatus
    int staffCount;
    StaffEntry staffList[MAX_STAFF_PER_EVENT];
} Event;

// Case 5: View all events Function
int inputEventStatus();
void displayAllEvent(int filterStatus);

#endif
