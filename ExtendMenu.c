#include <stdio.h>
#include "event.h"
#include "ExtendMenu.h"
#include "utils.h"
#include "paths.h"
void searchEventMenuBCN()
{
    int choice = -1;

    while (choice != 0)
    {
        printDivider("EXTEND MENU");
        printf("  1. Search by Event ID\n");
        printf("  2. Search by Start Date Range\n");
        printf("  0. Back\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n') {}
            printf("[!] Invalid input.\n");
            continue;
        }
        getchar(); /* clear '\n' */

        switch (choice)
        {
        case 1:
            printf("Search event\n");
            printEventResult();
            break;
        case 2:
            searchEventsByStartDateRange();
            break;
        case 0:
            break;
        default:
            printf("[!] Invalid choice!\n");
            break;
        }

        if (choice == 1 || choice == 2)
        {
            printf("Enter to continue...");
            getchar();
        }
    }
}

void  displayStaffCountPerEvent(){
    Event e;
    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (f == NULL)
    {
        return;
    }
    int i = 1;
    while(fread(&e, sizeof(Event), 1, f)){
        printf("%d : %s : %d %s\n", i, e.name, e.staffCount, e.staffCount > 1 ? "staffs" : "staff");
        i++;
    }
    fclose(f);
}
void findTopParticipant(){
    User temp,maxcnt;
    maxcnt.eventCount = -1;
    FILE *f = fopen(USER_DATA_PATH, "r+b");
    if (f == NULL)
    {
        f = fopen(USER_DATA_PATH, "wb");
        if (f == NULL)
        {
            return 0;
        }
    }
    // while(fread(&temp,sizeof(User),1,f)){
    //     if(temp.eventCount >)
    // }
}
void getInactiveStaffInSemester(){

}
void getStaffStatsByEvent(){
    int choice = -1;

    while (choice != 0)
    {
        printDivider("GET STAFF STATS");
        printf("  1. Display Staff Count Per Event\n");
        printf("  2. Find Top Participant\n");
        printf("  3. Get Inactive Staff In Semester\n");
        printf("  0. Back\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n') {}
            printf("[!] Invalid input.\n");
            continue;
        }
        getchar(); /* clear '\n' */

        switch (choice)
        {
        case 1:
            displayStaffCountPerEvent();
            break;
        case 2:
            
            break;
        case 3:
            break;
        case 0:
            break;
        default:
            printf("[!] Invalid choice!\n");
            break;
        }

        printf("Enter to continue...");
        getchar();
    }
}
