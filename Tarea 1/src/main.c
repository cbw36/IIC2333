#include <stdio.h>
#include <stdlib.h>
#include "allQueues.h"
#include "queue.h"
#include "process.h"

/**
 * Rule 1: If priority of process A>B, execute A
 * Rule 2: If priority of A=B, execute in RR
 * Rule 3: Processes enter system in queue 1
 * Rule 4: When a process uses all of its quantum, it goes to the next queue
 * Rule 5: After period S, all processes move up 1 queue
 */
const int q = 5;
const int Q = 3;
const int NUM_PROCESSES = 2;


int main(int argc, char * argv[])
{

    AllQueues* all_queues = initializeScheduler();


    for (int i = 0; i<NUM_PROCESSES; i++)    //test prints
    {
        printf("queue process pid %i \n", all_queues->queues[0].processes[i].pid);
        for (int j=0; j<3;j++)
        {
            printf("queue process %i subprocess %i length: %i \n", i, j, all_queues->queues[0].processes[i].subprocess[j]);
        }
    }

    int simulate = 1;
    int clock = 0;
    int executed;
    printf("Enter simulation.  Clock = %i \n", clock);
    while (simulate == 1)
    {
        printf("Time step: Clock = %i \n", clock);
        for (int i =0; i<Q; i++)
        {
            int num_proc = all_queues->queues[i].num_processes;
            for (int j = 0; j<all_queues->queues[i].num_processes; j++)
            {
                executed = 0;
                if (all_queues->queues[i].processes[j].state == 0)
                {
                    executed = 1;
                    if (all_queues->queues[i].processes[j].quantum_remaining == all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed])
                    {
                        printf(" Case 1: Execute process and stay in queue \n");
                        clock = clock + all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].quantum_remaining = 0;
                        all_queues->queues[i].processes[j].completed++;
                        if (all_queues->queues[i].processes[j].completed == all_queues->queues[i].processes[j].pos)
                        {
                            printf(" Change state to finished \n");
                            changeState(&all_queues->queues[i].processes[j], 2);
                        }
                    }
                    else if (all_queues->queues[i].processes[j].quantum_remaining < all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed])
                    {
                        printf(" Case 2: Execute quantum time and reduce queue \n");
                        clock = clock + all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed] = all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]-all_queues->queues[i].processes[j].quantum_remaining;
                        changeQueue(&all_queues->queues[i], &all_queues->queues[i+1], &all_queues->queues[i].processes[j]);
                    }
                    else
                    {
                        printf(" Case 3: Execute process and stay in queue \n");
                        clock = clock + all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].quantum_remaining = all_queues->queues[i].processes[j].quantum_remaining - all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed] ++;
                        if (all_queues->queues[i].processes[j].completed == all_queues->queues[i].processes[j].pos)
                        {
                            printf(" Change state to finished \n");
                            changeState(&all_queues->queues[i].processes[j], 2);
                        }
                    }
                }

            }
            if (executed==1)
                break;
            else if ( (executed == 0) && (i == Q ))
            { simulate = 0; }
        }
    }
    printf("exit simulation!");
}


AllQueues* initializeScheduler()
{
    AllQueues* all_queues = initAllQueues(Q);

    for (int i = 0; i<Q; i++)
    {
        Queue* new_queue = initQueue(i+1,q);
        appendQueue(all_queues, new_queue);
    }


    for (int i = 0; i<NUM_PROCESSES; i++ )
    {
        Process* new_process = initProcess(i, 3);
        for (int j=0; j<3; j++)
        {
            subprocessAppend(new_process, j*5);
        }
        appendProcess(&all_queues->queues[0], new_process);
    }
    return all_queues;

}