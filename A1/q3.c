#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
    int pipefd[2];
    char str[100];
    scanf("%s" , str);
    pipe(pipefd);
    pid_t op = fork();
    if(op > 0){
        char s[100];
        close(pipefd[1]);
        read(pipefd[0] , s , 100);
        printf("String read in parent process written in child process: %s\n" , s);
        close(pipefd[0]);
    }
    else if(op == 0){
        close(pipefd[0]);
        char s[100];
        for(int i = 0 ; i < 100 ; i++){
            s[i] = str[i];
        }
        write(pipefd[1] , s , 100);
        close(pipefd[1]);
        exit(0);
    }
    return 0;
}