#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

typedef struct neighbor
{
    struct vertex *neighbor_vertex;
    struct neighbor *next;

} * neighbor_t;

typedef struct vertex
{
    int vertex_value;
    neighbor_t neighborhood;
    struct vertex *next;
} * vertex_t;

struct graph
{
    int order;
    int is_oriented;
    vertex_t vertices;
};

vertex_t vertex_in_graph(graph_t graph, int vertex_value)
{
    vertex_t cursor = graph->vertices;
    while (cursor != NULL)
    {
        if (cursor->vertex_value == vertex_value)
        {
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
}

void free_neighborhood(vertex_t vertex)
{
    while (vertex->neighborhood)
    {
        neighbor_t freed_node = vertex->neighborhood;
        vertex->neighborhood = freed_node->next;
        free(freed_node);
    }
    return;
}

neighbor_t neighbor_in_vertex(vertex_t vertex, int neighbor_value)
{
    neighbor_t cursor = vertex->neighborhood;
    while (cursor)
    {
        if (cursor->neighbor_vertex->vertex_value == neighbor_value)
        {
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
}

graph_t graph_create(int is_oriented)
{
    graph_t new_graph = (graph_t)malloc(sizeof(struct graph));
    new_graph->is_oriented = is_oriented;
    new_graph->order = 0;
    new_graph->vertices = NULL;
    return new_graph;
}

void graph_insert_vertex(graph_t graph, int new_vertex_value)
{
    if (vertex_in_graph(graph, new_vertex_value))
    {
        printf("value already in.\n");
        return;
    }

    vertex_t new_vertex = (vertex_t)malloc(sizeof(struct vertex));
    new_vertex->vertex_value = new_vertex_value;
    new_vertex->neighborhood = NULL;
    new_vertex->next = graph->vertices;

    graph->vertices = new_vertex;
    graph->order++;

    return;
}

void insert_edge(vertex_t tail, vertex_t head)
{
    neighbor_t new_neighbor = (neighbor_t)malloc(sizeof(struct neighbor));
    new_neighbor->neighbor_vertex = head;
    new_neighbor->next = tail->neighborhood;
    tail->neighborhood = new_neighbor;
    return;
}

void graph_insert_edge(graph_t graph, int tail, int head)
{
    vertex_t tail_vertex = vertex_in_graph(graph, tail),
             head_vertex = vertex_in_graph(graph, head);

    if (!tail_vertex || !head_vertex)
    {
        printf("value not in graph.\n");
        return;
    }
    if (neighbor_in_vertex(tail_vertex, head))
    {
        printf("edge already exist.\n");
        return;
    }

    insert_edge(tail_vertex, head_vertex);
    if (!graph->is_oriented)
    {
        insert_edge(head_vertex, tail_vertex);
    }

    return;
}

void graph_print(graph_t graph)
{
    vertex_t cursor = graph->vertices;
    printf("\nadjacency lists:\n");
    while (cursor)
    {
        printf("(%d) :", cursor->vertex_value);
        neighbor_t cursor_n = cursor->neighborhood;
        while (cursor_n)
        {
            printf(" (%d -> %d)", cursor->vertex_value, cursor_n->neighbor_vertex->vertex_value);
            cursor_n = cursor_n->next;
        }
        printf("\n");
        cursor = cursor->next;
    }
    return;
}

void *graph_destroy(graph_t graph)
{
    vertex_t deleted_vertex = graph->vertices;
    while (deleted_vertex)
    {
        free_neighborhood(deleted_vertex);
        graph->vertices = deleted_vertex->next;
        free(deleted_vertex);
        deleted_vertex = graph->vertices;
    }
    free(graph);
    return NULL;
}

// recursiva
neighbor_t remove_edge(neighbor_t neighbor, int removed_neighbor_value)
{
    if (neighbor != NULL)
    {
        neighbor->next = remove_edge(neighbor->next, removed_neighbor_value);
        if (neighbor->neighbor_vertex->vertex_value == removed_neighbor_value)
        {
            neighbor_t next_neighbor = neighbor->next;
            free(neighbor);
            return next_neighbor;
        }
    }
    return neighbor;
}

void graph_remove_edge(graph_t graph, int tail, int head)
{
    if (!vertex_in_graph(graph, tail) || !vertex_in_graph(graph, head))
    {
        printf("value not in graph.\n");
        return;
    }

    vertex_t tail_vertex = vertex_in_graph(graph, tail);
    if (!neighbor_in_vertex(tail_vertex, head))
    {
        printf("no edge found.\n");
        return;
    }
    tail_vertex->neighborhood = remove_edge(tail_vertex->neighborhood, head);

    if (!graph->is_oriented)
    {
        vertex_t head_vertex = vertex_in_graph(graph, head);
        head_vertex->neighborhood = remove_edge(head_vertex->neighborhood, tail);
    }

    return;
}

void remove_edge_from_vertex(vertex_t vertex, int tail)
{
    if (!neighbor_in_vertex(vertex, tail))
    {
        return;
    }
    vertex->neighborhood = remove_edge(vertex->neighborhood, tail);
    return;
}

// recurssiva
vertex_t remove_vertex_oriented(vertex_t vertex, int removed_vertex_value)
{
    if (vertex != NULL)
    {
        vertex->next = remove_vertex_oriented(vertex->next, removed_vertex_value);
        if (vertex->vertex_value == removed_vertex_value)
        {
            vertex_t next_vertex = vertex->next;
            free_neighborhood(vertex);
            free(vertex);
            return next_vertex;
        }
        remove_edge_from_vertex(vertex, removed_vertex_value);
    }
    return vertex;
}

vertex_t remove_vertex_non_oriented(vertex_t removed_vertex)
{
    neighbor_t cursor = removed_vertex->neighborhood;
    while (cursor != NULL)
    {
        remove_edge_from_vertex(cursor->neighbor_vertex, removed_vertex->vertex_value);
    }
    vertex_t next_vertex = removed_vertex->next;
    free_neighborhood(removed_vertex);
    free(removed_vertex);
    return next_vertex;
}

void graph_remove_vertex(graph_t graph, int removed_vertex_value)
{
    if (!vertex_in_graph(graph, removed_vertex_value))
    {
        printf("value not in graph.\n");
        return;
    }

    if (graph->is_oriented)
    {
        graph->vertices = remove_vertex_oriented(graph->vertices, removed_vertex_value);
    }
    else
    {
        if (graph->vertices->vertex_value == removed_vertex_value)
        {
            graph->vertices = remove_vertex_non_oriented(graph->vertices);
        }
        else
        {
            vertex_t cursor = graph->vertices;
            while (cursor->next->vertex_value != removed_vertex_value)
            {
                cursor = cursor->next;
            }
            cursor->next = remove_vertex_non_oriented(cursor->next);
        }
    }

    graph->order--;
    return;
}