#ifndef EVENT_H
#define EVENT_H

typedef struct
{
    char studentId[9];
    int role;
    char description[255];
} StaffEntry;

typedef struct
{
    char eventId[9];
    char name[255];
    char description[255];
    char location[255];
    char startDate[11];
    char endDate[11];
    int status;
    int staffCount;
    StaffEntry staffList[30];
} Event;

#endif