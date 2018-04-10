#include "process.h"

#ifndef TAREA_1_QUEUE_H
#define TAREA_1_QUEUE_H

 struct queue{
     struct process * processes;
     int q;
     int priority;
     int size;
     int num_processes;

 };


typedef struct queue Queue;

Queue* initQueue(int priority, int q);
void appendProcess(Queue* queue, Process* process);
Process* removeProcess(Queue* queue,Process* process);
void changeQueue(Queue* cur_queue, Queue* new_queue, Process* process);


#endif //TAREA_1_QUEUE_H
