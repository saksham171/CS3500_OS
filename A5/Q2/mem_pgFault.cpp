#include <bits/stdc++.h>
#include <sys/resource.h>
#include <errno.h>
using namespace std;

void inplace_transpose(long** matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            swap(matrix[i][j], matrix[j][i]);
        }
    }
}

int main(){
    srand(time(NULL));
    struct rusage init;
    struct rusage stats;
    int who = RUSAGE_SELF;
    
    int n;
    cout<<"Enter value of n: \n";
    cin>>n;

    int mode;
    cout<<"Enter Choice\n";
    cout<<"Memory to be allocated once - 1\n";
    cout<<"Memory to be allocated every time - 2\n";
    cin>>mode;

    getrusage(who, &init);
    long init_mem = init.ru_maxrss;
    if(mode == 1){
        long** mat[5];
        for(int k = 0 ; k < 5 ; k++){
            mat[k] = (long**) malloc(n * sizeof(long*));
            for(int i = 0 ; i < n ; i++){
                mat[k][i] = (long*) malloc(n * sizeof(long));
            }
        }
        for(int i = 0 ; i < 10 ; i++){
            for(int j = 0 ; j < 5 ; j++){
                for(int row = 0 ; row < n ; row++){
                    for(int col = 0 ; col < n ; col++){
                        mat[j][row][col] = rand() % INT_MAX;
                    }
                }
                inplace_transpose(mat[j], n);
            }
            getrusage(who, &stats);
            cout<<"Memory usage: "<<init_mem<<" + "<<(stats.ru_maxrss - init_mem)<<", Page_faults: "<<stats.ru_minflt<<endl;
        }
        for(int k = 0 ; k < 5 ; k++) free(mat[k]);
    }

    else{
        for(int i = 0 ; i < 10 ; i++){
            long** mat[5];
            for(int k = 0 ; k < 5 ; k++){
                mat[k] = (long**) malloc(n * sizeof(long*));
                for(int it = 0 ; it < n ; it++){
                    mat[k][it] = (long*) malloc(n * sizeof(long));
                }
            }
            for(int j = 0 ; j < 5 ; j++){
                for(int row = 0 ; row < n ; row++){
                    for(int col = 0 ; col < n ; col++){
                        mat[j][row][col] = rand() % INT_MAX;
                    }
                }
                inplace_transpose(mat[j], n);
            }
            getrusage(who, &stats);
            cout<<"Memory usage: "<<init_mem<<" + "<<(stats.ru_maxrss - init_mem)<<", Page_faults: "<<stats.ru_minflt<<endl;
            for(int k = 0 ; k < 5 ; k++) free(mat[k]);
        }
    }

    return 0;

}