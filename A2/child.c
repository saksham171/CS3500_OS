#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define REQUEST 100
#define PIVOT 200
#define LARGE 300
#define SMALL 400
#define READY 500
#define TERMINATE 600
int main(int argc, char* argv[]) {
    printf("Child process starts\n");
    
    char* readF = argv[1];
    char* writeF = argv[2];

    int a[5] = {-1 , -1 , -1 , -1 , -1};
    int check[5] = {0 , 0 , 0 , 0 , 0};
    int opcode;
    int fdr = open(readF, O_RDONLY , 0666);
    read(fdr, &opcode, sizeof(int));
    printf("Child id recieved: %d\n",opcode);
    if(opcode == 1){
        FILE* fl = fopen("data1.txt" , "r");
        for(int i = 0 ; i < 5 ; i++){
            fscanf(fl , "%d" , &a[i]);
        }
        fclose(fl);
    }
    else if(opcode == 2){
        FILE* fl = fopen("data2.txt" , "r");
        for(int i = 0 ; i < 5 ; i++){
            fscanf(fl , "%d" , &a[i]);
        }
        fclose(fl);
    }
    else if(opcode == 3){
        FILE* fl = fopen("data3.txt" , "r");
        for(int i = 0 ; i < 5 ; i++){
            fscanf(fl , "%d" , &a[i]);
        }
        fclose(fl);
    }
    else if(opcode == 4){
        FILE* fl = fopen("data4.txt" , "r");
        for(int i = 0 ; i < 5 ; i++){
            fscanf(fl , "%d" , &a[i]);
        }
        fclose(fl);
    }
    else if(opcode == 5){
        FILE* fl = fopen("data5.txt" , "r");
        for(int i = 0 ; i < 5 ; i++){
            fscanf(fl , "%d" , &a[i]);
        }
        fclose(fl);
    }
    int writebuf = READY;
    printf("Child %d sends READY\n",opcode);
    int fdw = open(writeF, O_WRONLY , 0666);
    write(fdw, &writebuf, sizeof(int));
    srand(time(NULL));
    int flag = 1;
    while(flag){
        int rep;
        read(fdr, &rep, sizeof(int));
        int piv;
        if(rep == REQUEST){   
            printf("Child %d recieves REQUEST\n" , opcode);
            int num = rand() % 5;
            int cnt = 0;
            for(int i = 0 ; i < 5 ; i++){
                if(check[i] == 0){
                    cnt++;
                }
            }
            int writebuf;
            if(cnt == 5) writebuf = -1;
            while(check[num] != 0){
                num = rand() % 5;
            }
            writebuf = a[num];
            printf("Child %d sends %d to parent\n" , opcode , writebuf);
            write(fdw, &writebuf, sizeof(int));          
        }
        else if(rep == PIVOT){
            int tmp = 0;
            write(fdw, &tmp, sizeof(int));
            read(fdr, &piv, sizeof(int));
            printf("Child %d recieves pivot %d\n" , opcode , piv);
            int cnt = 0;
            for(int i = 0 ; i < 5 ; i++){
                if(a[i] > piv && check[i] == 0){
                    cnt++;
                }
            }
            int writebuf = cnt;
            printf("Child %d replies %d to parent\n" , opcode , writebuf);
            write(fdw, &writebuf, sizeof(int));
            //flag = 0;
        }
        else if(rep == LARGE){
            int tmp = 0;
            for(int i = 0 ; i < 5 ; i++){
                if(a[i] > piv) check[i] = 1;
            }
            printf("Child %d recieved LARGE and executed it\n" , opcode);
            write(fdw, &tmp, sizeof(int));
        }
        else if(rep == SMALL){
            int tmp = 0;
            for(int i = 0 ; i < 5 ; i++){
                if(a[i] < piv) check[i] = 1;
            }
            printf("Child %d recieved SMALL and executed it\n" , opcode);
            write(fdw, &tmp, sizeof(int));
        }
        else if(rep == TERMINATE){
            flag = 0;
            printf("Child %d terminated\n", opcode);
        }
    }
    close(fdw);
    close(fdr);
    return 0;
}