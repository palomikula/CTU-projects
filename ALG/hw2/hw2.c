#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>


int *mat;
int **connection_matrix;
int min_cost = 100000;
int cheapest_node = 10000;

void *scan_matrix(int *N)
{
    scanf("%d",N);

    int i,j;
    mat=(int*)malloc(*N * *N * sizeof(int));
    for(i=0;i<*N;i++){
        for(j=0;j<*N;j++){
            if((scanf("%d",&mat[i * *N+j]))==0)
            {
                free(mat);
                return NULL;
            }
            if(cheapest_node>mat[i * *N+j]){
                cheapest_node = mat[i * *N+j];
            }
        }
    }
}

bool crossing(int nodes_so_far[], int new_node, int slot, int n, bool free_nodes[]){
    int i,j,k,l;
    free_nodes[new_node]=false;
    for(i=0; i<slot-1; ++i){
        if(connection_matrix[nodes_so_far[i]][new_node]==1){
            for(j=i+1; j<slot; ++j){
                for(k=0; k<=n; ++k){
                    if(free_nodes[k] && connection_matrix[k][nodes_so_far[j]]){

                        free_nodes[new_node]=true;
                        return true;
                    }
                }
            }
        }
    }
    free_nodes[new_node]=true;
    return false;
}

void add(int drill_head[], bool free_nodes[], int i, int n, int cost){
    int j,tmp,l;
    if (i == n+1){

        if(min_cost>cost){
            min_cost = cost;
        }
    }else{
        for(j = 0; j <= n; ++j){
            if(free_nodes[j]){

                if(crossing(drill_head,j,i,n,free_nodes))continue;
                if(cost + (n-i+1)*cheapest_node >= min_cost)break;

                free_nodes[j] = false;
                drill_head[i] = j;
                add(drill_head, free_nodes, i+1, n, cost + mat[i*(n+1)+j]);
                free_nodes[j] = true;
            }
        }

    }

}



void scan_tree(int **connection_matrix)
{
    int n1,n2;

    while(scanf("%d %d",&n1,&n2)!=EOF){
        connection_matrix[n1][n2]=1;
        connection_matrix[n2][n1]=1;
    }
}

int main(){

    int N,i,j;
    int result;
    scan_matrix(&N);
    int drill_head[N];
    bool free_nodes[N];

    for (i = 0;i<N;++i){
        drill_head[i]=-1;
        free_nodes[i] = true;
    }


    connection_matrix = (int**) malloc(N * sizeof(int*));
    for(i=0;i<N;i++){
        *(connection_matrix+i) = (int*) calloc(N,sizeof(int));
    }

    scan_tree(connection_matrix);
    add(drill_head,free_nodes, 0, N-1,0);

    printf("%d\n",min_cost);
    return 0;
}
