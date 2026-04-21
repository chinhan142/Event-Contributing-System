#ifndef USER_H
#define USER_H

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

int findUserById(const char *id, User *result);
void searchUserByName(const char *name, User results[MAX_SEARCH_RESULTS], int *count);

#endif