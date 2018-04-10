#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


Queue* initQueue(int priority, int q)
{
    Queue * queue = malloc(sizeof(Queue));
    queue->q = q;
    queue->priority = priority  ;
    queue->size = 10;
    queue-> processes = malloc(queue->size * sizeof(Process));

    queue->num_processes = 0;
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
    queue->processes[queue->num_processes].quantum_remaining = queue->q;
    queue->num_processes += 1;
}

Process* removeProcess(Queue* queue,Process* process)
{
    int loc = process->queue_pos;
    for (int ind = loc; ind < queue->num_processes -1; ind++)
    {
        queue->processes[ind] = queue->processes[ind + 1];
        queue->processes[ind].queue_pos --;
    }
    queue->num_processes --;
    return process;
}


void changeQueue(Queue* cur_queue, Queue* new_queue, Process* process)
{
    appendProcess(new_queue, process);
    removeProcess(cur_queue, process);
}

