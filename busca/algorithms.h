
#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

#include "labirinto.h"
#include "deque.h"
#include "heap.h"


typedef struct
{
    int x;
    int y;
    int caminho;
    void *pai;
} Celula;

typedef struct
{
    Celula **caminho;
    double custo_caminho;
    int tamanho_caminho;
    int nos_expandidos;
    int sucesso;
} ResultData;

ResultData a_star(Labirinto *l, Celula inicio, Celula fim);
ResultData breadth_first_search(Labirinto *l, Celula inicio, Celula fim);
ResultData depth_first_search(Labirinto *l, Celula inicio, Celula fim);

double g(Celula *cel);
double h(Celula *cel, Celula* fim);
double f(Celula *cel, Celula* fim);

// algoritmo bobo de teste que tenta ir em linha reta do inicio ao fim
// e retorna impossível se encontrar um obstáculo
int Celula_valida(Labirinto *l, int linha, int coluna);
void celula_destroy(Celula *c);
int celula_hash(HashTable *h, void *key);
int celula_cmp(void *c1, void *c2);

#endif