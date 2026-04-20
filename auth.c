#include <stdio.h>
#include <string.h>
#include "auth.h"

// This function helps updating the value of Account attribute inside the .dat file.
// Why we need to have this function: Normally, if we perform straight update by using structName.attribute, then after the program exit, it will not save the updated value to the .dat file causing the logic of the program is wrong and does not meet the requirement of the given task.
void updateAccount(Account *updated)
{
    // r+b -> read and write in binary file
    // note that we're using .dat file is in binary file format
    FILE *f = fopen("data/accounts.dat", "r+b");

    if (f == NULL)
    {
        return;
    }

    Account temp;
    while (fread(&temp, sizeof(Account), 1, f))
    {
        if (strcmp(temp.studentId, updated->studentId) == 0)
        {
            fseek(f, -(long)sizeof(Account), SEEK_CUR);
            fwrite(updated, sizeof(Account), 1, f);
            break;
        }
    }

    fclose(f);
}

// This function helps find the account by ID inside the array of the Account struct
int findAccountById(char *id, Account *result)
{
    // open file in read binary mode
    FILE *f = fopen("data/accounts.dat", "rb");
    if (f == NULL)
        return 0;

    Account temp;
    // reading every line in the accounts.dat files
    while (fread(&temp, sizeof(Account), 1, f))
    {
        if (strcmp(temp.studentId, id) == 0)
        {
            *result = temp;
            fclose(f);
            return 1;
        }
    }

    // file must be closed when finishing!
    fclose(f);
    return 0;
}

// This function helps login the account of the user
int loginAccount(char id[], char password[])
{
    Account foundAccount;
    // account not found
    if (!findAccountById(id, &foundAccount))
        return -1;

    if (foundAccount.isLocked == 1)
        return -2;

    if (strcmp(foundAccount.password, password) == 0)
    {
        foundAccount.failCount = 0;
        // updateAccount implementation -> update the failCount to 0
        updateAccount(&foundAccount);
        return foundAccount.role;
    }
    else
    {
        foundAccount.failCount++;
        if (foundAccount.failCount >= 3)
        {
            // updateAccount inside the .dat files
            foundAccount.isLocked = 1;
            updateAccount(&foundAccount);
            return -2;
        }
        updateAccount(&foundAccount);
        return -1;
    }
}
