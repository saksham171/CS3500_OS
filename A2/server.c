// C Program for Server S2C queue
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>

// structure for message queue
struct msg{
    long type;
    char data[100];
};
  
int main()
{
    //key_t key;
    int S2C_id,C2S_id;
    ssize_t fd,rd,wr,cl;
    void* buffer = malloc(sizeof(ssize_t));
    int pid;
    struct msg message;
    // ftok to generate unique key
    //key = ftok("progfile", 65);
  
    // msgget creates a message queue
    // and returns identifier
    S2C_id = msgget((key_t) 66 , 0666 | IPC_CREAT);
    C2S_id = msgget((key_t) 65 , 0666 | IPC_CREAT);
    int flag = 1;
    while(flag == 1){
        // msgrcv to receive message
        msgrcv(C2S_id,(void*)&message,100, 0, 0);
        // display the message
        //printf("Data Received is : %s \n", message.data);
        // to destroy the message queue
        if(strncmp(message.data , "End" , 3) == 0){
            flag = 0;
            break;
        }
        else if(strncmp(message.data , "open" , 4) == 0) {
            fd = open("file.txt" , O_CREAT|O_RDWR , 0666);
            ssize_t* bu = malloc(sizeof(ssize_t));
            bu[0] = fd;
            msgsnd(S2C_id,bu, sizeof(bu) , 0);
        }
        else if(strncmp(message.data , "write" , 5) == 0){
            wr = write(fd , buffer , sizeof(buffer));
            ssize_t* bu = malloc(sizeof(ssize_t));
            bu[0] = wr;
            msgsnd(S2C_id,bu, sizeof(bu) , 0);
        }
        else if(strncmp(message.data , "getpid" , 6) == 0){
            pid = getpid();
            ssize_t* bu = malloc(sizeof(ssize_t));
            bu[0] = pid;
            msgsnd(S2C_id,bu, sizeof(bu) , 0);
        }
        else if(strncmp(message.data , "getuid" , 6) == 0){
            uid_t uid = getuid();
            ssize_t* bu = malloc(sizeof(ssize_t));
            bu[0] = uid;
            msgsnd(S2C_id,bu, sizeof(bu) , 0);
        }
        else if(strncmp(message.data , "time" , 4) == 0){
            time_t* loc = NULL;
            time_t timer = time(loc);
            ssize_t* bu = malloc(sizeof(ssize_t));
            bu[0] = timer;
            msgsnd(S2C_id,bu, sizeof(bu) , 0);
        }
        else printf("Invalid Command\n");  
    }
    msgctl(S2C_id, IPC_RMID, 0);
    msgctl(C2S_id, IPC_RMID, 0);
}