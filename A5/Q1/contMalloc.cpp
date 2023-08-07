#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cout<<"Enter the number of blocks: \n";
    cin>>n;

    vector<int> blockSize(n);
    vector<int> allocated(n , 0);
    cout<<"Enter the size of each block: \n";
    for(int i = 0 ; i < n ; i++){
        cin>>blockSize[i];
    }

    int m;
    cout<<"Enter the number of processes: \n";
    cin>>m;

    vector<int> processSize(m);
    cout<<"Enter the size of each process: \n";
    for(int i = 0 ; i < m ; i++){
        cin>>processSize[i];
    }

    cout<<"--------------------------------------------\n\n";
    cout<<"FIRST-FIT: \n";
    cout<<"Process Number\t|\tProcess Size\t|\tBlock Size\t|\tBlock Number\n";
    for(int i = 0 ; i < m ; i++){
        int flag = 0;
        for(int j = 0 ; j < n ; j++){
            if(processSize[i] <= blockSize[j] && allocated[j] == 0){
                cout<<i+1<<"\t\t|\t"<<processSize[i]<<"\t\t|\t"<<blockSize[j]<<"\t\t|\t"<<j+1<<endl;
                allocated[j] = 1;
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            cout<<i+1<<"\t\t|\t"<<processSize[i]<<"\t\t|\t"<<"Not Allocated"<<"\t|\t"<<"Not Allocated"<<endl;
        }
    }

    for(int i = 0 ; i < n ; i++) allocated[i] = 0;

    cout<<"\n--------------------------------------------\n\n";
    cout<<"BEST-FIT: \n";
    cout<<"Process Number\t|\tProcess Size\t|\tBlock Size\t|\tBlock Number\n";
    for(int i = 0 ; i < m ; i++){
        int flag = 0;
        int min = INT_MAX;
        int minIndex = -1;
        for(int j = 0 ; j < n ; j++){
            if(processSize[i] <= blockSize[j] && allocated[j] == 0){
                if(blockSize[j] < min){
                    min = blockSize[j];
                    minIndex = j;
                }
            }
        }
        if(minIndex != -1){
            cout<<i+1<<"\t\t|\t"<<processSize[i]<<"\t\t|\t"<<blockSize[minIndex]<<"\t\t|\t"<<minIndex+1<<endl;
            allocated[minIndex] = 1;
            flag = 1;
        }
        if(flag == 0){
            cout<<i+1<<"\t\t|\t"<<processSize[i]<<"\t\t|\t"<<"Not Allocated"<<"\t|\t"<<"Not Allocated"<<endl;
        }
    }

    for(int i = 0 ; i < n ; i++) allocated[i] = 0;
    
    cout<<"\n--------------------------------------------\n\n";
    cout<<"WORST-FIT: \n";
    cout<<"Process Number\t|\tProcess Size\t|\tBlock Size\t|\tBlock Number\n";
    for(int i = 0 ; i < m ; i++){
        int flag = 0;
        int max = INT_MIN;
        int maxIndex = -1;
        for(int j = 0 ; j < n ; j++){
            if(processSize[i] <= blockSize[j] && allocated[j] == 0){
                if(blockSize[j] > max){
                    max = blockSize[j];
                    maxIndex = j;
                }
            }
        }
        if(maxIndex != -1){
            cout<<i+1<<"\t\t|\t"<<processSize[i]<<"\t\t|\t"<<blockSize[maxIndex]<<"\t\t|\t"<<maxIndex+1<<endl;
            allocated[maxIndex] = 1;
            flag = 1;
        }
        if(flag == 0){
            cout<<i+1<<"\t\t|\t"<<processSize[i]<<"\t\t|\t"<<"Not Allocated"<<"\t|\t"<<"Not Allocated"<<endl;
        }
    }

    cout<<"\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";
    return 0;

}