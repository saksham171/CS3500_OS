// C Program for client C2S queue
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
// structure for message queue
struct msg {
    long type;
    char data[100];
};

int main()
{
    //key_t key;
    int C2S_id,S2C_id;
    char buf[100];
    struct msg message;
    // ftok to generate unique key
    //key = ftok("progfile", 65);
  
    //msgget either returns the message queue identifier for a newly created message queue
    //or returns the identifier for a queue which exists with the same key value.
    C2S_id = msgget((key_t) 65 , 0666 | IPC_CREAT);
    S2C_id = msgget((key_t) 66 , 0666 | IPC_CREAT);
    message.type = 1;
    int flag = 1; 
    while(flag == 1){
        printf("Give Input Data : ");
        fgets(buf , 100 , stdin);
        strcpy(message.data , buf);
        if(strncmp(buf , "End" , 3) == 0){        
            msgsnd(C2S_id,(void*)&message, 100 , 0);
            ssize_t* bu = malloc(sizeof(ssize_t));
            msgrcv(S2C_id,(void*) bu,sizeof(bu), 0, 0);
            flag = 0;
            break;
        }
        // msgsnd to send message
        msgsnd(C2S_id,(void*)&message, 100 , 0);
        ssize_t* bu = malloc(sizeof(ssize_t));
        msgrcv(S2C_id,(void*) bu,sizeof(bu), 0, 0);
        printf("Data recieved is : %d \n", bu[0]);
        // msgrcv(C2S_id,(void*)&message,100, 0, 0);
        // printf("Data Received is : %s \n", message.data);
        // display the message
        // printf("Data sent is : %s \n", message.data);
    }
    
    return 0;
}
