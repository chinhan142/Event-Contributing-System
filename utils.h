#ifndef UTILS_H
#define UTILS_H
#include "user.h"
// helper group
void inputString(char *str, int size);

void clearInputBuffer();
void clearScreen();
void printAppBanner();
void printDivider(char *title);
int  confirmAction(char *message);
void toLowerStr(char *dest, const char *src);
int stoi(char *str, int from, int to);
void toUpperStr(char *dest, const char *src);
int partitionByDate(MatchedEvent *arr, int low, int high);
void quicksortByDate(MatchedEvent *arr, int low, int high);
int partitionByIdAsc(MatchedEvent *arr, int low, int high);
void quicksortByIdAsc(MatchedEvent *arr, int low, int high);
int partitionByIdDesc(MatchedEvent *arr, int low, int high);
void quicksortByIdDesc(MatchedEvent *arr, int low, int high);
void sortUserEventsByName(MatchedEvent *events, int count);
void sortUserEventsByDate(MatchedEvent *events, int count);
void sortUserEventsByIdAsc(MatchedEvent *events, int count);
void sortUserEventsByIdDesc(MatchedEvent *events, int count);
int partitionByName(MatchedEvent *arr, int low, int high);
void quicksortByName(MatchedEvent *arr, int low, int high);
#endif