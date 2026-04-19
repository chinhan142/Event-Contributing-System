#ifndef AUTH_H
#define AUTH_H

typedef struct
{
    char studentId[9];
    char password[255];
    int role;
    int isLocked;
    int failCount;
} Account;

int loginAccount(char studentId[], char password[]);


#endif