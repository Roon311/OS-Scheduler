#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include "Data_Structures/pcb_linked_list.h"
#include "Data_Structures/priority_queue.h"
#include "output_files/output_utils.h"

int main(int argc, char * argv[])
{


    /*struct process p1=pu_create_process(1,0,7,0); //int identity,int arrival_time, int runtime, int priority
        p1.finish_time=10;
        add_new_finished_pros(p1);
    struct process p2=pu_create_process(2,3,90,0);
        p2.finish_time=130;
        add_new_finished_pros(p2);
    struct process p3=pu_create_process(1,5,15,0);
        p3.finish_time=30;
        add_new_finished_pros(p3);
    struct process p4=pu_create_process(1,4,7,0);
        p4.finish_time=300;
        add_new_finished_pros(p4);
    do_calculations();*/

    node_priority* pq = newNode(create_process(1,11,3,6), 6); //3
    push(&pq, 2,create_process(2,22,3,2));  //2
    push(&pq, 7,create_process(3,33,3,7));  //4
    push(&pq, 1,create_process(4,44,3,1)); //1

    while (!isEmpty(&pq))
    {
        printf("%d ", peek(&pq).arrival_time);
        pop(&pq);
    }
    return 0;
    /*insert_begin(pu_create_process(1,2,3,4));
    insert_begin(pu_create_process(2,2,3,4));
    insert_begin(pu_create_process(3,2,3,4));
    insert_begin(pu_create_process(4,2,3,4));
    insert_begin(pu_create_process(5,2,3,4));
    display();
    delete_end();
    display();*/

    /*struct pcb *pb;
    pb = pu_create_pcb_10();


    struct process *proc1=pu_create_process(1,2, 3,4);      //int prog_id,int arrival_time, int runtime, int priority)
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

    pu_update_process_at_identity(2,-1,WAITING, 5555,8888); //int prog_id,enum state curr_state,int exec_time,int finish_time
    pu_add_extra_exec_time_at_identity(2,3);
    printf("%i",pu_get_process_at_identity(2)->exec_time);*/

    return 0;
}