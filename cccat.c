#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

void readStream(FILE* fp);

int main(int argc, char* argv[]){
    char* filename;
    bool fileFlag = false;
    int option;

    while ((option = getopt(argc, argv, "-n")) != -1){
        printf("Option: %c\n", option);
        switch (option){
            case 'n':
                printf("Option n\n");
                break;
            case 1:
                fileFlag = true;
                filename = optarg;
                break;
            case '?':
                printf("Invalid option\n");
                break;
        }
    }
    
    if (fileFlag == false){
        printf("No file specified\n");
        return 1;
    }
    FILE* fp = fopen(filename, "r");
    readStream(fp);
    close(fp);
    
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