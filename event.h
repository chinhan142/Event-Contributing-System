#ifndef EVENT_H
#define EVENT_H

typedef struct
{
    char studentId[20];
    int role;
    char description[255];
} StaffEntry;

typedef struct
{
    char eventId[9];
    char name[255];
    char description[255];
    char location[255];
    char startDate[15];
    char endDate[15];
    int status;
    int staffCount;
    StaffEntry staffList[30];
} Event;

void createEvent();
#endif