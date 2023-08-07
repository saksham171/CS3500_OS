#include <bits/stdc++.h>
#include <time.h>
using namespace std;

#define CYL 5000
#define REQ 1000
#define FCFS 1
#define SSTF 2
#define SCAN 3
#define CSCAN 4
#define LOOK 5
#define CLOOK 6

int find_minIndex(vector<int> &v1, vector<int> &v2, int head)
{
    int index = -1;
    int minimum = 1e9;
   
    for(int i = 0; i < REQ; i++)
    {
        if (!v1[i] && minimum > v2[i])
        {
            minimum = v2[i];
            index = i;
        }
    }
    return index;
}

int main(){
    srand(time(NULL));
    int head = 2000;
    int algo;
    
    cout << "Enter the initial head position (Enter -1 to use default value): ";
    cin >> head;
    if(head == -1) head = 2000;

    cout << "Algorithm opcodes: \n1. FCFS\n2. SSTF\n3. SCAN\n4. C-SCAN\n5. LOOK\n6. C-LOOK\n";
    cout << "Enter the opcode of the algorithm to be used: ";
    cin >> algo;

    vector<int> req(REQ);
    for(int i = 0 ; i < REQ ; i++) req[i] = rand() % CYL;

    vector<int> exec_order;
    
    if(algo == FCFS){
        vector<int> tmp(REQ + 1);
        tmp[0] = head;
        for(int i = 1 ; i <= REQ ; i++) tmp[i] = req[i - 1];
        exec_order = tmp;
    }
    
    else if(algo == SSTF){
        vector<int> min_dist(REQ , 0);
        vector<int> min_index(REQ , 0);
        for(int i = 0 ; i < REQ ; i++){
            exec_order.push_back(head);
            for(int x = 0 ; x < REQ ; x++){
                min_dist[x] = abs(head - req[x]);
            }
            int min_ind = find_minIndex(min_index , min_dist , head);
            min_index[min_ind] = 1;
            head = req[min_ind];
        }
        exec_order.push_back(head);
        
    }
    
    else if(algo == SCAN){
        exec_order.push_back(head);
        vector<int> left;
        vector<int> right;
        for(int i = 0 ; i < REQ ; i++){
            if(req[i] < head){
                left.push_back(req[i]);
            }
            else if(req[i] > head){
                right.push_back(req[i]);
            }
        }
        sort(left.begin() , left.end());
        sort(right.begin() , right.end());
        for(int i = left.size() - 1 ; i >= 0 ; i--){
            exec_order.push_back(left[i]);
        }
        exec_order.push_back(0);
        for(int i = 0 ; i < right.size() ; i++){
            exec_order.push_back(right[i]);
        }
    }
    
    else if(algo == CSCAN){
        exec_order.push_back(head);
        vector<int> left;
        vector<int> right;
        for(int i = 0 ; i < REQ ; i++){
            if(req[i] < head){
                left.push_back(req[i]);
            }
            else if(req[i] > head){
                right.push_back(req[i]);
            }
        }
        sort(left.begin() , left.end());
        sort(right.begin() , right.end());
        for(int i = 0 ; i < right.size() ; i++){
            exec_order.push_back(right[i]);
        }
        exec_order.push_back(CYL - 1);
        exec_order.push_back(0);
        for(int i = 0 ; i < left.size() ; i++){
            exec_order.push_back(left[i]);
        }
    }

    else if(algo == LOOK){
        exec_order.push_back(head);
        vector<int> left;
        vector<int> right;
        for(int i = 0 ; i < REQ ; i++){
            if(req[i] < head){
                left.push_back(req[i]);
            }
            else if(req[i] > head){
                right.push_back(req[i]);
            }
        }
        sort(left.begin() , left.end());
        sort(right.begin() , right.end());
        for(int i = left.size() - 1 ; i >= 0 ; i--){
            exec_order.push_back(left[i]);
        }
        for(int i = 0 ; i < right.size() ; i++){
            exec_order.push_back(right[i]);
        }
    }
    
    else if(algo == CLOOK){
        exec_order.push_back(head);
        vector<int> left;
        vector<int> right;
        for(int i = 0 ; i < REQ ; i++){
            if(req[i] < head){
                left.push_back(req[i]);
            }
            else if(req[i] > head){
                right.push_back(req[i]);
            }
        }
        sort(left.begin() , left.end());
        sort(right.begin() , right.end());
        for(int i = 0 ; i < right.size() ; i++){
            exec_order.push_back(right[i]);
        }
        for(int i = 0 ; i < left.size() ; i++){
            exec_order.push_back(left[i]);
        }
    }

    else{
        cout << "Invalid algorithm selected" << endl;
        return 0;
    }

    cout << "Execution order: " << endl;
    for(int i = 0 ; i < exec_order.size() ; i++){
        cout << exec_order[i] << " ";
    }
    cout << endl;

    int total_mov = 0;
    for(int i = 0 ; i < exec_order.size() - 1 ; i++){
        total_mov += abs(exec_order[i] - exec_order[i + 1]);
    }

    double avg_mov = (double)total_mov / (double)1000;

    cout<<"Total head movements: "<<total_mov<<endl;
    cout<<"Average head movements: "<<avg_mov<<endl;

    return 0;            

}