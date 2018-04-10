#include <stdio.h>
#include <stdlib.h>
#include "allQueues.h"
#include "queue.h"
#include "process.h"
#include "list.h"
#include <unistd.h>

/**
 * Rule 1: If priority of process A>B, execute A
 * Rule 2: If priority of A=B, execute in RR
 * Rule 3: Processes enter system in queue 1
 * Rule 4: When a process uses all of its quantum, it goes to the next queue
 * Rule 5: After period S, all processes move up 1 queue
 */
const int q = 5;
const int Q = 3;
const int NUM_PROCESSES = 5;
const int PERIOD = 4;


int main(int argc, char * argv[])
{

//    char*txt = argv[2];
//    FILE*fr;
//    fr = fopen(txt, "r");
//    char name[10];
//    int arrival;
//    int num_p;
//
//    while(fscanf(fr, "%s %d %d", name, &arrival, &num_p) != EOF) {
//        printf(" name %s arrival %d processes %d \n", name, arrival, num_p);
//    }

    List* list = initList();
    Process* new_process = initProcess(1, 3, 5);
    Process* new_process1 = initProcess(2, 3, 3);
    Process* new_process2 = initProcess(3, 3, 4);
    Process* new_process3 = initProcess(4, 3, 2);
    list_appendProcess(list, new_process);
    printf("%i %i\n",list->processes[0].pid, list->processes[0].queue_pos );
    list_appendProcess(list, new_process1);
    printf("append2\n" );
    printf("%i %i\n",list->processes[0].pid, list->processes[0].queue_pos );
    printf("%i %i\n",list->processes[1].pid, list->processes[1].queue_pos );
    list_appendProcess(list, new_process2);
    printf("append3\n" );
    printf("%i %i\n",list->processes[0].pid, list->processes[0].queue_pos );
    printf("%i %i\n",list->processes[1].pid, list->processes[1].queue_pos );
    printf("%i %i\n",list->processes[2].pid, list->processes[2].queue_pos );
    list_appendProcess(list, new_process3);
    printf("append4\n" );
    printf("%i %i\n",list->processes[0].pid, list->processes[0].queue_pos );
    printf("%i %i\n",list->processes[1].pid, list->processes[1].queue_pos );
    printf("%i %i\n",list->processes[2].pid, list->processes[2].queue_pos );
    printf("%i %i\n",list->processes[3].pid, list->processes[3].queue_pos );
    listremoveProces(list, &list->processes[3]);
    printf("remove\n" );
    printf("%i\n",list->processes[0].pid );
    printf("%i\n",list->processes[1].pid );
    printf("%i\n",list->processes[2].pid );
    printf("%i\n",list->processes[3].pid );

    AllQueues* all_queues = initializeScheduler();
    printState(all_queues);


    printf("Enter simulation");
    MLFQV1(all_queues, list);
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
        Process* new_process = initProcess(i, 3, i);
        for (int j=0; j<3; j++)
        {
            subprocessAppend(new_process, 1 + i + j*3);
        }
        appendProcess(&all_queues->queues[0], new_process);
    }
    return all_queues;

}

void printState(AllQueues* all_queues)
{
    for (int i = 0; i<NUM_PROCESSES; i++)    //test prints
    {
        printf("queue process pid %i \n", all_queues->queues[0].processes[i].pid);
        for (int j=0; j<3;j++)
        {
            printf("queue process %i subprocess %i length: %i \n", i, j, all_queues->queues[0].processes[i].subprocess[j]);
        }
    }
}

