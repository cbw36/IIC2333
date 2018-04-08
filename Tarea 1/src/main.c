#include <stdio.h>
#include <stdlib.h>
#include "allQueues.h"
#include "queue.h"
#include "process.h"


int main(int argc, char * argv[])
{
    printf("Enter main \n");
    AllQueues* all_queues = initAllQueues(2);
    Process* process = initProcess(1, 0, 0, 3, 0, 0);
    Queue* queue = initQueue(1, 2,3,4);
    appendQueue(all_queues, queue);
    appendProcess(&all_queues->queues[0], process);

    printf("All Queue size = %i \n ", all_queues->size);
    printf("Queue size = %i \n ", all_queues->queues[0].size);
    printf("Process size = %i \n ", all_queues->queues[0].processes[0].size);
    printf("finished main \n");

}

