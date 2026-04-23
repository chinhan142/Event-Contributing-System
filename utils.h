#ifndef UTILS_H
#define UTILS_H
#include "user.h"
// helper group
void inputString(char *str, int size);
void clearScreen();
void printDivider(char *title);
int  confirmAction(char *message);
void toLowerStr(char *dest, const char *src);
int stoi(char *str, int from, int to);
void toUpperStr(char *dest, const char *src);
int partitionByDate(MatchedEvent *arr, int low, int high);
void quicksortByDate(MatchedEvent *arr, int low, int high);
#endif