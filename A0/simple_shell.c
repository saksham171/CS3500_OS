#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    system("clear");
    while(1){
        FILE *fptr = fopen("commandHistory.txt","a");
        system("pwd | tr -d '\n'");
        printf(">");
        char str[10];
        scanf("%s",str);
        if(strcmp(str,"clear") == 0){
            system("clear");
        }
        else if(strcmp(str,"pwd") == 0){
            system("pwd");
        }
        else if(strcmp(str,"mkdir") == 0){
            system("mkdir dir");
        }
        else if(strcmp(str,"rmdir") == 0){
            system("rmdir dir");//- deletes empty directory
            //system("rm -d dir") - deleting empty directory using rm
            //system("rm -r dir") - deleting non-empty directories
        }
        else if(strcmp(str,"ls") == 0){
            system("ls");
        }
        else if(strcmp(str,"history") == 0){
            system("cat commandHistory.txt");
        }
        else if(strcmp(str,"exit") == 0){
            // fclose(fptr);
            remove("commandHistory.txt");
            exit(0);
        }
        fprintf(fptr , "%s\n",str);
        fclose(fptr);
    }
    return 0;
}
