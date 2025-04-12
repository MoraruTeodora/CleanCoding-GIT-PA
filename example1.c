#include <stdlib.h>
#include <stdio.h>

// Structură pentru un nod în lista de adiacența
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Structură pentru graf
typedef struct Graph {
    int vertices;
    int *visited;
    Node **adj_list;
} Graph;

// Structură pentru stiva
typedef struct Stack {
    int top;
    int capacity;
    int *elements;
} Stack;

// Creează un nod nou pentru lista de adiacența
Node *create_node(int data) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// Adaugă o muchie neorientată intre src și dest
void add_edge(Graph *graph, int src, int dest) {
    Node *new_node = create_node(dest);
    new_node->next = graph->adj_list[src];
    graph->adj_list[src] = new_node;

    new_node = create_node(src);
    new_node->next = graph->adj_list[dest];
    graph->adj_list[dest] = new_node;
}

// Creează un graf cu un numar dat de noduri
Graph *create_graph(int vertices) {
    Graph *graph = malloc(sizeof(Graph));
    graph->vertices = vertices;
    graph->adj_list = malloc(vertices * sizeof(Node *));
    graph->visited = malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph->adj_list[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// Creează o stiva cu o capacitate data
Stack *create_stack(int capacity) {
    Stack *stack = malloc(sizeof(Stack));
    stack->elements = malloc(capacity * sizeof(int));
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

// Adauga un element în stiva
void push(int value, Stack *stack) {
    if (stack->top + 1 >= stack->capacity) {
        printf("Stiva este plina!\n");
        return;
    }
    stack->top = stack->top + 1;
    stack->elements[stack->top] = value;
}

// Golește stiva
void pop_all(Stack *stack) {
    stack->top = -1;
}

// Parcurge graful în adâncime (DFS) pornind de la un nod dat
void DFS(Graph *graph, Stack *stack, int vertex) {
    Node *adj_list = graph->adj_list[vertex];
    Node *current = adj_list;
    graph->visited[vertex] = 1;
    printf("%d ", vertex);
    push(vertex, stack);

    while (current != NULL) {
        int connected_vertex = current->data;
        if (graph->visited[connected_vertex] == 0) {
            DFS(graph, stack, connected_vertex);
        }
        current = current->next;
    }
}

// Adauga muchii în graf pe baza input-ului utilizatorului
void insert_edges(Graph *graph, int num_edges, int num_vertices) {
    int src, dest;
    printf("Adauga %d muchii (de la 1 la %d)\n", num_edges, num_vertices);
    for (int i = 0; i < num_edges; i++) {
        scanf("%d%d", &src, &dest);
        add_edge(graph, src - 1, dest - 1);
    }
}

// Reseteaza vectorul de vizitare
void reset_visited(Graph *graph, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        graph->visited[i] = 0;
    }
}

// Eliberează memoria alocată pentru graf
void free_graph(Graph *graph) {
    for (int i = 0; i < graph->vertices; i++) {
        Node *current = graph->adj_list[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adj_list);
    free(graph->visited);
    free(graph);
}

// Eliberează memoria alocată pentru stivă
void free_stack(Stack *stack) {
    free(stack->elements);
    free(stack);
}

// Verifică dacă exista drum direct între două restaurante
int has_direct_path(Graph *graph, int num_vertices, Stack *stack) {
    int start, end;
    printf("Introduceti restaurantul de start (1-%d): ", num_vertices);
    scanf("%d", &start);
    printf("Introduceti restaurantul de destinatie (1-%d): ", num_vertices);
    scanf("%d", &end);

    if (start < 1 || start > num_vertices || end < 1 || end > num_vertices) {
        printf("Noduri invalide! Trebuie sa fie intre 1 si %d.\n", num_vertices);
        return 0;
    }

    pop_all(stack);
    DFS(graph, stack, start - 1);

    int reachable = graph->visited[end - 1];
    reset_visited(graph, num_vertices);

    if (reachable) {
        printf("Exista drum direct intre restaurantul %d si restaurantul %d.\n", start, end);
    } else {
        printf("Nu exista drum direct intre restaurantul %d si restaurantul %d.\n", start, end);
    }

    return reachable;
}

int main() {
    int num_vertices, num_edges;

    printf("Cate noduri are graful? ");
    scanf("%d", &num_vertices);

    printf("Cate muchii are graful? ");
    scanf("%d", &num_edges);

    Graph *graph = create_graph(num_vertices);
    Stack *stack = create_stack(2 * num_vertices);

    insert_edges(graph, num_edges, num_vertices);

    has_direct_path(graph, num_vertices, stack);

    free_graph(graph);
    free_stack(stack);

    return 0;
}
