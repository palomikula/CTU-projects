#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <vector>
#include <map>

using namespace std;

char pattern[50];
bool itr[50];
vector<int> max_for_node;
int M,I,max_lenght=0;

typedef struct{
    int id;
    char status;
}to;

vector<vector<to>> g;
vector<vector<int>> lenghts;


int magic(int cur,int index){
    if(lenghts[cur][index]!=0)
        return lenghts[cur][index];
    int so_far,lenght = 0;
    for(to node : g[cur]){

        int i = index;
        while(itr[i]){
            if(pattern[i] == node.status){
                if((so_far = magic(node.id,i)) >= lenght)
                lenght = ++so_far;
            }
            i++;
        }
        if(pattern[i] == node.status){
            if((so_far = magic(node.id,++i)) >= lenght)
             lenght = ++so_far;
        }
    }
    lenghts[cur][index]=lenght;
    return lenght;
}

int main(){
    char c;
    int i=0,j=0,n1,n2,lenght;
    while(1){
        scanf("%c",&c);
        if(c=='*'){
            itr[i-1]=true;
            continue;
        }if(c=='\n' || c=='\r'){
            pattern[i]='\n';
            break;
        }
        pattern[i]=c;
        ++i;
    }
    scanf("%d %d",&M,&I);
    g.resize(M+1);
    lenghts.resize(M+1,vector<int>(50,0));
    for(i=0;i<I;++i){
        scanf("%d %d %c",&n1,&n2,&c);
        g[n1].push_back((to){n2,c});
    }
    for(i=0;i<M;++i){
        lenght = magic(i, 0);
        if(lenght > max_lenght)
            max_lenght = lenght;
    }
    printf("%d\n", max_lenght);

    return 0;
}
