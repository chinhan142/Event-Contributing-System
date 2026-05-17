#ifndef EVENT_H
#define EVENT_H
#include <time.h>
typedef struct Account Account;

#define ID_LENGTH 20
#define NAME_LENGTH 255
#define MAX_STAFF_PER_EVENT 30
#define EVENT_ID_LENGTH 10
#define DESC_LENGTH 255
#define LOCATION_LENGTH 255
#define DATE_LENGTH 20

//structures for event and staff related data
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
    StaffRole role;
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
    EventStatus status;
    int staffCount;
    StaffEntry staffList[MAX_STAFF_PER_EVENT];
    int isDeleted;
} Event;



//validation functions
int isValidDate(char *date);
int isChronological(char *start, char *end);
int checkTime(int year, int mon, int day);
int checkSemester(time_t timestampCheck);

//event related functions
void createEvent();
void updateStatus(Event *event);
int inputEventStatus();
void displayAllEvent(int filterStatus);

//event update functions
void updateEventDetails();
void updateName(Event *event);
void updateDescription(Event *event);
void updateLocation(Event *event);
void updateStartDate(Event *event);
void updateEndDate(Event *event);

//Event management functions
Event chooseEvent();
void deleteEvent();
void viewEventDetails();
void manualUpdateEventStatus();

//Searching funcion
void searchEventsByStartDateRange();

//others
time_t toTimestamp(Event *event);
void printEventByName();
#endif
