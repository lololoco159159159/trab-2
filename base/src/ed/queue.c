
#include "queue.h"
#include "forward_list.h"
#include <stdlib.h> 

struct Queue{
    ForwardList *list;
};

Queue *queue_construct(){
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    queue->list = list_construct();
    return queue;
}

void queue_destroy(Queue *s){
    list_destroy(s->list);
    free(s);
}


data_type queue_dequeue(Queue *queue){
    data_type data = list_pop_front(queue->list);
    return data;
}

void queue_enqueue(Queue *queue, data_type value){
    list_push_back(queue->list, value);

}

bool queue_empty(Queue *s){
    return !list_size(s->list);
}