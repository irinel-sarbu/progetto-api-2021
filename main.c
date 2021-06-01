#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NEW_GRAPH "AggiungiGrafo"
#define TOP_K "TopK"
#define INFINITY 4294967295

// Change to true / false to show / not show info
#define SHOW_MATRIX_INFO true
#define SHOW_MATRIX true
#define SHOW_HEAP true
#define SHOW_DIJKSTRA_SUM true;
#define SHOW_DIJKSTRA true;


typedef unsigned short int sint;
typedef unsigned int uint;

// #region Function Declaration

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
void Dijkstra_printMinHeap(DijkstraMinHeap *);

uint *Dijkstra(sint **, uint);
void Dijkstra_test();

sint **Parser_getMatrix(uint);
void Parser_freeMatrix(sint **, uint);
void Parser_matrixPrint(sint **, uint);

// #endregion

// #region Main

int main()
{
    uint matrix_size, k;
    char cmd[14];
    sint counter = 0;
    // Inizio Parser
    if (scanf("%d %d", &matrix_size, &k))
    {
    };
    if(SHOW_MATRIX_INFO)
        printf("[INFO] Matrix size set to %d\n[INFO] k set to %d\n \n",matrix_size, k);
    bool finished = false;
    while (!finished)
    {
        if (scanf("%s", cmd))
        {
        };
        if (strcmp(cmd, TOP_K) == 0)
        {
            finished = true;
        }
        if (strcmp(cmd, NEW_GRAPH) == 0)
        {
            sint **adj_matrix = Parser_getMatrix(matrix_size);
            if(SHOW_MATRIX_INFO)
                printf("[%*d] Matrix:\n", 5, counter);
            Parser_matrixPrint(adj_matrix, matrix_size);
            Parser_freeMatrix(adj_matrix, matrix_size);
            counter++;
        }
    }
    return 0;
}

// #endregion

// #region Parser Functions

sint **Parser_getMatrix(uint matrix_size)
{
    sint **adj_matrix = malloc(matrix_size * sizeof(sint *));
    for (int y = 0; y < matrix_size; y++)
    {
        adj_matrix[y] = calloc(matrix_size, sizeof(sint));
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
                adj_matrix[y][x] = cost;
            x++;
            cost_s = strtok(NULL, ",");
        }
        free(string);
    }
    return adj_matrix;
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
    printf("\n");
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
    for(int x = 0; x < capacity; x++){
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
    if (heap->size == 1){
        heap->size -= 1;
        return;
    }
    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size -= 1;
    Dijkstra_minHeapify(heap, 0);
}
void Dijkstra_minHeapInsert(DijkstraMinHeap *heap, uint id, uint cost){
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
void Dijkstra_printMinHeap(DijkstraMinHeap *heap)
{
    printf("Heap:\n");
    printf("+-----------------+\n");
    for (int y = 0; y < heap->size; y++)
    {
        printf("|");
        printf("%*d|", 6, heap->elements[y].id);
        if(heap->elements[y].cost == INFINITY)
            printf("  INFINITY|");
        else
            printf("%*d|", 10, heap->elements[y].cost);
        printf("\n");
        printf("+-----------------+\n");
    }
    printf("|      |          |\n");
    printf("+-----------------+\n\n");
}

uint *Dijkstra(sint **matrix, uint matrix_size)
{
}
void Dijkstra_test()
{
    DijkstraMinHeap *heap = Dijkstra_buildMinHeap(10);
    Dijkstra_minHeapInsert(heap,0,INFINITY);
    Dijkstra_minHeapInsert(heap,0,50);
    Dijkstra_minHeapInsert(heap,0,100);
    Dijkstra_minHeapInsert(heap,0,250);
    Dijkstra_minHeapInsert(heap,0,25);
    Dijkstra_minHeapInsert(heap,0,20);
    Dijkstra_minHeapInsert(heap,0,1000);

    while(heap->size > 0){
        Dijkstra_printMinHeap(heap);
        DijkstraNode min = Dijkstra_minHeapPeak(heap);
        if(min.cost == INFINITY)
            printf("Peak inside\nMin.id = %d, Min.cost = INFINITY\nHeap size = %d\n\n",min.id,heap->size);
        else
            printf("Peak inside\nMin.id = %d, Min.cost = %u\nHeap size = %d\n\n",min.id, min.cost,heap->size);
        printf("Popped min\n\n");
        Dijkstra_minHeapPop(heap);
        printf("+---------------------------------------+\n");
    }


    Dijkstra_freeMinHeap(heap);
}

// #endregion