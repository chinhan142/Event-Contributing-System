#include <stdio.h>
#include <string.h>
#include "fileio.h"
#include "auth.h"

// this function helps initialize first account for admin and 1 example user
void initData()
{
    FILE *check = fopen("data/accounts.dat", "rb");
    if (check != NULL)
    {
        fclose(check);
        return;
    }

    FILE *f = fopen("data/accounts.dat", "wb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot create accounts.dat file\n");
    }

    Account bcn;
    strcpy(bcn.studentId, "admin");
    strcpy(bcn.password, "admin");
    bcn.role = 1;
    bcn.isLocked = 0;
    bcn.failCount = 0;

    Account user_1;
    strcpy(user_1.studentId, "SE111111");
    strcpy(user_1.password, "SE111111");
    user_1.role = 0;
    user_1.isLocked = 0;
    user_1.failCount = 0;

    fwrite(&bcn, sizeof(Account), 1, f);
    fwrite(&user_1, sizeof(user_1), 1, f);
    fclose(f);
    printf("[INFO] Created accounts.dat with default BCN account\n");
}