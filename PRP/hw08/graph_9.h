#ifndef __GRAPH_H__
#define __GRAPH_H__

typedef struct {
   // TODO - implement your own struct for graph
    int capacity; // empty struct is not allowed
    int num_of_edges;
    int num_of_nodes;
    int *node;
    int *A;
    int *B;
    int *value;

} graph_t;


/* Allocate a new graph and return a reference to it. */
graph_t* allocate_graph();
/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(graph_t **graph);

/* Load a graph from the text file. */
void load_txt(const char *fname, graph_t *graph);
/* Load a graph from the binary file. */
void load_bin(const char *fname, graph_t *graph);

/* Save the graph to the text file. */
void save_txt(const graph_t * const graph, const char *fname);
/* Save the graph to the binary file. */
void save_bin(const graph_t * const graph, const char *fname);

#endif // __GRAPH_H__
