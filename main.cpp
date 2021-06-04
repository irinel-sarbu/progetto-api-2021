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

class Node
{
public:
    my_int id;
    my_int cost;
};

class Heap
{
public:
    Node *elements;
    my_int size;
    my_int capacity;

    Heap(my_int _capacity)
    {
        size = 0;
        capacity = _capacity;
        elements = (Node *)malloc(capacity * sizeof(Node));
    }

    void freeMem()
    {
        free(elements);
    }

    my_int parent(my_int index)
    {
        return (index - 1) / 2;
    }

    my_int left(my_int index)
    {
        return index * 2 + 1;
    }

    my_int right(my_int index)
    {
        return index * 2 + 2;
    }

    void swap(my_int index_a, my_int index_b)
    {
        Node tmp = elements[index_a];
        elements[index_a] = elements[index_b];
        elements[index_b] = tmp;
    }

    void minHeapify(my_int n)
    {
        int l = left(n);
        int r = right(n);
        int posmin = n;
        if (l < size && elements[l].cost < elements[n].cost)
            posmin = l;
        if (r < size && elements[r].cost < elements[posmin].cost)
            posmin = r;
        if (posmin != n)
        {
            swap(n, posmin);
            minHeapify(posmin);
        }
    }

    void popMin()
    {
        if (size <= 0)
            return;
        if (size == 1)
        {
            size--;
            return;
        }
        elements[0] = elements[size - 1];
        size--;
        minHeapify(0);
    }

    void insertMin(my_int id, my_int cost)
    {
        if (size == capacity)
        {
            printf("OVERFLOW SIZE == CAPACITY\n");
            return;
        }
        size++;
        int i = size - 1;
        elements[i].id = id;
        elements[i].cost = cost;

        while (i > 0 && elements[parent(i)].cost > elements[i].cost)
        {
            swap(i, parent(i));
            i = parent(i);
        }
    }

    void maxHeapify(my_int n)
    {
        int l = left(n);
        int r = right(n);
        int posmax = n;
        if (l < size && elements[l].cost > elements[n].cost)
            posmax = l;
        if (r < size && elements[r].cost > elements[posmax].cost)
            posmax = r;
        if (posmax != n)
        {
            swap(n, posmax);
            maxHeapify(posmax);
        }
    }

    void popMax()
    {
        if (size <= 0)
            return;
        if (size == 1)
        {
            size--;
            return;
        }
        elements[0] = elements[size - 1];
        size--;
        maxHeapify(0);
    }

    void insertMax(my_int id, my_int cost)
    {
        if (size == capacity)
        {
            popMax();
        }
        size++;
        int i = size - 1;
        elements[i].id = id;
        elements[i].cost = cost;

        while (i > 0 && elements[parent(i)].cost <= elements[i].cost)
        {
            swap(i, parent(i));
            i = parent(i);
        }
    }

    void decreaseKey(my_int id, my_int new_cost)
    {
        int i = 0;
        for (int x = 0; x < size; x++)
            if (elements[x].id == id)
                i = x;
        elements[i].cost = new_cost;
        while (i > 0 && elements[parent(i)].cost > elements[i].cost)
        {
            swap(i, parent(i));
            i = parent(i);
        }
    }

    Node peak()
    {
        return elements[0];
    }

    void print()
    {
        printf("Heap:\n");
        printf("   id      cost\n");
        for (my_int x = 0; x < size; x++)
        {
            printf("%*u", 5, elements[x].id);
            printf("%*u\n", 10, elements[x].cost);
        }
        printf("\n");
    }

    void printSolution()
    {
        if (size == 0)
        {
            printf("\n");
            return;
        }

        if (size == capacity)
            popMax();

        for (my_int x = 0; x < size; x++)
            if (x == size - 1)
                printf("%u\n", elements[x].id);
            else
                printf("%u ", elements[x].id);
    }
};

my_int *dijkstraQueue(my_int *matrix, my_int d)
{
    Heap Q(d);
    my_int *costs = (my_int *)calloc(d, sizeof(my_int));
    costs[0] = 0;
    Q.insertMin(0, 0);
    for (my_int x = 1; x < d; x++)
    {
        costs[x] = INFINITY;
        Q.insertMin(x, INFINITY);
    }

    while (Q.size > 0)
    {
        Node u = Q.peak();
        my_int u_id = u.id;
        my_int u_cost = u.cost;
        Q.popMin();
        for (my_int v = 0; v < d && u_cost != INFINITY; v++)
        {
            if (u_id != v && matrix[u_id * d + v] > 0)
            {
                my_int cost_u_v = matrix[u_id * d + v];
                my_int total_cost = u_cost + cost_u_v;
                if (costs[v] > total_cost)
                {
                    costs[v] = total_cost;
                    Q.decreaseKey(v, total_cost);
                }
            }
        }
    }
    Q.freeMem();
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

int main()
{
    int d = 0;
    int k = 0;
    if (scanf("%d %d", &d, &k))
    {
    };
    my_int *matrix = (my_int *)calloc(d * d, sizeof(my_int));
    int id = 0;
    int cmd = 0;

    Heap result(k + 1);

    // printf("  graph_id     cost\n");
    while ((cmd = get_command()) != FILE_END)
    {
        if (cmd == NEW_GRAPH)
        {
            for (my_int y = 0; y < d; y++)
                for (my_int x = 0; x < d; x++)
                {
                    my_int num = get_num();
                    if (x != y)
                        matrix[y * d + x] = num;
                }
            my_int *costs = dijkstraQueue(matrix, d);
            my_int sum = sumCosts(costs, d);

            // printf("[%*u]", 8, id);
            // printf("%*u\n", 9, sum);
            result.insertMax(id, sum);
            free(costs);
            id++;
        }

        if (cmd == TOP_K)
        {
            // printf("\n");
            // result.print();
            result.printSolution();
        }
    }
    result.freeMem();
    free(matrix);
    return 0;
}
