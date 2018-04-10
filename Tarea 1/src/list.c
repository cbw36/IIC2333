
#include <stdio.h>
#include <stdlib.h>
#include "list.h"


List* initList()
{
    List * list = malloc(sizeof(List));
    list->size = 10;
    list-> processes = malloc(list->size * sizeof(Process));
    list->num_processes = 0;
//    list->has_ready = has_ready;
    return list;

}

void list_appendProcess(List* list, Process* process)
{
    if (list->num_processes == list->size)
    {
        list->processes = realloc(list->processes, list->size*2* sizeof(Process));
        list->size *= 2;
    }
    int position = 0;
    if (list->num_processes == 0 || list->processes[list->num_processes - 1].arrival_time <= process->arrival_time) {
      list->processes[list->num_processes] = *process;
      if (list->num_processes == 0)
      {
        list->processes[list->num_processes].queue_pos = 0;
      }
      else
      {
        list->processes[list->num_processes].queue_pos = list->num_processes;
      }
      list->num_processes ++;
    }
    else{
    for (position = 0; position < list->num_processes; position++) {
      if (list->processes[position].arrival_time > process->arrival_time)
      {
        list_insert(list, process, position);
        break;
      }}
}}


void list_insert(List* list, Process* process, int position)
{
int a;
  for (a = list->num_processes-1; a>=position; a = a-1){
    list->processes[a].queue_pos ++;
    list->processes[a+1]=list->processes[a];
  }
  list->processes[position] = *process;
  list->processes[position].queue_pos = position;
  list->num_processes ++;
}

Process* listremoveProces(List* list,Process* process)
{
    int loc = process->queue_pos;
    for (int ind = loc; ind < list->num_processes -1; ind++)
    {
        list->processes[ind] = list->processes[ind + 1];
        list->processes[ind].queue_pos --;
    }
    list->num_processes --;
    return process;
}
