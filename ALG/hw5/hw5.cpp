#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <utility>
#include <queue>
#include <map>

using namespace std;

typedef struct node{
    int id;
    int depth;
    node *parent;
    node *left;
    node *right;
}node_t;

vector<node_t*> trees_to_merge;
vector<int> at_depth;
pair<int,int> interval;
map<int,node_t*> node_map;
int D,H;
node_t *final_tree=NULL;
node_t* tmp;

void delete_interval(node_t *cur_node, node_t* root){

    if(cur_node==NULL)return;
    node_map[cur_node->id]=root;
    if(cur_node->id>=interval.first && cur_node->id<=interval.second){
        if(cur_node->parent!=NULL){
            (cur_node->parent->left==cur_node)
            ?cur_node->parent->left=NULL
            :cur_node->parent->right=NULL;
            cur_node->parent=NULL;
        }
        root=cur_node->left;
        //
        if(cur_node->left!=NULL)cur_node->left->parent=NULL;
        if(root!=NULL&&(root->id<interval.first || root->id>interval.second)){
            trees_to_merge.push_back(root);
        }
        delete_interval(cur_node->left,root);
        root=cur_node->right;
        if(cur_node->right!=NULL)cur_node->right->parent=NULL;
        if(root!=NULL&&(root->id<interval.first || root->id>interval.second)){
            trees_to_merge.push_back(root);
        }
        delete_interval(cur_node->right,root);
    }else if((cur_node->parent!=NULL && cur_node->id<interval.first && cur_node->parent->id>interval.second)||
       (cur_node->parent!=NULL && cur_node->id>interval.second && cur_node->parent->id<interval.first)){
        (cur_node->parent->left==cur_node)
        ?cur_node->parent->left=NULL
        :cur_node->parent->right=NULL;
        cur_node->parent=NULL;
        trees_to_merge.push_back(cur_node);
        node_map[cur_node->id]=cur_node;
        delete_interval(cur_node->right,cur_node);
        delete_interval(cur_node->left,cur_node);
    }else{
        delete_interval(cur_node->right,root);
        delete_interval(cur_node->left,root);
    }
}

node_t* merge_bst(vector<int> nodes){

    if(nodes.empty()) return NULL;
    if((int)nodes.size()==1) return node_map[nodes[0]];
    int middle_index = (((int)nodes.size()-1)/2);

    int i;
    for(i=middle_index;node_map[nodes[middle_index]]==node_map[nodes[i]];i--);
    for(tmp=node_map[nodes[middle_index]];tmp->left!=NULL;tmp=tmp->left);

    tmp->left=merge_bst(vector<int>(nodes.begin(),nodes.begin()+i+1));

    if(tmp->left!=NULL)
        tmp->left->parent=tmp;

    for(i=middle_index;node_map[nodes[middle_index]]==node_map[nodes[i]];i++);
    for(tmp=node_map[nodes[middle_index]];tmp->right!=NULL;tmp=tmp->right);
    tmp->right=merge_bst(vector<int>(nodes.begin()+i,nodes.end()));
    if(tmp->right!=NULL)
        tmp->right->parent=tmp;

    return node_map[nodes[middle_index]];

}
int bfs(node_t* root)
{
    queue<node_t*>q;
    node_t* cur;
    root->depth=0;
    q.push(root);
    while(!q.empty()){
        cur=q.front(); q.pop();
        if((int)at_depth.size()<=cur->depth){
            at_depth.push_back(1);
        }else{
            at_depth[cur->depth]++;
        }
        if(cur->left!=NULL){
            cur->left->depth=cur->depth+1;
            q.push(cur->left);
        }
        if(cur->right!=NULL){
            cur->right->depth=cur->depth+1;
            q.push(cur->right);
        }
    }
    return (int)at_depth.size()-1;
}

int main(){
    int n,node_in,i;
    vector<int> nodes;
    node_t *cur,*root;
    root=(node_t*)malloc(sizeof(node_t));
    scanf("%d",&n);
    scanf("%d",&root->id);
    nodes.push_back(root->id);
    //printf("root: %d\n",root->id);
    root->depth=0;
    root->parent=NULL;
    root->left=NULL;
    root->right=NULL;
    for(i=0;i<n-1;i++){
        cur=root;
        scanf("%d",&node_in);
        nodes.push_back(node_in);
        node_map[node_in]=NULL;
        while(1){
            if(node_in<cur->id){
                //printf(" -> left");
                if(cur->left!=NULL){
                    cur=cur->left;
                    continue;
                }else{
                    cur->left=(node_t*)malloc(sizeof(node_t));
                    cur->left->id=node_in;
                    cur->left->depth=cur->depth+1;
                    cur->left->parent=cur;
                    cur->left->left=NULL;
                    cur->left->right=NULL;
                    //printf("\n");
                    break;
                }
            }else{
                //printf(" -> right");
                if(cur->right!=NULL){
                    cur=cur->right;
                    continue;
                }else{
                    cur->right=(node_t*)malloc(sizeof(node_t));
                    cur->right->id=node_in;
                    cur->right->depth=cur->depth+1;
                    cur->right->parent=cur;
                    cur->right->left=NULL;
                    cur->right->right=NULL;
                    //printf("\n");
                    break;
                }
            }
        }

    }

    scanf("%d%d",&interval.first,&interval.second);
    cur=root;
    if(cur->id<interval.first || cur->id>interval.second){
        trees_to_merge.push_back(root);
    }
    delete_interval(cur,root);
    //printf("there are %d trees to merge\n",(int)trees_to_merge.size());

    sort(nodes.begin(),nodes.end());

    for(i=0;nodes[i]<interval.first;i++);
    auto start=nodes.begin()+i;
    for(i=i;nodes[i]<=interval.second;i++);
    auto finish=nodes.begin()+i;

    if(nodes.front()>=interval.first){
        start=nodes.begin();
    }
    if(nodes.back()<=interval.second){
        finish=nodes.end();
    }
    nodes.erase(start,finish);
    root=merge_bst(nodes);

    int final_depth=bfs(root);
    int count=at_depth[final_depth-1];

    printf("%d %d\n",final_depth,count);

    return 0;
}

