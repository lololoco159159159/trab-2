#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

typedef struct HashTable HashTable;
typedef struct HashTableIterator HashTableIterator;

typedef int (*HashFunction)(HashTable *, void *);
typedef int (*CmpFunction)(void *k1, void *k2);

typedef struct{
    void *key;
    void *val;
} HashTableItem;

/**
 * @brief Cria a tabela hash
 * @param table_size
 * Número de buckets na tabela hash
 * @param hash_fn
 * Ponteiro de função que recebe como entrada uma chave do tipo void* e retorna um inteiro
 * @param cmp_fn
 * Ponteiro de função que recebe como entrada dois void* representando duas chaves e tem a mesma semântica da função strcmp
 * @return HashTable*
 * Retorna o ponteiro da tabela hash recém alocada.
 */
HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn);

// funcao para insercao/atualizacao de pares chave-valor em O(1).
void *hash_table_set(HashTable *h, void *key, void *val);

// retorna o valor associado com a chave key ou NULL se ela nao existir em O(1).
void *hash_table_get(HashTable *h, void *key);

// remove o par chave-valor e retorna o valor ou NULL se nao existir tal chave em O(1).
void *hash_table_pop(HashTable *h, void *key);

// numero de buckets
int hash_table_size(HashTable *h);

// numero de elementos inseridos
int hash_table_num_elems(HashTable *h);

// libera o espaco alocado para a tabela hash
void hash_table_destroy(HashTable *h);

/**
 * @brief cria o HashTableIterator
 * 
 * @param h 
 * tabela has usada
 * @return HashTableIterator* 
 * ponteiro para o iterator criado
 */
HashTableIterator *hash_table_iterator(HashTable *h);

//avanca para o proximo elemento salvo na tabela hash
HashTableItem* hash_table_iterator_next(HashTableIterator *it);

// retorna se o iterator ja chegou ao fim da tabela
int hash_table_iterator_is_over(HashTableIterator *it);

//desaloca o iterator da hash table
void hash_table_iterator_destroy(HashTableIterator *it);

CmpFunction get_CmpFunction(HashTable *h);

//retorna o HashTableItem associado a chave
HashTableItem *hash_pair_find(HashTable *h, void *key);

#endif