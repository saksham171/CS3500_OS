#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define REQUEST 100
#define PIVOT 200
#define LARGE 300
#define SMALL 400
#define READY 500
#define TERMINATE 600

int main() {
    char *writefifo[5] = {"Child_1_W","Child_2_W","Child_3_W","Child_4_W","Child_5_W"};
    char *readfifo[5] = {"Child_1_R","Child_2_R","Child_3_R","Child_4_R","Child_5_R"};
    
    int fdw[5];
    int fdr[5];

    for(int i = 0 ; i < 5 ; i++){
        mkfifo(writefifo[i], 0666);
        mkfifo(readfifo[i], 0666);
    }

    pid_t child[5];
    // fork and exec of the child process
    for (int i = 0; i < 5; i++) {
        child[i] = fork();
        if(child[i] == 0){
            char *args[] = {"./child" , writefifo[i], readfifo[i], NULL};
            execvp(args[0], args);
            exit(-1);
        }
    }
    printf("Parent process starts\n");
    printf("Parent allots id's to child processes\n");
    for(int i = 0 ; i < 5 ; i++)
    {
        // Send the message to child - write into writefifo
        int dat = (i + 1);
        fdw[i] = open(writefifo[i], O_WRONLY , 0666);
        write(fdw[i], &dat, sizeof(int));
        // Receive the message from child - To read from readfifo
        int val;
        fdr[i] = open(readfifo[i], O_RDONLY , 0666);
        read(fdr[i], &val, sizeof(int));
    }
    printf("Parent process READY\n");
    int med = 12;
    while(1){
        int req = -1;
        srand(time(NULL));
        while(req < 0){
            int id = rand() % 5;
            
            int x = REQUEST;
            printf("Parent sends REQUEST to child %d\n",(id + 1));
            write(fdw[id], &x, sizeof(int));
            
            int val;
            read(fdr[id], &val, sizeof(int));
            req = val;
        }
        int pivot = req;
        int cur = 0;
        int piv = PIVOT;
        printf("Parent broadcasts pivot %d to all children\n",pivot);
        for(int i = 0 ; i < 5 ; i++){
            write(fdw[i] , &piv , sizeof(int));
            int tmp;
            read(fdr[i] , &tmp , sizeof(int));
            write(fdw[i] , &pivot , sizeof(int));
            int val;
            read(fdr[i] , &val , sizeof(int));
            cur += val; 
        }
        printf("Parent returns %d as sum total of child responses\n",cur);
        if(cur == med){
            printf("Median Found: %d\n",pivot);
            int term = TERMINATE;
            printf("Parent sends kill signal to all children\n");
            int val;
            for(int i = 0 ; i < 5 ; i++){
                write(fdw[i] , &term , sizeof(int));
                read(fdr[i] , &val , sizeof(int));
            }
            break;
        }
        else if(cur > med){
            int sm = SMALL;
            printf("Parent sends SMALL to all children\n");
            for(int i = 0 ; i < 5 ; i++){
                write(fdw[i] , &sm , sizeof(int));
            }
            for(int i = 0 ; i < 5 ; i++){
                int val;
                read(fdr[i] , &val , sizeof(int));
            }
            printf("SMALL executed by all children\n");
        }
        else{
            int lg = LARGE;
            printf("Parent sends LARGE to all children\n");
            med = med - cur;
            for(int i = 0 ; i < 5 ; i++){
                write(fdw[i] , &lg , sizeof(int));
            }
            for(int i = 0 ; i < 5 ; i++){
                int tmp;
                read(fdr[i] , &tmp , sizeof(int));
            }
            printf("Median updated to %d\n",med);
            printf("LARGE executed by all children\n");
        }
    }
    for(int i = 0 ; i < 5 ; i++){
        close(fdr[i]);
        close(fdw[i]);
    }
    return 0;
}