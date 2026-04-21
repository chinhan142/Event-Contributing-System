#ifndef AUTH_H
#define AUTH_H

#include "user.h"

#define MAX_LOGIN_ATTEMPTS 3

typedef struct
{
    char studentId[ID_LENGTH];
    char password[PASSWORD_LENGTH];
    int role; // 0=Member, 1=BCN
    int isLocked;
    int failCount;
} Account;



void updateAccount(Account *updated);
int findAccoutById(char *id, Account *result);
int loginAccount(char id[], char password[]);

#endif