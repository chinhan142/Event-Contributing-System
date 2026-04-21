#include <event.h>
#include <stdio.h>
#include <utils.h>
#include <string.h>
#include <colors.h>

int isValidDate(char *start, char *end){
    if(start[1] == '/' || start[4] == '/' || end[1] == '/' || end[4] == '/'){
        prinf(RED "Input does not match the required format (dd/mm/yy)");
    }
    int year = strncmp(start, end, 2);
    int month = strncmp(start + 3, end + 3, )
}

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
        printf("Enter event's start date (dd/mm/yy): ");
        inputString(newEvent.startDate, sizeof(newEvent.startDate));
        if(newEvent.startDate[1] == '/') formatDate(newEvent.startDate,0);
        if(newEvent.startDate[4] == '/') formatDate(newEvent.startDate,3);

        printf("Enter event's end date (dd/mm/yy): ");
        inputString(newEvent.endDate,sizeof(newEvent.endDate));
        if(newEvent.endDate[1] == '/') formatDate(newEvent.endDate,0);
        if(newEvent.endDate[4] == '/') formatDate(newEvent.endDate,3);
    } while()

    

}
