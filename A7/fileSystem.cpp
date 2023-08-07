#include <bits/stdc++.h>
#include <chrono>
#include <ctime>
using namespace std;

#define block_size 128
#define block_addr_size 4

class inode{
public:
    string name;//size 16
    int type;//size 1
    int size;//size 4
    string date_created;//size 28
    string date_modified;//size 28
    int block_addr[3];//first 3 blocks - 12
    int index_block_addr;//pointer to index block - 4
    int all_addr[35];//index block contains addresses of rest of blocks
    void init(){
        for(int i = 0 ; i < 3 ; i++){
            block_addr[i] = -1;
        }
        for(int i = 0 ; i < 35 ; i++){
            all_addr[i] = -1;
        }
    }
    string Filedata = "";
};

class block{
public:
    char data[block_size];
    int rem = block_size;
};

string getCurTime(){
    auto now = chrono::system_clock::now();
    time_t tmNow = chrono::system_clock::to_time_t(now);
    return ctime(&tmNow);
}

int main(int argc, char* argv[]){
    string diskName = argv[1];
    int diskSize = atoi(argv[2]);
    int numBlocks = diskSize*1024/block_size;
    // cout<<"Disk Name: "<<diskName<<endl;
    // cout<<"Disk Size: "<<diskSize<<" KB"<<endl;
    // cout<<"Number of Blocks: "<<numBlocks<<endl;
    vector<bool> allocated(numBlocks, false);
    vector<block> blocks(numBlocks);
    vector<inode*> diskInfo;
    int debug = 1;
    string instruction;
    while(instruction != "exit"){
        cin>>instruction;
        string fileName;
        
        if(instruction == "load"){
            cin>>fileName;
            int sz = fileName.length();
            if(sz > 16){
                cout<<"ERROR: File name too long"<<endl;
                continue;
            }
            else{
                int dir = 0;
                for(int i = 0; i < sz; i++){
                    if(fileName[i] == '/'){
                        dir++;
                    }
                }
                if(dir > 2){
                    cout<<"ERROR: Too many directories"<<endl;
                    continue;
                }
                else if(dir == 2){
                    int dirName = 0;
                    int index = 0;
                    while(fileName[index] != '/'){
                        index++;
                    }
                    while(fileName[index] != '/'){
                        index++;
                        dirName++;
                    }
                    if(dirName > 4){
                        cout<<"ERROR: Directory name too long"<<endl;
                        continue;
                    }
                    else{
                        inode* newFile = new inode();
                        newFile->name = fileName;
                        newFile->type = 1;
                        newFile->size = 0;
                        newFile->date_created = getCurTime();
                        newFile->date_modified = getCurTime();
                        newFile->init();
                        for(int i = 0; i < 3; i++){
                            newFile->block_addr[i] = -1;
                        }
                        newFile->index_block_addr = -1;
                        diskInfo.push_back(newFile);
                    }
                }
                else{
                    inode* newFile = new inode();
                    newFile->name = fileName;
                    newFile->type = 0;
                    newFile->size = 0;
                    newFile->date_created = getCurTime();
                    newFile->date_modified = getCurTime();
                    newFile->init();
                    for(int i = 0; i < 3; i++){
                        newFile->block_addr[i] = -1;
                    }
                    newFile->index_block_addr = -1;
                    diskInfo.push_back(newFile);
                }
            }
        }

        else if(instruction == "delete"){
            cin>>fileName;
            int flag = 0;
            for(int i = 0; i < diskInfo.size(); i++){
                if(diskInfo[i]->name == fileName){
                    for(int j = 0 ; j < 3 ; j++){
                        if(diskInfo[i]->block_addr[j] != -1){
                            allocated[diskInfo[i]->block_addr[j]] = false;
                        }
                    }
                    if(diskInfo[i]->index_block_addr != -1){
                        allocated[diskInfo[i]->index_block_addr] = false;
                        for(int j = 0; j < 32; j++){
                            if(diskInfo[i]->all_addr[j] != -1){
                                allocated[diskInfo[i]->all_addr[j]] = false;
                            }
                        }
                    }
                    diskInfo.erase(diskInfo.begin() + i);
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                cout<<"ERROR: No such file exists"<<endl;
            }
        }

        else if(instruction == "print"){
            cin>>fileName;
            int flag = 0;
            for(int i = 0 ; i < diskInfo.size(); i++){
                if(diskInfo[i]->name == fileName){
                    cout<<"Data in file is: "<<diskInfo[i]->Filedata<<endl;
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                cout<<"ERROR: File not found"<<endl;
            }
        }

        else if(instruction == "append"){
            cin>>fileName;
            int flag = 0;
            int index = -1;
            for(int i = 0 ; i < diskInfo.size(); i++){
                if(diskInfo[i]->name == fileName){
                    index = i;
                    break;
                }
            }

            if(index == -1){
                cout<<"ERROR: No file with this name found"<<endl;
                continue;
            }

            string data;
            cin>>data;
            int sz = data.length();
            if(sz > 4480){
                cout<<"ERROR: File size too large"<<endl;
                continue;
            }
            else{
                int tmp = 1;
                int update = 0;
                vector<int> alloc;
                if(debug == 0) cout<<"Else start"<<endl;
                while(sz != 0){
                    if(diskInfo[index]->index_block_addr == -1){
                        if(diskInfo[index]->block_addr[2] != -1){
                            int tmpRem = blocks[diskInfo[index]->block_addr[2]].rem;
                            sz -= min(sz, tmpRem);
                            blocks[diskInfo[index]->block_addr[2]].rem -= min(sz, tmpRem);
                            if(sz != 0){
                                int k = 0;
                                while(allocated[k] == true){
                                    k++;
                                }
                                allocated[k] = true;
                                diskInfo[index]->index_block_addr = k;
                                alloc.push_back(k);
                                while(allocated[k] == true){
                                    k++;
                                }
                                allocated[k] = true;
                                diskInfo[index]->all_addr[0] = k;
                                alloc.push_back(k);
                                goto elsePart;
                            }
                        }
                        else{
                            if(diskInfo[index]->block_addr[0] == -1){
                                if(debug == 0) cout<<"First append"<<endl;
                                int k = 0;
                                while(allocated[k] == true){
                                    k++;
                                }
                                allocated[k] = true;
                                diskInfo[index]->block_addr[0] = k;
                                diskInfo[index]->all_addr[32] = k;
                                alloc.push_back(k);
                                blocks[k].rem = 128 - min(sz, 128);
                                sz -= min(sz, 128);
                                if(debug == 0) cout<<"Size should be zero: "<<sz<<endl;
                                if(sz != 0){
                                    int k = 0;
                                    while(allocated[k] == true){
                                        k++;
                                    }
                                    allocated[k] = true;
                                    diskInfo[index]->block_addr[1] = k;
                                    diskInfo[index]->all_addr[33] = k;
                                    alloc.push_back(k);
                                    blocks[k].rem -= min(sz, 128);
                                    sz -= min(sz, 128);
                                    if(sz != 0){
                                        int k = 0;
                                        while(allocated[k] == true){
                                            k++;
                                        }
                                        allocated[k] = true;
                                        diskInfo[index]->block_addr[2] = k;
                                        diskInfo[index]->all_addr[34] = k;
                                        alloc.push_back(k);
                                        blocks[k].rem -= min(sz, 128);
                                        sz -= min(sz, 128);
                                        if(sz != 0){
                                            int k = 0;
                                            while(allocated[k] == true){
                                                k++;
                                            }
                                            allocated[k] = true;
                                            diskInfo[index]->index_block_addr = k;
                                            alloc.push_back(k);
                                            while(allocated[k] == true){
                                                k++;
                                            }
                                            allocated[k] = true;
                                            diskInfo[index]->all_addr[0] = k;
                                            alloc.push_back(k);
                                            goto elsePart;
                                        }
                                        else{
                                            update = 1;
                                        }
                                    }
                                    else{
                                        update = 1;
                                    }
                                }
                                else{
                                    update = 1;
                                }
                            }
                            else if(diskInfo[index]->block_addr[1] == -1){
                                int tmpRem = blocks[diskInfo[index]->block_addr[0]].rem;
                                blocks[diskInfo[index]->block_addr[0]].rem -= min(sz, tmpRem);
                                sz -= min(sz, tmpRem);
                                if(sz != 0){
                                    int k = 0;
                                    while(allocated[k] == true){
                                        k++;
                                    }
                                    allocated[k] = true;
                                    diskInfo[index]->block_addr[1] = k;
                                    diskInfo[index]->all_addr[33] = k;
                                    alloc.push_back(k);
                                    blocks[k].rem = 128 - min(sz, 128);
                                    sz -= min(sz, 128);
                                    if(sz != 0){
                                        int k = 0;
                                        while(allocated[k] == true){
                                            k++;
                                        }
                                        allocated[k] = true;
                                        diskInfo[index]->block_addr[2] = k;
                                        diskInfo[index]->all_addr[34] = k;
                                        alloc.push_back(k);
                                        blocks[k].rem = 128 - min(sz, 128);
                                        sz -= min(sz, 128);
                                        if(sz != 0){
                                            int k = 0;
                                            while(allocated[k] == true){
                                                k++;
                                            }
                                            allocated[k] = true;
                                            diskInfo[index]->index_block_addr = k;
                                            alloc.push_back(k);
                                            while(allocated[k] == true){
                                                k++;
                                            }
                                            allocated[k] = true;
                                            diskInfo[index]->all_addr[0] = k;
                                            alloc.push_back(k);
                                            goto elsePart;
                                        }
                                    }
                                }
                            }
                            else{
                                int tmpRem = blocks[diskInfo[index]->block_addr[1]].rem;
                                blocks[diskInfo[index]->block_addr[1]].rem -= min(sz, tmpRem);
                                sz -= min(sz, tmpRem);
                                if(sz != 0){
                                    int k = 0;
                                    while(allocated[k] == true){
                                        k++;
                                    }
                                    allocated[k] = true;
                                    diskInfo[index]->block_addr[2] = k;
                                    diskInfo[index]->all_addr[34] = k;
                                    alloc.push_back(k);
                                    blocks[k].rem = 128 - min(sz, 128);
                                    sz -= min(sz, 128);
                                    if(sz != 0){
                                        int k = 0;
                                        while(allocated[k] == true){
                                            k++;
                                        }
                                        allocated[k] = true;
                                        diskInfo[index]->index_block_addr = k;
                                        alloc.push_back(k);
                                        while(allocated[k] == true){
                                            k++;
                                        }
                                        allocated[k] = true;
                                        diskInfo[index]->all_addr[0] = k;
                                        alloc.push_back(k);
                                        goto elsePart;
                                    }
                                }
                            }
                        }
                    }
                    else{
                    elsePart:
                        int j = 0;
                        while(diskInfo[index]->all_addr[j] != -1){
                            j++;
                        }
                        if(j == 32){
                            cout<<"ERROR: No space available"<<endl;
                            tmp = 0;
                            break;
                        }
                        else{
                            int init = j;
                            int initRem = blocks[diskInfo[index]->all_addr[j - 1]].rem;
                            if(blocks[diskInfo[index]->all_addr[j - 1]].rem >= sz){
                                //blocks[diskInfo[index]->all_addr[j - 1]].data += data;
                                blocks[diskInfo[index]->all_addr[j - 1]].rem -= sz;
                                sz = 0;
                            }
                            else{
                                //int rem = blocks[diskInfo[index]->all_addr[j - 1]].rem;
                                //blocks[diskInfo[index]->all_addr[j - 1]].data += data.substr(0, rem);
                                //data = data.substr(rem, sz - rem);
                                sz -= blocks[diskInfo[index]->all_addr[j - 1]].rem;
                                blocks[diskInfo[index]->all_addr[j - 1]].rem = 0;
                            }
                            while(sz != 0){
                                if(j == 32) tmp = 0;
                                int k = 0;
                                while(allocated[k] == true){
                                    k++;
                                }
                                allocated[k] = true;
                                diskInfo[index]->all_addr[j] = k;
                                alloc.push_back(k);
                                blocks[k].rem = block_size - min(sz, block_size);
                                sz -= min(block_size, sz);
                                j++;
                            }
                            if(tmp == 0){
                                cout<<"ERROR: No space available"<<endl;
                                for(int i = 0; i < alloc.size(); i++){
                                    allocated[alloc[i]] = false;
                                    blocks[alloc[i]].rem = block_size;
                                    //diskInfo[index]->all_addr[i] = -1;
                                }
                                for(int i = init ; i < j ; i++){
                                    diskInfo[index]->all_addr[i] = -1;
                                }
                                blocks[diskInfo[index]->all_addr[j - 1]].rem = initRem;
                                break;
                            }
                            else{
                                update = 1;
                                break;               
                            }
                        }

                    }
                }
                if(update == 1){
                    diskInfo[index]->size += alloc.size() * block_size;
                    diskInfo[index]->Filedata += data;
                    diskInfo[index]->date_modified = getCurTime();
                }
            }                                                                                                                                                                 
        }

        else if(instruction == "display"){
            for(int i = 0; i < diskInfo.size(); i++){
                cout<<diskInfo[i]->name<<" "<<diskInfo[i]->size<<endl;
            }
        }

        else if(instruction == "freespace"){
            int count = 0;
            for(int i = 0; i < numBlocks; i++){
                if(allocated[i] == false){
                    count++;
                }
            }
            cout<<"Number of available free blocks: "<<count<<endl;
            cout<<"Total free space: "<<count*block_size<<" bytes"<<endl;
        }

        else if(instruction == "printinode"){
            cin>>fileName;
            int flag = 0;
            for(int i = 0; i < diskInfo.size(); i++){
                if(diskInfo[i]->name == fileName){
                    cout<<"***************************"<<endl;
                    cout<<"Filename: "<<diskInfo[i]->name<<endl;
                    //cout<<"Type: "<<diskInfo[i]->type<<endl;
                    cout<<"Size: "<<diskInfo[i]->size<<endl;
                    cout<<"Date Created: "<<diskInfo[i]->date_created;
                    cout<<"Date Modified: "<<diskInfo[i]->date_modified;
                    cout<<"Direct Block Values: ";
                    for(int j = 0; j < 3; j++){
                        if(diskInfo[i]->block_addr[j] != -1) cout<<diskInfo[i]->block_addr[j]<<" ";
                    }
                    cout<<endl;
                    cout<<"Index Block is stored in: "<<diskInfo[i]->index_block_addr<<endl;
                    cout<<"Index Block contents: ";
                    for(int j = 0; j < 32; j++){
                        if(diskInfo[i]->all_addr[j] != -1) cout<<diskInfo[i]->all_addr[j]<<" ";
                    }
                    cout<<endl;
                    cout<<"***************************"<<endl;
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                cout<<"ERROR: File not found"<<endl;
            }
        }

        else if(instruction == "exit"){
            break;
        }

        else{
            cout<<"Invalid instruction"<<endl;
        }
    }

}