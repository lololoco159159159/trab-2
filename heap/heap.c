#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

#define HEAP_SIZE_INICIAL 40
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
    HeapNode *node = (HeapNode*)hash_pair_find(heap->h, data);

    if (node != NULL && priority < (*(double*)node->prioridade)){
        int pos = ((node - heap->nodes[0])/ sizeof(HashTableItem));
        printf("pos: %d\n", pos);
        printf("antes: ");
        imprime_lista_prioridade(heap);
        int i = 0;
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
        printf("depois: ");
        imprime_lista_prioridade(heap);
        return data;
    }

    //caso ja exista com valor menor
    else if(node != NULL && priority >= (*(double*)node->prioridade)){
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
            HeapNode **aux = (HeapNode**)malloc(sizeof(HeapNode*) *(heap->size * 2));
            for(int k = 0; k < heap->count; k++){
                aux[k] = heap->nodes[k];
            }
            free(heap->nodes);
            heap->nodes = aux;
            heap->size = heap->size*2;   
        }
 
        int i = heap->count;
        heap->nodes[i] = node;
        heap->count++;

        int pos = ((node - heap->nodes[0])/sizeof(node));
        printf("%p %p\n", node, heap->nodes[0]);
        printf("pos: %d\n", pos);
        printf("antes: \n");
        imprime_lista_prioridade(heap);

        while (i > 0 && *(double*)heap->nodes[i]->prioridade < *(double*)heap->nodes[Pai(i)]->prioridade){
            HeapNode_swap(heap->nodes, i, Pai(i));
            i = Pai(i);

        }
        printf("depois: \n");
        imprime_lista_prioridade(heap);

        return NULL;
    }

}

double heap_min_priority(Heap *heap){
    if(heap->count <= 0){
        printf("heap vazio!\n");
        exit(0);
    }
    return *(double*)heap->nodes[0]->prioridade;
}

void *heap_pop(Heap *heap){
    if(heap->count <= 0){
        printf("operacao invalida, heap vazio!");
        exit(0);
    }
    
    heap->count--;
    
    if(heap->count == 0){
        HeapNode *n = heap->nodes[0];
        void *aux = n->data;
        void *val = hash_table_pop(heap->h, n->data);
        if(val)
            free(val);
        return aux;
    }
    else{
        HeapNode_swap(heap->nodes, 0, heap->count);
        HeapNode *node = heap->nodes[heap->count];
        void *aux = node->data;
        heap_desce_node(heap, 0);
        void *val = hash_table_pop(heap->h, node->data);
        if (val)
            free(val);
        return aux;
    }
}

void heap_destroy(Heap *heap){
    free(heap->nodes);
    free(heap);
}






