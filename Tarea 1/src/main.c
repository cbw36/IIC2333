#include <stdio.h>
#include <stdlib.h>
#include "allQueues.h"
#include "queue.h"
#include "process.h"
#include "list.h"
#include <unistd.h>
#include <string.h>

/**
 * Rule 1: If priority of process A>B, execute A
 * Rule 2: If priority of A=B, execute in RR
 * Rule 3: Processes enter system in queue 1
 * Rule 4: When a process uses all of its quantum, it goes to the next queue
 * Rule 5: After period S, all processes move up 1 queue
 */
int q ;
int Q;
int NUM_PROCESSES = 5;
int PERIOD;


int main(int argc, char * argv[])
{


    char*version = argv[1];
    char*filepath = argv[2];
    char *str_q = argv[3];
    q = atoi(str_q);
    char * str_Q = argv[4];
    Q = atoi(str_Q);
    if (Q<1)
        return 0;


    FILE*fr;
    fr = fopen(filepath, "r");
    char name[256];
    int arrival;
    int num_p;

    List* list = initList();
    int pid = 0;

    while (fscanf(fr, "%s %i %i", name, &arrival, &num_p) == 3) {
        Process* new_process = initProcess(pid, num_p, arrival);
        printf(" name %s arrival %d processes %d \n", name, arrival, num_p);
        int *events = malloc(sizeof(int) * (num_p));
        for (int i = 0; i < (num_p); i++) {
            fscanf(fr, "%i", &events[i]);
            subprocessAppend(new_process, events[i]);
        }
        list_appendProcess(list, new_process);
        printf("num_proc = %i \n", list->num_processes);
        pid ++;
    }
    printf("PRINT LIST BEFORE MODIFY: num_proc = %i \n", list->num_processes);
    for (int i = 0; i< list->num_processes; i++)
    {
        printf("List index %i process % i = %i %i %i \n",i, list->processes[i].pid, list->processes[i].subprocess[0],  list->processes[i].subprocess[1],  list->processes[i].subprocess[2]);
    }

//    while(fscanf(fr, "%s %d %d", name, &arrival, &num_p) != EOF) {
//        printf(" name %s arrival %d processes %d \n", name, arrival, num_p);
//    }

//    List* list = initList();
//    Process* new_process1 = initProcess(2, 3, 10);
//    Process* new_process2 = initProcess(3, 3, 40);
//    Process* new_process3 = initProcess(4, 3, 50);
//    list_appendProcess(list, new_process);
//    list_appendProcess(list, new_process1);
//    list_appendProcess(list, new_process2);
//    list_appendProcess(list, new_process3);

    AllQueues* all_queues = initializeScheduler(list);
////    printState(all_queues);
    printf("Queue 0 length = % i state = %i % i %i %i %i \n", all_queues->queues[0].num_processes, all_queues->queues[0].processes[0].pid,all_queues->queues[0].processes[1].pid,all_queues->queues[0].processes[2].pid,all_queues->queues[0].processes[3].pid,all_queues->queues[0].processes[4].pid);
    printf("Queue 1 length = % i state = %i % i %i %i %i \n", all_queues->queues[1].num_processes, all_queues->queues[1].processes[0].pid,all_queues->queues[1].processes[1].pid,all_queues->queues[1].processes[2].pid,all_queues->queues[1].processes[3].pid,all_queues->queues[1].processes[4].pid);
    printf("Queue 2 length = % i state = %i % i %i %i %i \n", all_queues->queues[2].num_processes, all_queues->queues[2].processes[0].pid,all_queues->queues[2].processes[1].pid,all_queues->queues[2].processes[2].pid,all_queues->queues[2].processes[3].pid,all_queues->queues[2].processes[4].pid);
    printf("num processes %i \n", list->num_processes);
    for (int i = 0; i< list->num_processes; i++)
    {
        printf("List index %i process % i = %i %i %i \n",i, list->processes[i].pid, list->processes[i].subprocess[0],  list->processes[i].subprocess[1],  list->processes[i].subprocess[2]);
    }

    if ((argc == 5)  && (strcmp(version, "v1") == 0) && (Q>0)) {
        printf("Enter simulation in Version 1 \n");
        MLFQV1(all_queues, list);
    }
    else if ((argc == 6)  && strcmp(version, "v2") == 0 && (Q>0)) {
        char*raw_period = argv[5];
        PERIOD = atoi(raw_period);
        printf("Enter simulation in Version 2 \n");
        MLFQV2(all_queues, list);
    }
    else if ((argc == 6)  && strcmp(version, "v3") == 0 && (Q>0)) {
        char*raw_period = argv[5];
        PERIOD = atoi(raw_period);
        printf("Enter simulation in Version 3 \n");
        MLFQV3(all_queues, list);
    }
    else
        if ((strcmp(version, "v1") != 0) && (strcmp(version, "v2") != 0) && (strcmp(version, "v3") != 0))
            printf("Improper Version specified.  Please specify either v1, v2, or v3 \n");
        else if ((argc>6) || (argc<5) || ((argc == 6) && (strcmp(version, "v1") == 1)) || ((argc == 5) && (strcmp(version, "v2") == 1)) || ((argc == 5) && (strcmp(version, "v3") == 1)))
            printf("Improper number of inputs \n");
        else if( (Q>0))
            printf("Q must be greater than 0 \n");

    printf("exit simulation!\n");
}

