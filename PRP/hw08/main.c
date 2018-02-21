#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "graph.c"

int main()
{
    graph_t *graph=allocate_graph();
    load_bin("out.bin",graph);
    printf("%d\n",*(graph->A+1));
    printf("%d\n",*graph->B);
    printf("%d\n",*graph->value);
    //free_graph(graph);
    save_txt(graph,"out.txt");
    free_graph(&graph);

    /*load_bin("in.bin",graph);
    printf("%d\n",*graph->edge[0].A);
    printf("%d\n",*graph->edge[0].B);
    printf("%d\n",*graph->edge[0].value);*/
    return 0;
}
