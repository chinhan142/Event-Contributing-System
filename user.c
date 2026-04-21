#include <stdio.h>
#include <string.h>
#include "user.h"
#include "utils.h"

// Find a single user member by their exact MSSV
int findUserById(const char *id, User *result)
{
    FILE *f = fopen("data/users.dat", "rb");
    if (f == NULL)
    {
        return 0;
    }

    User temp;
    while (fread(&temp, sizeof(User), 1, f))
    {
        if (strcmp(temp.studentId, id) == 0)
        {
            if (result != NULL)
            {
                *result = temp;
            }
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

// Search for users by name or MSSV (partial match, case-insensitive)
void searchUserByName(const char *name, User results[MAX_SEARCH_RESULTS], int *count)
{
    FILE *f = fopen("data/users.dat", "rb");
    if (f == NULL)
    {
        *count = 0;
        return;
    }

    User temp;
    *count = 0;
    char searchLower[NAME_LENGTH], targetLower[NAME_LENGTH];
    toLowerStr(searchLower, name);

    while (fread(&temp, sizeof(User), 1, f))
    {
        toLowerStr(targetLower, temp.studentName);
        // Match name (substring) or MSSV (exact)
        if (strstr(targetLower, searchLower) != NULL || strcmp(temp.studentId, name) == 0)
        {
            results[*count] = temp;
            (*count)++;
            if (*count >= MAX_SEARCH_RESULTS)
            {
                break;
            }
        }
    }


    fclose(f);
}
