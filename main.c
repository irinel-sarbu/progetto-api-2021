#include <stdlib.h>
#include <stdio.h>

#define NEW_GRAPH 1
#define TOP_K 2
#define FILE_END -1

typedef unsigned int uInt;
#define INFINITY ((uInt)-1)

typedef struct Node
{
    uInt val;
    uInt id;
} Node;

/**
 * @param array Lista di elementi contenuti nell'heap
 * @param capacity Capacità massima dell'heap
 * @param size Dimensione dell'heap in un dato istante
*/
typedef struct Heap
{
    Node *array;
    uInt capacity;
    uInt size;
} Heap;

/**
 * Dato un albero binario memorizzato come un array
 * @param index Indice attuale nell'array
 * @return Posizione del padre
*/
uInt parent(uInt index)
{
    return (index - 1) / 2;
}

/**
 * Dato un albero binario memorizzato come un array
 * @param index Indice attuale nell'array
 * @return Posizione del figlio sinistro
*/
uInt left(uInt index)
{
    return 2 * index + 1;
}

/**
 * Dato un albero binario memorizzato come un array
 * @param index Indice attuale nell'array
 * @return Posizione del figlio destro
*/
uInt right(uInt index)
{
    return 2 * index + 2;
}

/**
 * La funzione scambia la posizione in memoria dei nodi
*/
void swap(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * Crea un heap vuoto
 * @param capacity Capacità massima dell'heap da creare
 * @return Heap appena creato
*/
Heap *createHeap(uInt capacity)
{
    Heap *newHeap = (Heap *)malloc(sizeof(Heap));
    newHeap->size = 0;
    newHeap->capacity = capacity;
    newHeap->array = (Node *)calloc(capacity, sizeof(Node));
    return newHeap;
}

/**
 * Libera lo heap dallo stack
 * @param heap Heap da liberare
*/
void freeHeap(Heap *heap)
{
    free(heap->array);
    free(heap);
}

/**
 * Riordina l'array dello heap in modo da rispettare le regole di un min-heap
 * @param heap Heap sul quale effettuare lo heapify
 * @param pos Posizione dell'array in cui effettuare lo heapify ricorsivamente
*/
void minHeapify(Heap *heap, uInt pos)
{
    int l = left(pos);
    int r = right(pos);
    int posmin = pos;
    if (l < heap->size && heap->array[l].val < heap->array[pos].val)
        posmin = l;
    if (r < heap->size && heap->array[r].val < heap->array[posmin].val)
        posmin = r;
    if (posmin != pos)
    {
        swap(&heap->array[pos], &heap->array[posmin]);
        minHeapify(heap, posmin);
    }
}

/**
 * Rimuove dall'array l'elemento minimo e riordina l'heap in modo da rispettare le regole di un min-heap
 * @param heap Heap dal quale eliminare l'elemento "minimo"
*/
void heapDeleteMin(Heap *heap)
{
    if (heap->size <= 0)
        return;
    if (heap->size == 1)
    {
        heap->size--;
        return;
    }
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    minHeapify(heap, 0);
}

/**
 * Inserisce un elemento nello heap
 * @param heap Heap nel quale inserire l'elemento
 * @param id Id con il quale identificare il nuovo elemento
 * @param cost Valore del nuovo elemento
*/
void heapInsertMin(Heap *heap, uInt id, uInt cost)
{
    heap->size++;
    int i = heap->size - 1;
    heap->array[i].id = id;
    heap->array[i].val = cost;

    while (i > 0 && heap->array[parent(i)].val > heap->array[i].val)
    {
        swap(&heap->array[i], &heap->array[parent(i)]);
        i = parent(i);
    }
}

/**
 * Riordina l'array dello heap in modo da rispettare le regole di un max-heap
 * @param heap Heap sul quale effettuare lo heapify
 * @param pos Posizione dell'array in cui effettuare lo heapify ricorsivamente
*/
void maxHeapify(Heap *heap, uInt pos)
{
    int l = left(pos);
    int r = right(pos);
    int posmax = pos;
    if (l < heap->size && heap->array[l].val > heap->array[pos].val)
        posmax = l;
    if (r < heap->size && heap->array[r].val > heap->array[posmax].val)
        posmax = r;
    if (posmax != pos)
    {
        swap(&heap->array[pos], &heap->array[posmax]);
        maxHeapify(heap, posmax);
    }
}

/**
 * Rimuove dall'array l'elemento minimo e riordina l'heap in modo da rispettare le regole di un max-heap
 * @param heap Heap dal quale eliminare l'elemento "massimo"
*/
void heapDeleteMax(Heap *heap)
{
    if (heap->size <= 0)
        return;
    if (heap->size == 1)
    {
        heap->size--;
        return;
    }
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    maxHeapify(heap, 0);
}

/**
 * Inserisce un elemento nello heap
 * @param heap Heap nel quale inserire l'elemento
 * @param id Id con il quale identificare il nuovo elemento
 * @param cost Valore del nuovo elemento
*/
void heapInsertMax(Heap *heap, uInt id, uInt cost)
{
    if (heap->size == heap->capacity)
    {
        heapDeleteMax(heap);
    }
    heap->size++;
    int i = heap->size - 1;
    heap->array[i].id = id;
    heap->array[i].val = cost;

    while (i > 0 && heap->array[parent(i)].val <= heap->array[i].val)
    {
        swap(&heap->array[i], &heap->array[parent(i)]);
        i = parent(i);
    }
}

/**
 * Aggiorna il valore di un elemento nello heap e lo riordina per rispettare le regole di un min-heap
 * @param heap Heap sul quale effettuare l'operazione
 * @param id Elemento sul quale effettuare l'operazione
 * @param new_cost Nuovo valore che l'elemento deve assumere
*/
void heapDecreaseKey(Heap *heap, uInt id, uInt new_cost)
{
    uInt i = 0;
    int found = 0;
    for (uInt x = 0; x < heap->size && found == 0; x++)
        if (heap->array[x].id == id)
        {
            i = x;
            found = 1;
        }

    heap->array[i].val = new_cost;
    while (i > 0 && heap->array[parent(i)].val > heap->array[i].val)
    {
        swap(&heap->array[i], &heap->array[parent(i)]);
        i = parent(i);
    }
}

/**
 * @param heap Heap sul quale effettuare il "peak"
 * @return Nodo "root" dello heap
*/
Node heapPeak(Heap *heap)
{
    return heap->array[0];
}

/**
 * Visualizza lo heap in ordine crescente
 * @param heap Heap da visualizzare
*/
void printHeap(Heap *heap)
{
    if (heap->size == 0)
    {
        printf("\n");
        return;
    }

    if (heap->size == heap->capacity)
        heapDeleteMax(heap);

    for (uInt x = 0; x < heap->size; x++)
        if (x == heap->size - 1)
            printf("%u\n", heap->array[x].id);
        else
            printf("%u ", heap->array[x].id);
}

/**
 * Algoritmo di Dijkstra per trovare la distanza minima tra i nodi di un grafo
 * @param matrix Matrice di adiacenza di un grafo
 * @param d Dimensione della matrice di adiacenza
 * @return Array con le distanze dal nodo 0
 * NOTA: Se la distanza è INFINITY il nodo è irragiungibile
*/
uInt *dijkstraQueue(uInt *matrix, uInt d)
{
    Heap *Q = createHeap(d);
    uInt *costs = calloc(d, sizeof(uInt));
    costs[0] = 0;
    heapInsertMin(Q, 0, 0);
    for (uInt x = 1; x < d; x++)
    {
        costs[x] = INFINITY;
        heapInsertMin(Q, x, INFINITY);
    }

    while (Q->size > 0)
    {
        Node u = heapPeak(Q);
        uInt u_id = u.id;
        uInt u_cost = u.val;
        heapDeleteMin(Q);
        for (uInt v = 0; v < d && u_cost != INFINITY; v++)
        {
            if (u_id != v && matrix[u_id * d + v] > 0)
            {
                uInt cost_u_v = matrix[u_id * d + v];
                uInt total_cost = u_cost + cost_u_v;
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

/**
 * @param distances Array sul quale effettuare l'operazione
 * @param d Numero di elementi da sommare
 * @return Somma gli elementi dell'array, da elemento array[0] a elemento array[d-1]
*/
uInt sumDistances(uInt *distances, uInt d)
{
    uInt sum = 0;
    for (uInt x = 0; x < d; x++)
        if (distances[x] != INFINITY)
            sum += distances[x];
    return sum;
}

int getCommand()
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

uInt getNum()
{
    uInt num = 0;
    int c = getchar_unlocked();
    while (c != '\n' && c != ',')
    {
        num = num * 10 + (c - '0');
        c = getchar_unlocked();
    }
    return num;
}

int main()
{
    int d = 0;
    int k = 0;
    if (scanf("%d %d", &d, &k))
    {
    };

    uInt *matrix = calloc(d * d, sizeof(uInt));
    int id = 0;
    int cmd = 0;

    Heap *result = createHeap(k + 1);

    while ((cmd = getCommand()) != FILE_END)
    {
        switch (cmd)
        {
        case NEW_GRAPH:
            for (uInt y = 0; y < d; y++)
                for (uInt x = 0; x < d; x++)
                {
                    uInt num = getNum();
                    if (x != y)
                        matrix[y * d + x] = num;
                }
            uInt *distances = dijkstraQueue(matrix, d);
            uInt sum = sumDistances(distances, d);
            heapInsertMax(result, id, sum);
            free(distances);
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