#include <stdlib.h>
#include <stdio.h>

// Structura pentru un nod în lista de adiacenta
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Structură pentru graf
typedef struct Graph {
    int num_vertices;
    int *visited;
    struct Node **adj_list;
} Graph;

// Creează un nod nou pentru lista de adiacenta
Node *create_node(int data) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// Creeaza un graf cu un număr dat de noduri
Graph *create_graph(int num_vertices) {
    Graph *graph = malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->adj_list = malloc(num_vertices * sizeof(Node *));
    graph->visited = malloc(num_vertices * sizeof(int));

    for (int i = 0; i < num_vertices; i++) {
        graph->adj_list[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// Adaugă o muchie neorientata intre src și dest
void add_edge(Graph *graph, int src, int dest) {
    Node *new_node = create_node(dest);
    new_node->next = graph->adj_list[src];
    graph->adj_list[src] = new_node;

    new_node = create_node(src);
    new_node->next = graph->adj_list[dest];
    graph->adj_list[dest] = new_node;
}

// Adauga muchii în graf pe baza input-ului utilizatorului
void insert_edges(int num_vertices, int num_edges, Graph *graph) {
    int src, dest;
    printf("Adauga %d muchii (de la 1 la %d)\n", num_edges, num_vertices);
    for (int i = 0; i < num_edges; i++) {
        scanf("%d%d", &src, &dest);
        add_edge(graph, src - 1, dest - 1);
    }
}

// Verifica dacă coada este goala
int is_empty(Node *queue) {
    return queue == NULL;
}

// Adauga un nod în coada
void enqueue(Node **queue, int data) {
    Node *new_node = create_node(data);
    if (is_empty(*queue)) {
        *queue = new_node;
    } else {
        Node *temp = *queue;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

// Scoate un nod din coada si returneaza valoarea lui
int dequeue(Node **queue) {
    int data = (*queue)->data;
    Node *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

// Afiseaza graful (listele de adiacenta)
void print_graph(Graph *graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("Nod %d: ", i);
        Node *temp = graph->adj_list[i];
        while (temp) {
            printf("%d ", temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}

// Afiseaza coada (pentru debugging)
void print_queue(Node *queue) {
    while (queue != NULL) {
        printf("%d ", queue->data);
        queue = queue->next;
    }
}

// Reseteaza vectorul de vizitare
void wipe_visited_list(Graph *graph, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        graph->visited[i] = 0;
    }
}

// Parcurge graful cu DFS
void DFS(Graph *graph, int vertex) {
    Node *adj_list = graph->adj_list[vertex];
    Node *temp = adj_list;

    graph->visited[vertex] = 1;
    printf("%d -> ", vertex);

    while (temp != NULL) {
        int connected_vertex = temp->data;
        if (graph->visited[connected_vertex] == 0) {
            DFS(graph, connected_vertex);
        }
        temp = temp->next;
    }
}

// Parcurge graful cu BFS
void BFS(Graph *graph, int start) {
    Node *queue = NULL;

    graph->visited[start] = 1;
    enqueue(&queue, start);

    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);

        Node *temp = graph->adj_list[current];
        while (temp) {
            int adj_vertex = temp->data;
            if (graph->visited[adj_vertex] == 0) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
}

int main() {
    int num_vertices;
    int num_edges;
    int starting_vertex;

    printf("Cate noduri are graful? ");
    scanf("%d", &num_vertices);
    printf("Cate muchii are graful? ");
    scanf("%d", &num_edges);

    Graph *graph = create_graph(num_vertices);
    insert_edges(num_vertices, num_edges, graph);

    printf("De unde plecam in DFS? ");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu DFS: ");
    DFS(graph, starting_vertex - 1);
    wipe_visited_list(graph, num_vertices);

    printf("\nDe unde plecam in BFS? ");
    scanf("%d", &starting_vertex);
    printf("Parcurgere cu BFS: ");
    BFS(graph, starting_vertex - 1);

    return 0;
}
