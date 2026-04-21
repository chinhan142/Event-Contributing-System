#include "init.h"
#include "auth.h"
#include "user.h"
#include "event.h"
#include <stdio.h>
#include <string.h>

static void initAccountData()
{
    FILE *check = fopen("data/accounts.dat", "rb");
    if (check != NULL)
    {
        fclose(check);
        return;
    }

    FILE *f = fopen("data/accounts.dat", "wb");
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

    fwrite(&bcn, sizeof(Account), 1, f);
    fwrite(&user_1, sizeof(Account), 1, f);
    fclose(f);
    printf("[INFO] Created default accounts successfully.\n");
}

static void initUserPersonaData()
{
    FILE *check = fopen("data/users.dat", "rb");
    if (check != NULL)
    {
        fclose(check);
        return;
    }

    FILE *f = fopen("data/users.dat", "wb");
    if (f == NULL)
    {
        return;
    }

    User samples[] = {
        {"SE203237", "Bui Pham Chi Nhan", "nhanbpc@fpt.edu.vn"},
        {"SE200441", "Tran Le Anh Quan", "quanta@fpt.edu.vn"},
        {"SE210556", "Vo Le Kien Huy", "huyvlk@fpt.edu.vn"},
        {"SE210041", "Tran Cao Thanh", "thanhct@fpt.edu.vn"},
        {"SE211059", "Nguyen Tan Loi", "loint@fpt.edu.vn"}
    };

    fwrite(samples, sizeof(User), sizeof(samples) / sizeof(samples[0]), f);
    fclose(f);
    printf("[INFO] Created default user persona data successfully.\n");
}

void initializeSystem()
{
    // Ensure data directory exists could be done here if needed

    initAccountData();
    initUserPersonaData();

    // Ensure events.dat exists
    FILE *fe = fopen("data/events.dat", "ab");
    if (fe != NULL)
    {
        fclose(fe);
    }
}

