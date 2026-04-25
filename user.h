#ifndef USER_H
#define USER_H
#include <stddef.h>
#include "event.h" // for access Event struct, StaffRole
typedef struct Account Account;  // Forward declaration to avoid circular includes
#define ID_LENGTH 20
#define NAME_LENGTH 255
#define EMAIL_LENGTH 255
#define PASSWORD_LENGTH 255
#define MAX_SEARCH_RESULTS 50
#define phoneNumber_LENGTH 20
#define specialize_LENGTH 100
#define CHUNK_SIZE 1000

int findStaffInEventUser(const Event *event, const char *studentId, StaffRole *role);
void cleanUserEventData(Event *event);
void printUserEventRowRole(const Event *event, StaffRole role);
typedef struct
{
    char studentId[ID_LENGTH];
    char studentName[NAME_LENGTH];
    char email[EMAIL_LENGTH];
    char phoneNumber[phoneNumber_LENGTH];
    char specialize[specialize_LENGTH];
} User;

typedef struct
{
    Event event;
    StaffRole studentRole; // curent user role in this event
} MatchedEvent;


int findUserById(const char *id, User *result);
void searchUserByName(const char *name, User results[MAX_SEARCH_RESULTS], int *count);
void displayEventHistory(const char *studentId);
void displayCurrentUserEventHistory(const Account *acc);
MatchedEvent *getEventsByStudentId(const char *studentId, int *outFoundCount);
void viewCurrentEvents(const Account *acc, int wait);
void viewProfile(const Account *acc);
void viewUserEventDetails(const Account *acc, const char *eventId);
void userEventDetails(const Account *acc, const char *eventId);
#endif