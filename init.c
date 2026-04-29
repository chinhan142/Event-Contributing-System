#include "init.h"
#include "auth.h"
#include "user.h"
#include "event.h"
#include <stdio.h>
#include <string.h>
#include "paths.h"

static void initAccountData()
{
    FILE *check = fopen(ACCOUNT_DATA_PATH, "rb");
    if (check != NULL)
    {
        fclose(check);
        return;
    }

    FILE *f = fopen(ACCOUNT_DATA_PATH, "wb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot create accounts.dat file\n");
        return;
    }

    Account bcn;
    strcpy(bcn.studentId, "admin");
    strcpy(bcn.password, "admin");
    bcn.role = 1;
    bcn.isLocked = 0;
    bcn.failCount = 0;

    Account user_1;
    strcpy(user_1.studentId, "SE111111");
    strcpy(user_1.password, "SE111111");
    user_1.role = 0;
    user_1.isLocked = 0;
    user_1.failCount = 0;

Account user_2;
    strcpy(user_2.studentId, "SE203237");
    strcpy(user_2.password, "SE203237");
    user_2.role = 0;
    user_2.isLocked = 0;
    user_2.failCount = 0;

    fwrite(&bcn, sizeof(Account), 1, f);
    fwrite(&user_1, sizeof(Account), 1, f);
    fwrite(&user_2, sizeof(Account), 1, f);
    fclose(f);
    printf("[INFO] Created default accounts successfully.\n");
}

static void initUserPersonaData()
{
    FILE *check = fopen(USER_DATA_PATH, "rb");
    if (check != NULL)
    {
        fclose(check);
        return;
    }

    FILE *f = fopen(USER_DATA_PATH, "wb");
    if (f == NULL)
    {
        return;
    }

    User samples[] = {
        {"SE203237", "Bui Pham Chi Nhan", "nhanbpc@fpt.edu.vn", "", ""},
        {"SE200441", "Tran Le Anh Quan", "quanta@fpt.edu.vn", "", ""},
        {"SE210556", "Vo Le Kien Huy", "huyvlk@fpt.edu.vn", "0123456789", "SoftwareDeveloper"},
        {"SE210041", "Tran Cao Thanh", "thanhct@fpt.edu.vn", "", ""},
        {"SE211059", "Nguyen Tan Loi", "loint@fpt.edu.vn", "", ""}};

    fwrite(samples, sizeof(User), sizeof(samples) / sizeof(samples[0]), f);
    fclose(f);
    printf("[INFO] Created default user persona data successfully.\n");
}

static void initMockEvents()
{
    // Check to see if events.dat already have data or not
    FILE *check = fopen(EVENT_DATA_PATH, "rb");
    if (check != NULL)
    {
        Event e;
        if (fread(&e, sizeof(Event), 1, check) == 1)
        {
            fclose(check);

            return;
        }
        fclose(check);
    }

    FILE *f = fopen(EVENT_DATA_PATH, "wb");
    if (f == NULL)
        return;

    Event samples[3] = {
        {.eventId = "EV000001",
         .name = "Workshop Lap trinh C",
         .description = "Huong dan co ban ve C",
         .location = "Hall A",
         .startDate = "2024-05-01",
         .endDate = "2024-05-02",
         .status = 0, // STATUS_UPCOMING
         .staffCount = 1,
         .staffList = {{"SE203237", 0, "Leader Workshop"}}},
        {.eventId = "EV000002",
         .name = "F-Code Teambuilding",
         .description = "Giao luu thanh vien",
         .location = "Vung Tau",
         .startDate = "2024-04-15",
         .endDate = "2024-04-17",
         .status = 1, // STATUS_ONGOING
         .staffCount = 1,
         .staffList = {{"SE200441", 1, "Hau can"}}},
        {.eventId = "EV000003",
         .name = "R.ODE Battle 2023",
         .description = "Cuoc thi code",
         .location = "P.402",
         .startDate = "2023-12-20",
         .endDate = "2023-12-21",
         .status = 2, // STATUS_FINISHED
         .staffCount = 0}};

    fwrite(samples, sizeof(Event), 3, f);
    fclose(f);
    printf("[INFO] Created 3 mock events for testing.\n");
}

void initializeSystem()
{
    // Ensure data directory exists could be done here if needed

    initAccountData();
    initUserPersonaData();
    initMockEvents();

    // Ensure events.dat exists
    FILE *fe = fopen(EVENT_DATA_PATH, "ab");
    if (fe != NULL)
    {
        fclose(fe);
    }
}
