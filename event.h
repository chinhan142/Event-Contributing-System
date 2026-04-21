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

typedef struct
{
    char studentId[ID_LENGTH];
    int role;
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
    int status; // Will use EventStatus values
    int staffCount;
    StaffEntry staffList[MAX_STAFF_PER_EVENT];
} Event;


#endif