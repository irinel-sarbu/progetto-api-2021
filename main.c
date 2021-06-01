#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define NEW_GRAPH "AggiungiGrafo"
#define TOP_K "TopK"
#define INFINITY 4294967295

// #region Debugger settings

// Change to true / false to show / not show info
#define SHOW_MATRIX_INFO false
#define SHOW_MATRIX false
#define SHOW_HEAP false
#define SHOW_DIJKSTRA_SUM false
#define SHOW_DIJKSTRA_DISTANCES false
#define SHOW_DIJKSTRA false
#define SLOW_DIJKSTRA false

// #endregion

typedef unsigned short int sint;
typedef unsigned int uint;

// #region Function Declaration

typedef struct ListNode
{
    uint id;
    uint cost;
    struct ListNode *next;
} ListNode;

typedef struct List
{
    ListNode *head;
} List;

ListNode *List_createNode(uint, uint);
List *List_createList();
void List_insert(List *, uint, uint);
void List_print(List *, uint);
void List_free(List *);

typedef struct DijkstraNode
{
    uint id;
    uint cost;
} DijkstraNode;

typedef struct DijkstraMinHeap
{
    DijkstraNode *elements;
    uint capacity;
    uint size;
} DijkstraMinHeap;

int Heap_leftChild(int);
int Heap_rightChild(int);
int Heap_parent(int);

void Dijkstra_freeNode(DijkstraNode *);

void Dijkstra_heapSwap(DijkstraNode *, DijkstraNode *);
DijkstraMinHeap *Dijkstra_buildMinHeap(uint);
void Dijkstra_freeMinHeap(DijkstraMinHeap *);
DijkstraNode Dijkstra_minHeapPeak(DijkstraMinHeap *);
DijkstraNode Dijkstra_minHeapify(DijkstraMinHeap *, int);
void Dijkstra_minHeapPop(DijkstraMinHeap *);
void Dijkstra_minHeapInsert(DijkstraMinHeap *, uint, uint);
void Dijkstra_minHeapDecPri(DijkstraMinHeap *, uint, uint);
void Dijkstra_printMinHeap(DijkstraMinHeap *);

void Dijkstra_Print(uint *, uint);
uint *Dijkstra(sint **, uint);
void Dijkstra_freeCost(uint *);
uint Dijkstra_sumCost(uint *, uint);
void Dijkstra_test();

sint **Parser_getMatrix(uint);
void Parser_freeMatrix(sint **, uint);
void Parser_matrixPrint(sint **, uint);

// #endregion

// #region Main

int main()
{
    uint d, k;
    char cmd[14];
    sint counter = 0;
    List *result = List_createList();
    // Inizio Parser
    if (scanf("%d %d", &d, &k))
    {
    };
    if (SHOW_MATRIX_INFO)
        printf("[INFO] Matrix size set to %d\n[INFO] k set to %d\n", d, k);
    bool finished = false;

    while (!finished)
    {
        if (scanf("%s", cmd))
        {
        };
        if (strcmp(cmd, TOP_K) == 0)
        {
            List_print(result, k);
            List_free(result);
            finished = true;
        }
        if (strcmp(cmd, NEW_GRAPH) == 0)
        {
            sint **adj_matrix = Parser_getMatrix(d);
            if (SHOW_MATRIX_INFO)
                printf("\n[%*d] Matrix:\n", 5, counter);
            if (SHOW_MATRIX)
                Parser_matrixPrint(adj_matrix, d);

            uint *cost = Dijkstra(adj_matrix, d);
            uint sum = Dijkstra_sumCost(cost, d);
            List_insert(result,counter,sum);
            if (SHOW_DIJKSTRA_SUM)
            {
                printf("Edges sum to %d\n", sum);
            }
            if (SHOW_DIJKSTRA_DISTANCES)
            {
                Dijkstra_Print(cost, d);
            }
            Dijkstra_freeCost(cost);

            Parser_freeMatrix(adj_matrix, d);
            counter++;
        }
    }
    return 0;
}

// #endregion

// #region Parser Functions

