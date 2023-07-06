
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "algorithms.h"

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

int celula_hash(HashTable *h, void *key){
    Celula *c = (Celula *)key;
    // 83 e 97 sao primos e o operador "^" é o XOR bit a bit
    return ((c->x * 83) ^ (c->y * 97)) % hash_table_size(h);
}

int celula_cmp(void *c1, void *c2){
    Celula *a = (Celula *)c1;
    Celula *b = (Celula *)c2;
    if (a->x == b->x && a->y == b->y)
        return 0;
    else
        return 1;
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

ResultData depth_first_search(Labirinto *l, Celula _inicio, Celula _fim){
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
    deque_push_front(deque, inicio);
    matriz_visitadas[inicio->x][inicio->y] = 1;

    Celula *ultima;
    while (!deque_empty(deque)){
        Celula *celula_atual = deque_pop_front(deque);
        deque_push_front(deque_lixo, celula_atual);
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
                    deque_push_front(deque, nova_celula);
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

double g(Celula *cel){
    double custo = 0;
    while(cel->pai != NULL){
        Celula *pai = (Celula*)cel->pai;
        custo += h(cel, pai);
        cel = cel->pai;
    }
    return custo;
}

double h(Celula *cel, Celula* fim){
    double custo = 0;
    custo = sqrt(pow(cel->x - fim->x, 2) + pow(cel->y - fim->y, 2));
    return custo;
}

double f(Celula *cel, Celula* fim){
    double prioridade = 0;
    prioridade = g(cel) + h(cel, fim);
    return prioridade;
}

ResultData a_star(Labirinto *l, Celula _inicio, Celula _fim){
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

    
    HashTable *h = hash_table_construct(19, celula_hash, celula_cmp);
    Heap *heap = heap_construct(h);

    //HashTable *h_lixo = hash_table_construct(19, celula_hash, celula_cmp);
    //Heap *lixo = heap_construct(h_lixo);
    
    heap_push(heap, inicio, f(inicio, fim));
    matriz_visitadas[inicio->x][inicio->y] = 1;

    Celula *ultima;
    //Celula *aux;
    Celula **lixos = malloc(sizeof(Celula*) * 2);
    int allocated_lixo = 2;
    int size_lixo = 0;
    while (!heap_empty(heap)){
        Celula *celula_atual = heap_pop(heap);
        result.nos_expandidos++;
        /*aux = heap_push(lixo, celula_atual, f(celula_atual, fim));
        if(aux){
            celula_destroy(aux);
        }*/
        if(size_lixo+1 == allocated_lixo){
            allocated_lixo *= 2;
            lixos = (Celula**)realloc(lixos, sizeof(Celula*) * allocated_lixo);
        }
        lixos[size_lixo] = celula_atual;
        size_lixo++;

        

        if(celula_atual->x == fim->x && celula_atual->y == fim->y){
            ultima = celula_atual;
            result.sucesso = 1;
            break;
        }
        for(int i = 0; i < 8; i++){
            int linha = celula_atual->x + direcao[i][0];
            int coluna = celula_atual->y + direcao[i][1];

            if(Celula_valida(l, linha, coluna)){
                if(matriz_visitadas[linha][coluna] ==  0){
                    Celula *nova_celula = celula_create(linha, coluna, celula_atual);
                    matriz_visitadas[linha][coluna] = 1;   
                    nova_celula = heap_push(heap, nova_celula, f(nova_celula, fim));
                    if(nova_celula){
                        celula_destroy(nova_celula);
                    }
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
    for(int i = 0; i < size_lixo; i++){
        Celula *aux = lixos[i];
        if(aux->caminho == 0){
            celula_destroy(lixos[i]);
        }
    }
    free(lixos);
    /*
    HashTableIterator *it_lixo = hash_table_iterator(h_lixo);
    while (!hash_table_iterator_is_over(it_lixo))
    {
        HashTableItem *item = hash_table_iterator_next(it_lixo);
        Celula *cel = (Celula *)item->key;
        if(cel->caminho == 0){
            int *pos = (int *)item->val;
            celula_destroy(cel);
            free(pos);
        }
    }
    hash_table_iterator_destroy(it_lixo);
    hash_table_destroy(h_lixo);
    heap_destroy(lixo);
    */

    //libera hash e heap usados
    HashTableIterator *it = hash_table_iterator(h);
    while (!hash_table_iterator_is_over(it))
    {
        HashTableItem *item = hash_table_iterator_next(it);
        Celula *cel = (Celula *)item->key;
        if(cel->caminho == 0){
            int *pos = (int *)item->val;
            celula_destroy(cel);
            free(pos);
        }
    }

    hash_table_iterator_destroy(it);
    hash_table_destroy(h);
    heap_destroy(heap);
    free(fim);
    return result;
}




