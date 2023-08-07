#include <bits/stdc++.h>
using namespace std;

#define FIFO 1
#define OPTIMAL 2
#define LRU 3

int main(){
    int n;
    cout<<"Number of Memory Requests: \n";
    cin>>n;

    int pageRef[n];
    cout<<"Enter the sequence: \n";
    for(int i = 0 ; i < n ; i++){
        cin>>pageRef[i];
    }

    int x;
    cout<<"Enter the page-frame size: \n";
    cin>>x;

    vector<int> phyMem;

    cout<<"FIFO - "<<FIFO<<endl;
    cout<<"OPTIMAL - "<<OPTIMAL<<endl;
    cout<<"LRU - "<<LRU<<endl<<endl;

    cout<<"Enter page replacement strategy: \n";
    int id;
    cin>>id;

    if(id == FIFO){
        cout<<"\nFIFO\n\n";
        int count = 0; //number of page faults
        for(int i = 0 ; i < n ; i++){
            if(phyMem.size() < x){
                int fault = 0;
                for(int j = 0 ; j < phyMem.size() ; j++){
                    if(phyMem[j] == pageRef[i]){
                        cout<<"No page fault\n";
                        fault = 1;
                        break;
                    }
                }
                if(fault == 0){
                    phyMem.push_back(pageRef[i]);
                    for(int j = 0 ; j < phyMem.size() ; j++){
                        cout<<phyMem[j]<<" ";
                    }
                    cout<<endl;
                    count++;
                }
            }
            else{
                int flag = 0;
                for(int j = 0 ; j < phyMem.size() ; j++){
                    if(phyMem[j] == pageRef[i]){
                        flag = 1;
                        break;
                    }
                }
                if(flag == 1){
                    cout<<"No page fault\n";
                }
                else{
                    phyMem.erase(phyMem.begin());
                    phyMem.push_back(pageRef[i]);
                    for(int j = 0 ; j < phyMem.size() ; j++){
                        cout<<phyMem[j]<<" ";
                    }
                    cout<<endl;
                    count++;
                }
            }
        }
        cout<<"\nTotal number of page faults = "<<count<<endl;
    }
    
    else if(id == OPTIMAL){
        cout<<"\nOPTIMAL\n\n";
        int count = 0; //number of page faults
        for(int i = 0 ; i < n ; i++){
            if(phyMem.size() < x){
                int fault = 0;
                for(int j = 0 ; j < phyMem.size() ; j++){
                    if(phyMem[j] == pageRef[i]){
                        cout<<"No page fault\n";
                        fault = 1;
                        break;
                    }
                }
                if(fault == 0){
                    phyMem.push_back(pageRef[i]);
                    for(int j = 0 ; j < phyMem.size() ; j++){
                        cout<<phyMem[j]<<" ";
                    }
                    cout<<endl;
                    count++;
                }
            }
            else{
                if(i == n - 1){
                    int flag = 0;
                    for(int j = 0 ; j < phyMem.size() ; j++){
                        if(phyMem[j] == pageRef[i]){
                            cout<<"No page fault\n";
                            flag = 1;
                            break;
                        }
                    }
                    if(flag == 0){
                        phyMem.erase(phyMem.begin());
                        phyMem.push_back(pageRef[i]);
                        for(int j = 0 ; j < phyMem.size() ; j++){
                            cout<<phyMem[j]<<" ";
                        }
                        cout<<endl;
                        count++;
                    }
                }
                else{
                    int flag = 0;
                    int cur_max = INT_MIN;
                    int cur_max_index = -1;
                    for(int j = 0 ; j < phyMem.size() ; j++){
                        if(phyMem[j] == pageRef[i]){
                            cout<<"No page fault\n";
                            flag = 1;
                            break;
                        }
                        else{
                            int temp = INT_MIN;
                            for(int k = i + 1 ; k < n ; k++){
                                if(phyMem[j] == pageRef[k]){
                                    temp = k;
                                    break;
                                }
                            }
                            if(temp == INT_MIN) temp = n;
                            if(temp > cur_max){
                                cur_max = temp;
                                cur_max_index = j;
                            }
                        }
                    }
                    if(flag == 0){
                        phyMem.erase(phyMem.begin() + cur_max_index);
                        phyMem.push_back(pageRef[i]);
                        for(int j = 0 ; j < phyMem.size() ; j++){
                            cout<<phyMem[j]<<" ";
                        }
                        cout<<endl;
                        count++;
                    }
                }
            }
        }
        cout<<"\nTotal number of page faults = "<<count<<endl;
    }

    else{
        cout<<"\nLRU\n\n";
        int count = 0; //number of page faults
        for(int i = 0 ; i < n ; i++){
            if(phyMem.size() < x){
                int fault = 0;
                for(int j = 0 ; j < phyMem.size() ; j++){
                    if(phyMem[j] == pageRef[i]){
                        phyMem.erase(phyMem.begin() + j);
                        cout<<"No page fault\n";
                        fault = 1;
                        break;
                    }
                }
                phyMem.push_back(pageRef[i]);
                if(fault == 0){
                    for(int j = 0 ; j < phyMem.size() ; j++){
                        cout<<phyMem[j]<<" ";
                    }
                    cout<<endl;
                    count++;
                }
            }
            else{
                int flag = 0;
                for(int j = 0 ; j < phyMem.size() ; j++){
                    if(phyMem[j] == pageRef[i]){
                        phyMem.erase(phyMem.begin() + j);
                        flag = 1;
                        break;
                    }
                }
                if(flag == 1){
                    phyMem.push_back(pageRef[i]);
                    cout<<"No page fault\n";
                }
                else{
                    phyMem.erase(phyMem.begin());
                    phyMem.push_back(pageRef[i]);
                    for(int j = 0 ; j < phyMem.size() ; j++){
                        cout<<phyMem[j]<<" ";
                    }
                    cout<<endl;
                    count++;
                }
            }
        }
        cout<<"\nTotal number of page faults = "<<count<<endl;
    }
}