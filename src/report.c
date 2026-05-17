#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "report.h"
#include "event.h"
#include "fileio.h"
#include "paths.h"

#ifdef _WIN32
    #include <direct.h>
    #define mkdir(path, mode) _mkdir(path)
#else
    #include <sys/types.h>
#endif

// Ensure export directory exists
void ensureExportDirExists()
{
    mkdir("data/exports", 0755);
}

// Convert role enum to readable string
const char* getRoleName(StaffRole role)
{
    switch(role)
    {
        case STAFF_LEADER:
            return "Leader";
        case STAFF_MEMBER:
            return "Member";
        case STAFF_SUPPORT:
            return "Support";
        default:
            return "Unknown";
    }
}

// Get current timestamp as string
void getCurrentTimestamp(char *buffer, int size)
{
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// Write events to a text file
int writeEventsToFile(const char *filename, Event *eventList, int count)
{
    if (filename == NULL || eventList == NULL || count < 0)
    {
        return 0;
    }

    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        return 0;
    }

    char timestamp[30];
    getCurrentTimestamp(timestamp, sizeof(timestamp));

    // Write header
    fprintf(f, "========== EXPORTED EVENTS ==========\n");
    fprintf(f, "Export Time: %s\n", timestamp);
    fprintf(f, "Total Events: %d\n\n", count);

    // Write each event
    for (int i = 0; i < count; i++)
    {
        Event *e = &eventList[i];
        fprintf(f, "------- EVENT -------\n");
        fprintf(f, "Event Name: %s\n", e->name);
        fprintf(f, "Location: %s\n", e->location);
        fprintf(f, "Dates: %s - %s\n", e->startDate, e->endDate);
        fprintf(f, "\nStaff:\n");

        // Write staff members
        if (e->staffCount > 0)
        {
            for (int j = 0; j < e->staffCount; j++)
            {
                StaffEntry *staff = &e->staffList[j];
                fprintf(f, "  %s - Role: %s - Task: %s\n", 
                    staff->studentId, 
                    getRoleName(staff->role), 
                    staff->description);
            }
        }
        else
        {
            fprintf(f, "  (No staff assigned)\n");
        }

        fprintf(f, "\n");
    }

    // Write footer
    fprintf(f, "========== END OF EXPORT ==========\n");
    fclose(f);

    return 1;
}

// Export a single event by ID
int exportSingleEvent(const char *eventId)
{
    if (eventId == NULL || eventId[0] == '\0')
    {
        printf("Error: Invalid event ID\n");
        return 0;
    }

    // Ensure export directory exists
    ensureExportDirExists();

    // Find event by ID
    int index = findEventIndexById(eventId);
    if (index < 0)
    {
        printf("Error: Event not found\n");
        return 0;
    }

    // Load event
    Event event;
    if (!loadEventAt(index, &event))
    {
        printf("Error: Could not load event\n");
        return 0;
    }

    // Generate filename with timestamp
    char timestamp[20];
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", timeinfo);

    char filename[256];
    snprintf(filename, sizeof(filename), "data/exports/event_%s_%s.txt", eventId, timestamp);

    // Write to file
    if (!writeEventsToFile(filename, &event, 1))
    {
        printf("Error: Could not write to file\n");
        return 0;
    }

    printf("Export successful! File saved to: %s\n", filename);
    return 1;
}

// Export all events
int exportAllEvents()
{
    // Ensure export directory exists
    ensureExportDirExists();

    int totalEvents = getNextEventIndex();
    if (totalEvents <= 0)
    {
        printf("Error: No events to export\n");
        return 0;
    }

    // Allocate memory for all events
    Event *eventList = (Event *)malloc(totalEvents * sizeof(Event));
    if (eventList == NULL)
    {
        printf("Error: Could not allocate memory\n");
        return 0;
    }

    // Load all events
    int loadedCount = 0;
    for (int i = 0; i < totalEvents; i++)
    {
        if (loadEventAt(i, &eventList[loadedCount]))
        {
            if (eventList[loadedCount].isDeleted == 1) continue;
            loadedCount++;
        }
    }

    if (loadedCount == 0)
    {
        printf("Error: Could not load any events\n");
        free(eventList);
        return 0;
    }

    // Generate filename with timestamp
    char timestamp[20];
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", timeinfo);

    char filename[256];
    snprintf(filename, sizeof(filename), "data/exports/events_all_%s.txt", timestamp);

    // Write to file
    int result = writeEventsToFile(filename, eventList, loadedCount);

    if (result)
    {
        printf("Export successful! File saved to: %s\n", filename);
        printf("Total events exported: %d\n", loadedCount);
    }
    else
    {
        printf("Error: Could not write to file\n");
    }

    free(eventList);
    return result;
}
