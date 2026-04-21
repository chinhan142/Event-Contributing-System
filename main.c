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

bool loginSession = false;

int main()
{
    char studentId[20];
    char password[255];

    // init sample account - using only once - after done using, comment this shit!
    initData();

    printf("-- WELCOME TO EVENT CONTRIBUTING SYSTEM --\n");
    printf("YOU'RE NOT LOGIN! LOGIN DOWN HERE!\n");
    while (!loginSession)
    {
        printf("Enter MSSV: ");
        inputString(studentId, sizeof(studentId));

        printf("Enter Password: ");
        inputString(password, sizeof(password));

        int loginStatus = loginAccount(studentId, password);
        if (loginStatus == LOGIN_FAILED)
        {
            printf("ACCOUNT NOT FOUND! REENTER!\n");
        }
        else if (loginStatus == LOGIN_LOCKED)
        {
            printf("THIS ACCOUNT IS LOCKED! CONTACT BCN\n");
        }
        else
        {
            loginSession = true;
            if (loginStatus == LOGIN_SUCCESS_BCN)
            {
                bcnMenu();
            }
            else
            {
                staffMenu();
            }
        }

    }

    return 0;
}