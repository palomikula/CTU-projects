#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

int nuzky=0,vrtacka=0,ohybacka=0,svarecka=0,lakovna=0,sroubovak=0,freza=0;
pthread_mutex_t queue_lock;
pthread_cond_t cond_q;

int count=0;
int working=0;
bool end=false;
bool eof=false;

const char process[3][6][20]={{"nuzky","vrtacka","ohybacka","svarecka","vrtacka","lakovna"},
                                {"vrtacka","nuzky","freza","vrtacka","lakovna","sroubovak"},
                                {"freza","vrtacka","sroubovak","vrtacka","freza","lakovna"}};


typedef struct{
    char* name;
    char workspace[200];
    pthread_t tid;
    bool done;
}thread_t;

thread_t threads[50];

typedef struct queue{
    char type;
    int stage;
    struct queue *next;
}production_queue;

production_queue *head;

production_queue* delete_from_queue(production_queue* before){

    production_queue *tmp;

    tmp=before->next;
    before->next=before->next->next;
    return tmp;
}

void print_queue(){
    int i;
    production_queue* cur = head;

    for(i=0;cur->next!=NULL;i++){

        printf("%d ---> %c %d %s\n",i,cur->next->type,cur->next->stage,process[cur->next->type-'A'][cur->next->stage]);
        cur=cur->next;
    }
}

int* map_workspaces(char* sem_string){
    if(strcmp(sem_string,"nuzky")==0){
        return &nuzky;
    }else if(strcmp(sem_string, "sroubovak")==0){
        return &sroubovak;
    }else if(strcmp(sem_string, "vrtacka")==0){
        return &vrtacka;
    }else if(strcmp(sem_string, "ohybacka")==0){
        return &ohybacka;
    }else if(strcmp(sem_string, "svarecka")==0){
        return &svarecka;
    }else if(strcmp(sem_string, "freza")==0){
        return &freza;
    }else if(strcmp(sem_string, "lakovna")==0){
        return &lakovna;
    }
    return NULL;
}

int sleep_time(char* sem_string){
    if(strcmp(sem_string,"nuzky")==0){
        return 100;
    }else if(strcmp(sem_string, "sroubovak")==0){
        return 250;
    }else if(strcmp(sem_string, "vrtacka")==0){
        return 200;
    }else if(strcmp(sem_string, "ohybacka")==0){
        return 150;
    }else if(strcmp(sem_string, "svarecka")==0){
        return 300;
    }else if(strcmp(sem_string, "freza")==0){
        return 500;
    }else if(strcmp(sem_string, "lakovna")==0){
        return 400;
    }
    return -1;
}

bool can_work(thread_t* worker){
    int i;
    production_queue *cur = head;

    while(cur->next!=NULL){
        cur=cur->next;
        for(i=0;i<count;i++){
            if((strcmp(process[cur->type -'A'][cur->stage],threads[i].workspace)==0 && *map_workspaces(threads[i].workspace)>0 && !threads[i].done)){

                if(strcmp(threads[i].workspace,worker->workspace)==0){
                    return true;
                }
                return false;
            }
        }

    }
    if(eof&&working==1){
        for(i=0;i<count;i++){
            if(worker->name!=NULL){
                free(threads[i].name);
            }
        }
        exit(0);

    }

    return false;
}

void insert_to_queue(char product, int stage){

    production_queue *tmp = (production_queue*) malloc(sizeof(production_queue));

    tmp->type = product;
    tmp->stage = stage;
    tmp->next = NULL;

    production_queue *cur = head;

    while(1){
        if (cur->next == NULL){
            cur->next = tmp;
            return;
        }if(cur->next->stage > stage){
            cur=cur->next;
        }else if(cur->next->stage == stage && cur->next->type < product){
            cur=cur->next;
        }else{
            tmp->next=cur->next;
            cur->next = tmp;
            return;
        }
    }

}

