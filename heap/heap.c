#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

#define HEAP_SIZE_INICIAL 10
#define HEAP_SIZE_SOMADOR 10

struct Heap{
    HashTable *h;
    HeapNode **nodes;
    int count;
    int size;
};

void HeapNode_swap(HeapNode **nodes, int i, int j) {
    HeapNode *temp = nodes[i];
    nodes[i] = nodes[j];
    nodes[j] = temp;
}

int Pai(int i) {
    return (i - 1) / 2;
}

int filho_esquerdo(int i) {
    return (2 * i) + 1;
}

int filho_direito(int i) {
    return (2 * i) + 2;
}



Heap *heap_construct(HashTable *h){
    Heap *heap = (Heap*)malloc(sizeof(Heap));
    heap->nodes = (HeapNode**)malloc(sizeof(HeapNode*) * HEAP_SIZE_INICIAL);
    heap->size = HEAP_SIZE_INICIAL;
    heap->count = 0;
    heap->h = h;
    
    return heap;
}

void imprime_lista_prioridade(Heap *heap){
    for(int i = 0; i < heap->count; i++){
        printf("%.0lf ", *(double*)heap->nodes[i]->prioridade);
    }
    printf("\n");
}

void *heap_push(Heap *heap, void *data, double priority){
    HeapNode *node = (HeapNode*)hash_pair_find(heap->h, data);

    if (node != NULL){
        printf("existe! ");
        double *val = malloc(sizeof(double));
        *val = priority;
        void *aux = hash_table_set(heap->h, data, val);
        if(aux)
            free(aux);
        int pos = (int)((node - heap->nodes[0])/sizeof(node));
        printf("pos: %ld\n", (node - heap->nodes[0])/sizeof(node));
        
        return data;
    }
    //caso n existe ainda 
    else{
        double *val = malloc(sizeof(double));
        *val = priority;
        void *aux = hash_table_set(heap->h, data, val);
        if (aux)
            free(aux);

        node = (HeapNode*)hash_pair_find(heap->h, data);

        if (heap->count >= heap->size){
            heap->nodes = realloc(heap->nodes, heap->size + HEAP_SIZE_SOMADOR);
            heap->size += HEAP_SIZE_SOMADOR;
        }
            
        int i = heap->count;
        heap->nodes[i] = node;
        heap->count++;
    
        while (i > 0 && *(double*)heap->nodes[i]->prioridade < *(double*)heap->nodes[Pai(i)]->prioridade) {
            HeapNode_swap(heap->nodes, i, Pai(i));
            i = Pai(i);
        }
   
        imprime_lista_prioridade(heap);
        return NULL;
    }
}




