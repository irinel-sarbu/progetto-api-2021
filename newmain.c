#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define NEW_GRAPH "AggiungiGrafo"
#define TOP_K "TopK"
#define ENTER "\n"
#define FILE_END "\n"
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

typedef unsigned int sint;
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
    uint size;
    uint capacity;
    ListNode *head;
} List;

ListNode *List_createNode(uint, uint);
List *List_createList(uint);
void List_freeLast(List *);
void List_insert(List *, uint, uint);
void List_print(List *);
void List_free(List *);

typedef struct Node
{
    uint id;
    uint cost;
} Node;

typedef struct MinHeap
{
    Node *elements;
    uint capacity;
    uint size;
} MinHeap;

int Heap_leftChild(int);
int Heap_rightChild(int);
int Heap_parent(int);
void Heap_heapSwap(Node *, Node *);
void Heap_freeMinHeap(MinHeap *);

void Solution_minHeapify(MinHeap *, int);
void Solution_minHeapInsert(MinHeap *, uint, uint);
void Solution_printHeapSort(MinHeap *);

void Dijkstra_freeNode(Node *);
MinHeap *Heap_buildMinHeap(uint);
Node Heap_minHeapPeak(MinHeap *);
void Heap_minHeapPop(MinHeap *);
void Dijkstra_minHeapify(MinHeap *, int);

void Dijkstra_minHeapInsert(MinHeap *, uint, uint);
void Dijkstra_minHeapDecPri(MinHeap *, uint, uint);
void Dijkstra_printMinHeap(MinHeap *);

void Dijkstra_print(uint *, uint);
uint *Dijkstra(sint **, uint);
uint *Dijkstra_queue(sint *, uint);
void Dijkstra_freeCost(uint *);
uint Dijkstra_sumCost(uint *, uint);
void Dijkstra_test();

void Parser_matrixPrint(sint *, uint);

// #endregion

// #region Main

int main()
{
    uint d = 0, k = 0;
    char cmd[14];
    sint id = 0;

    sint *matrix = NULL;

    int r = 0;
    r = scanf("%d %d", &d, &k);

    matrix = (sint *)malloc(d * d * sizeof(sint));
    for (int y = 0; y < d; y++)
        for (int x = 0; x < d; x++)
            matrix[y * x + x] = 0;

    MinHeap *minheap_solution = Heap_buildMinHeap(k + 1);

    bool finished = false;

    do
    {
        r = scanf("%s", cmd);
        r = getchar_unlocked();
        if (r == EOF)
        {
            finished = true;
        }

        if (strcmp(cmd, NEW_GRAPH) == 0 && !finished)
        {
            for (int y = 0; y < d; y++)
            {

                for (int x = 0; x < d; x++)
                {
                    uint num = 0;
                    int c = getchar_unlocked();
                    while (c != ',' && c != '\n')
                    {
                        num = num * 10 + (c - '0');
                        c = getchar_unlocked();
                    }
                    matrix[y * d + x] = num;
                }
            }
            uint *cost = Dijkstra_queue(matrix, d);
            uint sum = Dijkstra_sumCost(cost, d);
            Solution_minHeapInsert(minheap_solution, id, sum);
            Dijkstra_freeCost(cost);
            id++;
        }

        if (strcmp(cmd, TOP_K) == 0 && !finished)
        {
            Solution_printHeapSort(minheap_solution);
            //List_print(list_solution);
        }
    } while (finished == false);

    Heap_freeMinHeap(minheap_solution);
    free(matrix);

    return 0;
}

// #endregion

// #region Parser Functions

