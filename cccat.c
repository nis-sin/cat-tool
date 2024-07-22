#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

void readStream(FILE* fp, char option);
unsigned int addLineCount(char* concatenateStr, unsigned int lineCount);

int main(int argc, char* argv[]){
    char* filenames[argc];
    bool fileFlag = false;
    int arg;
    char option = ' ';
    setlocale (LC_ALL, "");

    int i = 0;
    while ((arg = getopt(argc, argv, "-nb")) != -1){
        switch (arg){
            case 'n':
                option = 'n';
                break;
            case 'b':
                option = 'b';
                break;
            case 1:
                fileFlag = true;
                filenames[i] = optarg;
                i++;
                break;
            case '?':
                printf("Invalid option\n");
                break;
        }
    }
    
    FILE* fp = stdin;
    if (fileFlag == true){
        for (int j = 0; j < i; j++){
            if (strcmp(filenames[j], "-") != 0){
                fp = fopen(filenames[j], "r");
            }
            if (fp == NULL){
                perror("Error opening file.\n");
                return 1;
            }
            readStream(fp, option);
            fclose(fp);
            printf("\n");
        }
    }
    else{
        readStream(fp, option);
    }
    
    return 0;
}

void readStream(FILE* fp, char option){
    unsigned int lineCount = 0;
    wint_t charBuffer;
    char* prefix = (char*) malloc(1);
    char* suffix = (char*) malloc(1);
    bool newLine = true;

    while ((charBuffer = fgetwc(fp)) != WEOF){

        prefix = "";
        suffix = "";
        if (option == 'n'){
            if (lineCount == 0){
                addLineCount(prefix, lineCount);
                printf("PREFIX: %s", prefix);
            }
            else if (charBuffer == '\n'){
                lineCount++;
                int length = snprintf(NULL, 0, "%d", lineCount);        // https://stackoverflow.com/questions/8257714/how-can-i-convert-an-int-to-a-string-in-c
                suffix = (char*) malloc(length + 2);        // length + 2 because must accomodate for th null terminator and the tab character
                snprintf(suffix, length + 2, "%d\t", lineCount);
            }
        }

        else if (option == 'b'){
            if (newLine == true && !iswspace(charBuffer)){
                lineCount++;
                int length = snprintf(NULL, 0, "%d", lineCount);        // https://stackoverflow.com/questions/8257714/how-can-i-convert-an-int-to-a-string-in-c
                prefix = (char*) malloc(length + 2);        // length + 2 because must accomodate for th null terminator and the tab character
                snprintf(prefix, length + 2, "%d\t", lineCount);
                newLine = false;
            }

            if (charBuffer == '\n'){
                newLine = true;
            }
        }

        printf("%s%c%s", prefix, charBuffer, suffix);
    }

    if (!feof(fp)){
        perror("Error reading file");
    }
}

unsigned int addLineCount(char* concatenateStr, unsigned int lineCount){
    lineCount++;
    int length = snprintf(NULL, 0, "%d", lineCount);        // https://stackoverflow.com/questions/8257714/how-can-i-convert-an-int-to-a-string-in-c
    char* tempStr = (char*) malloc(length + 2);        // length + 2 because must accomodate for th null terminator and the tab character
    if (tempStr == NULL){
        perror("Error allocating memory");
        return 1;
    }
    snprintf(tempStr, length + 2, "%d\t", lineCount);
    printf("TEMPSTR: %s\n", tempStr);
    concatenateStr = (char*) realloc(concatenateStr, length + 2);
    memcpy(concatenateStr, tempStr, length + 2);
    printf("STR: %s\n", concatenateStr);
    free(tempStr);
    return lineCount;
}