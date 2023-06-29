#include "deque.h"

#include <stdio.h>
#include <stdlib.h>

#define TAM_BLOCO 3
#define TAM_INICIAL 1

struct Deque{
    void*** blocos;
    int blocos_size;
    int bloco_ini;
    int bloco_fim;
    int inicio;
    int fim;
};

typedef struct
{
    int x, y;
} Celula_fake;



Deque *deque_construct(void){
    Deque *deque = (Deque*)malloc(sizeof(Deque));
    deque->blocos = NULL;

    deque->blocos_size = TAM_INICIAL;
    deque->bloco_ini = 0;
    deque->bloco_fim = 0;
    deque->inicio = 0;
    deque->fim = 0;


    return deque;
}

Deque *redimensiona_deque(Deque *d){
    int novo_tam = d->blocos_size + 2;
    void ***novos_blocos = (void***)malloc(sizeof(void**) * novo_tam);
    for(int i = 0; i < novo_tam; i++){
        if(i == 0 || i == novo_tam-1){
            novos_blocos[i] = NULL;
            continue;
        }
        novos_blocos[i] = d->blocos[i-1];
    }
    d->blocos_size = novo_tam;
    d->bloco_ini++;
    d->bloco_fim++;
    d->inicio += TAM_BLOCO;
    d->fim += TAM_BLOCO;

    free(d->blocos);
    d->blocos = novos_blocos;
    return d;
}

void deque_push_back(Deque *d, void *val){
    if(d->blocos == NULL){
        d->blocos = (void***)malloc(sizeof(void**) * d->blocos_size);
        for(int i = 0; i < TAM_INICIAL; i++){
            d->blocos[i] = NULL;
        }
        d->bloco_ini = d->blocos_size/2;
        d->bloco_fim = d->blocos_size/2;
        d->inicio = d->bloco_ini * TAM_BLOCO;
        d->fim = d->bloco_ini * TAM_BLOCO;
        d->blocos[d->bloco_fim] = (void**)malloc(sizeof(void*) * TAM_BLOCO);
        //printf("inicio: %d  Fim: %d     bloco_inicio: %d    bloco_fim: %d\n", d->inicio, d->fim, d->bloco_ini, d->bloco_fim);
    }
    
    if(d->bloco_fim >= d->blocos_size){
        d = redimensiona_deque(d);
        d->blocos[d->bloco_fim] = (void**)malloc(sizeof(void*) * TAM_BLOCO);
        for(int i = 0; i < TAM_BLOCO; i++){
            d->blocos[d->bloco_fim][i] = NULL;
        }
    }
    else if(d->blocos[d->fim/TAM_BLOCO] == NULL){
        d->blocos[d->fim/TAM_BLOCO] = (void**)malloc(sizeof(void*) * TAM_BLOCO);
        for(int i = 0; i < TAM_BLOCO; i++){
            d->blocos[d->bloco_fim][i] = NULL;
        }
    }
    
    //printf("%d - %d\n", d->fim/TAM_BLOCO, d->fim%TAM_BLOCO);
    d->blocos[d->fim/TAM_BLOCO][d->fim%TAM_BLOCO] = val;
    d->fim++;
    d->bloco_fim = d->fim / TAM_BLOCO;

    //imprime_deque(d);
}

void deque_push_front(Deque *d, void *val){
    if(d->blocos == NULL){
        d->blocos = (void***)malloc(sizeof(void**) * d->blocos_size);
        for(int i = 0; i < d->blocos_size; i++){
            d->blocos[i] = NULL;
        }
        d->bloco_ini = d->blocos_size/2;
        d->bloco_fim = d->blocos_size/2;
        d->inicio = d->bloco_ini * TAM_BLOCO;
        d->fim = d->bloco_ini * TAM_BLOCO;
        d->blocos[d->bloco_fim] = (void**)malloc(sizeof(void*) * TAM_BLOCO);
        //printf("inicio: %d  Fim: %d     bloco_inicio: %d    bloco_fim: %d\n", d->inicio, d->fim, d->bloco_ini, d->bloco_fim);
        //printf("iniciei\n");
    }
    
    d->inicio--;
    d->bloco_ini = d->inicio / TAM_BLOCO;
    if(d->inicio < 0){
        d->bloco_ini = -1;
        d = redimensiona_deque(d);
        d->blocos[d->bloco_ini] = (void**)malloc(sizeof(void*) * TAM_BLOCO);
        for(int i = 0; i < TAM_BLOCO; i++){
            d->blocos[d->bloco_ini][i] = NULL;
        }
    }
    else if(d->blocos[d->bloco_ini] == NULL){
        d->blocos[d->bloco_ini] = (void**)malloc(sizeof(void*) * TAM_BLOCO);
        for(int i = 0; i < TAM_BLOCO; i++){
            d->blocos[d->bloco_ini][i] = NULL;
        }

    }
    //printf("%d - %d\n", d->inicio/TAM_BLOCO, d->inicio%TAM_BLOCO);
    d->blocos[d->bloco_ini][d->inicio%TAM_BLOCO] = val;
    //imprime_deque(d);
}

void imprime_deque(Deque *d){
    int count = d->inicio, primeira_vez = 1;
    while(count < d->fim){
        Celula_fake *cel = (Celula_fake*)d->blocos[count/TAM_BLOCO][count%TAM_BLOCO];
        
        if(count % TAM_BLOCO == 0 || primeira_vez){
            primeira_vez = 0;
            printf("bloco %d: ", count/TAM_BLOCO);
        }
        printf("%d", count);
        printf("(%d %d) ", cel->x, cel->y);
        count++;
        if(count % TAM_BLOCO == 0){
            printf("\n");
        }
    }
    printf("\n");
}

void deque_destroy(Deque *d){
    for(int i = 0; i < d->blocos_size; i++){
        if(d->blocos[i] != NULL){
            for(int k = 0; k < TAM_BLOCO; k++){
                if(d->blocos[i][k] != NULL)
                    free(d->blocos[i][k]);
                else
                    continue;
            }
            free(d->blocos[i]);
        }
    }
    if(d->blocos != NULL){
        free(d->blocos);
    }
    free(d);
}

void *deque_pop_front(Deque *d){
    if((d->inicio == d->fim) || d->blocos == NULL){
        return NULL;
    }
    void *ret = d->blocos[d->inicio/TAM_BLOCO][d->inicio%TAM_BLOCO];
    d->blocos[d->inicio/TAM_BLOCO][d->inicio%TAM_BLOCO] = NULL;
    d->inicio++;
    d->bloco_ini = d->inicio / TAM_BLOCO;
    //imprime_deque(d);
    return ret;
}

void *deque_pop_back(Deque *d){
    if((d->inicio == d->fim) || d->blocos == NULL){
        return NULL;
    }
    d->fim--;
    d->bloco_fim = d->fim / TAM_BLOCO;
    void *ret = d->blocos[d->fim/TAM_BLOCO][d->fim%TAM_BLOCO];
    d->blocos[d->fim/TAM_BLOCO][d->fim%TAM_BLOCO] = NULL;
    //imprime_deque(d);
    return ret;
}