void Parser_matrixPrint(sint *matrix, uint matrix_size)
{
    printf("+");
    for (int x = 1; x < matrix_size * 7; x++)
        printf("-");
    printf("+\n");
    for (int y = 0; y < matrix_size; y++)
    {
        printf("|");
        for (int x = 0; x < matrix_size; x++)
            printf("%*d|", 6, matrix[y * matrix_size + x]);
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

void Dijkstra_freeNode(Node *node)
{
    free(node);
}

void Heap_heapSwap(Node *a, Node *b)
{
    Node temp = *a;
    *a = *b;
    *b = temp;
}

MinHeap *Heap_buildMinHeap(uint capacity)
{
    MinHeap *heap = malloc(sizeof(MinHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->elements = calloc(capacity, sizeof(Node));
    for (int x = 0; x < capacity; x++)
    {
        heap->elements[x].id = -1;
        heap->elements[x].cost = -1;
    }
    return heap;
}

void Heap_freeMinHeap(MinHeap *heap)
{
    free(heap->elements);
    free(heap);
}

Node Heap_minHeapPeak(MinHeap *heap)
{
    return heap->elements[0];
}

void Dijkstra_minHeapify(MinHeap *heap, int i)
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
        Heap_heapSwap(&heap->elements[i], &heap->elements[posmin]);
        Dijkstra_minHeapify(heap, posmin);
    }
}

void Heap_minHeapPop(MinHeap *heap)
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

void Dijkstra_minHeapInsert(MinHeap *heap, uint id, uint cost)
{
    heap->size += 1;
    int i = heap->size - 1;
    heap->elements[i].id = id;
    heap->elements[i].cost = cost;
    while (i > 0 && heap->elements[Heap_parent(i)].cost > heap->elements[i].cost)
    {
        Heap_heapSwap(&heap->elements[i], &heap->elements[Heap_parent(i)]);
        i = Heap_parent(i);
    }
}

void Dijkstra_minHeapDecPri(MinHeap *heap, uint id, uint cost)
{
    int i = 0;
    for (int x = 0; x < heap->size; x++)
        if (heap->elements[x].id == id)
            i = x;
    heap->elements[i].cost = cost;
    while (i > 0 && heap->elements[Heap_parent(i)].cost > heap->elements[i].cost)
    {
        Heap_heapSwap(&heap->elements[i], &heap->elements[Heap_parent(i)]);
        i = Heap_parent(i);
    }
}

void Dijkstra_printMinHeap(MinHeap *heap)
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
        printf("\n");
        printf("+-----------------+\n");
    }
    printf("|      |          |\n");
    printf("+-----------------+\n\n");
}

uint *Dijkstra_queue(sint *matrix, uint matrix_size)
{
    uint *cost = malloc(matrix_size * sizeof(uint));
    MinHeap *Q = Heap_buildMinHeap(matrix_size);
    cost[0] = 0;
    Dijkstra_minHeapInsert(Q, 0, cost[0]);
    for (int x = 1; x < matrix_size; x++)
    {
        cost[x] = INFINITY;
        Dijkstra_minHeapInsert(Q, x, cost[x]);
    }
    if (SHOW_DIJKSTRA)
    {
        Dijkstra_print(cost, matrix_size);
        Dijkstra_printMinHeap(Q);
    }

    while (Q->size > 0)
    {
        Node u = Heap_minHeapPeak(Q);
        uint u_cost = u.cost;
        uint u_id = u.id;
        Heap_minHeapPop(Q);
        if (SHOW_DIJKSTRA)
            printf("Peaking min node %d with cost %d\n", u_id, u_cost);

        for (int v = 0; v < matrix_size && u_cost != INFINITY; v++)
        {
            if (u_id != v && matrix[u_id * matrix_size + v] > 0)
            {
                uint cost_u_v = matrix[u_id * matrix_size + v];
                uint ndis = u_cost + cost_u_v;
                if (cost[v] > ndis)
                {
                    cost[v] = ndis;
                    Dijkstra_minHeapDecPri(Q, v, ndis);
                }
            }
        }

        if (SHOW_DIJKSTRA)
        {
            Dijkstra_print(cost, matrix_size);
            Dijkstra_printMinHeap(Q);
        }
        if (SLOW_DIJKSTRA)
            sleep(2);
    }
    Heap_freeMinHeap(Q);
    return cost;
}

