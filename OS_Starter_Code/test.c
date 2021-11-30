#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"
#include <stdlib.h>

int main(int argc, char * argv[])
{

    struct pcb *pb;
    pb = pu_create_pcb_10();
    
  
    struct process *proc1=pu_create_process(1,2,RUNNING, 3,4); /*int prog_id,int arrival_time,enum state curr_state, int runtime,int exec_time*/
    struct process *proc2=pu_create_process(2,22,RUNNING, 23,24);
    struct process *proc3=pu_create_process(3,32,RUNNING, 33,34);



    pu_add_process(proc1);
    pu_add_process(proc2);
    pu_add_process(proc3);

    printf("%i",pb->processes[1].exec_time);
    //update_size(20);
    printf("\n");
    pu_delete_process(2);
    printf("%i",pb->processes[1].exec_time);
    printf("\n");

    pu_update_process(3,WAITING, 5555,8888);
    pu_add_extra_exec_time(3,3);
    printf("%i",pu_get_process_at_pid(3)->exec_time);

    return 0;
}