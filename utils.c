#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"
#include "colors.h"

void inputString(char *str, int size)
{
    if (fgets(str, size, stdin) == NULL)
    {
        str[0] = '\0';
        return;
    }
    str[strcspn(str, "\r\n")] = '\0';
}

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n');
}

void pressEnterToContinue()
{
    printf(BOLD "\nPress Enter to continue..." RESET);
    clearInputBuffer();
}

int stoi(char *str, int fromIndex, int toIndex)
{
    int result = 0;
    for (int i = fromIndex; i <= toIndex; i++)
    {
        result *= 10;
        result += str[i] - '0';
    }
    return result;
}

void clearScreen()
{
    system("cls || clear");
}

// Modern UI Manchester United Banner
void printAppBanner()
{
    printf(BRIGHT_RED BOLD);
    printf("  /$$$$$$  /$$        /$$$$$$  /$$$$$$$  /$$     /$$ /$$      /$$  /$$$$$$  /$$   /$$ /$$   /$$ /$$   /$$ /$$   /$$ /$$$$$$$$ /$$$$$$$$ /$$$$$$$ \n");
    printf(" /$$__  $$| $$       /$$__  $$| $$__  $$|  $$   /$$/| $$$    /$$$ /$$__  $$| $$$ | $$| $$  | $$| $$$ | $$|_  $$_/|__  $$__/| $$_____/| $$__  $$\n");
    printf("| $$  \\__/| $$      | $$  \\ $$| $$  \\ $$ \\  $$ /$$/ | $$$$  /$$$$| $$  \\ $$| $$$$| $$| $$  | $$| $$$$| $$  | $$     | $$   | $$      | $$  \\ $$\n");
    printf("| $$ /$$$$| $$      | $$  | $$| $$$$$$$/  \\  $$$$/  | $$ $$/$$ $$| $$$$$$$$| $$ $$ $$| $$  | $$| $$ $$ $$  | $$     | $$   | $$$$$   | $$  | $$\n");
    printf("| $$|_  $$| $$      | $$  | $$| $$__  $$   \\  $$/   | $$  $$$| $$| $$__  $$| $$  $$$$| $$  | $$| $$  $$$$  | $$     | $$   | $$__/   | $$  | $$\n");
    printf("| $$  \\ $$| $$      | $$  | $$| $$  \\ $$    | $$    | $$\\  $ | $$| $$  | $$| $$\\  $$$| $$  | $$| $$\\  $$$  | $$     | $$   | $$      | $$  \\ $$\n");
    printf("|  $$$$$$/| $$$$$$$$|  $$$$$$/| $$  | $$    | $$    | $$ \\/  | $$| $$  | $$| $$ \\  $$|  $$$$$$/| $$ \\  $$ /$$$$$$   | $$   | $$$$$$$$| $$$$$$$/\n");
    printf(" \\______/ |________/ \\______/ |__/  |__/    |__/    |__/     |__/|__/  |__/|__/  \\__/ \\______/ |__/  \\__/|______/   |__/   |________/|_______/ \n");
    printf(RESET "\n");
}

void printDivider(char *title)
{
    printf(CYAN BOLD);
    printf("\n==================================================\n");
    if (title != NULL)
        printf("  %s\n", title);
    printf("==================================================\n");
    printf(RESET);
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

void toUpperStr(char *dest, const char *src)
{
    for (int i = 0; src[i]; i++)
        dest[i] = toupper(src[i]);
    dest[strlen(src)] = '\0';
}

void quicksortByDate(MatchedEvent *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partitionByDate(arr, low, high);
        quicksortByDate(arr, low, pi - 1);
        quicksortByDate(arr, pi + 1, high);
    }
}

void quicksortByIdDesc(MatchedEvent *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partitionByIdDesc(arr, low, high);
        quicksortByIdDesc(arr, low, pi - 1);
        quicksortByIdDesc(arr, pi + 1, high);
    }
}

void quicksortByIdAsc(MatchedEvent *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partitionByIdAsc(arr, low, high);
        quicksortByIdAsc(arr, low, pi - 1);
        quicksortByIdAsc(arr, pi + 1, high);
    }
}

int partitionByIdAsc(MatchedEvent *arr, int low, int high)
{
    // choose the last element as the pivot
    MatchedEvent pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        // compare the ID (ascending)
        if (strcmp(arr[j].event.eventId, pivot.event.eventId) < 0)
        {
            i++;
            // swap arr[i] and arr[j]
            MatchedEvent temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    // swap arr[i+1] and arr[high]
    MatchedEvent temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

int partitionByIdDesc(MatchedEvent *arr, int low, int high)
{
    // choose the last element as the pivot
    MatchedEvent pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        // compare the ID (descending)
        if (strcmp(arr[j].event.eventId, pivot.event.eventId) > 0)
        {
            i++;
            // swap arr[i] and arr[j]
            MatchedEvent temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    // swap arr[i+1] and arr[high]
    MatchedEvent temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

int partitionByDate(MatchedEvent *arr, int low, int high)
{
    // choose the last element as the pivot
    MatchedEvent pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        // compare the date (descending)
        if (strcmp(arr[j].event.startDate, pivot.event.startDate) > 0)
        {
            i++;
            // swap arr[i] and arr[j]
            MatchedEvent temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    // swap arr[i+1] and arr[high]
    MatchedEvent temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

int partitionByName(MatchedEvent *arr, int low, int high)
{
    MatchedEvent pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (strcmp(arr[j].event.name, pivot.event.name) < 0)
        {
            i++;
            MatchedEvent temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    MatchedEvent temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

void quicksortByName(MatchedEvent *arr, int low, int high)
{
    if (low < high)
    {
        int pi = partitionByName(arr, low, high);
        quicksortByName(arr, low, pi - 1);
        quicksortByName(arr, pi + 1, high);
    }
}



void sortUserEventsByName(MatchedEvent *events, int count)
{
    if (count > 1)
    {
        quicksortByName(events, 0, count - 1);
    }
}

void sortUserEventsByDate(MatchedEvent *events, int count)
{
    if (count > 1)
    {
        quicksortByDate(events, 0, count - 1);
    }
}

void sortUserEventsByIdAsc(MatchedEvent *events, int count)
{
    if (count > 1)
    {
        quicksortByIdAsc(events, 0, count - 1);
    }
}

void sortUserEventsByIdDesc(MatchedEvent *events, int count)
{
    if (count > 1)
    {
        quicksortByIdDesc(events, 0, count - 1);
    }
}