AllQueues* initializeScheduler(List * list)
{
    AllQueues* all_queues = initAllQueues(Q);

    //Initialize Queues
    for (int i = 0; i<Q; i++)
    {
        Queue* new_queue = initQueue(i+1,q);
        appendQueue(all_queues, new_queue);
    }

    //Generate processes with all start times = 0
//    for (int i = 0; i<NUM_PROCESSES; i++ )
//    {
//        Process* new_process = initProcess(i, 3, 0);
//        for (int j=0; j<3; j++)
//        {
//            subprocessAppend(new_process, 1 + i + j*3);
//        }
//        appendProcess(&all_queues->queues[0], new_process);
//    }

    //Read processes from list with variable start times
    for (int i = 0; i<list->num_processes; i++ )
    {
        if (list->processes[i].arrival_time == 0) {
            appendProcess(&all_queues->queues[0], &list->processes[i]);
            listremoveProces(list, &list->processes[i]);
            i--;
        }
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


    while (simulate == 1)
    {
        //First see if we need to add any processes
        int g;
        for (g = 0; g < list-> num_processes; g++)
        {
            if (clock >= list->processes[g].arrival_time)
            {
                appendProcess(&all_queues->queues[0], &list->processes[g]);
                listremoveProces(list, &list->processes[g]);
                g--;
            }
        }

        for (int i =0; i<Q; i++)
        {
            executed = 0;
//            printf("Time step: Clock = %i \n", clock);
            int num_proc = all_queues->queues[i].num_processes;
            for (int j = 0; j<all_queues->queues[i].num_processes; j++)
            {
                if (all_queues->queues[i].processes[j].state == 0)
                {
                    executed = 1;
                    all_queues->queues[i].processes[j].cpu_turns ++;
                    if (all_queues->queues[i].processes[j].response_bool == 0)
                    {
                        all_queues->queues[i].processes[j].response_bool = 1;
                        all_queues->queues[i].processes[j].response_time = clock - all_queues->queues[i].processes[j].arrival_time;
                    }
                    if (all_queues->queues[i].processes[j].quantum_remaining == all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed])
                    {
                        printf(" Case 1: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i );
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
                            int turnaround_time = clock - all_queues->queues[i].processes[j].arrival_time;
                            printf(" Change state of process %i to finished, CPU TIME =  %i BLOCKED = %i TURNAROUND TIME = %i RESPONSE TIME = %i  \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].cpu_turns, all_queues->queues[i].processes[j].blocked, turnaround_time, all_queues->queues[i].processes[j].response_time );
                            changeState(&all_queues->queues[i].processes[j], 2);
                        }
                        else
                        {
                            all_queues->queues[i].processes[j].blocked++;
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
                        printf(" Case 2: Execute quantum time and reduce queue %i %i, %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i);
                        printf(" Case 2: Execute process %i subprocess %i from queue %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].quantum_remaining );
                        clock = clock + all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed] = all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]-all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].blocked++;
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
                        printf(" Case 3: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid,  all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],all_queues->queues[i].processes[j].completed);
                        printf(" Case 3: Execute process %i subprocess %i from queue  %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]);
                        clock = clock + all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].quantum_remaining = all_queues->queues[i].processes[j].quantum_remaining - all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]=0;//TODO ADDED!
                        all_queues->queues[i].processes[j].completed ++;
                        if (all_queues->queues[i].processes[j].completed == all_queues->queues[i].processes[j].pos)
                        {
                            int turnaround_time = clock - all_queues->queues[i].processes[j].arrival_time;
                            printf(" Change state of process %i to finished, CPU TIME =  %i BLOCKED = %i TURNAROUND TIME = %i RESPONSE TIME = %i  \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].cpu_turns, all_queues->queues[i].processes[j].blocked, turnaround_time, all_queues->queues[i].processes[j].response_time );
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

            else if ( (executed == 0) && (i == Q-1) && (list->num_processes == 0))
            {
                simulate = 0;
            }
            if((executed == 0) && (i == Q-1 ) && (list->num_processes > 0)){
                clock = list->processes[0].arrival_time;
            }
        }
        usleep(100000);
    }
}

void MLFQV2(AllQueues* all_queues, List* list)
{
    int S = PERIOD;
    int simulate = 1;
    int clock = 0;
    int executed;

    while (simulate == 1)
    {
        //First Check for next process check if period S has elapsed
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

        //Second see if we need to add any processes
        int g;
        for (g = 0; g < list-> num_processes; g++)
        {
            if (clock >= list->processes[g].arrival_time)
            {
                appendProcess(&all_queues->queues[0], &list->processes[g]);
                listremoveProces(list, &list->processes[g]);
                g--;
            }
        }


        //Find next process to execute
        for (int i =0; i<Q; i++)
        {
            executed = 0;
//            printf("Time step: Clock = %i \n", clock);
            int num_proc = all_queues->queues[i].num_processes;
            for (int j = 0; j<all_queues->queues[i].num_processes; j++)
            {
                if (all_queues->queues[i].processes[j].state == 0)
                {
                    executed = 1;
                    all_queues->queues[i].processes[j].cpu_turns ++;
                    if (all_queues->queues[i].processes[j].response_bool == 0)
                    {
                        all_queues->queues[i].processes[j].response_bool = 1;
                        all_queues->queues[i].processes[j].response_time = clock - all_queues->queues[i].processes[j].arrival_time;
                    }
                    if (all_queues->queues[i].processes[j].quantum_remaining == all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed])
                    {
                        printf(" Case 1: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i );
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
                            int turnaround_time = clock - all_queues->queues[i].processes[j].arrival_time;
                            printf(" Change state of process %i to finished, CPU TIME =  %i BLOCKED = %i TURNAROUND TIME = %i RESPONSE TIME = %i  \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].cpu_turns, all_queues->queues[i].processes[j].blocked, turnaround_time, all_queues->queues[i].processes[j].response_time );
                            changeState(&all_queues->queues[i].processes[j], 2);
                        }
                        else
                        {
                            all_queues->queues[i].processes[j].blocked++;
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
                        printf(" Case 2: Execute quantum time and reduce queue %i %i, %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i);
                        printf(" Case 2: Execute process %i subprocess %i from queue %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].quantum_remaining );
                        clock = clock + all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed] = all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]-all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].blocked++;
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
                        printf(" Case 3: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid,  all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],all_queues->queues[i].processes[j].completed);
                        printf(" Case 3: Execute process %i subprocess %i from queue  %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]);
                        clock = clock + all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].quantum_remaining = all_queues->queues[i].processes[j].quantum_remaining - all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]=0;//TODO ADDED!
                        all_queues->queues[i].processes[j].completed ++;
                        if (all_queues->queues[i].processes[j].completed == all_queues->queues[i].processes[j].pos)
                        {
                            int turnaround_time = clock - all_queues->queues[i].processes[j].arrival_time;
                            printf(" Change state of process %i to finished, CPU TIME =  %i BLOCKED = %i TURNAROUND TIME = %i RESPONSE TIME = %i  \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].cpu_turns, all_queues->queues[i].processes[j].blocked, turnaround_time, all_queues->queues[i].processes[j].response_time );
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

            else if ( (executed == 0) && (i == Q-1) && (list->num_processes == 0))
            {
                simulate = 0;
            }
            if((executed == 0) && (i == Q-1 ) && (list->num_processes > 0)){
                clock = list->processes[0].arrival_time;
            }
        }
        usleep(100000);
    }
}

void MLFQV3(AllQueues* all_queues, List* list)
{
    //Change quantum values
    int p = 0;
    for ( p = 0; p < all_queues->pos; p++)
    {
        all_queues->queues[p].q = (all_queues->pos-p)*all_queues->queues[p].q;
    }
    for(int i = 0; i< Q; i++)
    {
        printf("QUEUE %i = %i \n", i, all_queues->queues[i].q);
    }

    int S = PERIOD;
    int simulate = 1;
    int clock = 0;
    int executed;

    while (simulate == 1)
    {
        //First Check for next process check if period S has elapsed
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

        //Second see if we need to add any processes
        int g;
        for (g = 0; g < list-> num_processes; g++)
        {
            if (clock >= list->processes[g].arrival_time)
            {
                printf("ADDING PROCESS FROM LIST!!! \n");
                appendProcess(&all_queues->queues[0], &list->processes[g]);
                listremoveProces(list, &list->processes[g]);
                g--;
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
                    all_queues->queues[i].processes[j].cpu_turns ++;
                    if (all_queues->queues[i].processes[j].response_bool == 0)
                    {
                        all_queues->queues[i].processes[j].response_bool = 1;
                        all_queues->queues[i].processes[j].response_time = clock - all_queues->queues[i].processes[j].arrival_time;
                    }
                    if (all_queues->queues[i].processes[j].quantum_remaining == all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed])
                    {
                        printf(" Case 1: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i );
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
                            int turnaround_time = clock - all_queues->queues[i].processes[j].arrival_time;
                            printf(" Change state of process %i to finished, CPU TIME =  %i BLOCKED = %i TURNAROUND TIME = %i RESPONSE TIME = %i  \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].cpu_turns, all_queues->queues[i].processes[j].blocked, turnaround_time, all_queues->queues[i].processes[j].response_time );
                            changeState(&all_queues->queues[i].processes[j], 2);
                        }
                        else
                        {
                            all_queues->queues[i].processes[j].blocked++;
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
                        printf(" Case 2: Execute quantum time and reduce queue %i %i, %i\n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],i);
                        printf(" Case 2: Execute process %i subprocess %i from queue %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].quantum_remaining );
                        clock = clock + all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed] = all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]-all_queues->queues[i].processes[j].quantum_remaining;
                        all_queues->queues[i].processes[j].blocked++;
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
                        printf(" Case 3: Execute process and stay in queue %i %i %i\n", all_queues->queues[i].processes[j].pid,  all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed],all_queues->queues[i].processes[j].completed);
                        printf(" Case 3: Execute process %i subprocess %i from queue  %i for %i seconds \n",all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].completed, i, all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]);
                        clock = clock + all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].quantum_remaining = all_queues->queues[i].processes[j].quantum_remaining - all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed];
                        all_queues->queues[i].processes[j].subprocess[all_queues->queues[i].processes[j].completed]=0;//TODO ADDED!
                        all_queues->queues[i].processes[j].completed ++;
                        if (all_queues->queues[i].processes[j].completed == all_queues->queues[i].processes[j].pos)
                        {
                            int turnaround_time = clock - all_queues->queues[i].processes[j].arrival_time;
                            printf(" Change state of process %i to finished, CPU TIME =  %i BLOCKED = %i TURNAROUND TIME = %i RESPONSE TIME = %i  \n", all_queues->queues[i].processes[j].pid, all_queues->queues[i].processes[j].cpu_turns, all_queues->queues[i].processes[j].blocked, turnaround_time, all_queues->queues[i].processes[j].response_time );
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

            else if ( (executed == 0) && (i == Q-1) && (list->num_processes == 0))
            {
                simulate = 0;
            }
            if((executed == 0) && (i == Q-1 ) && (list->num_processes > 0)){
                printf("NO process executed, change clock! \n");
                clock = list->processes[0].arrival_time;
            }
        }
        usleep(100000);
    }
}
