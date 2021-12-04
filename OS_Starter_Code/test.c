#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Data_Structures/pcb.h"
#include <stdlib.h>

int main(int argc, char * argv[])
{

    struct pcb *pb;
    pb = pu_create_pcb_10();
    
  
    struct process *proc1=pu_create_process(1,2, 3,4);      /*int prog_id,int arrival_time, int runtime, int priority)*/
    struct process *proc2=pu_create_process(2,22, 23,24);
    struct process *proc3=pu_create_process(3,32, 33,34);



    pu_add_process(proc1);
    pu_add_process(proc2);
    pu_add_process(proc3);

    printf("%i",pb->processes[1].arrival_time);
    printf("\n");

    pu_delete_process_at_identity(1);
    printf("%i",pb->processes[1].arrival_time);
    printf("\n");

    pu_update_process_at_identity(2,-1,WAITING, 5555,8888); /*int prog_id,enum state curr_state,int exec_time,int finish_time*/
    pu_add_extra_exec_time_at_identity(2,3);
    printf("%i",pu_get_process_at_identity(2)->exec_time);

    return 0;
}