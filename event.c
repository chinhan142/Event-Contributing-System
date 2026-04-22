#include "event.h"
#include "colors.h"
#include "utils.h"
#include "fileio.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

//Returns the difference in seconds between now and the specified date (start date and end date)
int checkTime(int year, int mon, int day){
    time_t now = time(NULL);
    struct tm info = {0};
    info.tm_year = year - 1900;
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
// Checks if two dates follow the yyyy/mm/dd format ensure the date is valid
int isValidDate(char *date){
    if(date[4] != '/' || date[7] != '/' || date[10] != '\0'){
        printf(RED "Input does not match the required format (yyyy/mm/dd) !\n" RESET);
        return 0;
    }

    int checkY = stoi(date,0,3);
    int checkM = stoi(date,5,6);
    int checkD = stoi(date,8,9); 
    if(checkM > 12 || checkM < 1){
        printf(RED "Invalid date, Please try again !\n"RESET);
        return 0;
    }
    else if(checkM == 4 || checkM == 6 || checkM == 9 || checkM == 11){
        if(1 <= checkD && checkD <= 30){
            return 1;
        }
        else {
            printf(RED "Invalid date, Please try again !\n"RESET);
            return 0;
        }
    }
    else if(checkM == 2){
        if(1 <= checkD && checkD <= (checkY % 4 == 0 && checkY % 100 != 0 ? 29 : 28)){
            return 1;
        }
        else{
            printf(RED "Invalid date, Please try again !\n"RESET);
            return 0;
        }
    }
    else{
        if(1 <= checkD && checkD <= 31){
            return 1;
        }
        else{
            printf(RED "Invalid date, Please try again !\n"RESET);
            return 0;
        }
    }
}
// Check if  Start Date ≤ End Date.
int isChronological(char *start, char *end){
    int year = strncmp(start, end, 4);
    int month = strncmp(start + 5, end + 5, 2);
    int day = strncmp(start + 8, end + 8, 2);

    if(year < 0) return 1;
    else if(year <= 0){
        if(month < 0) return 1;
        else if(month == 0){
            if(day <= 0) return 1;
            else{
                printf(RED "Invalid date, Please try again !\n" RESET);
                return 0;
            }
        }
        else return 0; 
    }
    return 0;
}
// Collects information to initialize a new Event and save it.
void createEvent(){
    Event newEvent;
    printf("Enter event's name: ");
    inputString(newEvent.name, sizeof(newEvent.name));  

    printf("Enter event's decription: ");
    inputString(newEvent.description, sizeof(newEvent.description));
    do{
        do{
            printf("Enter event's start date (yyyy/mm/dd): ");
            inputString(newEvent.startDate, sizeof(newEvent.startDate));
            getchar();
        } while(!isValidDate(newEvent.startDate));
        
        do{
            printf("Enter event's end date (yyyy/mm/dd): ");
            inputString(newEvent.endDate,sizeof(newEvent.endDate));
            getchar();
        } while(!isValidDate(newEvent.endDate));

    } while(!isChronological(newEvent.startDate,newEvent.endDate));
    
    printf("Enter event's location: ");
    inputString(newEvent.location,sizeof(newEvent.location));
    
    newEvent.staffCount = 0;
    updateStatus(&newEvent);
    int nextIndex = getNextEventIndex();
    int index = 7;
    while(nextIndex > 0){
        newEvent.eventId[index--] = nextIndex%10 + '0';
        nextIndex/=10;
    }
    for(int i = 0;i <= index;i++){
        switch (i)
        {
        case 0:
            newEvent.eventId[i] = 'E';
            break;
        case 1:
            newEvent.eventId[i] = 'V';
            break;
        default:
            newEvent.eventId[i] = '0';
            break;
        }
    }
    saveEventAt(nextIndex, &newEvent);

}
