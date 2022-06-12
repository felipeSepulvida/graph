#ifndef GRAPH_H
#define GRAPH_H

typedef struct graph * graph_t;

graph_t graph_create(int is_oriented);
void graph_insert_vertex(graph_t graph, int new_vertex_value);
void graph_insert_edge(graph_t graph, int tail, int head);
void graph_print(graph_t graph);
void *graph_destroy(graph_t graph);
void graph_remove_edge(graph_t graph, int tail, int head);
void graph_remove_vertex(graph_t graph, int removed_vertex_value);

#endif