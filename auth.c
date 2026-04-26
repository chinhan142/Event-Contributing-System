#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "utils.h"

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
    {
        return 0;
    }

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
    {
        return LOGIN_FAILED;
    }

    if (foundAccount.isLocked == ACCOUNT_LOCKED)
    {
        return LOGIN_LOCKED;
    }

    if (strcmp(foundAccount.password, password) == 0)
    {
        foundAccount.failCount = 0;
        // updateAccount implementation -> update the failCount to 0
        updateAccount(&foundAccount);

        // Return success role
        if (foundAccount.role == ROLE_BCN)
        {
            return LOGIN_SUCCESS_BCN;
        }
        else
        {
            return LOGIN_SUCCESS_MEMBER;
        }
    }
    else
    {
        foundAccount.failCount++;
        if (foundAccount.failCount >= MAX_LOGIN_ATTEMPTS)
        {
            // updateAccount inside the .dat files
            foundAccount.isLocked = ACCOUNT_LOCKED;
            updateAccount(&foundAccount);
            return LOGIN_LOCKED;
        }
        updateAccount(&foundAccount);
        return LOGIN_FAILED;
    }

    return LOGIN_FAILED;
}

void changeOwnPassword(Account *acc)
{
    char oldPass[PASSWORD_LENGTH], newPass[PASSWORD_LENGTH],
        confirmPass[PASSWORD_LENGTH];

    printDivider("CHANGE PASSWORD");
    printf("Enter current password: ");
    inputString(oldPass, sizeof(oldPass));

    if (strcmp(acc->password, oldPass) != 0)
    {
        printf("\033[31m[ERROR] Wrong current password!\033[0m\n");
        return;
    }

    printf("Enter new password: ");
    inputString(newPass, sizeof(newPass));
    printf("Confirm new password: ");
    inputString(confirmPass, sizeof(confirmPass));

    if (strcmp(newPass, confirmPass) != 0)
    {
        printf("\033[31m[ERROR] New passwords do not match!\033[0m\n");
        return;
    }

    strcpy(acc->password, newPass);
    updateAccount(acc);

    printf("\033[32m[SUCCESS] Password changed successfully!\033[0m\n");
}

void resetMemberPassword()
{
    char targetId[ID_LENGTH];
    printDivider("RESET MEMBER PASSWORD");
    printf("Enter Member Student ID to reset: ");
    inputString(targetId, sizeof(targetId));

    Account targetAcc;
    if (findAccountById(targetId, &targetAcc))
    {
        strcpy(targetAcc.password, targetAcc.studentId);

        targetAcc.isLocked = ACCOUNT_UNLOCKED;
        targetAcc.failCount = 0;

        if (confirmAction("Are you sure you want to reset this password to default?"))
        {
            updateAccount(&targetAcc);
            printf("\033[32m[SUCCESS] Password for %s has been reset to default.\033[0m\n", targetId);
        }
    }
    else
    {
        printf("\033[31m[ERROR] Student ID not found!\033[0m\n");
    }
}
