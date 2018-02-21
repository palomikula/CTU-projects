#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <algorithm>
#include <queue>
#include <iostream>
#include <vector>
#include <array>

#define LEFT 0
#define RIGHT 1

using namespace std;

int N,M;
int *visited;
vector<int> potential_sockets;

typedef struct node{
    int num;
    int num_connections;
    int depth;
    bool side;
    vector<struct node*>neighbors;
}node_t;

node_t* nodes;

bool is_socket(node_t *cur_node){
    int i;
    int depth;
    potential_sockets.clear();
    queue<int> q;
    cur_node->depth=0;
    vector<array<int,2>> in_depth (N,{0,0});
    memset(visited,0,(N+1)*sizeof(int));

    cur_node->neighbors[LEFT]->side=LEFT;
    cur_node->neighbors[RIGHT]->side=RIGHT;

    q.push(cur_node->num);
    while(!q.empty()){
        cur_node = &nodes[q.front()];
        q.pop();
        //printf("visited = %d\n",visited[cur_node->num]);
        if(visited[cur_node->num]==2)continue;
        for(i=0;i<cur_node->num_connections;i++){
            if(visited[cur_node->neighbors[i]->num]==0){
                visited[cur_node->neighbors[i]->num]++;
                q.push(cur_node->neighbors[i]->num);

                cur_node->neighbors[i]->depth = cur_node->depth+1;
                if(cur_node->neighbors[i]->depth!=1){
                    cur_node->neighbors[i]->side = cur_node->side;
                }
                in_depth[cur_node->depth+1][cur_node->neighbors[i]->side]++;

            }else if(cur_node->depth!=0 && cur_node->neighbors[i]->side!=cur_node->side){
                in_depth[cur_node->depth+1][cur_node->side]++;
                visited[cur_node->neighbors[i]->num]++;
                potential_sockets.push_back(cur_node->neighbors[i]->num);
            }

        }

    }
    //printf("end\n");
    depth = cur_node->depth;
    for(i=1;i<=depth;++i){
        //printf("%d %d\n",in_depth[i][LEFT],in_depth[i][RIGHT]);
        if(in_depth[i][LEFT]!=in_depth[i][RIGHT]){
            return false;
        }
    }
    return true;
}

int main(){
    int i,j,n1,n2,sz;
    //printf("%d",sizeof(node_t));
    scanf("%d %d",&N,&M);
    nodes = (node_t*) calloc(N+1,sizeof(node_t));
    for(i=0;i<M;++i){
        scanf("%d %d",&n1,&n2);

        nodes[n1].num = n1;
        nodes[n2].num = n2;
        nodes[n1].neighbors.push_back(&(nodes[n2])); //= &(nodes[n2]);
        nodes[n2].neighbors.push_back(&(nodes[n1]));//= &(nodes[n1]);
        nodes[n1].num_connections++;
        nodes[n2].num_connections++;
    }

    visited = (int*)calloc(N+1,sizeof(int));


    potential_sockets.push_back(N);

    for(i=1;i<=N;++i){

        if(nodes[i].num_connections==2){
            if(is_socket(&nodes[i])){
                sort(potential_sockets.begin(),potential_sockets.end());
                    break;
            }
        }
    }

    sz=(100>potential_sockets.size())?potential_sockets.size():100;
    for(i=0;i<sz;++i){
        printf("%d",potential_sockets[i]);
        if(i<sz-1){
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}