sint **Parser_getMatrix(uint matrix_size)
{
    sint **matrix = malloc(matrix_size * sizeof(sint *));
    for (int y = 0; y < matrix_size; y++)
    {
        matrix[y] = calloc(matrix_size, sizeof(sint));
    }
    for (int y = 0; y < matrix_size; y++)
    {
        char *string;
        if (scanf("%ms", &string))
        {
        };
        char *cost_s = strtok(string, ",");
        int x = 0;
        while (cost_s != NULL)
        {
            sint cost = atoi(cost_s);
            if (x != y)
                matrix[y][x] = cost;
            x++;
            cost_s = strtok(NULL, ",");
        }
        free(string);
    }
    return matrix;
}
void Parser_freeMatrix(sint **matrix, uint size)
{
    for (uint x = 0; x < size; x++)
    {
        free(matrix[x]);
    }
    free(matrix);
}
void Parser_matrixPrint(sint **matrix, uint matrix_size)
{
    printf("+");
    for (int x = 1; x < matrix_size * 7; x++)
        printf("-");
    printf("+\n");
    for (int y = 0; y < matrix_size; y++)
    {
        printf("|");
        for (int x = 0; x < matrix_size; x++)
            printf("%*d|", 6, matrix[y][x]);
        printf("\n");
        printf("+");
        for (int x = 1; x < matrix_size * 7; x++)
            printf("-");
        printf("+\n");
    }
}

// #endregion

// #region Dijkstra Functions

int Heap_leftChild(int index)
{
    return (2 * index + 1);
}
int Heap_rightChild(int index)
{
    return (2 * index + 2);
}
int Heap_parent(int index)
{
    return (index - 1) / 2;
}

void Dijkstra_freeNode(DijkstraNode *node)
{
    free(node);
}

