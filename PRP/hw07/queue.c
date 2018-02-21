#include "queue.h"
#include "string.h"

// TODO - your code

queue_t* create_queue(int capacity)
{
    queue_t *queue;
    queue=(queue_t*)malloc(sizeof(queue_t));
    queue->sz=0;
    queue->alloc=capacity;
    queue->arr=(int*)calloc(capacity,sizeof(int));
    queue->tail=queue->arr;
    queue->head=queue->tail;
    queue->start=queue->tail;
    queue->end=queue->start+capacity;
    return queue;
}

void delete_queue(queue_t *queue)
{
    free(queue->arr);
    free(queue);
}


bool push_to_queue(queue_t *queue, void *data)
{
    int *tmp;
    tmp=(int*)malloc(queue->sz*sizeof(int));
    if(queue->alloc<=queue->sz){
        queue->alloc*=2;
        queue->arr=(int*)realloc(queue->arr,queue->alloc*sizeof(int));
        queue->tail+=queue->arr-queue->start;
        memmove(tmp,queue->arr,(int)(1+queue->head-queue->start)*sizeof(int));
        memmove(queue->arr,queue->tail+1,sizeof(int)*(int)(-1+queue->end-(queue->head)));
        memmove(queue->arr+(queue->end-queue->head)-1,tmp,(int)(1+queue->head-queue->start)*sizeof(int));
        queue->start=queue->arr;
        queue->tail=queue->arr+queue->alloc-1;
        queue->head=queue->start+queue->sz-1;
        queue->end=queue->tail+1;
    }

    *queue->tail = *((int*)data);

    free(data);
    free(tmp);
    queue->sz++;
    if(queue->tail==queue->arr){
        queue->tail+=(queue->alloc-1);
    }
    else{
        queue->tail--;
    }
    return true;
}


void* pop_from_queue(queue_t *queue)
{
    int *tmp;
    int *temp;
    if(queue->sz==0){
        return NULL;
    }
    if(queue->alloc>queue->sz*3&&queue->sz>10){
        temp=queue->tail;
        tmp=(int*)malloc(queue->alloc*sizeof(int));
        queue->alloc/=1;
        queue->tail+=queue->arr-queue->start;
        memmove(tmp,queue->arr,(int)(1+queue->head-queue->start)*sizeof(int));
        memmove(queue->arr,queue->tail+1,sizeof(int)*(int)(-1+queue->end-temp));
        queue->arr=(int*)realloc(queue->arr,(queue->alloc*2)*sizeof(int));
        memmove(queue->arr+(queue->end-temp),tmp+1,(int)(queue->head-queue->start)*sizeof(int));
        queue->arr=(int*)realloc(queue->arr,queue->alloc*sizeof(int));
        queue->start=queue->arr;
        queue->tail=queue->arr+queue->alloc-1;
        queue->head=queue->start+queue->sz-1;
        queue->end=queue->tail+1;
        free(tmp);
    }
    queue->sz-=1;
    int *ptr;
    ptr=(int*)malloc(sizeof(int));
    memcpy(ptr,queue->head,sizeof(int));
    if(queue->head==queue->arr){
        queue->head+=queue->alloc-1;
    }
    else{
        queue->head--;
    }
    return ptr;
}


void* get_from_queue(queue_t *queue, int idx)
{
    int* ptr;
    if(queue->sz<=idx||idx<0){
        return NULL;
    }
    if(queue->head>=queue->tail){
        ptr = (queue->head-idx);
    }else if(queue->head-queue->arr>=idx){
        ptr = (queue->head-idx);
    }else{
        ptr = (queue->alloc-idx)+queue->head;
    }

    return ptr;
}


int get_queue_size(queue_t *q)
{
    return q->sz;
}
