#include "../init.h"
#include "../staff.h"
#include "../user.h"
#include <stdio.h>
#include <string.h>

static int assert_true(const char *label, int condition)
{
    if (condition)
    {
        printf("[PASS] %s\n", label);
        return 1;
    }

    printf("[FAIL] %s\n", label);
    return 0;
}

int main(void)
{
    initializeSystem();

    int passed = 0;
    int total = 0;

    printf("--- TEST MODULE: STAFF / USER LOOKUP ---\n");

    User user;
    User results[MAX_SEARCH_RESULTS];
    int count = 0;

    total++;
    if (findUserById("SE203237", &user))
    {
        passed += assert_true("findUserById finds SE203237", strcmp(user.studentName, "Bui Pham Chi Nhan") == 0);
    }
    else
    {
        printf("[FAIL] findUserById finds SE203237\n");
    }

    total++;
    searchUserById("se203237", results, &count);
    passed += assert_true("searchUserById is case-insensitive", count >= 1 && strcmp(results[0].studentId, "SE203237") == 0);

    total++;
    searchUserByName("tran le anh quan", results, &count);
    passed += assert_true("searchUserByName is case-insensitive", count >= 1 && strcmp(results[0].studentId, "SE200441") == 0);

    Event e;
    memset(&e, 0, sizeof(e));
    strcpy(e.staffList[0].studentId, "SE203237");
    e.staffList[0].role = STAFF_LEADER;
    e.staffCount = 1;

    total++;
    passed += assert_true("isStaffInEvent detects existing staff", isStaffInEvent(&e, "SE203237") == 1);

    total++;
    passed += assert_true("isStaffInEvent rejects missing staff", isStaffInEvent(&e, "SE999999") == 0);

    total++;
    passed += assert_true("canModifyStaff allows UPCOMING", canModifyStaff(&(Event){.status = STATUS_UPCOMING}) == 1);

    total++;
    passed += assert_true("canModifyStaff blocks ONGOING", canModifyStaff(&(Event){.status = STATUS_ONGOING}) == 0);

    printf("--- RESULT: %d/%d checks passed ---\n", passed, total);
    return (passed == total) ? 0 : 1;
}
