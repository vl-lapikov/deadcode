#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void Usage(char *filename) {
    printf("Usage: %s <file> <string>\n", filename);
    printf("%s version 1.0 \nCopyright(c) CodingUnit.com\n", filename);
}

int Search_in_File(char *fname, char *str) {
    FILE *fp;
    int line_num = 1;
    int find_result = 0;
    char temp[512];
    
    if((fp = fopen(fname, "r")) == NULL) {
      return(-1);
    }

    while(fgets(temp, 512, fp) != NULL) {
        if((strstr(temp, str)) != NULL) {
            //printf("A match found on line: %d\n", line_num);
            //printf("\n%s\n", temp);
            find_result++;
        }
        line_num++;
    }

    if(find_result == 0) {
        printf("\nSorry, couldn't find a match.\n");
    }
    
    //Close the file if still open.
    if(fp) {
        fclose(fp);
    }
    return(0);
}

struct path {
    char *name;
    path **nodes;
};

//Our main function.
int main(int argc, char *argv[]) {
    int result, errno;

    path root, apps, cron, partners;

    root.name = "work";
    root.nodes = apps;
    

    return 0;

    if(argc < 3 || argc > 3) {
        Usage(argv[0]);
        exit(1);
    }

    //Use system("cls") on windows
    //Use system("clear") on Unix/Linux
    system("clear");
    int count = 1000;
    while (count--) {
        result = Search_in_File(argv[1], argv[2]);
        if(result == -1) {
            perror("Error");
            printf("Error number = %d\n", errno);
            exit(1);
        }
    }
    return(0);
}


