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
#include "app.h"

bool loginSession = false;

int runApp(void)
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
        printf(YELLOW "\n[ LOGIN MENU ]\n" RESET);
        printf(BOLD "1. Login\n" RESET);
        printf(BOLD "2. Register\n" RESET);
        printf(BOLD "3. Exit\n" RESET);
        printf(BOLD "Choose an option (1-3): " RESET);
        
        int menuChoice;
        scanf("%d", &menuChoice);
        getchar(); // consume newline
        
        if (menuChoice == 2)
        {
            clearScreen();
            registerAccount();
            pressEnterToContinue();
            clearScreen();
            printAppBanner();
            continue;
        }
        else if (menuChoice == 3)
        {
            printf(YELLOW BOLD "\nThank you for using F-Code Event Staff Management System!\n" RESET);
            return 0;
        }
        else if (menuChoice != 1)
        {
            printf(RED BOLD "[ERROR] " RESET "Invalid option! Please try again.\n");
            pressEnterToContinue();
            clearScreen();
            printAppBanner();
            continue;
        }
        
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
                printf(RED BOLD "[!] " RESET "Cannot load account details.\n");
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