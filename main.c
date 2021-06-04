#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define NEW_GRAPH 1
#define TOP_K 2
#define FILE_END -1

typedef unsigned int my_int;
#define INFINITY ((my_int)-1)

typedef struct Node
{
    my_int cost;
    my_int id;
} Node;

typedef struct Heap
{
    Node *elements;
    my_int capacity;
    my_int size;
} Heap;

my_int parent(my_int index)
{
    return (index - 1) / 2;
}

my_int left(my_int index)
{
    return 2 * index + 1;
}

my_int right(my_int index)
{
    return 2 * index + 2;
}

void swap(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

Heap *createHeap(my_int capacity)
{
    Heap *newHeap = (Heap *)malloc(sizeof(Heap));
    newHeap->size = 0;
    newHeap->capacity = capacity;
    newHeap->elements = (Node *)calloc(capacity, sizeof(Node));
    return newHeap;
}

void freeHeap(Heap *heap)
{
    free(heap->elements);
    free(heap);
}

void minHeapify(Heap *heap, my_int n)
{
    int l = left(n);
    int r = right(n);
    int posmin = n;
    if (l < heap->size && heap->elements[l].cost < heap->elements[n].cost)
        posmin = l;
    if (r < heap->size && heap->elements[r].cost < heap->elements[posmin].cost)
        posmin = r;
    if (posmin != n)
    {
        swap(&heap->elements[n], &heap->elements[posmin]);
        minHeapify(heap, posmin);
    }
}

void heapPopMin(Heap *heap)
{
    if (heap->size <= 0)
        return;
    if (heap->size == 1)
    {
        heap->size--;
        return;
    }
    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
}

void heapInsertMin(Heap *heap, my_int id, my_int cost)
{
    if (heap->size == heap->capacity)
    {
        printf("OVERFLOW SIZE == CAPACITY\n");
        return;
    }
    heap->size++;
    int i = heap->size - 1;
    heap->elements[i].id = id;
    heap->elements[i].cost = cost;

    while (i > 0 && heap->elements[parent(i)].cost > heap->elements[i].cost)
    {
        swap(&heap->elements[i], &heap->elements[parent(i)]);
        i = parent(i);
    }
}

void maxHeapify(Heap *heap, my_int n)
{
    int l = left(n);
    int r = right(n);
    int posmax = n;
    if (l < heap->size && heap->elements[l].cost > heap->elements[n].cost)
        posmax = l;
    if (r < heap->size && heap->elements[r].cost > heap->elements[posmax].cost)
        posmax = r;
    if (posmax != n)
    {
        swap(&heap->elements[n], &heap->elements[posmax]);
        maxHeapify(heap, posmax);
    }
}

void heapPopMax(Heap *heap)
{
    if (heap->size <= 0)
        return;
    if (heap->size == 1)
    {
        heap->size--;
        return;
    }
    heap->elements[0] = heap->elements[heap->size - 1];
    heap->size--;
    maxHeapify(heap, 0);
}

void heapInsertMax(Heap *heap, my_int id, my_int cost)
{
    if (heap->size == heap->capacity)
    {
        heapPopMax(heap);
    }
    heap->size++;
    int i = heap->size - 1;
    heap->elements[i].id = id;
    heap->elements[i].cost = cost;

    while (i > 0 && heap->elements[parent(i)].cost <= heap->elements[i].cost)
    {
        swap(&heap->elements[i], &heap->elements[parent(i)]);
        i = parent(i);
    }
}

void heapDecreaseKey(Heap *heap, my_int id, my_int new_cost)
{
    int i = 0;
    for (int x = 0; x < heap->size; x++)
        if (heap->elements[x].id == id)
            i = x;
    heap->elements[i].cost = new_cost;
    while (i > 0 && heap->elements[parent(i)].cost > heap->elements[i].cost)
    {
        swap(&heap->elements[i], &heap->elements[parent(i)]);
        i = parent(i);
    }
}

Node heapPeak(Heap *heap)
{
    return heap->elements[0];
}

void printHeap(Heap *heap)
{
    if (heap->size == 0)
    {
        printf("\n");
        return;
    }

    if (heap->size == heap->capacity)
        heapPopMax(heap);

    for (my_int x = 0; x < heap->size; x++)
        if (x == heap->size - 1)
            printf("%u\n", heap->elements[x].id);
        else
            printf("%u ", heap->elements[x].id);
}

my_int *dijkstraQueue(my_int *matrix, my_int d)
{
    Heap *Q = createHeap(d);
    my_int *costs = calloc(d, sizeof(my_int));
    costs[0] = 0;
    heapInsertMin(Q, 0, 0);
    for (my_int x = 1; x < d; x++)
    {
        costs[x] = INFINITY;
        heapInsertMin(Q, x, INFINITY);
    }

    while (Q->size > 0)
    {
        Node u = heapPeak(Q);
        my_int u_id = u.id;
        my_int u_cost = u.cost;
        heapPopMin(Q);
        for (my_int v = 0; v < d && u_cost != INFINITY; v++)
        {
            if (u_id != v && matrix[u_id * d + v] > 0)
            {
                my_int cost_u_v = matrix[u_id * d + v];
                my_int total_cost = u_cost + cost_u_v;
                if (costs[v] > total_cost)
                {
                    costs[v] = total_cost;
                    heapDecreaseKey(Q, v, total_cost);
                }
            }
        }
    }
    freeHeap(Q);
    return costs;
}

my_int sumCosts(my_int *costs, my_int d)
{
    my_int sum = 0;
    for (my_int x = 0; x < d; x++)
        if (costs[x] != INFINITY)
            sum += costs[x];
    return sum;
}

// #region parser

int get_command()
{
    int c = getchar_unlocked();
    int command = 0;
    if (c == EOF)
    {
        return -1;
    }
    while (c != '\n' && c != EOF)
    {
        if (c == 'A')
            command = 1;
        if (c == 'T')
            command = 2;
        c = getchar_unlocked();
    }
    return command;
}

my_int get_num()
{
    my_int num = 0;
    int c = getchar_unlocked();
    while (c != '\n' && c != ',')
    {
        num = num * 10 + (c - '0');
        c = getchar_unlocked();
    }
    return num;
}

// #endregion

// #region MAIN

int main()
{
    int d = 0;
    int k = 0;
    if (scanf("%d %d", &d, &k))
    {
    };
    my_int *matrix = calloc(d * d, sizeof(my_int));
    int id = 0;
    int cmd = 0;

    Heap *result = createHeap(k + 1);

    while ((cmd = get_command()) != FILE_END)
    {
        switch (cmd)
        {
        case NEW_GRAPH:
            for (my_int y = 0; y < d; y++)
                for (my_int x = 0; x < d; x++)
                {
                    my_int num = get_num();
                    if (x != y)
                        matrix[y * d + x] = num;
                }
            my_int *costs = dijkstraQueue(matrix, d);
            my_int sum = sumCosts(costs, d);
            heapInsertMax(result, id, sum);
            free(costs);
            id++;
            break;
        case TOP_K:
            printHeap(result);
            break;
        default:
            break;
        }
    }
    freeHeap(result);
    free(matrix);
    return 0;
}

// #endregion