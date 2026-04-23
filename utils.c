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

int stoi(char *str, int fromIndex, int toIndex){
    int result = 0;
    for(int i = fromIndex;i <= toIndex;i++){
        result *= 10;
        result += str[i] - '0';
    }
    return result;
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

int partitionByDate(MatchedEvent *arr, int low, int high)
{
    // Chọn pivot là phần tử cuối cùng
    MatchedEvent pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        // So sánh theo ngày (descending)
        if (strcmp(arr[j].event.startDate, pivot.event.startDate) > 0)
        {
            i++;
            // Swap arr[i] và arr[j]
            MatchedEvent temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    // Swap arr[i+1] và arr[high]
    MatchedEvent temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return i + 1;
}
