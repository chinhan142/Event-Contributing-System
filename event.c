#include "event.h"
#include "colors.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


int checkTime(int year, int mon, int day){
    time_t now = time(NULL);
    struct tm info = {0};
    info.tm_year = year - 100;
    info.tm_mon = mon - 1;
    info.tm_mday = day;
    info.tm_isdst = -1;
    return now - mktime(&info);
}

void updateStatus(Event *event){
    int startY = stoi(event->startDate,0,3);
    int startM = stoi(event->startDate,5,6);
    int startD = stoi(event->startDate,8,9);
    int endY = stoi(event->endDate,0,3);
    int endM = stoi(event->endDate,5,6);
    int endD = stoi(event->endDate,8,9);
    int start = checkTime(startY, startM, startD);
    int end = checkTime(endY, endM, endD);
    if(start < 0) event->status = STATUS_UPCOMING;
    else if(start >= 0 && end <= 0) event->status = STATUS_ONGOING;
    else event->status = STATUS_FINISHED;
}
// Checks if two dates follow the yyyy/mm/dd format and ensures Start Date ≤ End Date.
int isValidDate(char *start, char *end){
    if(start[4] != '/' || end[4] != '/' || start[7] != '/' || end[7] != '/' || start[10] != '\n' || end[10] != '\n'){
        printf(RED "Input does not match the required format (yyyy/mm/dd)");
        return 0;
    }

    int year = strncmp(start, end, 4);
    int month = strncmp(start + 5, end + 5, 2);
    int day = strncmp(start + 8, end + 8, 2);

    if(year < 0) return 1;
    else if(year <= 0){
        if(month < 0) return 1;
        else if(month == 0){
            if(day <= 0) return 1;
            else{
                printf(RED "Invalid date");
                return 0;
            }
        }
        else return 0; 
    }
    return 0;
}
// Collects information to initialize a new Event and save it.
void createEvent(){
    FILE *f = fopen("data/events.dat", "r+b");

    if (f == NULL)
    {
        return;
    }
    Event newEvent;
    printf("Enter event's name: ");
    inputString(newEvent.name, sizeof(newEvent.name)); 

    printf("Enter event's decription: ");
    inputString(newEvent.description, sizeof(newEvent.description));
    do{
        printf("Enter event's start date (yyyy/mm/dd): ");
        inputString(newEvent.startDate, sizeof(newEvent.startDate));

        printf("Enter event's end date (yyyy/mm/dd): ");
        inputString(newEvent.endDate,sizeof(newEvent.endDate));
    
    } while(isValidDate(newEvent.startDate,newEvent.endDate));
    printf("Enter event's location: ");
    inputString(newEvent.location,sizeof(newEvent.location));
    newEvent.staffCount = 0;
    updateStatus(&newEvent);
}
