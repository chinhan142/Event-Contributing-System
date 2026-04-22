#include <stdio.h>
#include <string.h>
#include "fileio.h"
#include "auth.h"
#include "user.h"
// Get the index of the next event record in the events.dat file
int getNextEventIndex()
{
    FILE *f = fopen("data/events.dat", "rb");
    if (f == NULL)
    {
        return 0;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);
    return (int)(size / sizeof(Event));
}

// Save an event struct at a specific record index
int saveEventAt(int index, Event *e)
{
    FILE *f = fopen("data/events.dat", "r+b");
    if (f == NULL)
    {
        f = fopen("data/events.dat", "wb");
        if (f == NULL)
        {
            return 0;
        }
    }
    fseek(f, index * sizeof(Event), SEEK_SET);
    fwrite(e, sizeof(Event), 1, f);
    fclose(f);
    return 1;
}

// Load an event struct from a specific record index
int loadEventAt(int index, Event *e)
{
    FILE *f = fopen("data/events.dat", "rb");
    if (f == NULL)
    {
        return 0;
    }
    fseek(f, index * sizeof(Event), SEEK_SET);
    int readCount = fread(e, sizeof(Event), 1, f);
    fclose(f);
    return (readCount > 0);
}

// Find the record index of an event by its ID
int findEventIndexById(const char *id)
{
    FILE *f = fopen("data/events.dat", "rb");
    if (f == NULL)
    {
        return -1;
    }
    
    Event temp;
    int index = 0;
    while (fread(&temp, sizeof(Event), 1, f))
    {
        if (strcmp(temp.eventId, id) == 0)
        {
            fclose(f);
            return index;
        }
        index++;
    }
    fclose(f);
    return -1;
}
