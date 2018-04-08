#include <stdlib.h>
#include <string.h>
#include "process.h"

Process* initProcess(int pid, int state, int completed, int size, int pos, int queue_pos)
//Process* initProcess(int pid, char name[256], int state, int completed, int size, int pos, int queue_pos)
{
    Process * process = malloc(sizeof(Process));
    process->subprocess = malloc(size * sizeof(int));
    process->pid = pid;
//    strcpy(process->name, name);
    process->state = state;
    process-> size = size;
    process-> pos = pos;
    process->queue_pos = queue_pos;
    process->completed = completed;
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