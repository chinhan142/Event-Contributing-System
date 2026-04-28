#include "../../event.h"
#include "../../fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../paths.h"

#define TARGET_COUNT 1000000
#define FILE_PATH EVENT_1M_DATA_PATH

int main() {
    printf("Generating %d events to %s...\n", TARGET_COUNT, FILE_PATH);

    FILE *file = NULL;
    int attempts = 0;

    // Try to open file, create directory if it fails
    while (file == NULL && attempts < 2) {
        file = fopen(FILE_PATH, "wb");
        if (file == NULL && attempts == 0) {
            printf("Could not open file. Attempting to create 'data' directory...\n");
            #ifdef _WIN32
                system("mkdir " DATA_DIR " 2>nul");
            #else
                system("mkdir -p " DATA_DIR);
            #endif
        }
        attempts++;
    }

    if (!file) {
        perror("Failed to open file for writing");
        return 1;
    }

    Event *e = malloc(sizeof(Event));
    if (!e) {
        printf("Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    for (int i = 1; i <= TARGET_COUNT; i++) {
        memset(e, 0, sizeof(Event));
        
        sprintf(e->eventId, "EV%07d", i);
        sprintf(e->name, "Event Number %d", i);
        strcpy(e->description, "Auto-generated test event for performance testing.");
        strcpy(e->location, "Virtual Location");
        strcpy(e->startDate, "2024-01-01");
        strcpy(e->endDate, "2024-12-31");
        e->status = (i % 3);
        e->staffCount = 0;

        if (fwrite(e, sizeof(Event), 1, file) != 1) {
            perror("Failed to write event");
            free(e);
            fclose(file);
            return 1;
        }

        if (i % 100000 == 0) {
            printf("Progress: %d/%d (%.0f%%)\n", i, TARGET_COUNT, (double)i / TARGET_COUNT * 100);
        }
    }

    free(e);
    fclose(file);
    printf("Done! Successfully generated %d events to %s.\n", TARGET_COUNT, FILE_PATH);
    return 0;
}
