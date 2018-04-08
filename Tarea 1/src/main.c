#include <stdio.h>
#include <stdlib.h>
#include "allQueues.h"
#include "queue.h"
#include "process.h"


int main(int argc, char * argv[])
{
    FILE *input_data;
    char *mode = "r";
    input_data = fopen("processes.txt", mode);

    while (fscanf(input_data, "%s %d", username, &score) != EOF) {
        fprintf(ofp, "%s %d\n", username, score+10);

    printf("Enter main \n");
    AllQueues* all_queues = initAllQueues(2);
    Queue* queue1 = initQueue(0,0,4,0);
    Queue* queue2 = initQueue(0,0,8,0);

    Process* process = initProcess(1, 0, 0, 3, 0, 0);
    appendProcess(queue1, process);

    printf("finished main \n");

}

