#include <stdio.h>
#include "event.h"
#include "ExtendMenu.h"
#include "utils.h"
#include "paths.h"
#include <string.h>
#include <stdlib.h>
#include "colors.h"
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
int partition(User *user,int l,int r){
    User pivot = user[r],temp;
    int j = l - 1;
    for(int i = l;i < r;i++){
        if(user[i].eventCount <= pivot.eventCount){
            j++;
            temp = user[i];
            user[i] = user[j];
            user[j] = temp;
        }
    }
    j++;
    user[r] = user[j];
    user[j] = pivot;
    return j;
}
void sort(User *user,int l,int r){
    if(l < r){
        int x = partition(user,l,r);
        sort(user,l,x - 1);
        sort(user,x + 1,r);
    }
}
void findTopParticipant(){
    FILE *f = fopen(USER_DATA_PATH, "r+b");
    if (f == NULL)
    {
        f = fopen(USER_DATA_PATH, "wb");
        if (f == NULL)
        {
            return;
        }
    }
    int index = 0;
    int size = 2;
    User *result = (User *)malloc(size*sizeof(User));
    if(result == NULL) return;
    
    while(fread(result + index,sizeof(User),1,f)){
        index++;
        if(index == size){
            size *= 2;
            result = (User *)realloc(result,size*sizeof(User));
        } 
    }
    if(index > 0){
        result = (User *)realloc(result,(index + 1)*sizeof(User));
    }
    sort(result,0,index - 1);
    printf("==========TOP PARTICIPANT==========\n");
    if(index == 0){
        printf("There are no staff\n");
    }
    if(index == 1){
        printf("1. %s : 1 event\n", result[0].studentName);
    }
    else{
        for(int i = index - 1;i >= 0;i--){
            printf("%d. %s : %d %s\n", index - i, result[i].studentName, result[i].eventCount, result[i].eventCount > 1 ? "events" : "event");
            if (i == 0 || result[i].eventCount != result[i - 1].eventCount){
                break;
            }
        }
    }
    free(result);
    fclose(f);
    
}
void getInactiveStaffInSemester(){
    FILE *f = fopen(USER_DATA_PATH, "r+b");
    if (f == NULL)
    {
        f = fopen(USER_DATA_PATH, "wb");
        if (f == NULL)
        {
            return;
        }
    }
    int index = 0;
    int size = 2;
    User *result = (User *)malloc(size*sizeof(User));
    if(result == NULL) return;
    
    while(fread(result + index,sizeof(User),1,f)){
        index++;
        if(index == size){
            size *= 2;
            result = (User *)realloc(result,size*sizeof(User));
        } 
    }
    if(index > 0){
        result = (User *)realloc(result,(index + 1)*sizeof(User));
    }
    int choice = -1;

    while (choice != 0)
    {
        printDivider("Choose Semester");
        printf("  1. Spring\n");
        printf("  2. Summer\n");
        printf("  3. Fall\n");
        printf("  0. Back\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n') {}
            printf("[!] Invalid input.\n");
            continue;
        }
        getchar(); /* clear '\n' */
        int cnt = 1;
        switch (choice)
        {
        case 1:
            for(int i = 0;i < index;i++){
                if(result[i].isSpringActive == 0){
                    printf("%d. : %s\n", cnt, result[i].studentName);
                    cnt++;
                }
            }
            break;
        case 2:
            for(int i = 0;i < index;i++){
                if(result[i].isSummerActive == 0){
                    printf("%d. : %s\n", cnt, result[i].studentName);
                    cnt++;
                }
            }
            break;
        case 3:
            for(int i = 0;i < index;i++){
                if(result[i].isFallActive == 0){
                    printf("%d. : %s\n", cnt, result[i].studentName);
                    cnt++;
                }
            }
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
            findTopParticipant();
            break;
        case 3:
            getInactiveStaffInSemester();
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
