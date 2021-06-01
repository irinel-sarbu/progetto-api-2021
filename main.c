#include <stdlib.h>
#include <stdio.h>

#define NEW_GRAPH "AggiungiGrafo";
#define TOP_K "TopK";
typedef unsigned short int sint;
typedef unsigned int uint;

void *Parser_getInfo();
sint **Parser_getMatrix(uint);
void Parser_freeMatrix(sint **, uint);
void Parser_freeInfo(void *);

int main()
{
    struct graph_info
    {
        uint size;
        uint k;
    };
    struct graph_info *info;
    info = (struct graph_info *)Parser_getInfo();
    printf("Inserted k = %d, size = %d\n",info->k,info->size);
    Parser_freeInfo((void *)info);
    return 0;
}

void *Parser_getInfo()
{
    uint size = 0;
    uint k = 0;
    if (scanf("%d %d", &size, &k))
    {
    };
    struct graph_info
    {
        uint size;
        uint k;
    };
    struct graph_info *info = malloc(sizeof(struct graph_info));
    info->k = k;
    info->size = size;

    return (void *)info;
}
sint **Parser_getMatrix(uint size)
{
    sint **matrix = malloc(sizeof(sint *));
    for (uint y = 0; y < size; y++)
    {
        matrix[y] = calloc(size, sizeof(sint));
    }

    return NULL;
}
void Parser_freeMatrix(sint **matrix, uint size)
{
    for (uint x = 0; x < size; x++)
    {
        free(matrix[x]);
    }
    free(matrix);
}
void Parser_freeInfo(void *info)
{
    free(info);
}