void *worker_thread(void* arg){
    thread_t *cur_worker = (thread_t*)arg;
    pthread_mutex_lock(&queue_lock);
    working++;

    production_queue* tmp;
    production_queue* current;
    int work_time = sleep_time(cur_worker->workspace);

    while(1){
        current=head;

        while(!(can_work(cur_worker)||end)){
            working--;
            if(cur_worker->done){
                free(cur_worker->name);
                cur_worker->name=NULL;
                pthread_cond_broadcast(&cond_q);
                pthread_mutex_unlock(&queue_lock);
                pthread_exit(0);
            }
            pthread_cond_wait(&cond_q,&queue_lock);
            working++;

        }


        while(current->next!=NULL){

            if(strcmp(process[current->next->type -'A'][current->next->stage],cur_worker->workspace)==0){
                tmp = delete_from_queue(current);
                printf("%s %s %c %d\n",cur_worker->name,cur_worker->workspace,tmp->type,tmp->stage+1);

                --*map_workspaces(cur_worker->workspace);
                pthread_cond_broadcast(&cond_q);
                pthread_mutex_unlock(&queue_lock);

                usleep(1000*work_time);
                pthread_mutex_lock(&queue_lock);

                if(tmp->stage<5){

                    insert_to_queue(tmp->type,tmp->stage+1);

                }else{
                    printf("done %c\n",tmp->type);
                }
                free(tmp);

                ++*map_workspaces(cur_worker->workspace);
                pthread_cond_broadcast(&cond_q);
                if(cur_worker->done){
                    free(cur_worker->name);
                    cur_worker->name=NULL;
                    working--;
                    pthread_mutex_unlock(&queue_lock);
                    pthread_exit(0);
                }
                break;
            }
            current=current->next;

        }

    }
    pthread_mutex_unlock(&queue_lock);
    return NULL;
}

int main(){
    int i;
    char command[200];
    char product[5];
    char *name = (char*)malloc(1000) ;
    char workspace[200];

    head = (production_queue*) malloc(sizeof(production_queue));
    head->next=NULL;

    pthread_mutex_init(&queue_lock,NULL);
    pthread_cond_init(&cond_q,NULL);

    //pthread_mutex_lock(&queue_lock);
    while(scanf("%s",command)!=EOF){
        //fprintf(stderr,"%s ",command);
        if (strcmp(command,"make")==0){
            if(scanf("%s",product)!=1){
                continue;
            }
            //fprintf(stderr,"%s\n",product);
            pthread_mutex_lock(&queue_lock);
            insert_to_queue(product[0],0);
            pthread_mutex_unlock(&queue_lock);
            pthread_cond_broadcast(&cond_q);

        }else if(strcmp(command,"start")==0){
            if (scanf("%s%s",name,workspace)!=2){
                continue;
            }
            //fprintf(stderr,"%s %s\n",name,workspace);
            threads[count].name=(char*)malloc(1000);
            strcpy(threads[count].name,name);
            strcpy(threads[count].workspace,workspace);
            threads[count].done=false;
            pthread_create(&threads[count].tid,NULL,worker_thread,(void*)&(threads[count]));
            count++;

        }else if(strcmp(command,"end")==0){
            if(scanf("%s",name)!=1){
                continue;
            }
            //fprintf(stderr,"%s\n",name);
            for(i=0;strcmp(name,threads[i].name)!=0;i++);
            threads[i].done=true;

        }else if(strcmp(command,"add")==0){
            if(scanf("%s",workspace)!=1){
                continue;
            }
            //fprintf(stderr,"%s\n",workspace);
            ++*map_workspaces(workspace);
            pthread_cond_broadcast(&cond_q);

        }else if(strcmp(command,"remove")==0){
            if(scanf("%s",workspace)!=1){
                continue;
            }
            //fprintf(stderr,"%s\n",workspace);

            pthread_mutex_lock(&queue_lock);
            --*map_workspaces(workspace);
            pthread_mutex_unlock(&queue_lock);
        }
    }

    eof=true;
    pthread_cond_broadcast(&cond_q);

    free(name);
    for(i=0;i<=count;++i){
        pthread_join(threads[i].tid,NULL);
    }


    return 0;
}
