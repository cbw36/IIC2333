#include <stdlib.h>
#include <string.h>
#include "process.h"

Process* initProcess(int pid, int size, int arrival_time)
//Process* initProcess(int pid, char name[256], int state, int completed, int size, int pos, int queue_pos)
{
    Process * process = malloc(sizeof(Process));
    process->subprocess = malloc(size * sizeof(int));
    process->pid = pid;
//    strcpy(process->name, name);
    process->priority = 0;
    process->state = 0;
    process-> size = size;
    process-> pos = 0;
    process->queue_pos = 0;
    process->completed = 0;
    process->quantum_remaining = 0;
    process->arrival_time = arrival_time;
    process -> cpu_turns = 0;
    
    return process;
}

void changeState(Process* process, int new_state)
{
    process->state = new_state;
}

int getState(Process* process)
{
    return process->state;
}

void subprocessAppend(Process* process, int val)
{
    process->subprocess[process->pos] = val;
    process->pos ++;
}
