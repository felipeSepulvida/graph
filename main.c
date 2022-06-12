#include "graph.h"

int main(){

    graph_t graph = graph_create(1);

    graph_insert_vertex(graph, 1);
    graph_insert_vertex(graph, 2);
    graph_insert_vertex(graph, 3);
    graph_insert_vertex(graph, 4);

    graph_insert_edge(graph, 1, 2);
    graph_insert_edge(graph, 2, 1);
    graph_insert_edge(graph, 2, 3);
    graph_insert_edge(graph, 3, 2);
    graph_insert_edge(graph, 3, 1);
    graph_insert_edge(graph, 1, 3);

    graph_print(graph);

    graph_remove_edge(graph, 3, 1);
    graph_remove_edge(graph, 3, 2);
    graph_remove_edge(graph, 1, 3); 

    graph_remove_vertex(graph, 4);

    graph_print(graph);

    graph_destroy(graph);

    return 0;
}