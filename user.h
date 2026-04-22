#ifndef USER_H
#define USER_H
#include "event.h" // for access Event struct, StaffRole
#define ID_LENGTH 20
#define NAME_LENGTH 255
#define EMAIL_LENGTH 255
#define PASSWORD_LENGTH 255
#define MAX_SEARCH_RESULTS 50

typedef struct
{
    char studentId[ID_LENGTH];
    char studentName[NAME_LENGTH];
    char email[EMAIL_LENGTH];
} User;

// Struct for In-memory cache + sort once time
typedef struct
{
    Event event;
    StaffRole studentRole; //curent user role in this event
} CacheEvent;

//Extern global cache
extern CacheEvent *eventCache;
extern int cacheSize;
extern int cacheCapacity;
extern int cacheInitialized;
void initializeEventCache();
int findUserById(const char *id, User *result);
void searchUserByName(const char *name, User results[MAX_SEARCH_RESULTS], int *count);
void displayEventHistory();
#endif