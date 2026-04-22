#ifndef AUTH_H
#define AUTH_H

#include "user.h"

#define MAX_LOGIN_ATTEMPTS 3

typedef enum
{
    LOGIN_FAILED = -1,
    LOGIN_LOCKED = -2,
    LOGIN_SUCCESS_MEMBER = 0,
    LOGIN_SUCCESS_BCN = 1
} LoginStatus;

typedef enum
{
    ROLE_MEMBER = 0,
    ROLE_BCN = 1
} AccountRole;

typedef enum
{
    ACCOUNT_UNLOCKED = 0,
    ACCOUNT_LOCKED = 1
} AccountLockState;

typedef struct
{
    char studentId[ID_LENGTH];
    char password[PASSWORD_LENGTH];
    AccountRole role;
    AccountLockState isLocked;
    int failCount;
} Account;

void updateAccount(Account *updated);
int findAccountById(char *id, Account *result);
int loginAccount(char id[], char password[]);

#endif