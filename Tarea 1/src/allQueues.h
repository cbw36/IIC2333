#include "queue.h"

#ifndef TAREA_1_ALLQUEUES_H
#define TAREA_1_ALLQUEUES_H

struct allQueues{
    struct Queue* queues;
    int size;
    int pos;

};


typedef struct allQueues AllQueues;


AllQueues* initAllQueues(int size);

void appendQueue(AllQueues* all_queues,Queue* new_queue);

#endif //TAREA_1_ALLQUEUE_H