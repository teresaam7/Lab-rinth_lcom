#include "queue.h"

Queue* newQueue(unsigned capacity){
    Queue* queue = (Queue*)malloc( sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    queue->last = capacity - 1;
    queue->values = (int*)malloc( queue->capacity * sizeof(int));
    return queue;
}

bool (queueIsFull)(Queue* queue){
    return (queue->size == (int) queue->capacity);
}

bool (queueIsEmpty)(Queue* queue){
    return (queue->size == 0);
}

int (sizeQueue)(Queue * queue){
    return queue->size;
}

bool (enqueue)(Queue* queue, uint8_t element){
    if (queueIsFull(queue))
        return false;
    queue->last = (queue->last + 1) % queue->capacity;
    queue->values[queue->last] = element;
    queue->size = queue->size + 1;
    return true;
}

uint8_t (dequeue)(Queue* queue){
    if (queueIsEmpty(queue))
        return 0;
    int element = queue->values[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return element;
}

uint8_t (frontQueue)(Queue* queue){
    if (queueIsEmpty(queue))
        return 0;
    return queue->values[queue->front];
}

uint8_t (backQueue)(Queue* queue){
    if (queueIsEmpty(queue))
        return 0;
    return queue->values[queue->last];
}

void clearQueue(Queue* queue){
    free(queue->values);
    free(queue);
}