#include <stdio.h>
#include <stdlib.h>
#include "allQueues.h"
#include "queue.h"
#include "process.h"


int main(int argc, char * argv[])
{
    printf("Enter main \n");
    AllQueues* all_queues = initAllQueues(2);
    Queue* queue = initQueue(0,0,6,0);
    Queue* queue1 = initQueue(0,0,6,0);
    Process* process = initProcess(1, 0, 0, 3, 0, 0);
    appendProcess(queue, process);
    appendQueue(all_queues, queue);
    appendQueue(all_queues, queue1);

    //printf("All Queue size = %i \n ", all_queues->size);
    printf("Cantidad de proceso en cola 1 = %i \n ", all_queues->queues[0].num_processes);
    printf("Cantidad de proceso en cola 2 = %i \n ", all_queues->queues[1].num_processes);
    changeQueue(&all_queues->queues[0], &all_queues->queues[1], process);
    printf("Cantidad de proceso en cola 1 = %i \n ", all_queues->queues[0].num_processes);
    printf("Cantidad de proceso en cola 2 = %i \n ", all_queues->queues[1].num_processes);
    printf("finished main \n");



}
