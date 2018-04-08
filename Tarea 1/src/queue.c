#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


Queue* initQueue(int priority, int q)
{
    Queue * queue = malloc(sizeof(Queue));
    queue-> processes = malloc(size * sizeof(Process));
    queue->q = q;
    queue->priority = priority  ;
    queue->size = 10;
    queue->num_processes = 0;
//    queue->has_ready = has_ready;
    return queue;

}

void appendProcess(Queue* queue, Process* process)
{
    if (queue->num_processes == queue->size)
    {
        queue->processes = realloc(queue->processes, queue->size*2* sizeof(Process));
        queue->size *= 2;
    }
    queue->processes[queue->num_processes] = *process;
    queue->processes[queue->num_processes].queue_pos = queue->num_processes;
    queue->processes[queue->num_processes].priority = queue->priority;
    queue->num_processes += 1;
}

Process* removeProcess(Queue* queue,Process* process)
{
    int loc = process->queue_pos;
    for (int ind = loc; ind < queue->size -1; ind++)
    {
        queue->processes[ind] = queue->processes[ind + 1];
    }
    queue->num_processes --;
    return process;
}


void changeQueue(Queue* cur_queue, Queue* new_queue, Process* process)
{   printf("inicio \n" );
    removeProcess(cur_queue, process);
    printf("Cantidad de proceso en cola 1 = %i \n ", cur_queue->num_processes);
    appendProcess(new_queue, process);
    printf("fin \n");

}