void Dijkstra_print(uint *cost, uint matrix_size)
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
    MinHeap *heap = Heap_buildMinHeap(10);
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
        Node min = Heap_minHeapPeak(heap);
        if (min.cost == INFINITY)
            printf("Peak inside\nMin.id = %d, Min.cost = INFINITY\nHeap size = %d\n\n", min.id, heap->size);
        else
            printf("Peak inside\nMin.id = %d, Min.cost = %u\nHeap size = %d\n\n", min.id, min.cost, heap->size);
        printf("Popped min\n\n");
        Heap_minHeapPop(heap);
        printf("+---------------------------------------+\n");
    }

    Heap_freeMinHeap(heap);
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

List *List_createList(uint capacity)
{
    List *new = malloc(sizeof(List));
    new->head = NULL;
    new->capacity = capacity;
    new->size = 0;
    return new;
}

void List_freeLast(List *list)
{
    ListNode *curr = list->head;
    ListNode *prev = curr;
    while (curr->next != NULL)
    {
        prev = curr;
        curr = curr->next;
    }
    prev->next = NULL;
    free(curr);
}

void List_insert(List *list, uint id, uint cost)
{
    ListNode *new = List_createNode(id, cost);
    if (list->head == NULL || list->head->cost >= cost)
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

        if (list->size == list->capacity)
        {
            new->next = c->next;
            c->next = new;
            List_freeLast(list);
        }
        else
        {
            new->next = c->next;
            c->next = new;
            list->size++;
        }
    }
}

void List_print(List *list)
{
    uint counter = 0;
    ListNode *tmp = list->head;
    while (tmp != NULL && counter < list->capacity)
    {
        if (tmp->next == NULL)
            printf("%d\n", tmp->id);
        else
            printf("%d ", tmp->id);
        tmp = tmp->next;
        counter++;
    }
}

void List_free(List *list)
{
    ListNode *curr = list->head;
    ListNode *next;
    while (curr != NULL)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

// #endregion

// #region HeapSort Solution

void Solution_minHeapPop(MinHeap *heap)
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
    Solution_minHeapify(heap, 0);
}

void Solution_minHeapify(MinHeap *heap, int i)
{
    int l = Heap_leftChild(i);
    int r = Heap_rightChild(i);
    int posmax = i;
    if (l < heap->size && heap->elements[l].cost > heap->elements[i].cost)
        posmax = l;
    if (r < heap->size && heap->elements[r].cost > heap->elements[posmax].cost)
        posmax = r;
    if (posmax != i)
    {
        Heap_heapSwap(&heap->elements[i], &heap->elements[posmax]);
        Solution_minHeapify(heap, posmax);
    }
}

void Solution_minHeapInsert(MinHeap *heap, uint id, uint cost)
{
    if (heap->size == heap->capacity)
        Solution_minHeapPop(heap);
    heap->size++;
    int i = heap->size - 1;
    heap->elements[i].id = id;
    heap->elements[i].cost = cost;
    while (i > 0 && heap->elements[Heap_parent(i)].cost <= heap->elements[i].cost)
    {
        Heap_heapSwap(&heap->elements[i], &heap->elements[Heap_parent(i)]);
        i = Heap_parent(i);
    }
}

void Solution_printHeapSort(MinHeap *heap)
{
    // while (heap->size >= 0)
    // {
    //     if (heap->capacity == 0){
    //         printf("\n");
    //         return;
    //     }
            
    //     else
    //         for (int x = 0; x < heap->capacity; x++)
    //             if (x == heap->capacity - 1)
    //                 printf("%d\n", heap->elements[x].id);
    //             else
    //                 printf("%d ", heap->elements[x].id);
    // }
    if (heap->size == 0)
    {
        printf("\n");
        return;
    }
    MinHeap *temp = Heap_buildMinHeap(heap->capacity);
    temp->size = heap->size;
    for (int x = 0; x < heap->size; x++)
        temp->elements[x] = heap->elements[x];

    if (temp->size == temp->capacity)
        Solution_minHeapPop(temp);
    while (temp->size > 0)
    {
        Node u = Heap_minHeapPeak(temp);
        if (temp->size > 1)
            printf("%d ", u.id);
        else
            printf("%d\n", u.id);
        Solution_minHeapPop(temp);
    }
    //Dijkstra_printMinHeap(temp);
    Heap_freeMinHeap(temp);
}

// #endregion

// made by Razvan Irinel Sarbu
