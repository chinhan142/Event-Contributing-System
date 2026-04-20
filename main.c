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
        if (loginStatus == -1)
        {
            printf("ACCOUNT NOT FOUND! REENTER!\n");
        }
        else if (loginStatus == -2)
        {
            printf("THIS ACCOUNT IS LOCKED! CONTACT BCN\n");
        }
        else
        {
            loginSession = true;
            int role = loginStatus;
            if (role == 1)
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