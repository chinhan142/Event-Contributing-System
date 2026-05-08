#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "auth.h"
#include "event.h"
#include "fileio.h"
#include "report.h"
#include "staff.h"
#include "user.h"
#include "utils.h"
#include "menuBCN.h"
#include "menuStaff.h"
#include "init.h"
#include "colors.h"

bool loginSession = false;

int main()
{
    char studentId[20];
    char password[255];
    Account currentAcc; // track the current logged in account info for functions related to self account management

    // Initialize default(mocktest) datas and files
    initializeSystem();

    clearScreen();
    printAppBanner();
    
    while (!loginSession)
    {
        printf(YELLOW "\n[ LOGIN REQUIRED ]\n" RESET);
        printf(BOLD "Username : " RESET);
        inputString(studentId, sizeof(studentId));

        printf(BOLD "Password : " RESET);
        inputString(password, sizeof(password));

        int loginStatus = loginAccount(studentId, password);
        if (loginStatus == LOGIN_FAILED)
        {
            printf(RED BOLD "[!] " RESET "Account not found or wrong password! Please re-enter.\n");
            pressEnterToContinue();
        }
        else if (loginStatus == LOGIN_LOCKED)
        {
            printf(RED BOLD "[!] " RESET "This account is locked! Please contact BCN.\n");
            printf(RED BOLD "[!] " RESET "The program will now terminate for security reasons.\n");
            pressEnterToContinue();
            return 0; // Rule STT 0.1: Exit program
        }
        else
        {
            loginSession = true;
            if (!findAccountById(studentId, &currentAcc))
            {
                printf(RED "[ERROR] Cannot load account details.\n" RESET);
                return 1;
            }
            if (loginStatus == LOGIN_SUCCESS_BCN)
            {
                if (bcnMenu(&currentAcc))
                {
                    loginSession = false;
                    clearScreen();
                    printAppBanner();
                }
            }
            else
            {
                if (staffMenu(&currentAcc))
                {
                    loginSession = false;
                    clearScreen();
                    printAppBanner();
                }
            }
        }
    }

    return 0;
}