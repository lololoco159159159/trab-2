#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

#define HEAP_SIZE_INICIAL 1
#define HEAP_SIZE_SOMADOR 10

struct Heap{
    HashTable *h;
    HeapNode **nodes;
    int count;
    int size;
};


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
        printf("%.0lf ", heap->nodes[i]->prioridade);
    }
    printf("\n");
}

int heap_sobe_node(Heap *heap, int i){
    while(i > 0 && heap->nodes[i]->prioridade < heap->nodes[Pai(i)]->prioridade){
        Heap_node_swap(heap, i, Pai(i));
        i = Pai(i);
    }
    return i;
}

int heap_desce_node(Heap *heap, int i){
    int filho_menor = 0;
    while ((filho_esquerdo(i) < heap->count && heap->nodes[i]->prioridade > heap->nodes[filho_esquerdo(i)]->prioridade)||
    (filho_direito(i) < heap->count && heap->nodes[i]->prioridade > heap->nodes[filho_direito(i)]->prioridade)){
        filho_menor = filho_esquerdo(i);
        if(filho_direito(i) < heap->count && heap->nodes[filho_direito(i)]->prioridade < heap->nodes[filho_esquerdo(i)]->prioridade)
            filho_menor = filho_direito(i);
        Heap_node_swap(heap, i, filho_menor);
        i = filho_menor;
    }
    return i;
}

void Heap_node_swap(Heap *heap, int a, int b){
    HashTableItem *node_aux = NULL;
    node_aux = (HashTableItem*)hash_pair_find(heap->h, heap->nodes[a]->data);
    *(int*)node_aux->val = b;
    node_aux = (HashTableItem*)hash_pair_find(heap->h, heap->nodes[b]->data);
    *(int*)node_aux->val = a;

    HeapNode *aux;
    aux = heap->nodes[a];
    heap->nodes[a] = heap->nodes[b];
    heap->nodes[b] = aux;
}

HeapNode *Heap_node_construct(void *key, double prioridade){
    HeapNode *node = (HeapNode*)malloc(sizeof(HeapNode));
    node->data = key;
    node->prioridade = prioridade; 
    return node;
}   

void *heap_push(Heap *heap, void *data, double priority){
    HashTableItem *node = (HashTableItem*)hash_pair_find(heap->h, data);
    //caso ja exista
    if (node != NULL){
        int idx = *(int*)node->val;

        if(priority >= heap->nodes[idx]->prioridade){
            //imprime_lista_prioridade(heap);
            return data;
        }
        heap->nodes[idx]->prioridade = priority;
        
        if(idx > 0 && heap->nodes[idx]->prioridade < heap->nodes[Pai(idx)]->prioridade){
            heap_sobe_node(heap, idx);
        }
        else{
            heap_desce_node(heap, idx);
        }
        //imprime_lista_prioridade(heap);
        return data;
    }
    
    //caso n existe ainda 
    else{      
        if (heap->count >= heap->size){
            HeapNode **aux = (HeapNode**)malloc(sizeof(HeapNode*) *(heap->size * 2));
            for(int k = 0; k < heap->count; k++){
                aux[k] = heap->nodes[k];
            }
            free(heap->nodes);
            heap->nodes = aux;
            heap->size = heap->size*2; 
        }

        HeapNode *item = Heap_node_construct(data, priority);

        int i = heap->count;
        heap->nodes[i] = item;
        heap->count++;

        int *idx = malloc(sizeof(int));
        *idx = i;
        hash_table_set(heap->h, data, idx);

        heap_sobe_node(heap, i);
        //imprime_lista_prioridade(heap);
        return NULL;
    }
}

double heap_min_priority(Heap *heap){
    return heap->nodes[0]->prioridade;
}

void *heap_pop(Heap *heap){    
    if(heap->count <= 0){
        printf("operacao invalida, heap vazio!");
        exit(0);
    }
    heap->count--;
    HeapNode *n = NULL;
    
    if(heap->count == 0){
        n = heap->nodes[0];
        void *aux = n->data;
        void *val = hash_table_pop(heap->h, n->data);
        if(val)
            free(val);
        free(n);
        //imprime_lista_prioridade(heap);
        return aux;
    }
    else{
        Heap_node_swap(heap, 0, heap->count);
        n = heap->nodes[heap->count];
        void *aux = n->data;
        heap_desce_node(heap, 0);
        void *val = hash_table_pop(heap->h, n->data);
        if (val)
            free(val);
        free(n);
        //imprime_lista_prioridade(heap);
        return aux;
    }
}

void heap_destroy(Heap *heap){
    for(int i = 0; i < heap->count; i++)
        free(heap->nodes[i]);
    free(heap->nodes);
    free(heap);
}







