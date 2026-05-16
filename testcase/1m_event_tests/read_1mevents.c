#include "../../event.h"
#include "../../fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../paths.h"

#define FILE_PATH EVENT_1M_DATA_PATH

void test_search_performance_1m() {
    const char *targetId = "EV1000000";
    printf("[PERF TEST] Searching for %s in 1,000,000 events...\n", targetId);

    clock_t start = clock();
    
    FILE *f = fopen(FILE_PATH, "rb");
    if (!f) {
        printf("[ERROR] Could not open %s. Please run generate_1mevents first.\n", FILE_PATH);
        return;
    }

    Event temp;
    int index = -1;
    int i = 0;
    while (fread(&temp, sizeof(Event), 1, f)) {
        if (strcmp(temp.eventId, targetId) == 0) {
            index = i;
            break;
        }
        i++;
    }
    fclose(f);
    
    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    if (index != -1) {
        printf("PASSED: Found at index %d\n", index);
    } else {
        printf("FAILED: Event %s not found in %s.\n", targetId, FILE_PATH);
    }

    printf("Search Time: %.4f seconds\n", time_taken);
    
    if (time_taken > 1.0) {
        printf("[RESULT] Performance is POOR (> 1s). Optimization (Indexing) is highly recommended.\n");
    } else {
        printf("[RESULT] Performance is ACCEPTABLE.\n");
    }
}

int main() {
    test_search_performance_1m();
    return 0;
}
