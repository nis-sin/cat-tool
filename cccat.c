#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

void readStream(FILE* fp, char option);
char* addLineCount(unsigned int* lineCount);

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
            return 0;
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
    char* prefix = NULL;
    char* suffix = NULL;
    bool newLine = true;

    while ((charBuffer = fgetwc(fp)) != WEOF){

        prefix = "";
        suffix = "";
        if (option == 'n'){
            if (lineCount == 0){
                prefix = addLineCount(&lineCount);
            }
            else if (charBuffer == '\n'){
                suffix = addLineCount(&lineCount);
            }
        }

        else if (option == 'b'){
            if (newLine == true && !iswspace(charBuffer)){
                prefix = addLineCount(&lineCount);
                newLine = false;
            }

            if (charBuffer == '\n'){
                newLine = true;
            }
        }

        printf("%s%c%s", prefix, charBuffer, suffix);

        if (prefix != NULL){
            free(prefix);
        }
        if (suffix != NULL){
            free(suffix);
        }
    }

    if (!feof(fp)){
        perror("Error reading file");
    }
}

char* addLineCount(unsigned int* lineCount){
    *lineCount = *lineCount + 1;
    int length = snprintf(NULL, 0, "%d", *lineCount) + 2;        // https://stackoverflow.com/questions/8257714/how-can-i-convert-an-int-to-a-string-in-c
    char* tempStr = (char*) malloc(length);        // length + 2 because must accomodate for th null terminator and the tab character

    if (tempStr == NULL){
        perror("Error allocating memory");
        exit(1);
    }

    snprintf(tempStr, length, "%d\t", *lineCount);
    char* result = strdup(tempStr);             // https://stackoverflow.com/questions/252782/strdup-what-does-it-do-in-c
    free(tempStr);

    if (result == NULL){
        perror("Error allocating memory");
        exit(1);
    }

    return result;
}