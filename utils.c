#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"

void inputString(char *str, int size)
{
    fgets(str, size, stdin);
    str[strcspn(str, "\n")] = '\0';
}

void clearScreen()
{
    system("cls || clear");
}

void printDivider(char *title)
{
    printf("\n========================================\n");
    if (title != NULL)
        printf("  %s\n", title);
    printf("========================================\n");
}

int confirmAction(char *message)
{
    char choice[5];
    printf("%s (Y/N): ", message);
    inputString(choice, sizeof(choice));
    return (choice[0] == 'Y' || choice[0] == 'y');
}

void toLowerStr(char *dest, const char *src)
{
    for (int i = 0; src[i]; i++)
        dest[i] = tolower(src[i]);
    dest[strlen(src)] = '\0';
}

void bcnMenu()
{
    int choice;
    do
    {
        printDivider("BCN MENU");
        printf("  [Event Management]\n");
        printf("  1. Create new event\n");
        printf("  2. Edit event info\n");
        printf("  3. Delete event\n");
        printf("  4. Update event status\n");
        printf("  5. View all events\n");
        printf("  6. View event detail\n");
        printf("  7. Search event\n");
        printf("\n");
        printf("  [Staff Management]\n");
        printf("  8.  Add staff to event\n");
        printf("  9.  Edit staff role/task\n");
        printf("  10. Remove staff from event\n");
        printf("  11. View staff history\n");
        printf("\n");
        printf("  [Reports]\n");
        printf("  12. Staff statistics\n");
        printf("\n");
        printf("  [Account]\n");
        printf("  13. View my profile\n");
        printf("  14. Change password\n");
        printf("  15. Reset member password\n");
        printf("\n");
        printf("  0. Logout\n");
        printf("========================================\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline sau scanf

        switch (choice)
        {
        case 1:
            printf("[TODO] Create event\n");
            break;
        case 2:
            printf("[TODO] Edit event\n");
            break;
        case 3:
            printf("[TODO] Delete event\n");
            break;
        case 4:
            printf("[TODO] Update status\n");
            break;
        case 5:
            printf("[TODO] View all events\n");
            break;
        case 6:
            printf("[TODO] View event detail\n");
            break;
        case 7:
            printf("[TODO] Search event\n");
            break;
        case 8:
            printf("[TODO] Add staff\n");
            break;
        case 9:
            printf("[TODO] Edit staff role\n");
            break;
        case 10:
            printf("[TODO] Remove staff\n");
            break;
        case 11:
            printf("[TODO] Staff history\n");
            break;
        case 12:
            printf("[TODO] Statistics\n");
            break;
        case 13:
            printf("[TODO] View profile\n");
            break;
        case 14:
            printf("[TODO] Change password\n");
            break;
        case 15:
            printf("[TODO] Reset password\n");
            break;
        case 0:
            printf("Logging out...\n");
            break;
        default:
            printf("[!] Invalid choice!\n");
            break;
        }
    } while (choice != 0);
}

void staffMenu()
{
    int choice;
    do
    {
        printDivider("STAFF MENU");
        printf("  [My Events]\n");
        printf("  1. View current events\n");
        printf("  2. View event detail\n");
        printf("  3. View event history\n");
        printf("\n");
        printf("  [Account]\n");
        printf("  4. View my profile\n");
        printf("  5. Change password\n");
        printf("\n");
        printf("  0. Logout\n");
        printf("========================================\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            printf("[TODO] Current events\n");
            break;
        case 2:
            printf("[TODO] Event detail\n");
            break;
        case 3:
            printf("[TODO] Event history\n");
            break;
        case 4:
            printf("[TODO] View profile\n");
            break;
        case 5:
            printf("[TODO] Change password\n");
            break;
        case 0:
            printf("Logging out...\n");
            break;
        default:
            printf("[!] Invalid choice!\n");
            break;
        }
    } while (choice != 0);
}