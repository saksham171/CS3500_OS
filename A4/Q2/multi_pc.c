#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

// A structure to represent a stack
struct Stack {
    int top;
    unsigned capacity;
    int* array;
};
  
// function to create a stack of given capacity. It initializes size of stack as 0
struct Stack* createStack(unsigned capacity)
{
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}
  
// Stack is full when top is equal to the last index
int isFull(struct Stack* stack)
{
    if(stack->top == stack->capacity - 1) return 1;
    else return 0;
}
  
// Stack is empty when top is equal to -1
int isEmpty(struct Stack* stack)
{
    if(stack->top == -1) return 1;
    else return 0;
}
  
// Function to add an item to stack. It increases top by 1
void push(struct Stack* stack, int item)
{
    if (isFull(stack) == 1) return;
    stack->array[++stack->top] = item;
}
  
// Function to remove an item from stack. It decreases top by 1
int pop(struct Stack* stack)
{
    if (isEmpty(stack) == 1) return INT_MIN;
    return stack->array[stack->top--];
}
  
// Function to return the top from stack without removing it
int peek(struct Stack* stack)
{
    if (isEmpty(stack) == 1) return INT_MIN;
    return stack->array[stack->top];
}

int p; //number of producers
int c; //number of consumers
int pc; //producer capacity
int cc; //consumer capacity
struct Stack* buffer; //buffer or shared memory using stack
sem_t mutex1; //mutex for access to buffer
sem_t mutex2; //mutex for checking if buffer is empty
int* produced; //number of items produced by each producer
int* consumed; //number of items consumed by each consumer
int item;

void* produce(void* arg)
{
    int* x = arg; //producer id
    //sleep(5);
    while(produced[*x] < pc){
        sem_wait(&mutex1); //wait for stack access signal
        if(isFull(buffer) == 0){
            item = (rand() % 1000) + 1; //random number between 1 and 1000
            push(buffer, item); //push item to stack
            produced[*x]++;
            printf("Producer %d produced %d\n", *x, item);
        }
        sem_post(&mutex1); //signal stack access
        sem_post(&mutex2); //signal stack is not empty
    }
}

void* consume(void* arg)
{
    int* x = arg; //consumer id
    while(consumed[*x] < cc){
        sem_wait(&mutex2); //wait for stack not empty signal
        sem_wait(&mutex1); //wait for stack access signal
        if(isEmpty(buffer) == 0){
            item = pop(buffer); //pop item from stack
            consumed[*x]++;
            printf("Consumer %d consumed %d\n", *x, item);
        }
        sem_post(&mutex1); //signal stack access
    }
}

int main(int argv , char* args[]){
    srand(time(NULL));
    
    p = atoi(args[2]);
    pc = atoi(args[4]);
    c = atoi(args[6]);
    cc = atoi(args[8]);

    sem_init(&mutex1 , 0 , 1);
    sem_init(&mutex2 , 0 , 0);
    
    buffer = createStack(100);
    produced = malloc(p * sizeof(int));
    consumed = malloc(c * sizeof(int));

    pthread_t consumer[c];
    int consumer_id[c];
    for(int i = 0 ; i < c ; i++){
        consumer_id[i] = i;
        consumed[i] = 0;
        pthread_create(&consumer[i] , NULL , consume , &consumer_id[i]);
    }
    
    pthread_t producer[p];
    int producer_id[p];
    for(int i = 0 ; i < p ; i++){
        producer_id[i] = i;
        produced[i] = 0;
        pthread_create(&producer[i] , NULL , produce , &producer_id[i]);
    }

    for(int i = 0 ; i < c ; i++){
        pthread_join(consumer[i], NULL);
    }

    for(int i = 0 ; i < p ; i++){
        pthread_join(producer[i], NULL);
    }
    
    sem_destroy(&mutex1);
    sem_destroy(&mutex2);

    return 0;
}