#include "process.h"

#ifndef TAREA_1_LIST_H
#define TAREA_1_LIST_H

 struct list{
     struct process * processes;
     int size;
     int num_processes;
//     bool has_ready;
 };


typedef struct list List;

List* initList();
void list_appendProcess(List* list, Process* process);
void list_insert(List* list,Process* process, int position);
Process* listremoveProces(List* list, Process* process);
//bool hasReadyProcess(Queue* queue);


#endif //TAREA_1_QUEUE_H
