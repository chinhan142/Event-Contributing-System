#include "../../include/event.h"
#include "../../include/staff.h"
#include "../../include/user.h"
#include "../../include/auth.h"
#include "../../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *INPUT_FILE = "testcase/1m_event_tests/.bench_input.txt";

static int write_input_file(const char *content)
{
    FILE *f = fopen(INPUT_FILE, "wb");
    if (f == NULL)
    {
        return 0;
    }
    fwrite(content, 1, strlen(content), f);
    fclose(f);
    return 1;
}

static double benchmark_void_with_input(const char *label, void (*fn)(void), const char *input)
{
    if (!write_input_file(input))
    {
        printf("[ERROR] Cannot create benchmark input file for %s\n", label);
        return -1.0;
    }

    if (freopen(INPUT_FILE, "rb", stdin) == NULL)
    {
        printf("[ERROR] Cannot redirect stdin for %s\n", label);
        remove(INPUT_FILE);
        return -1.0;
    }

    clock_t start = clock();
    fn();
    clock_t end = clock();

    remove(INPUT_FILE);

    {
        double sec = (double)(end - start) / CLOCKS_PER_SEC;
        printf("[TIME] %-34s : %.4f s\n", label, sec);
        return sec;
    }
}

static double benchmark_display_event_history(const char *studentId)
{
    clock_t start = clock();
    displayEventHistory(studentId);
    clock_t end = clock();
    {
        double sec = (double)(end - start) / CLOCKS_PER_SEC;
        printf("[TIME] %-34s : %.4f s\n", "displayEventHistory(SE203237)", sec);
        return sec;
    }
}

static double benchmark_display_current_user_history(const Account *acc)
{
    clock_t start = clock();
    displayCurrentUserEventHistory(acc);
    clock_t end = clock();
    {
        double sec = (double)(end - start) / CLOCKS_PER_SEC;
        printf("[TIME] %-34s : %.4f s\n", "displayCurrentUserEventHistory", sec);
        return sec;
    }
}

static void run_all(void)
{
    Account acc;
    memset(&acc, 0, sizeof(acc));
    strcpy(acc.studentId, "SE203237");

    printf("=== 1M EVENTS BENCHMARK (MENU BCN CASE 7/11 + MENU STAFF CASE 2) ===\n");
    printf("Input assumptions:\n");
    printf("- printEventResult: ID = EV1000000\n");
    printf("- printEventByName: Name contains \"EVENT NUMBER 1000000\"\n");
    printf("- searchEventsByStartDateRange: 2030-01-01 -> 2030-12-31, filter All\n\n");

    benchmark_void_with_input("printEventResult (Case 7 - ID)", printEventResult, "EV1000000\n");
    benchmark_void_with_input("printEventByName (Case 7 - Name)", printEventByName, "EVENT NUMBER 1000000\n");
    benchmark_void_with_input("searchEventsByStartDateRange (Case 7 - Date)", searchEventsByStartDateRange, "2030-01-01\n2030-12-31\n0\n");
    benchmark_display_event_history("SE203237");
    benchmark_display_current_user_history(&acc);
}

int main(int argc, char *argv[])
{
    Account acc;
    memset(&acc, 0, sizeof(acc));
    strcpy(acc.studentId, "SE203237");

    if (argc < 2 || strcmp(argv[1], "all") == 0)
    {
        run_all();
        return 0;
    }

    if (strcmp(argv[1], "id") == 0)
    {
        benchmark_void_with_input("printEventResult (Case 7 - ID)", printEventResult, "EV1000000\n");
    }
    else if (strcmp(argv[1], "name") == 0)
    {
        benchmark_void_with_input("printEventByName (Case 7 - Name)", printEventByName, "EVENT NUMBER 1000000\n");
    }
    else if (strcmp(argv[1], "date") == 0)
    {
        benchmark_void_with_input("searchEventsByStartDateRange (Case 7 - Date)", searchEventsByStartDateRange, "2030-01-01\n2030-12-31\n0\n");
    }
    else if (strcmp(argv[1], "case11") == 0)
    {
        benchmark_display_event_history("SE203237");
    }
    else if (strcmp(argv[1], "case2") == 0)
    {
        benchmark_display_current_user_history(&acc);
    }
    else
    {
        printf("Usage: benchmark_menu_funcs_1m.exe [all|id|name|date|case11|case2]\n");
        return 1;
    }

    return 0;
}
