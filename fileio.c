#include <stdio.h>
#include <string.h>
#include "fileio.h"
#include "auth.h"
#include "user.h"
#include "event.h"
#include "paths.h"
// Get the index of the next event record in the events.dat file
int getNextEventIndex()
{
    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (f == NULL)
    {
        return 0;
    }
    fseeko64(f, 0, SEEK_END);
    long long size = ftello64(f);
    fclose(f);
    return (int)(size / sizeof(Event));
}

// Save an event struct at a specific record index
int saveEventAt(int index, Event *e)
{
    if (index < 0) return 0;
    FILE *f = fopen(EVENT_DATA_PATH, "r+b");
    if (f == NULL)
    {
        f = fopen(EVENT_DATA_PATH, "wb");
        if (f == NULL)
        {
            return 0;
        }
    }
    // Ép kiểu index sang long long để đảm bảo tính toán offset 64-bit
    fseeko64(f, (long long)index * sizeof(Event), SEEK_SET);
    fwrite(e, sizeof(Event), 1, f);
    fclose(f);
    return 1;
}
int saveUserAt(int index, User *user)
{
    if (index < 0) return 0;
    FILE *f = fopen(USER_DATA_PATH, "r+b");
    if (f == NULL)
    {
        f = fopen(USER_DATA_PATH, "wb");
        if (f == NULL)
        {
            return 0;
        }
    }
    // Ép kiểu index sang long long để đảm bảo tính toán offset 64-bit
    fseeko64(f, (long long)index * sizeof(User), SEEK_SET);
    fwrite(user, sizeof(User), 1, f);
    fclose(f);
    return 1;
}
int loadEventWithFile(FILE *f, int index, Event *e)
{
    if (f == NULL) return 0;
    
    //jump to the position of the record
    fseeko64(f, (long long)index * sizeof(Event), SEEK_SET);
    int readCount = fread(e, sizeof(Event), 1, f);
    
    return (readCount > 0);
}
// Load an event struct from a specific record index
int loadEventAt(int index, Event *e)
{
    if (index < 0) return 0;
    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (f == NULL)
    {
        return 0;
    }
    fseeko64(f, (long long)index * sizeof(Event), SEEK_SET);
    int readCount = fread(e, sizeof(Event), 1, f);
    fclose(f);
    return (readCount > 0);
}

// Find the record index of an event by its ID
int findEventIndexById(const char *id)
{
    if (id == NULL || id[0] == '\0')
    {
        return -1;
    }

    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (f == NULL)
    {
        return -1;
    }

    _fseeki64(f, 0, SEEK_END);
    long long size = _ftelli64(f);
    int total = (int)(size / sizeof(Event));

    int left = 0;
    int right = total - 1;
    Event temp;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        _fseeki64(f, (long long)mid * sizeof(Event), SEEK_SET);
        if (fread(&temp, sizeof(Event), 1, f) != 1)
        {
            fclose(f);
            return -1;
        }

        int cmp = strcasecmp(temp.eventId, id);

        if (cmp == 0)
        {
            fclose(f);
            return mid;
        }

        if (cmp < 0)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    fclose(f);
    return -1;
}

void deleteEventById(char *id){
    FILE *f = fopen(EVENT_DATA_PATH, "rb");
    if (f == NULL){
        return;
    }
    FILE *temp = fopen(TEMP_DATA_PATH, "wb");
    if (temp == NULL){
        fclose(f);
        return;
    }
    Event event;
    while(fread(&event,sizeof(Event),1,f)){
        // Đã sửa: Dùng strcasecmp để đồng bộ với hàm tìm kiếm
        if(strcasecmp(event.eventId,id) == 0) continue;
        fwrite(&event,sizeof(Event),1,temp);
    }
    fclose(f);
    fclose(temp);
    
    if (remove(EVENT_DATA_PATH) == 0) {
        rename(TEMP_DATA_PATH, EVENT_DATA_PATH);
    }
}

