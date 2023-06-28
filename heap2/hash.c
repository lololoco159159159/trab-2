#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "forward_list.h"
 

struct HashTable{
    int count;
    int size;
    HashFunction hash_fn;
    CmpFunction cmp_fn;
    ForwardList **buckets;
};

struct HashTableIterator{
    HashTable *hash_table;
    int bucket_id;
    Node *node;
    int current_element;
};

HashTableIterator *hash_table_iterator(HashTable *h){
    HashTableIterator *it = (HashTableIterator*)malloc(sizeof(HashTableIterator));
    it->hash_table = h;
    it->current_element = 0;
    it->bucket_id = 0;
    
    if(h->buckets[0] != NULL)
        it->node = h->buckets[0]->head;
    else
        it->node = NULL;

    return it;
}

int hash_table_iterator_is_over(HashTableIterator *it){
    return(it->current_element >= it->hash_table->count);
}

HashTableItem *_hash_pair_construct(void *key, void *val){
    HashTableItem *item = (HashTableItem*)malloc(sizeof(HashTableItem));
    item->key = key;
    item->val = val;
    return item;
}

HashTableItem *hash_pair_find(HashTable *h, void *key){
    int id = h->hash_fn(h, key) % h->size;
    ForwardList *l = h->buckets[id];
    
    if(l == NULL){
        return NULL;
    }

    Node *n = l->head;
    while(n!= NULL){
        HashTableItem *item = (HashTableItem*)n->value;
        if (h->cmp_fn(item->key, key) == 0)
            return item;
        n = n->next;
    }


    return NULL;
}

void _hash_pair_destruct(HashTableItem *item){
    free(item);
}

void _hash_pair_forwarlist_destruct(ForwardList *l){
    if (l == NULL){
        return;
    }

    Node *n = l->head;
    while (n != NULL){
        _hash_pair_destruct(n->value);
        n = n->next;
    }
}

HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn){
    HashTable *h = (HashTable*)malloc(sizeof(struct HashTable));
    h->buckets = (ForwardList**)malloc(sizeof(ForwardList*) * table_size);
    for(int i = 0; i < table_size; i++){
        h->buckets[i] = NULL;
    }
    h->hash_fn = hash_fn;
    h->cmp_fn = cmp_fn;
    h->size = table_size;
    h->count = 0;
    return h;
}

int hash_table_size(HashTable *h){
    return h->size;
}

void *hash_table_set(HashTable *h, void *key, void *val){
    int id = h->hash_fn(h, key) % h->size;
    HashTableItem *item = hash_pair_find(h, key);

    if(item != NULL){
        void *prev_val = item->val;
        item->val = val;
        return prev_val;
    }

    if(h->buckets[id] == NULL){
        h->buckets[id] = forward_list_construct();
    }

    
    h->count++;
    forward_list_push_back(h->buckets[id], _hash_pair_construct(key, val));
    return NULL;
}

void *hash_table_get(HashTable *h, void *key){
    int id = h->hash_fn(h, key) % h->size;
    if(h->buckets[id] == NULL){
        return NULL;
    }

    else{
        HashTableItem *item = hash_pair_find(h, key);
        if(item == NULL){
            return NULL;
        }
        return item->val;
    }
}

void *hash_table_pop(HashTable *h, void *key){
    if(h->count == 0){
        printf("hash table vazia\n");
        exit(0);
    }
    int id = h->hash_fn(h, key) % h->size;
    HashTableItem *item = hash_pair_find(h, key);

    if(item == NULL){
        return NULL;
    }

    void *val = item->val;
    h->count--;
    int aux = forward_list_remove(h->buckets[id], item);
    if(aux == 1){
        h->buckets[id] = NULL;
    }
    _hash_pair_destruct(item);

    return val;
}

void hash_table_destroy(HashTable *h){
    int id = 0;
    for(id = 0; id < h->size; id++){
        if (h->buckets[id] == NULL)
            continue;
        forward_list_destroy(h->buckets[id]);
    }
    free(h->buckets);
    free(h);
}

HashTableItem* hash_table_iterator_next(HashTableIterator *it){
    HashTableItem *item;
    if(it->node == NULL){
        do{
            it->bucket_id++;
        }while(it->hash_table->buckets[it->bucket_id] == NULL);
        it->node = it->hash_table->buckets[it->bucket_id]->head;
    }

    item = it->node->value;
    it->node = it->node->next;
    it->current_element++;
    
    return item;
}

void hash_table_iterator_destroy(HashTableIterator *it){
    free(it);
}

CmpFunction get_CmpFunction(HashTable *h){
    return h->cmp_fn;
}





