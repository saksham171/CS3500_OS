#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define COUNT 5

struct timespec start_time , cur_time;
int n; //number of philosphers
int thinking_time; //random
int eating_time = 100; //fixed
sem_t me; //mutex
sem_t* chopsticks; //semaphore for chopsitcks
int* pflag; //philosopher state
int* pval; //philosopher id
int* eaten; //number of times each philosopher has eaten

void test(int pnum)
{
    int l = (pnum + (n - 1)) % n;
    int r = (pnum + 1) % n;
    if (pflag[pnum] == HUNGRY && pflag[l] != EATING && pflag[r] != EATING && eaten[pnum] < 5) {
        pflag[pnum] = EATING; //philosopher can eat
        
        clock_gettime(CLOCK_MONOTONIC_RAW, &cur_time);
        int delta = ((cur_time.tv_sec - start_time.tv_sec) * 1000) + ((cur_time.tv_nsec - start_time.tv_nsec) / 1000000);
        printf("<%d> Philosopher %d in EATING state - %d\n", delta , pnum , ++eaten[pnum]);
        
        nanosleep((const struct timespec[]){{0, eating_time * 1000000L}}, NULL); //eating time is 100ms

        sem_post(&chopsticks[pnum]);
    }
}
 
// pick up chopsticks
void take_chopsticks(int pnum)
{ 
    sem_wait(&me); //wait for mutex to unlock

    pflag[pnum] = HUNGRY; //philosopher's thinking is complete and is ready to eat
    
    thinking_time = (rand() % 50) + 50; //random thinking time between 50ms and 100ms
    nanosleep((const struct timespec[]){{0, thinking_time * 1000000L}}, NULL);

    test(pnum); //check if philosopher can eat
    
    sem_post(&me); //unlock mutex
    
    sem_wait(&chopsticks[pnum]); //wait for chopsticks to be available again
}
 
// put down chopsticks
void put_chopsticks(int pnum)
{
    int l = (pnum + (n - 1)) % n;
    int r = (pnum + 1) % n;
    sem_wait(&me); //wait for mutex to unlock
 
    pflag[pnum] = THINKING; //philosopher is thinking again
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &cur_time);
    long delta = ((cur_time.tv_sec - start_time.tv_sec) * 1000) + ((cur_time.tv_nsec - start_time.tv_nsec) / 1000000);
    if(eaten[pnum] < 5) printf("<%ld> Philosopher %d in THINKING state\n",delta ,  pnum);

    test(l); //now test for neighbours
    test(r);

    sem_post(&me); //unlock mutex
}

void* phil_fn (void* arg){
    int *x = arg; //philosopher id
    while(eaten[*x] < 5){
        take_chopsticks(*x);
        put_chopsticks(*x);
    }
}

int main(int argv, char* args[]){
    srand(time(NULL));
    
    n = atoi(args[1]);
    
    eaten = malloc(sizeof(int) * n);
    pval = malloc(sizeof(int) * n);
    pflag = (int *) malloc(sizeof(int) * n);
    for(int i = 0 ; i < n ; i++){
        eaten[i] = 0;
        pval[i] = i;
        pflag[i] = THINKING;
    }
    
    sem_init(&me, 0, 1);
    
    chopsticks = (sem_t *) malloc(sizeof(sem_t) * n);
    for(int i = 0 ; i < n ; i++){
        sem_init(&chopsticks[i], 0, 0);
    }
    
    pthread_t phil[n];
    clock_gettime(CLOCK_MONOTONIC_RAW , &start_time);
    
    for(int i = 0 ; i < n ; i++){
        pthread_create(&phil[i], NULL, phil_fn , &pval[i]);
        clock_gettime(CLOCK_MONOTONIC_RAW , &cur_time);
        int delta = ((cur_time.tv_sec - start_time.tv_sec) * 1000) + ((cur_time.tv_nsec - start_time.tv_nsec) / 1000000);
        printf("<%d> Philosopher %d in THINKING state\n" , delta , i);
    }

    for(int i = 0 ; i < n ; i++){
        pthread_join(phil[i], NULL);
    }

    sem_destroy(&me);
    for(int i = 0 ; i < n ; i++){
        sem_destroy(&chopsticks[i]);
    }
    
    return 0;
}