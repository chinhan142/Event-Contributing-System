#ifndef FILEIO_H
#define FILEIO_H
#include "event.h"

int getNextEventIndex();
int saveEventAt(int index, Event *e);
int loadEventAt(int index, Event *e);
int findEventIndexById(const char *id);


#endif