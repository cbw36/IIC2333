#include "allQueues.h"
#include "process.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>



AllQueues* initAllQueues(int size)
{
    AllQueues* all_queues = malloc(sizeof(AllQueues));
    all_queues->size = size;
    all_queues->pos = 0;
    all_queues->queues =malloc(all_queues->size * sizeof(Queue));
    return all_queues;
}

void appendQueue(AllQueues* all_queues, Queue* new_queue)
{
    all_queues->queues[all_queues->pos] = *new_queue;
    all_queues->pos += 1;
}