#ifndef FILEIO_H
#define FILEIO_H
#include <time.h>
#include "event.h"
#include "user.h"
#include <stdio.h>
int getNextEventIndex();
int saveEventAt(int index, Event *e);
int loadEventAt(int index, Event *e);
int findEventIndexById(const char *id);
void deleteEventById(char *id);
int saveUserAt(int index, User *user);
int loadEventWithFile(FILE *f, int index, Event *e);// improve version of loadEventAt that takes an already opened file pointer, to avoid reopening file multiple times when searching by ID
#endif