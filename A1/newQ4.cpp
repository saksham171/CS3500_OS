#include <iostream>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int part;
int elem;
int n;
int ans = 0;
vector<int> arr;
int half = 0;
int start = 0;

void* SortingThread(void* arg){
    if(half == 0) sort(arr.begin() , arr.begin() + (n / 2));
    else sort(arr.begin() + (n / 2) , arr.end());
    pthread_exit(NULL);
}

void* MergerThread(void* arg){
    sort(arr.begin() , arr.end());
    pthread_exit(NULL);
}

void* CountMultiples(void* arg){
    int x = (n + part - 1)/part;
    for(int i = start*x ; i < min((start + 1)*x , n) ; i++){
        if(arr[i] == 0) ans++;
        else if(arr[i] % elem == 0) ans++;
    }
    pthread_exit(NULL);
}

int main(){
    cout<<"Enter the size of the array"<<endl;
    cin>>n;

    cout<<"Enter the number N"<<endl;
    cin>>part;

    cout<<"Enter the elements of the array"<<endl;
    for(int i = 0 ; i < n ; i++){
        int x;
        cin>>x;
        arr.push_back(x);
    }

    cout<<"Enter the number 'element'"<<endl;
    cin>>elem;
    
    pthread_t t1,t2,t3;
    
    pthread_create(&t1 , NULL , &SortingThread , NULL);
    half = 1;
    pthread_create(&t2 , NULL , &SortingThread , NULL);
    pthread_join(t1 , NULL);
    pthread_join(t2 , NULL);

    pthread_create(&t3 , NULL , &MergerThread , NULL);
    pthread_join(t3 , NULL);
 
    vector<pthread_t> cnt(part);
    for(int i = 0 ; i < part ; i++){
        start = i;
        pthread_create(&cnt[i] , NULL , &CountMultiples , NULL);
    }
    for(int i = 0 ; i < part ; i++){
        pthread_join(cnt[i] , NULL);
    }
    cout<<ans<<endl;
    pthread_exit(NULL);
    return 0;
}
