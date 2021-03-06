#include "queue.h"
#include "list.h"

#ifndef TAREA_1_ALLQUEUES_H
#define TAREA_1_ALLQUEUES_H

struct allQueues{
    struct queue* queues;
    int size;
    int pos;
    int total_process;

};


typedef struct allQueues AllQueues;


AllQueues* initAllQueues(int size);

void appendQueue(AllQueues* all_queues,Queue* new_queue);
AllQueues* initializeScheduler(List* list);
void printState(AllQueues* all_queues);
void MLFQV1(AllQueues* all_queues, List* list);
void MLFQV2(AllQueues* all_queues, List* list);
void MLFQV3(AllQueues* all_queues, List* list);

#endif //TAREA_1_ALLQUEUE_H
