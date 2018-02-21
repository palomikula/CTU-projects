#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

/* This is a testing file just to be able to compile the
 * examples of binary files to convert graph from txt->bin
 * and bin -> txt
 */

void swap_memory(char *A, char* B){

    char* tmp = (char*)malloc(1);
    memmove(tmp, B, 1);
    memmove(B, A, 1);
    memmove(A, tmp, 1);
    free(tmp);
}

// - function -----------------------------------------------------------------
graph9_t* allocate_graph()
{
    graph9_t* g;
    int alloc=2;
    g=(graph9_t*)malloc(sizeof(graph9_t));
    g->A=(int*)malloc(alloc*sizeof(int));
    g->B=(int*)malloc(alloc*sizeof(int));
    g->value=(int*)malloc(alloc*sizeof(int));
    return g;
}

// - function -----------------------------------------------------------------
void free_graph(graph9_t **graph)
{
    free((*graph)->A);
    free((*graph)->B);
    free((*graph)->value);
    free(*graph);
}

// - function -----------------------------------------------------------------
void load_txt(const char *fname, graph9_t *graph)
{
    int alloc=2,i=0;
    FILE* file = fopen(fname,"r");
    while(1){
        if(i==alloc){
            alloc*=2;
            graph->A=(int*)realloc(graph->A,alloc*sizeof(int));
            graph->B=(int*)realloc(graph->B,alloc*sizeof(int));
            graph->value=(int*)realloc(graph->value,alloc*sizeof(int));
        }

        if(fscanf(file,"%d",(graph->A+i))==EOF){
            i++;
            break;
        }
        fscanf(file,"%d",graph->B+i);
        fscanf(file,"%d",graph->value+i);
        i++;
    }
    fclose(file);
    graph->num_of_edges=i;

}

// - function -----------------------------------------------------------------
void load_bin(const char *fname, graph9_t *graph)
{
    int alloc=2,i=0;
    char *p1,*p2,*p3,*p4;
    FILE* file = fopen(fname,"r");
    while(1){
        if(i==alloc){
            alloc*=2;
            graph->A=(int*)realloc(graph->A,alloc*sizeof(int));
            graph->B=(int*)realloc(graph->B,alloc*sizeof(int));
            graph->value=(int*)realloc(graph->value,alloc*sizeof(int));
        }

        if(fread(graph->A+i,sizeof(int),1,file)==0){
            i++;
            break;
        }
        p1=(char*)(void*)(graph->A+i);
            p2=p1+1;
            p3=p2+1;
            p4=p3+1;
            swap_memory(p1,p4);
            swap_memory(p2,p3);

        fread(graph->B+i,sizeof(int),1,file);
        p1=(char*)(void*)(graph->B+i);
            p2=p1+1;
            p3=p2+1;
            p4=p3+1;
            swap_memory(p1,p4);
            swap_memory(p2,p3);

        fread(graph->value+i,sizeof(int),1,file);
        p1=(char*)(void*)(graph->value+i);
            p2=p1+1;
            p3=p2+1;
            p4=p3+1;
            swap_memory(p1,p4);
            swap_memory(p2,p3);

        i++;
    }
    fclose(file);
    graph->num_of_edges=i;

}

// - function -----------------------------------------------------------------
void save_txt(const graph9_t * const graph, const char *fname)
{
    int i=0;
    FILE* file = fopen(fname,"w");
    while(i+1 < graph->num_of_edges){
        fprintf(file,"%d ",*(graph->A+i));
        fprintf(file,"%d ",*(graph->B+i));
        fprintf(file,"%d\n",*(graph->value+i));
        i++;
    }
    fclose(file);

}

// - function -----------------------------------------------------------------
void save_bin( const graph9_t * const graph, const char *fname)
{
    int i=0;
    char *p1,*p2,*p3,*p4;
    FILE* file = fopen(fname,"w");
    while(i+1 < graph->num_of_edges){
        p1=(char*)(void*)(graph->A+i);
        p2=p1+1;
        p3=p2+1;
        p4=p3+1;
        swap_memory(p1,p4);
        swap_memory(p2,p3);


        p1=(char*)(void*)(graph->B+i);
        p2=p1+1;
        p3=p2+1;
        p4=p3+1;
        swap_memory(p1,p4);
        swap_memory(p2,p3);

        p1=(char*)(void*)(graph->value+i);
        p2=p1+1;
        p3=p2+1;
        p4=p3+1;
        swap_memory(p1,p4);
        swap_memory(p2,p3);

        //printf("%d\n",*(graph->A+i));
        fwrite(graph->A+i,sizeof(int),1,file);
        fwrite(graph->B+i,sizeof(int),1,file);
        fwrite(graph->value+i,sizeof(int),1,file);
        i++;
    }
    fclose(file);

}


