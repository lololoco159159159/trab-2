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

int heap_sobe_node(Heap *heap, int i){
    while(i > 0 && (*(double*)heap->nodes[i]->prioridade < *(double*)heap->nodes[Pai(i)]->prioridade)){
        HeapNode_swap(heap->nodes, i, Pai(i));
        i = Pai(i);
    }
    return i;
}

int heap_desce_node(Heap *heap, int i){
    int filho_menor = 0;
    while ((filho_esquerdo(i) < heap->count && (*(double*)heap->nodes[i]->prioridade > *(double*)heap->nodes[filho_esquerdo(i)]->prioridade))||
    (filho_direito(i) < heap->count && (*(double*)heap->nodes[i]->prioridade > *(double*)heap->nodes[filho_direito(i)]->prioridade))){
        filho_menor = filho_esquerdo(i);
        if(filho_direito(i) < heap->count && (*(double*)heap->nodes[filho_direito(i)]->prioridade < *(double*)heap->nodes[filho_esquerdo(i)]->prioridade))
            filho_menor = filho_direito(i);
        HeapNode_swap(heap->nodes, i, filho_menor);
        i = filho_menor;
    }
    return i;
}

void *heap_push(Heap *heap, void *data, double priority){
    //pega o node buscado caso existe e NULL caso contrario
    static int st = 0;
    st++;
    HeapNode *node = (HeapNode*)hash_pair_find(heap->h, data);

    if (node != NULL){
        int i = 0;
        printf("exist!\n");
        exit(0);
        //atualiza na hash
        double *val = malloc(sizeof(double));
        *val = priority;
        void *aux = hash_table_set(heap->h, data, val);
        if(aux)
            free(aux);

        for(i = 0; i < heap->count; i++){
            if(heap->nodes[i] == node){
                break;
            }
        }
        //caso precise subir na arvore
        if(i > 0 && (*(double*)heap->nodes[i]->prioridade < *(double*)heap->nodes[Pai(i)]->prioridade)){
            heap_sobe_node(heap, i);
        }
        //caso precise descer na arvore
        else{
            heap_desce_node(heap, i);
        }
        imprime_lista_prioridade(heap);
        return data;
    }
    //caso n existe ainda 
    else{
        printf("n existe\n");
        double *val = malloc(sizeof(double));
        *val = priority;
        void *aux = hash_table_set(heap->h, data, val);
        if (aux)
            free(aux);

        node = (HeapNode*)hash_pair_find(heap->h, data);
        if(st == 2){
            printf("aqui\n");
            //exit(0);
        }
        
        if (heap->count >= heap->size){
            heap->nodes = realloc(heap->nodes, heap->size + HEAP_SIZE_SOMADOR);
            heap->size += HEAP_SIZE_SOMADOR;
        }
        
        int i = heap->count;
        heap->nodes[i] = node;
        heap->count++;
        
        while (i > 0 && *(double*)heap->nodes[i]->prioridade < *(double*)heap->nodes[Pai(i)]->prioridade){
            HeapNode_swap(heap->nodes, i, Pai(i));
            i = Pai(i);

        }
        imprime_lista_prioridade(heap);
        return NULL;
    }
}

double heap_min_priority(Heap *heap){
    return *(double*)heap->nodes[0]->prioridade;
}

void *heap_pop(Heap *heap){
    if(heap->count == 0){
        printf("operacao invalida, heap vazio!");
        exit(0);
    }
    
    heap->count--;
    HeapNode_swap(heap->nodes, 0, heap->count);
    HeapNode *node = heap->nodes[heap->count];
    heap_desce_node(heap, 0);
   
    hash_table_pop(heap->h, node->data);
    void *aux;
    if(node){
        if(node->data)
            aux = node->data;
        if(node->prioridade)
            free(node->prioridade);
    }
    free(node);
    return aux;
}

void heap_destroy(Heap *heap){
    free(heap->nodes);
    free(heap);
}






