#include <stdio.h>
#include <stdbool.h>
#include "auth.h"
#include "event.h"
#include "user.h"

bool loginStatus = false;

int main()
{

    printf("-- WELCOME TO EVENT CONTRIBUTING SYSTEM --\n");
    int choice;
    do
    {
        if (loginStatus == false)
        {
            printf("YOU'RE NOT LOGIN YET! PLEASE LOGIN NOW!");
            // routing to login logic
        }
        else
        {
            // print menu here, divide function for role
        }
    } while (choice != 0);

    return 0;
}