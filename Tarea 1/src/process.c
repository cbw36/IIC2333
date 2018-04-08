#include <stdlib.h>
#include "process.h"

Process* initProcess(int pid, char name[256], int state, int size, int pos, int queue_pos, int completed)
{
    Process * process = malloc(sizeof(Process));
    process->subprocess = malloc(size * sizeof(int));
    process->pid = pid;
    process->name = name;
    process->state = state;
    process-> size = size;
    process-> pos = pos;
    process->queue_pos = queue_pos;
    process->completed = completed;

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