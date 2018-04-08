#include <stdio.h>
#include <stdlib.h>
#include "allQueues.h"
#include "queue.h"
#include "process.h"


int main(int argc, char * argv[])
{
    printf("Enter main \n");
    AllQueues* all_queues = initAllQueues(2);
    Queue* queue = initQueue(0,0,4,0);
    Process* process = initProcess(1, 0, 0, 3, 0, 0);
    appendProcess(queue, process);
    appendQueue(all_queues, queue);

    printf("All Queue size = %i \n ", all_queues->size);
    printf("Queue size = %i \n ", all_queues->queues[0].size);
    printf("Process size in queue = %i \n ", queue->processes[0].size);
    printf("finished main \n");

}

