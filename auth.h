#ifndef AUTH_H
#define AUTH_H

typedef struct
{
    char studentId[20];
    char password[255];
    int role;
    int isLocked;
    int failCount;
} Account;

void updateAccount(Account *updated);
int findAccoutById(char *id, Account *result);
int loginAccount(char id[], char password[]);

#endif