
#ifndef _HEAP_H_
#define _HEAP_H_

#include "types.h"
#include "hash.h"

typedef struct Heap Heap;

typedef struct{
    void *data;
    double prioridade;
}HeapNode;

// a funcao de comparacao para o heap está dentro da hash table
Heap *heap_construct(HashTable *h);
void *heap_push(Heap *heap, void *data, double priority);
void *heap_min(Heap *heap);
double heap_min_priority(Heap *heap);
void *heap_pop(Heap *heap);
void heap_destroy(Heap *heap);

void HeapNode_swap(HeapNode **nodes, int i, int j);
int Pai(int i);
int filho_esquerdo(int i);
int filho_direito(int i); 
HeapNode *Heap_node_construct(void *key, double prioridade);
void Heap_node_swap(Heap *heap, int a, int b);
int heap_empty(Heap *heap);
int heap_verifica_celula_contida(Heap *heap, void *key);

#endif