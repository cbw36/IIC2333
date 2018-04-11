
//0 = READY
//1 = RUNNING
//2 = FINISHED


#ifndef TAREA_1_PROCESS_H
#define TAREA_1_PROCESS_H

struct process{
    int pid;
//    char* name[256];
    int state;
    int priority;
    int* subprocess;
    int size;
    int pos; //
    int queue_pos;  //location in queue its stored in
    int completed;  //number of subprocesses completed (also the index of current subprocess)
    int quantum_remaining;
    int response_bool;
    int arrival_time;
    int cpu_turns;
    int blocked;
    int turnaround_time;
    int response_time;
    int waiting_time;
};

typedef struct process Process;


//Process* initProcess(int pid, char name[256], int state, int completed,
//                     int size, int pos, int queue_pos);
Process* initProcess(int pid, int size, int arrival_time);
void changeState(Process* process, int new_state);
int getState(Process* process);
void subprocessAppend(Process* process, int val);

#endif //TAREA_1_PROCESS_H