void Dijkstra_heapSwap(DijkstraNode *a, DijkstraNode *b)
{
    DijkstraNode temp = *a;
    *a = *b;
    *b = temp;
}
DijkstraMinHeap *Dijkstra_buildMinHeap(uint capacity)
{
    DijkstraMinHeap *heap = malloc(sizeof(DijkstraMinHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->elements = calloc(capacity, sizeof(DijkstraNode));
    for (int x = 0; x < capacity; x++)
    {
        heap->elements[x].id = -1;
        heap->elements[x].cost = -1;
    }
    return heap;
}
void Dijkstra_freeMinHeap(DijkstraMinHeap *heap)
{
    free(heap->elements);
    free(heap);
}
DijkstraNode Dijkstra_minHeapPeak(DijkstraMinHeap *heap)
{
    return heap->elements[0];
}
DijkstraNode Dijkstra_minHeapify(DijkstraMinHeap *heap, int i)
{
    int l = Heap_leftChild(i);
    int r = Heap_rightChild(i);
    int posmin = i;
    if (l < heap->size && heap->elements[l].cost < heap->elements[i].cost)
        posmin = l;
    if (r < heap->size && heap->elements[r].cost < heap->elements[posmin].cost)
        posmin = r;
    if (posmin != i)
    {
        Dijkstra_heapSwap(&heap->elements[i], &heap->elements[posmin]);
        Dijkstra_minHeapify(heap, posmin);
    }
}
void Dijkstra_minHeapPop(DijkstraMinHeap *heap)
{
    if (heap->size < 1)
        return;
    if (heap->size == 1)
    {
        heap->size -= 1;
        return;
    }
    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size -= 1;
    Dijkstra_minHeapify(heap, 0);
}
void Dijkstra_minHeapInsert(DijkstraMinHeap *heap, uint id, uint cost)
{
    heap->size += 1;
    int i = heap->size - 1;
    heap->elements[i].id = id;
    heap->elements[i].cost = cost;
    while (i > 0 && heap->elements[Heap_parent(i)].cost > heap->elements[i].cost)
    {
        Dijkstra_heapSwap(&heap->elements[i], &heap->elements[Heap_parent(i)]);
        i = Heap_parent(i);
    }
}
void Dijkstra_minHeapDecPri(DijkstraMinHeap *heap, uint id, uint cost)
{
    int i = 0;
    for (int x = 0; x < heap->size; x++)
        if (heap->elements[x].id == id)
            i = x;
    heap->elements[i].cost = cost;
    while (i > 0 && heap->elements[Heap_parent(i)].cost > heap->elements[i].cost)
    {
        Dijkstra_heapSwap(&heap->elements[i], &heap->elements[Heap_parent(i)]);
        i = Heap_parent(i);
    }
}
void Dijkstra_printMinHeap(DijkstraMinHeap *heap)
{
    printf("Heap:\n");
    printf("+-----------------+\n");
    for (int y = 0; y < heap->size; y++)
    {
        printf("|");
        printf("%*d|", 6, heap->elements[y].id);
        if (heap->elements[y].cost == INFINITY)
            printf("  INFINITY|");
        else
            printf("%*d|", 10, heap->elements[y].cost);
        if (y == 0)
            printf("<-");
        printf("\n");
        printf("+-----------------+\n");
    }
    printf("|      |          |\n");
    printf("+-----------------+\n\n");
}

uint *Dijkstra(sint **matrix, uint matrix_size)
{
    uint *cost = calloc(matrix_size, sizeof(uint));
    for (int x = 1; x < matrix_size; x++)
        cost[x] = INFINITY;
    DijkstraMinHeap *Q = Dijkstra_buildMinHeap(matrix_size);
    Dijkstra_minHeapInsert(Q, 0, 0);

    while (Q->size > 0)
    {
        DijkstraNode v = Dijkstra_minHeapPeak(Q);
        uint index = v.id;
        uint v_cost = v.cost;
        if (SHOW_DIJKSTRA)
            printf("Peaking min node %d with cost %d\n", index, v_cost);
        for (uint x = 0; x < matrix_size; x++)
        {
            if (x != index && matrix[index][x] > 0)
            {
                int total_cost = v_cost + matrix[index][x];
                // if(SHOW_DIJKSTRA)
                //     printf("Edge between %d - %d found. Cost %d\n",index,x,total_cost);
                if (total_cost < cost[x] && cost[x] == INFINITY)
                {
                    cost[x] = total_cost;
                    Dijkstra_minHeapInsert(Q, x, total_cost);
                }
                else if (total_cost < cost[x])
                {
                    cost[x] = total_cost;
                    Dijkstra_minHeapDecPri(Q, x, total_cost);
                }
            }
        }
        if (SHOW_DIJKSTRA)
        {
            Dijkstra_Print(cost, matrix_size);
            Dijkstra_printMinHeap(Q);
        }
        Dijkstra_minHeapPop(Q);
        if (SLOW_DIJKSTRA)
            sleep(2);
    }
    Dijkstra_freeMinHeap(Q);
    return cost;
}
void Dijkstra_Print(uint *cost, uint matrix_size)
{
    printf("Distances from origin:\n");
    printf("+-----------------------+\n");
    for (int y = 0; y < matrix_size; y++)
    {
        printf("|");
        printf("%*d|", 11, y);
        if (cost[y] == INFINITY)
            printf("%*s|", 11, "INFINITY");
        else
            printf("%*u|", 11, cost[y]);
        printf("\n");
        printf("+-----------------------+\n");
    }
}
void Dijkstra_freeCost(uint *cost)
{
    free(cost);
}
uint Dijkstra_sumCost(uint *cost, uint matrix_size)
{
    uint sum = 0;
    for (int x = 0; x < matrix_size; x++)
        if (cost[x] != INFINITY)
            sum += cost[x];
    return sum;
}
void Dijkstra_test()
{
    DijkstraMinHeap *heap = Dijkstra_buildMinHeap(10);
    Dijkstra_minHeapInsert(heap, 0, INFINITY);
    Dijkstra_minHeapInsert(heap, 0, 50);
    Dijkstra_minHeapInsert(heap, 0, 100);
    Dijkstra_minHeapInsert(heap, 0, 250);
    Dijkstra_minHeapInsert(heap, 0, 25);
    Dijkstra_minHeapInsert(heap, 0, 20);
    Dijkstra_minHeapInsert(heap, 0, 1000);

    while (heap->size > 0)
    {
        Dijkstra_printMinHeap(heap);
        DijkstraNode min = Dijkstra_minHeapPeak(heap);
        if (min.cost == INFINITY)
            printf("Peak inside\nMin.id = %d, Min.cost = INFINITY\nHeap size = %d\n\n", min.id, heap->size);
        else
            printf("Peak inside\nMin.id = %d, Min.cost = %u\nHeap size = %d\n\n", min.id, min.cost, heap->size);
        printf("Popped min\n\n");
        Dijkstra_minHeapPop(heap);
        printf("+---------------------------------------+\n");
    }

    Dijkstra_freeMinHeap(heap);
}

// #endregion

// #region Linked List Functions

ListNode *List_createNode(uint id, uint cost)
{
    ListNode *new = malloc(sizeof(ListNode));
    new->id = id;
    new->cost = cost;
    new->next = NULL;
    return new;
}

List *List_createList()
{
    List *new = malloc(sizeof(List));
    new->head = NULL;
    return new;
}

void List_insert(List *list, uint id, uint cost)
{
    ListNode *new = List_createNode(id, cost);
    if (list->head == NULL || list->head->cost > cost)
    {
        new->next = list->head;
        list->head = new;
    }
    else
    {
        ListNode *c = list->head;
        while (c->next != NULL && c->next->cost < cost)
        {
            c = c->next;
        }

        new->next = c->next;
        c->next = new;
    }
}

void List_print(List *list, uint k)
{
    uint counter = 0;
    ListNode *tmp = list->head;
    while (tmp != NULL && counter < k)
    {
        printf("%d ", tmp->id);
        tmp = tmp->next;
        counter ++;
    }
    printf("\n");
}

void List_free(List *list){
    ListNode *curr = list->head;
    ListNode *next;
    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

// #endregion