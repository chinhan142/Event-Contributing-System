#ifndef FILEIO_H
#define FILEIO_H
#include <time.h>
#include "event.h"
#include "user.h"

int getNextEventIndex();
int saveEventAt(int index, Event *e);
int loadEventAt(int index, Event *e);
int findEventIndexById(const char *id);
void deleteEventById(char *id);
int saveUserAt(int index, User *user);
#endif