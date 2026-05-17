#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "colors.h"
#include "utils.h"
#include "paths.h"

// This function helps updating the value of Account attribute inside the .dat file.
// Why we need to have this function: Normally, if we perform straight update by using structName.attribute, then after the program exit, it will not save the updated value to the .dat file causing the logic of the program is wrong and does not meet the requirement of the given task.
void updateAccount(Account *updated)
{
    // r+b -> read and write in binary file
    // note that we're using .dat file is in binary file format
    FILE *f = fopen(ACCOUNT_DATA_PATH, "r+b");

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
    FILE *f = fopen(ACCOUNT_DATA_PATH, "rb");
    if (f == NULL)
    {
        printf(RED BOLD "[ERROR] " RESET "Cannot open accounts.dat file\n");
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
    printf(BOLD "Enter current password: " RESET);
    inputString(oldPass, sizeof(oldPass));

    if (strcmp(acc->password, oldPass) != 0)
    {
        printf(RED BOLD "[ERROR] " RESET "Wrong current password!\n");
        return;
    }

    printf(BOLD "Enter new password: " RESET);
    inputString(newPass, sizeof(newPass));
    printf(BOLD "Confirm new password: " RESET);
    inputString(confirmPass, sizeof(confirmPass));

    if (strcmp(newPass, confirmPass) != 0)
    {
        printf(RED BOLD "[ERROR] " RESET "New passwords do not match!\n");
        return;
    }

    strcpy(acc->password, newPass);
    updateAccount(acc);

    printf(GREEN "[SUCCESS] " RESET "Password changed successfully!\n");
}

void resetMemberPassword()
{
    char targetId[ID_LENGTH];
    printDivider("RESET MEMBER PASSWORD");
    printf(BOLD "Enter Member Student ID to reset: " RESET);
    inputString(targetId, sizeof(targetId));

    Account targetAcc;
    if (findAccountById(targetId, &targetAcc))
    {
        strcpy(targetAcc.password, targetAcc.studentId);

        targetAcc.isLocked = ACCOUNT_UNLOCKED;
        targetAcc.failCount = 0;

        if (confirmAction(RED "Are you sure you want to reset this password to default?" RESET))
        {
            updateAccount(&targetAcc);
            printf(GREEN BOLD "[SUCCESS] " RESET "Password for %s has been reset to default.\n", targetId);
        }
    }
    else
    {
        printf(RED BOLD "[ERROR] " RESET "Student ID not found!\n");
    }
}

int logoutMain()
{
    if (confirmAction("Logging out?"))
    {
        return 1;
    }
    return 0;
}

// Validate student ID format: must be SE followed by 6 digits (SExxxxxx)
int validateStudentId(const char *id)
{
    if (strlen(id) != 8)
        return 0;
    
    if (id[0] != 'S' || id[1] != 'E')
        return 0;
    
    for (int i = 2; i < 8; i++)
    {
        if (id[i] < '0' || id[i] > '9')
            return 0;
    }
    
    return 1;
}

// Register a new account for club members
void registerAccount(void)
{
    char studentId[ID_LENGTH];
    char password[PASSWORD_LENGTH];
    char confirmPass[PASSWORD_LENGTH];
    char fullName[NAME_LENGTH];
    char email[EMAIL_LENGTH];
    Account newAccount;
    User newUser;
    
    printDivider("REGISTER NEW ACCOUNT");
    
    // Get student ID
    printf(BOLD "Enter Student ID (SExxxxxx format): " RESET);
    inputString(studentId, sizeof(studentId));
    
    // Validate format
    if (!validateStudentId(studentId))
    {
        printf(RED BOLD "[ERROR] " RESET "Invalid format! Student ID must be SExxxxxx (e.g., SE203237)\n");
        return;
    }
    
    // Check if account already exists
    Account tempAcc;
    if (findAccountById(studentId, &tempAcc))
    {
        printf(RED BOLD "[ERROR] " RESET "Account with Student ID %s already exists!\n", studentId);
        return;
    }
    
    // Get full name
    printf(BOLD "Enter Full Name: " RESET);
    inputString(fullName, sizeof(fullName));
    
    // Get email
    printf(BOLD "Enter Email: " RESET);
    inputString(email, sizeof(email));
    
    // Get password
    printf(BOLD "Enter password: " RESET);
    inputString(password, sizeof(password));
    
    printf(BOLD "Confirm password: " RESET);
    inputString(confirmPass, sizeof(confirmPass));
    
    if (strcmp(password, confirmPass) != 0)
    {
        printf(RED BOLD "[ERROR] " RESET "Passwords do not match!\n");
        return;
    }
    
    // Create new account with member role
    strcpy(newAccount.studentId, studentId);
    strcpy(newAccount.password, password);
    newAccount.role = ROLE_MEMBER;
    newAccount.isLocked = ACCOUNT_UNLOCKED;
    newAccount.failCount = 0;
    
    // Append to accounts.dat file
    FILE *f = fopen(ACCOUNT_DATA_PATH, "ab");
    if (f == NULL)
    {
        printf(RED BOLD "[ERROR] " RESET "Cannot open accounts.dat file\n");
        return;
    }
    
    if (fwrite(&newAccount, sizeof(Account), 1, f) != 1)
    {
        printf(RED BOLD "[ERROR] " RESET "Failed to save account\n");
        fclose(f);
        return;
    }
    
    fclose(f);
    
    // Create user profile with initial values
    strcpy(newUser.studentId, studentId);
    strcpy(newUser.studentName, fullName);
    strcpy(newUser.email, email);
    strcpy(newUser.phoneNumber, "");
    strcpy(newUser.specialize, "");
    newUser.eventCount = 0;
    newUser.isActiveInThisSemester = 1;  // Active by default
    
    // Append to users.dat file
    FILE *uf = fopen(USER_DATA_PATH, "ab");
    if (uf == NULL)
    {
        printf(RED BOLD "[ERROR] " RESET "Cannot open users.dat file\n");
        return;
    }
    
    if (fwrite(&newUser, sizeof(User), 1, uf) != 1)
    {
        printf(RED BOLD "[ERROR] " RESET "Failed to save user profile\n");
        fclose(uf);
        return;
    }
    
    fclose(uf);
    printf(GREEN BOLD "[SUCCESS] " RESET "Account and user profile registered successfully!\n");
    printf(GREEN "You can now log in with your Student ID: %s\n" RESET, studentId);
}
