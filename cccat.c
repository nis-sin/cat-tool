#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

void readStream(FILE* fp);

int main(int argc, char* argv[]){
    char* filenames[argc];
    bool fileFlag = false;
    int option;

    int i = 0;
    while ((option = getopt(argc, argv, "-nb")) != -1){
        switch (option){
            case 'n':
                printf("Option n\n");
                break;
            case 'b':
                printf("Option b\n");
                break;
            case 1:
                fileFlag = true;
                filenames[i] = optarg;
                break;
            case '?':
                printf("Invalid option\n");
                break;
        }
        i++;
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
            readStream(fp);
            fclose(fp);
            printf("\n");
        }
    }
    else{
        readStream(fp);
    }
    
    return 0;
}

void readStream(FILE* fp){
    char buffer[1024];
    while (fgets(buffer,1024, fp) != NULL){
        printf("%s", buffer);
    }
    if (!feof(fp)){
        perror("Error reading file");
    }
}