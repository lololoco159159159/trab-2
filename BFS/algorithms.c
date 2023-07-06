
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"
#include "deque.h"

#define result_allocated_inicial 2

ResultData _default_result()
{
    ResultData result;

    result.caminho = NULL;
    result.custo_caminho = 0;
    result.nos_expandidos = 0;
    result.tamanho_caminho = 0;
    result.sucesso = 0;

    return result;
}

void celula_inutilizavel_destroy(void *c){
    Celula *aux = (Celula*)c;
    if(aux->caminho == 0){
        celula_destroy(c);
    }
}

Celula *celula_create(int x, int y, void *pai){
    Celula *c = malloc(sizeof(Celula));
    c->x = x;
    c->y = y;
    c->caminho = 0;
    c->pai = pai;
    return c;
}

void celula_destroy(Celula *c){
    free(c);
}


ResultData a_star(Labirinto *l, Celula inicio, Celula fim)
{
    // TODO!
    return _default_result();
}


void inicializa_matriz_visitadas(int i, int j, int matriz_visitadas[i][j]){
    for(int k = 0; k < i; k++){
        for(int l = 0; l < j; l++){
            matriz_visitadas[k][l] = 0;
        }
    }
}

int Celula_valida(Labirinto *l, int linha, int coluna){
    if(linha < 0 || linha >= labirinto_n_linhas(l) || coluna < 0 || coluna >= labirinto_n_colunas(l)){
        return 0;
    }
    unsigned char pos = labirinto_obter(l, linha, coluna);
    if(pos == OCUPADO){
        return 0;
    }
    return 1;
}

ResultData breadth_first_search(Labirinto *l, Celula _inicio, Celula _fim){
    ResultData result;
    int result_allocated = result_allocated_inicial;
    result = _default_result();
    result.caminho = (Celula**)malloc(sizeof(Celula*) * result_allocated_inicial);

    Celula *inicio = celula_create(_inicio.x, _inicio.y, NULL);
    Celula *fim = celula_create(_fim.x, _fim.y, NULL);


    int direcao[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1},  {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
    int tam_linha = labirinto_n_linhas(l);
    int tam_coluna = labirinto_n_colunas(l);
    int matriz_visitadas[tam_linha][tam_coluna];
    inicializa_matriz_visitadas(tam_linha, tam_coluna, matriz_visitadas);

    Deque *deque = deque_construct();
    Deque *deque_lixo = deque_construct();
    deque_push_back(deque, inicio);
    matriz_visitadas[inicio->x][inicio->y] = 1;

    Celula *ultima;
    while (!deque_empty(deque)){
        Celula *celula_atual = deque_pop_front(deque);
        //printf("%d %d\n", celula_atual->x, celula_atual->y);
        deque_push_back(deque_lixo, celula_atual);
        result.nos_expandidos++;
        
        if(celula_atual->x == fim->x && celula_atual->y == fim->y){
            ultima = celula_atual;
            result.sucesso = 1;
            break;
        }
        for(int i = 0; i < 8; i++){
            int linha = celula_atual->x + direcao[i][0];
            int coluna = celula_atual->y + direcao[i][1];

            if(Celula_valida(l, linha, coluna)){
                if(matriz_visitadas[linha][coluna] == 0){
                    matriz_visitadas[linha][coluna] = 1;
                    Celula *nova_celula = celula_create(linha, coluna, celula_atual);
                    deque_push_back(deque, nova_celula);
                }
            }
        }
    }
    
    if(result.sucesso){
        result.custo_caminho = 0;
        while(ultima != NULL){
            ultima->caminho = 1;
            if(result.tamanho_caminho+1 == result_allocated){
                result_allocated *= 2;
                result.caminho = (Celula**)realloc(result.caminho, sizeof(Celula*) * result_allocated);
            }
            result.caminho[result.tamanho_caminho] = ultima;
            result.tamanho_caminho++;

            if(ultima->pai != NULL){
                Celula *pai = (Celula*)ultima->pai;
                result.custo_caminho += sqrt(pow(ultima->x - pai->x, 2) + pow(ultima->y - pai->y, 2));
            }
            ultima = ultima->pai;
        }
    }
    if(deque_lixo != NULL)
        deque_free_fn(deque_lixo, celula_inutilizavel_destroy);
    if(deque != NULL)
        deque_free_fn(deque, celula_inutilizavel_destroy);

    free(fim);
    return result;
}

ResultData depth_first_search(Labirinto *l, Celula inicio, Celula fim)
{
    // TODO!
    return _default_result();
}
