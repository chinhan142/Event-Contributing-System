#include "auth.h"
#include "user.h"
#include <stdio.h>
#include <string.h>
#include "init.h"
#include "utils.h"
#include <stdlib.h>
void viewProfile(const Account *acc){
    char tempId[strlen(acc->studentId)];
    User persona;
    int userFound = 0;
    FILE *f = fopen("data/users.dat", "rb");
    if (f == NULL)
    {
        printf("[ERROR] Cannot open users.dat file\n");
        return;
    }
    while (fread(&persona, sizeof(tempId), 1, f) == 1)
    {
        if (strcmp(persona.studentId, (acc->studentId)+1) == 0)
        {
            userFound = 1;
            break;
        }
    }
    if (userFound == 0){
        printf("[ERROR] How did we get here? This should never happen.\n");
        system("pause");
        fclose(f);
        return;
    }
    printf("================== PROFILE ==================\n");
    if(userFound ==1){
        printf("Student ID: %s\n", persona.studentId);
        printf("Name: %s\n", persona.studentName);
        printf("Email: %s\n", persona.email);
        printf("Phone Number: %s\n", persona.phoneNumber);
        printf("Position:%s\n", (acc->role == 1 ? "BCN" : "Member"));
        printf("Specialize: %s\n", persona.specialize);
        system("pause");
    } else {
        printf("[ERROR] How did we get here?.\n");
        system("pause");
    }
    printf("============================================\n");
   
    fclose(f);
    clearScreen();
}