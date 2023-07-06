
#include "queue.h"
#include "forward_list.h"
#include <stdlib.h> 

struct Queue{
    ForwardList *list;
};

Queue *queue_construct(){
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    queue->list = forward_list_construct();
    return queue;
}

void queue_destroy(Queue *s){
    forward_list_destroy(s->list);
    free(s);
}


data_type queue_dequeue(Queue *queue){
    data_type data = forward_list_pop_front(queue->list);
    return data;
}

void queue_enqueue(Queue *queue, data_type value){
    forward_list_push_back(queue->list, value);

}

bool queue_empty(Queue *s){
    return !forward_list_size(s->list);
}