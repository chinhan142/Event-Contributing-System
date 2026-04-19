#ifndef USER_H
#define USER_H
#include "event.h"

typedef struct
{
    char studentId[9];
    char studentName[255];
    char email[255];
    Event event[30];
} Staff;

#endif