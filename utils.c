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
