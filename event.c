#include <event.h>
#include <stdio.h>
#include <utils.h>
#include <string.h>
#include <colors.h>

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
    newEvent.status = 1;
    

}
