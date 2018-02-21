#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int n=1;
int ret = 0;
bool should_return=false;
sem_t sem_commands;
int sem_value=1;
pthread_mutex_t take_command;
pthread_mutex_t write_to_stdout;

pthread_t prod;
pthread_t *con;

typedef struct command{
    unsigned int x;
    char *word;
    struct command* next;
}command_t;

command_t* head;

void *producer(){
    unsigned int x;
    int scan;
    char some_junk[200];
    //int i=0;
    char *word;
    head = malloc(sizeof(command_t));
    command_t * current;
    current=head;
    //printf("allocated %d\n",current);
    while((scan = scanf("%u %ms", &x,&word))!=EOF){
        if(scan == 2){
            current->x = x;
            current->word = word;
            current->next = malloc(sizeof(command_t));
            current = current->next;
            //printf("allocated %d\n",current);
            sem_post(&sem_commands);
        }else{
            if(scanf("%s",some_junk)==0)exit(1);
            ret = 1;
        }

    }
   // printf("freeing %d\n",current);
    free(current);
    pthread_exit(0);
}

void *consumer(void* arg){
    int *id = (int*)arg;
    int i;
    command_t *current;
    //printf("thread %d created\n", *id);
    //pthread_join(prod,NULL);
    while(1){
        //fprintf(stderr,"Thread %d: going in\n",*id);
        sem_wait(&sem_commands);
        if(should_return)pthread_exit(0);
       // fprintf(stderr,"Thread %d: going out\n",*id);

        pthread_mutex_lock(&take_command);

        current = head;
        head = head->next;

        pthread_mutex_unlock(&take_command);
        pthread_mutex_lock(&write_to_stdout);
        printf("Thread %d:",*id);
        for(i=0;i<current->x;i++){
            printf(" %s",current->word);
        }
        printf("\n");
        fflush(stdout);
        pthread_mutex_unlock(&write_to_stdout);
        //printf("freeing %d\n",current);
		free(current->word);
        free(current);
    }
    pthread_exit(0);
}

int main(int argc, char* argv[]){
    //printf(strlen("5"));
    int i;
    if(argc>1){
        n=0;
        for(i=0;i<strlen(argv[1]);i++){
            n+=pow(10,strlen(argv[1])-i-1)*(argv[1][i] - '0');
        }
        //n = argv[1][0] - '0';
        if(n > sysconf(_SC_NPROCESSORS_ONLN) || n < 1 )return 1;
    }
    con = (pthread_t*)malloc(50*sizeof(pthread_t));
    sem_init(&sem_commands, 0, 0);
    pthread_mutex_init(&take_command, NULL);
    pthread_mutex_init(&write_to_stdout, NULL);
    int args[n];
    pthread_create(&prod, NULL, producer,NULL);

    for (i = 1; i <= n; i++){
        args[i]=i;
        pthread_create(&con[i], NULL, consumer, (void*)&args[i]);
    }
    pthread_join(prod, NULL);
    sleep(1);
    should_return=true;
    //prod_running=false;
    //last_one=true;
    //sem_post(&sem_commands);

    for (i = 1; i <= n; i++){
        sem_post(&sem_commands);
    }

    for (i = 1; i <= n; i++){
        pthread_join(con[i], NULL);
    }
    sem_destroy(&sem_commands);
    pthread_mutex_destroy(&take_command);
    pthread_mutex_destroy(&write_to_stdout);
    free(con);
    return ret;
}
