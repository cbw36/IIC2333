#include <stdlib.h>
#include "queue.h"


Queue* initQueue(int priority, int q, int size, int num_processes)
{
    Queue * queue = malloc(sizeof(Queue));
    queue-> processes = malloc(size * sizeof(Process));
    queue->q = q;
    queue->priority = priority  ;
    queue->size = size;
    queue->num_procceses = num_processes;
//    queue->has_ready = has_ready;

}


void appendProcess(Queue* queue, Process* process)
{
    if (queue->num_processes == queue->size)
    {
        queue->processes = realloc(queue->processes, queue->size*2* sizeof(Process));
        queue->size *= 2;
    }
    queue->processes[queue->num_processes] = process;
    queue->num_processes += 1;
    process->queue_pos = num_processes;

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
{
    removeProcess(queue = cur_queue, process = process);
    appendProcess(queue = new_queue, process = process);

}

