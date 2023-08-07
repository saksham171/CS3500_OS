/*You have to implement a multilevel queue scheduling algorithm with five queues, listed below:
1. System processes (sys)
2. Interactive processes (ip)
3. Interactive editing processes (iep)
4. Batch processes (bp)
5. Student processes (std)

All queues run in round robin fashion

sys: priority scheduling
ip: FCFS
iep: FCFS
bp: SJF
std: SJF
*/

#include <bits/stdc++.h>
using namespace std;

class Process {
public:
    string pid;
    int arrival_time;
    int burst_time;
    int priority;
    int wait_time;
    int remaining_time;
    int turnaround_time;
    int completion_time;
    string type;
    /*Process(int pid, int arrival_time, int burst_time, int priority, string type) {
        pid = pid;
        arrival_time = arrival_time;
        burst_time = burst_time;
        priority = priority;
        type = type;
        wait_time = 0;
        remaining_time = burst_time;
        turnaround_time = 0;
        completion_time = 0;
    }*/
    void printProcess(){
        cout << "PID:\t" << pid << "\tCompletion time:\t" << completion_time << "\tTurnaround time:\t" << turnaround_time << "\tWaiting time:\t" << wait_time << endl;
    }
};

int main(){
    int qtime;
    cin >> qtime;

    int n;
    cin >> n;

    vector<Process> processes(n);
    vector<Process> sys,ip,iep,bp,std;
    int atime,btime;
    string id , ptype;
    int priority = 0;
    for(int i = 0; i < n; i++){
        cin>>id>>atime>>btime>>ptype>>priority;
        processes[i].pid = id;
        processes[i].arrival_time = atime;
        processes[i].burst_time = btime;
        processes[i].priority = priority;
        processes[i].type = ptype;
        processes[i].remaining_time = btime;
        if(ptype == "sys"){
            sys.push_back(processes[i]);
        }
        else if(ptype == "ip"){
            ip.push_back(processes[i]);
        }
        else if(ptype == "iep"){
            iep.push_back(processes[i]);
        }
        else if(ptype == "bp"){
            bp.push_back(processes[i]);
        }
        else if(ptype == "std"){
            std.push_back(processes[i]);
        }
    }
    int time = 0;
    int qIndex = 0;
    int idle = 0;
    int left = n;
    int init;
    vector<string> ans;
    while(left > 0){
        if(qIndex == 0){
            //if(sys.size() == 0) break;
            init = time;
            vector<Process> tmp;
            for(int i = 0; i < sys.size(); i++){
                if(sys[i].arrival_time <= time){
                    tmp.push_back(sys[i]);
                }
            }
            /*if(tmp.size() == 0){
                break;
            }*/
            sort(tmp.begin(),tmp.end(),[](Process a, Process b){
                return a.priority < b.priority;
            });
            int cur = qtime;
            while(cur > 0){
                if(tmp.size() == 0) break;
                if(tmp[0].remaining_time > cur){
                    //tmp[0].remaining_time -= cur;
                    string procId = tmp[0].pid;
                    for(int i = 0; i < sys.size(); i++){
                        if(sys[i].pid == procId){
                            sys[i].remaining_time -= cur;
                            break;
                        }
                    }
                    ans.push_back(procId);
                    time += cur;
                    cur = 0;
                }
                else{
                    cur -= tmp[0].remaining_time;
                    time += tmp[0].remaining_time;
                    string procId = tmp[0].pid;
                    for(int i = 0; i < n ; i++){
                        if(processes[i].pid == procId){
                            processes[i].completion_time = time;
                            processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                            processes[i].wait_time = processes[i].turnaround_time - processes[i].burst_time;
                        }
                    }
                    for(int i = 0 ; i < sys.size() ; i++){
                        if(sys[i].pid == procId){
                            sys.erase(sys.begin()+i);
                        }
                    }
                    ans.push_back(procId);
                    left--;
                    tmp.erase(tmp.begin());
                }
            }
            //cout<<"No problem with sys"<<endl;
            if(time == init){
                idle++;
            }
            if(idle == 5){
                idle = 0;
                time++;
            }
        }
        else if(qIndex == 1){
            init = time;
            if(ip.size() > 0){
                int cur = qtime;
                while(cur > 0){
                    if(ip.size() == 0) break;
                    if(ip[0].remaining_time > cur){
                        ip[0].remaining_time -= cur;
                        string procId = ip[0].pid;
                        time += cur;
                        ans.push_back(procId);
                        cur = 0;
                    }
                    else{
                        cur -= ip[0].remaining_time;
                        time += ip[0].remaining_time;
                        string procId = ip[0].pid;
                        for(int i = 0; i < n ; i++){
                            if(processes[i].pid == procId){
                                processes[i].completion_time = time;
                                processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                                processes[i].wait_time = processes[i].turnaround_time - processes[i].burst_time;
                                break;
                            }
                        }
                        left--;
                        ans.push_back(procId);
                        ip.erase(ip.begin());
                    }
                }
            }
            //cout<<"No problem with ip"<<endl;
            if(time == init){
                idle++;
            }
            if(idle == 5){
                idle = 0;
                time++;
            }
        }
        else if(qIndex == 2){
            init = time;
            if(iep.size() > 0){
                int cur = qtime;
                while(cur > 0){
                    if(iep.size() == 0) break;
                    if(iep[0].remaining_time > cur){
                        iep[0].remaining_time -= cur;
                        string procId = iep[0].pid;
                        time += cur;
                        ans.push_back(procId);
                        cur = 0;
                    }
                    else{
                        cur -= iep[0].remaining_time;
                        time += iep[0].remaining_time;
                        string procId = iep[0].pid;
                        for(int i = 0; i < n ; i++){
                            if(processes[i].pid == procId){
                                processes[i].completion_time = time;
                                processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                                processes[i].wait_time = processes[i].turnaround_time - processes[i].burst_time;
                                break;
                            }
                        }
                        left--;
                        ans.push_back(procId);
                        iep.erase(iep.begin());
                    }
                }
            }
            //cout<<"No problem with iep"<<endl;
            if(time == init){
                idle++;
            }
            if(idle == 5){
                idle = 0;
                time++;
            }
        }
        else if(qIndex == 3){
            //if(bp.size() == 0) break;
            init = time;
            vector<Process> tmp;
            for(int i = 0; i < bp.size(); i++){
                if(bp[i].arrival_time <= time){
                    tmp.push_back(bp[i]);
                }
            }
            /*if(tmp.size() == 0){
                break;
            }*/
            sort(tmp.begin(),tmp.end(),[](Process a, Process b){
                return a.remaining_time < b.remaining_time;
            });
            int cur = qtime;
            while(cur > 0){
                if(tmp.size() == 0) break;
                if(tmp[0].remaining_time > cur){
                    tmp[0].remaining_time -= cur;
                    string procId = tmp[0].pid;
                    for(int i = 0; i < bp.size(); i++){
                        if(bp[i].pid == procId){
                            bp[i].remaining_time -= cur;
                            break;
                        }
                    }
                    ans.push_back(procId);
                    time += cur;
                    cur = 0;
                }
                else{
                    cur -= tmp[0].remaining_time;
                    time += tmp[0].remaining_time;
                    string procId = tmp[0].pid;
                    for(int i = 0; i < processes.size(); i++){
                        if(processes[i].pid == procId){
                            processes[i].completion_time = time;
                            processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                            processes[i].wait_time = processes[i].turnaround_time - processes[i].burst_time;
                            break;
                        }
                    }
                    left--;
                    for(int i = 0 ; i < bp.size() ; i++){
                        if(bp[i].pid == procId){
                            bp.erase(bp.begin()+i);
                        }
                    }
                    ans.push_back(procId);
                    tmp.erase(tmp.begin());
                }
            }
            //cout<<"No problem with bp"<<endl;
            if(time == init){
                idle++;
            }
            if(idle == 5){
                idle = 0;
                time++;
            }
        }
        else{
            //if(std.size() == 0) break;
            vector<Process> tmp;
            for(int i = 0; i < std.size(); i++){
                if(std[i].arrival_time <= time){
                    tmp.push_back(std[i]);
                }
            }
            /*if(tmp.size() == 0){
                break;
            }*/
            sort(tmp.begin(),tmp.end(),[](Process a, Process b){
                return a.remaining_time < b.remaining_time;
            });
            int cur = qtime;
            while(cur > 0){
                if(tmp.size() == 0) break;
                if(tmp[0].remaining_time > cur){
                    tmp[0].remaining_time -= cur;
                    string procId = tmp[0].pid;
                    for(int i = 0; i < std.size(); i++){
                        if(std[i].pid == procId){
                            std[i].remaining_time -= cur;
                            break;
                        }
                    }
                    ans.push_back(procId);
                    time += cur;
                    cur = 0;
                }
                else{
                    cur -= tmp[0].remaining_time;
                    time += tmp[0].remaining_time;
                    string procId = tmp[0].pid;
                    for(int i = 0; i < processes.size(); i++){
                        if(processes[i].pid == procId){
                            processes[i].completion_time = time;
                            processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                            processes[i].wait_time = processes[i].turnaround_time - processes[i].burst_time;
                            break;
                        }
                    }
                    left--;
                    for(int i = 0 ; i < std.size() ; i++){
                        if(std[i].pid == procId){
                            std.erase(std.begin()+i);
                        }
                    }
                    ans.push_back(procId);
                    tmp.erase(tmp.begin());
                }
            }
            //cout<<"No problem with std"<<endl;
            if(time == init){
                idle++;
            }
            if(idle == 5){
                idle = 0;
                time++;
            }
        }
        qIndex++;
        if(qIndex == 5){
            qIndex = 0;
        }
    }
    //cout<<left<<endl;
    for(int i = 0 ; i < ans.size() - 1 ; i++){
        cout<<ans[i]<<" -> ";
    }
    cout<<ans[ans.size() - 1]<<endl;
    for(int i = 0 ; i < n ; i++){
        processes[i].printProcess();
    }
    /*for(int i = 0 ; i < ip.size() ; i++){
        ip[i].printProcess();
        cout<<ip[i].remaining_time<<" "<<ip[i].arrival_time<<" "<<ip[i].burst_time<<" "<<endl;
    }*/
    return 0;
}