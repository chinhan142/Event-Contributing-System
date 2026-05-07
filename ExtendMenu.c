#include <stdio.h>
#include "event.h"
#include "ExtendMenu.h"
#include "utils.h"
#include "paths.h"
#include <string.h>
#include <stdlib.h>
#include "colors.h"
#include "user.h"
void searchEventMenuBCN()
{
    int choice = -1;

    while (choice != 0)
    {
        clearScreen();
        printDivider("EVENT SEARCH");
        
        printf(YELLOW BOLD "  [ SEARCH OPTIONS ]\n" RESET);
        printf(GREEN "  1." RESET " Search by Unique Event ID\n");
        printf(GREEN "  2." RESET " Search by Start Date Range\n");
        printf("\n");
        
        printf(RED BOLD "  0. Return to Dashboard\n" RESET);
        printf(CYAN "==================================================\n" RESET);
        printf(BOLD "Your Selection >> " RESET);

        if (scanf("%d", &choice) != 1)
        {
            if (feof(stdin)) return;
            clearInputBuffer();
            printf(RED BOLD "[!] " RESET "Invalid input. Please enter a number.\n");
            continue;
        }
        getchar(); /* clear '\n' */

        switch (choice)
        {
        case 1:
            // printf("Search event\n");
            printEventResult();
            break;
        case 2:
            searchEventsByStartDateRange();
            break;
        case 0:
            break;
        default:
            printf(RED BOLD "[!] " RESET "Invalid choice!\n");
            break;
        }

        if (choice == 1 || choice == 2)
        {
            printf(BOLD "Enter to continue..." RESET);
            int c = getchar();
            if (c == EOF) return;
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
        printf("%d. %s : %d %s\n", i, e.name, e.staffCount, e.staffCount > 1 ? "staffs" : "staff");
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
    int index = 0,cnt;
    int size = 2;
    User *result = (User *)malloc(size*sizeof(User));
    if(result == NULL) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    while(fread(result + index,sizeof(User),1,f)){
        result[index].isActiveInThisSemester = 0;
        MatchedEvent *list = getEventsByStudentId(result[index].studentId,&cnt);
        for(int i = 0;i < cnt;i++){
            char *startDate = list[i].event.startDate;
           
            int currentYear = t->tm_year + 1900;
            // if(stoi(startDate,0,3) == 2026){
            //     printf("%ld %ld",checkSemester(now), checkSemester(toTimestamp(&(list[i].event))));
            // }
            if(currentYear == stoi(startDate,0,3) && list[i].event.status != STATUS_UPCOMING){
                if(checkSemester(now) == checkSemester(toTimestamp(&(list[i].event)))){
                    result[index].isActiveInThisSemester = 1;
                }
            }
        } 
        if (list != NULL) {
            free(list);
        }
        index++;
        if(index == size){
            size *= 2;
            result = (User *)realloc(result,size*sizeof(User));
        }
    }
    if(index > 0){
        result = (User *)realloc(result,(index + 1)*sizeof(User));
    }
    int dem = 1;
    for(int i = 0;i < index;i++){
        if(result[i].isActiveInThisSemester == 0){
            printf("%d. %s\n", dem, result[i].studentName);
            dem++;
        }
    }
    free(result);
    
    
}
void generateStaffStatistics(){
    printDivider("Staff Count Per Event");
    displayStaffCountPerEvent();
    printDivider("Top Participant");
    findTopParticipant();
    printDivider("Inactive Staff In Semester");
    getInactiveStaffInSemester();
    printf("Enter to continue");
    getchar();
}