void MLFQV1(AllQueues* all_queues, List* list)
{
    int simulate = 1;
    int clock = 0;
    int executed;
    int g = 0;
    for (g = 0; g < list-> num_processes; g++) {
      printf("hola\n");
    }

    while (simulate == 1)
    {
        for (int i =0; i<Q; i++)
        {
            executed = 0;
            printf("Time step: Clock = %i \n", clock);
            int num_proc = all_queues->queues[i].num_processes;
            for (int j = 0; j<all_queues->queues[i].num_processes; j++)
            {
                if (all_queues->queues[i].processes[j].state == 0)
                {
                    executed = 1;
                    if (all_queues->queues[i].processes[j].quantum_remaining == all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed])
                    {
//                        printf(" Case 1: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i );
                        printf(" Case 1: Execute process %i subprocess %i from queue %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed,
                               i, all_queues->queues[i].processes[j].quantum_remaining ); // TODO ADDED!
                        clock = clock + all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].quantum_remaining = 0;
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]=0;//TODO ADDED!
                        all_queues->queues[i].processes[j].completed++;
                        printf("Burst state for process %i: %i %i %i \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[0],
                               all_queues->queues[i].processes[j].subprocess[1], all_queues->queues[i].processes[j].subprocess[2]); //TODO ADDED!

                        if (all_queues->queues[i].processes[j].completed == all_queues->queues[i].processes[j].pos)
                        {
                            printf(" Change state to finished \n");
                            changeState(&all_queues->queues[i].processes[j], 2);
                        }

                        if (i+1 != Q)
                        {
                            changeQueue(&all_queues->queues[i], &all_queues->queues[i+1], &all_queues->queues[i].processes[j]);
                            printf("Case 1 change queue \n");
                            j--;
                        }

                        else
                        {
                            all_queues->queues[i].processes[j].quantum_remaining =all_queues->queues[i].q;
                        }
                        printf("Queue 0 length = % i state = %i % i %i %i %i \n", all_queues->queues[0].num_processes, all_queues->queues[0].processes[0].pid,all_queues->queues[0].processes[1].pid,all_queues->queues[0].processes[2].pid,all_queues->queues[0].processes[3].pid,all_queues->queues[0].processes[4].pid);
                        printf("Queue 1 length = % i state = %i % i %i %i %i \n", all_queues->queues[1].num_processes, all_queues->queues[1].processes[0].pid,all_queues->queues[1].processes[1].pid,all_queues->queues[1].processes[2].pid,all_queues->queues[1].processes[3].pid,all_queues->queues[1].processes[4].pid);
                        printf("Queue 2 length = % i state = %i % i %i %i %i \n", all_queues->queues[2].num_processes, all_queues->queues[2].processes[0].pid,all_queues->queues[2].processes[1].pid,all_queues->queues[2].processes[2].pid,all_queues->queues[2].processes[3].pid,all_queues->queues[2].processes[4].pid);

                    }
                    else if (all_queues->queues[i].processes[j].quantum_remaining < all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed])
                    {
//                        printf(" Case 2: Execute quantum time and reduce queue %i %i, %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i);
                        printf(" Case 2: Execute process %i subprocess %i from queue %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].quantum_remaining );
                        clock = clock + all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed] = all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]-all_queues->queues[i].processes[j].quantum_remaining;
                        if (i+1 != Q)
                        {
                            changeQueue(&all_queues->queues[i], &all_queues->queues[i+1], &all_queues->queues[i].processes[j]);
                            j--;
                            printf("Case 2 change queue \n"); //TODO ADDED
                        }

                        else
                        {
                            all_queues->queues[i].processes[j].quantum_remaining =all_queues->queues[i].q;
                        }
                        printf("Queue 0 length = % i state = %i % i %i %i %i \n", all_queues->queues[0].num_processes, all_queues->queues[0].processes[0].pid,all_queues->queues[0].processes[1].pid,all_queues->queues[0].processes[2].pid,all_queues->queues[0].processes[3].pid,all_queues->queues[0].processes[4].pid);
                        printf("Queue 1 length = % i state = %i % i %i %i %i \n", all_queues->queues[1].num_processes, all_queues->queues[1].processes[0].pid,all_queues->queues[1].processes[1].pid,all_queues->queues[1].processes[2].pid,all_queues->queues[1].processes[3].pid,all_queues->queues[1].processes[4].pid);
                        printf("Queue 2 length = % i state = %i % i %i %i %i \n", all_queues->queues[2].num_processes, all_queues->queues[2].processes[0].pid,all_queues->queues[2].processes[1].pid,all_queues->queues[2].processes[2].pid,all_queues->queues[2].processes[3].pid,all_queues->queues[2].processes[4].pid);

                    }
                    else
                    {
//                        printf(" Case 3: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid,  all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],all_queues->queues[i].processes[j].completed);
                        printf(" Case 3: Execute process %i subprocess %i from queue  %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]);
                        clock = clock + all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].quantum_remaining = all_queues->queues[i].processes[j].quantum_remaining - all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]=0;//TODO ADDED!
                        all_queues->queues[i].processes[j].completed ++;
                        if (all_queues->queues[i].processes[j].completed == all_queues->queues[i].processes[j].pos)
                        {
                            printf(" Change state to finished \n");
                            changeState(&all_queues->queues[i].processes[j], 2);
                        }
                        printf("Burst state for process %i: %i %i %i \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[0],
                               all_queues->queues[i].processes[j].subprocess[1], all_queues->queues[i].processes[j].subprocess[2]); //TODO ADDED
                        printf("Queue 0 length = % i state = %i % i %i %i %i \n", all_queues->queues[0].num_processes, all_queues->queues[0].processes[0].pid,all_queues->queues[0].processes[1].pid,all_queues->queues[0].processes[2].pid,all_queues->queues[0].processes[3].pid,all_queues->queues[0].processes[4].pid);
                        printf("Queue 1 length = % i state = %i % i %i %i %i \n", all_queues->queues[1].num_processes, all_queues->queues[1].processes[0].pid,all_queues->queues[1].processes[1].pid,all_queues->queues[1].processes[2].pid,all_queues->queues[1].processes[3].pid,all_queues->queues[1].processes[4].pid);
                        printf("Queue 2 length = % i state = %i % i %i %i %i \n", all_queues->queues[2].num_processes, all_queues->queues[2].processes[0].pid,all_queues->queues[2].processes[1].pid,all_queues->queues[2].processes[2].pid,all_queues->queues[2].processes[3].pid,all_queues->queues[2].processes[4].pid);

                    }
                }
            }

            if (executed==1)
                break;

            else if ( (executed == 0) && (i == Q-1 ))
            { simulate = 0; }
        }
        usleep(100000);
    }
}

