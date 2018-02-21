#include "dijkstra.h"

#include <stdio.h>

int main(int argc, char *argv[]) 
{
   int ret = 0;

   if (argc < 3) {
      fprintf(stderr, "Call as\n %s graph_file solution_file\n", argv[0]);
   } else {
      fprintf(stderr, "Load graph from %s\n", argv[1]);
      void *dij = dijkstra_init();
		int b[23][3];
		b[0][0]=0;
		b[0][1]=20;
		b[0][2]=1;
		b[1][0]=1;
		b[1][1]=30;
		b[1][2]=2;
		b[2][0]=2;
		b[2][1]=40;
		b[2][2]=3;
      dijkstra_set_graph(3, b, dij);
      fprintf(stderr, "Find all shortest paths from the node 0\n");
      dijkstra_solve(dij, 0);
      fprintf(stderr, "Save solution to %s\n", argv[2]);
      //dijkstra_save_path(dij, argv[2]);
	  int a[500][3];
	  dijkstra_get_solution(dij, 3, a);
      fprintf(stderr, "Free allocated memory\n");
      dijkstra_free(dij);
      ret = 0;
   }
   return ret;
}

/* end of tgraph_search.c */