void MLFQV2(AllQueues* all_queues)
{
    int S = PERIOD;
    int simulate = 1;
    int clock = 0;
    int executed;

    while (simulate == 1)
    {
        //Before Check for next process check if period S has elapsed
        if (clock> S)  // Move all processes up a queue (unless at queue 0 already
        {
            while (clock >S)
            {
                S += PERIOD;
            }
            printf("Change Period at clock = %i S = %i \n", clock, S);


            for (int i = 1; i<Q; i++)
            {
                for (int j = 0; all_queues->queues[i].num_processes; j++) // each process in each queue
                {
                    printf("Rule 5: Move process %i from queue %i \n", all_queues->queues[i].processes[j].pid, i);
                    changeQueue(&all_queues->queues[i], &all_queues->queues[i-1],
                                &all_queues->queues[i].processes[j]);
                    j--;
                }
            }
        }

        //Find next process to execute
        for (int i =0; i<Q; i++)
        {
            executed = 0;
            printf("Time step: Clock = %i \n", clock);
            int num_proc = all_queues->queues[i].num_processes;
            for (int j = 0; j<all_queues->queues[i].num_processes; j++)
            {
                if (all_queues->queues[i].processes[j].state == 0)
                {
                    executed = 1;
                    if (all_queues->queues[i].processes[j].quantum_remaining == all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed])
                    {
//                        printf(" Case 1: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i );
                        printf(" Case 1: Execute process %i subprocess %i from queue %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed,
                               i, all_queues->queues[i].processes[j].quantum_remaining ); // TODO ADDED!
                        clock = clock + all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].quantum_remaining = 0;
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]=0;//TODO ADDED!
                        all_queues->queues[i].processes[j].completed++;
                        printf("Burst state for process %i: %i %i %i \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[0],
                               all_queues->queues[i].processes[j].subprocess[1], all_queues->queues[i].processes[j].subprocess[2]); //TODO ADDED!

                        if (all_queues->queues[i].processes[j].completed == all_queues->queues[i].processes[j].pos)
                        {
                            printf(" Change state to finished \n");
                            changeState(&all_queues->queues[i].processes[j], 2);
                        }

                        if (i+1 != Q)
                        {
                            changeQueue(&all_queues->queues[i], &all_queues->queues[i+1], &all_queues->queues[i].processes[j]);
                            printf("Case 1 change queue \n");
                            j--;
                        }

                        else
                        {
                            all_queues->queues[i].processes[j].quantum_remaining =all_queues->queues[i].q;
                        }
                        printf("Queue 0 length = % i state = %i % i %i %i %i \n", all_queues->queues[0].num_processes, all_queues->queues[0].processes[0].pid,all_queues->queues[0].processes[1].pid,all_queues->queues[0].processes[2].pid,all_queues->queues[0].processes[3].pid,all_queues->queues[0].processes[4].pid);
                        printf("Queue 1 length = % i state = %i % i %i %i %i \n", all_queues->queues[1].num_processes, all_queues->queues[1].processes[0].pid,all_queues->queues[1].processes[1].pid,all_queues->queues[1].processes[2].pid,all_queues->queues[1].processes[3].pid,all_queues->queues[1].processes[4].pid);
                        printf("Queue 2 length = % i state = %i % i %i %i %i \n", all_queues->queues[2].num_processes, all_queues->queues[2].processes[0].pid,all_queues->queues[2].processes[1].pid,all_queues->queues[2].processes[2].pid,all_queues->queues[2].processes[3].pid,all_queues->queues[2].processes[4].pid);

                    }
                    else if (all_queues->queues[i].processes[j].quantum_remaining < all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed])
                    {
//                        printf(" Case 2: Execute quantum time and reduce queue %i %i, %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i);
                        printf(" Case 2: Execute process %i subprocess %i from queue %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].quantum_remaining );
                        clock = clock + all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed] = all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]-all_queues->queues[i].processes[j].quantum_remaining;
                        if (i+1 != Q)
                        {
                            changeQueue(&all_queues->queues[i], &all_queues->queues[i+1], &all_queues->queues[i].processes[j]);
                            j--;
                            printf("Case 2 change queue \n"); //TODO ADDED
                        }

                        else
                        {
                            all_queues->queues[i].processes[j].quantum_remaining =all_queues->queues[i].q;
                        }
                        printf("Queue 0 length = % i state = %i % i %i %i %i \n", all_queues->queues[0].num_processes, all_queues->queues[0].processes[0].pid,all_queues->queues[0].processes[1].pid,all_queues->queues[0].processes[2].pid,all_queues->queues[0].processes[3].pid,all_queues->queues[0].processes[4].pid);
                        printf("Queue 1 length = % i state = %i % i %i %i %i \n", all_queues->queues[1].num_processes, all_queues->queues[1].processes[0].pid,all_queues->queues[1].processes[1].pid,all_queues->queues[1].processes[2].pid,all_queues->queues[1].processes[3].pid,all_queues->queues[1].processes[4].pid);
                        printf("Queue 2 length = % i state = %i % i %i %i %i \n", all_queues->queues[2].num_processes, all_queues->queues[2].processes[0].pid,all_queues->queues[2].processes[1].pid,all_queues->queues[2].processes[2].pid,all_queues->queues[2].processes[3].pid,all_queues->queues[2].processes[4].pid);

                    }
                    else
                    {
//                        printf(" Case 3: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid,  all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],all_queues->queues[i].processes[j].completed);
                        printf(" Case 3: Execute process %i subprocess %i from queue  %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]);
                        clock = clock + all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].quantum_remaining = all_queues->queues[i].processes[j].quantum_remaining - all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]=0;//TODO ADDED!
                        all_queues->queues[i].processes[j].completed ++;
                        if (all_queues->queues[i].processes[j].completed == all_queues->queues[i].processes[j].pos)
                        {
                            printf(" Change state to finished \n");
                            changeState(&all_queues->queues[i].processes[j], 2);
                        }
                        printf("Burst state for process %i: %i %i %i \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[0],
                               all_queues->queues[i].processes[j].subprocess[1], all_queues->queues[i].processes[j].subprocess[2]); //TODO ADDED
                        printf("Queue 0 length = % i state = %i % i %i %i %i \n", all_queues->queues[0].num_processes, all_queues->queues[0].processes[0].pid,all_queues->queues[0].processes[1].pid,all_queues->queues[0].processes[2].pid,all_queues->queues[0].processes[3].pid,all_queues->queues[0].processes[4].pid);
                        printf("Queue 1 length = % i state = %i % i %i %i %i \n", all_queues->queues[1].num_processes, all_queues->queues[1].processes[0].pid,all_queues->queues[1].processes[1].pid,all_queues->queues[1].processes[2].pid,all_queues->queues[1].processes[3].pid,all_queues->queues[1].processes[4].pid);
                        printf("Queue 2 length = % i state = %i % i %i %i %i \n", all_queues->queues[2].num_processes, all_queues->queues[2].processes[0].pid,all_queues->queues[2].processes[1].pid,all_queues->queues[2].processes[2].pid,all_queues->queues[2].processes[3].pid,all_queues->queues[2].processes[4].pid);

                    }
                }
            }

            if (executed==1)
                break;

            else if ( (executed == 0) && (i == Q-1 ))
            { simulate = 0; }
        }
        usleep(100000);
    }
}

void MLFQV3(AllQueues* all_queues)
{
    int S = PERIOD;
    int simulate = 1;
    int clock = 0;
    int executed;

    while (simulate == 1)
    {
        //Before Check for next process check if period S has elapsed
        if (clock> S)  // Move all processes up a queue (unless at queue 0 already
        {
            while (clock >S)
            {
                S += PERIOD;
            }
            printf("Change Period at clock = %i S = %i \n", clock, S);


            for (int i = 1; i<Q; i++)
            {
                for (int j = 0; all_queues->queues[i].num_processes; j++) // each process in each queue
                {
                    printf("Rule 5: Move process %i from queue %i \n", all_queues->queues[i].processes[j].pid, i);
                    changeQueue(&all_queues->queues[i], &all_queues->queues[i-1],
                                &all_queues->queues[i].processes[j]);
                    j--;
                }
            }
        }

        //Find next process to execute
        for (int i =0; i<Q; i++)
        {
            executed = 0;
            printf("Time step: Clock = %i \n", clock);
            int num_proc = all_queues->queues[i].num_processes;
            for (int j = 0; j<all_queues->queues[i].num_processes; j++)
            {
                if (all_queues->queues[i].processes[j].state == 0)
                {
                    executed = 1;
                    if (all_queues->queues[i].processes[j].quantum_remaining == all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed])
                    {
//                        printf(" Case 1: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i );
                        printf(" Case 1: Execute process %i subprocess %i from queue %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed,
                               i, all_queues->queues[i].processes[j].quantum_remaining ); // TODO ADDED!
                        clock = clock + all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].quantum_remaining = 0;
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]=0;//TODO ADDED!
                        all_queues->queues[i].processes[j].completed++;
                        printf("Burst state for process %i: %i %i %i \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[0],
                               all_queues->queues[i].processes[j].subprocess[1], all_queues->queues[i].processes[j].subprocess[2]); //TODO ADDED!

                        if (all_queues->queues[i].processes[j].completed == all_queues->queues[i].processes[j].pos)
                        {
                            printf(" Change state to finished \n");
                            changeState(&all_queues->queues[i].processes[j], 2);
                        }

                        if (i+1 != Q)
                        {
                            changeQueue(&all_queues->queues[i], &all_queues->queues[i+1], &all_queues->queues[i].processes[j]);
                            printf("Case 1 change queue \n");
                            j--;
                        }

                        else
                        {
                            all_queues->queues[i].processes[j].quantum_remaining =all_queues->queues[i].q;
                        }
                        printf("Queue 0 length = % i state = %i % i %i %i %i \n", all_queues->queues[0].num_processes, all_queues->queues[0].processes[0].pid,all_queues->queues[0].processes[1].pid,all_queues->queues[0].processes[2].pid,all_queues->queues[0].processes[3].pid,all_queues->queues[0].processes[4].pid);
                        printf("Queue 1 length = % i state = %i % i %i %i %i \n", all_queues->queues[1].num_processes, all_queues->queues[1].processes[0].pid,all_queues->queues[1].processes[1].pid,all_queues->queues[1].processes[2].pid,all_queues->queues[1].processes[3].pid,all_queues->queues[1].processes[4].pid);
                        printf("Queue 2 length = % i state = %i % i %i %i %i \n", all_queues->queues[2].num_processes, all_queues->queues[2].processes[0].pid,all_queues->queues[2].processes[1].pid,all_queues->queues[2].processes[2].pid,all_queues->queues[2].processes[3].pid,all_queues->queues[2].processes[4].pid);

                    }
                    else if (all_queues->queues[i].processes[j].quantum_remaining < all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed])
                    {
//                        printf(" Case 2: Execute quantum time and reduce queue %i %i, %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i);
                        printf(" Case 2: Execute process %i subprocess %i from queue %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].quantum_remaining );
                        clock = clock + all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed] = all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]-all_queues->queues[i].processes[j].quantum_remaining;
                        if (i+1 != Q)
                        {
                            changeQueue(&all_queues->queues[i], &all_queues->queues[i+1], &all_queues->queues[i].processes[j]);
                            j--;
                            printf("Case 2 change queue \n"); //TODO ADDED
                        }

                        else
                        {
                            all_queues->queues[i].processes[j].quantum_remaining =all_queues->queues[i].q;
                        }
                        printf("Queue 0 length = % i state = %i % i %i %i %i \n", all_queues->queues[0].num_processes, all_queues->queues[0].processes[0].pid,all_queues->queues[0].processes[1].pid,all_queues->queues[0].processes[2].pid,all_queues->queues[0].processes[3].pid,all_queues->queues[0].processes[4].pid);
                        printf("Queue 1 length = % i state = %i % i %i %i %i \n", all_queues->queues[1].num_processes, all_queues->queues[1].processes[0].pid,all_queues->queues[1].processes[1].pid,all_queues->queues[1].processes[2].pid,all_queues->queues[1].processes[3].pid,all_queues->queues[1].processes[4].pid);
                        printf("Queue 2 length = % i state = %i % i %i %i %i \n", all_queues->queues[2].num_processes, all_queues->queues[2].processes[0].pid,all_queues->queues[2].processes[1].pid,all_queues->queues[2].processes[2].pid,all_queues->queues[2].processes[3].pid,all_queues->queues[2].processes[4].pid);

                    }
                    else
                    {
//                        printf(" Case 3: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid,  all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],all_queues->queues[i].processes[j].completed);
                        printf(" Case 3: Execute process %i subprocess %i from queue  %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]);
                        clock = clock + all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].quantum_remaining = all_queues->queues[i].processes[j].quantum_remaining - all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]=0;//TODO ADDED!
                        all_queues->queues[i].processes[j].completed ++;
                        if (all_queues->queues[i].processes[j].completed == all_queues->queues[i].processes[j].pos)
                        {
                            printf(" Change state to finished \n");
                            changeState(&all_queues->queues[i].processes[j], 2);
                        }
                        printf("Burst state for process %i: %i %i %i \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[0],
                               all_queues->queues[i].processes[j].subprocess[1], all_queues->queues[i].processes[j].subprocess[2]); //TODO ADDED
                        printf("Queue 0 length = % i state = %i % i %i %i %i \n", all_queues->queues[0].num_processes, all_queues->queues[0].processes[0].pid,all_queues->queues[0].processes[1].pid,all_queues->queues[0].processes[2].pid,all_queues->queues[0].processes[3].pid,all_queues->queues[0].processes[4].pid);
                        printf("Queue 1 length = % i state = %i % i %i %i %i \n", all_queues->queues[1].num_processes, all_queues->queues[1].processes[0].pid,all_queues->queues[1].processes[1].pid,all_queues->queues[1].processes[2].pid,all_queues->queues[1].processes[3].pid,all_queues->queues[1].processes[4].pid);
                        printf("Queue 2 length = % i state = %i % i %i %i %i \n", all_queues->queues[2].num_processes, all_queues->queues[2].processes[0].pid,all_queues->queues[2].processes[1].pid,all_queues->queues[2].processes[2].pid,all_queues->queues[2].processes[3].pid,all_queues->queues[2].processes[4].pid);

                    }
                }
            }

            if (executed==1)
                break;

            else if ( (executed == 0) && (i == Q-1 ))
            { simulate = 0; }
        }
        usleep(100000);
    }
}
