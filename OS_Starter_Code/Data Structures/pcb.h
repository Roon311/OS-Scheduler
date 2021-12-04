#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
//pu: public 
//pr: private
enum state {RUNNING, WAITING, BLOCKED}; /*to express the state of process*/

struct process
{
    /* representation for process in memory */
    int         prog_id;        /*process id id, you can access it using getpid()*/
    int         arrival_time;   /*Arrival time at which process arrived*/
    enum state  curr_state;     /*state of process*/
    int         rum_time;       /*Time process that should be spent on CPU, source: file*/
    int         exec_time;      /*Exection time. time already spent on CPU*/
    /*
        The waiting time and remaining time should are to be caluculed in fuctions
        Remaining time= run_time - exec_time
        wainting time = (current time - arrival time) -execution time
    */
};

struct pcb
{
    /*This struct is to represent PCB, where it will look like a dataframe in python*/
    /*Why I choose list?
        1- first in each iteration i will need to loop more(Fast access ) more times than appending as it shouldn't be frequent that many programs come each minut
        2- appending to pcd will be at the end, so no copying will happen frequently*/

   struct process *processes;

};

struct pcb *g_pcb;      /*Global variable  for pcb*/
int g_n_elemenets;      /*number of processes pcb can hold*/
int g_index;            /*index at whict next elemet to be inserted should be intialized first to zero*/

struct process* pu_create_process(int prog_id,int arrival_time,enum state curr_state, int runtime,int exec_time)
{
    struct process *tmp_p;
    tmp_p=malloc(sizeof *tmp_p); /*Intializing the variable*/

    tmp_p->prog_id=prog_id;
    tmp_p->arrival_time=arrival_time;
    tmp_p->curr_state=curr_state;
    tmp_p->rum_time=runtime;
    tmp_p->exec_time=exec_time;

}



struct pcb* pu_create_pcb(int n_elements)
{
    /*intialize pcb to store processes in it.*/
    g_n_elemenets=n_elements;    /*number of processes pcb can hold*/
    //Intializing pcb
    g_pcb = malloc(sizeof *g_pcb);
    //Intializing process list
    g_pcb->processes =(struct process*)malloc(n_elements*sizeof(struct process));
    // as it's first create index =0
    g_index=0;
    return g_pcb;
}

struct pcb* pu_create_pcb_10()
{
    /*intialize pcb to store processes in it with size =10.*/
    return pu_create_pcb(10);
}

struct pcb* pu_create_pcb_20()
{
    /*intialize pcb to store processes in it with size =20.*/
    return pu_create_pcb(20);
}

struct pcb* pu_create_pcb_50()
{
    /*intialize pcb to store processes in it with size =20.*/
    return pu_create_pcb(50);
}

struct pcb* pu_create_pcb_100()
{
    /*intialize pcb to store processes in it with size =20.*/
    return pu_create_pcb(100);
}

int pu_get_cur_size()
{
    return g_n_elemenets;
}
void pu_update_size(int new_size)
{
    /* it  update the size of pcb to new_size
        new_size should be larger than old one, otherwise data will be truncated
        if new_size is larger, it will copy old data and resize it.
    */
   
    g_pcb = realloc(g_pcb, new_size);
    g_n_elemenets=new_size;
}

void pu_add_process(struct process *p)
{
    /*
        Add new process to PCB, is size if filled it resized the pcb 
        to be equal new size= old size + 10    
        if you want to resize to double or anything else, you should call update_size(int new_size) first before calling add_process()
    */
    if (g_index ==g_n_elemenets)
    {
        pu_update_size(g_n_elemenets+10);
    }
    g_pcb->processes[g_index]=*p;
    g_index= g_index +1;

}


int pu_find_index_at_pid(int pid)
{
    /*
        It finds index at given program id
    */
    for (int i =0; i< g_index; i++)
    {
        //Looping all over the processes
        if(g_pcb->processes[i].prog_id == pid)
        {
            return i;
        }
    }
}

void pu_delete_process(int pid)
{

    pr_delete_at_index(pu_find_index_at_pid(pid));

}

void pr_delete_at_index(int i_to_delete)
{
    /*
        You shouldn't call it considered private
    */
    for(int i =i_to_delete; i< g_n_elemenets-1; i++)
    {
        g_pcb->processes[i] = g_pcb->processes[i+1];
    }
    
}

void pu_change_state_for_pid(int pid, enum state cur_state)
{
    /*
        It changes state for cetain process id
    */
    int i= pu_find_index_at_pid(pid);
    g_pcb->processes[i].curr_state=cur_state;
}

void pu_update_state_to_wainting(int pid)
{   
    /*
        Quickly update state of certain pid to waiting
    */
    pu_change_state_for_pid(pid, WAITING);
}

void pu_update_state_to_running(int pid)
{
    /*
        Quickly update state of certain pid to running
    */
    pu_change_state_for_pid(pid, RUNNING);
}

struct process*  pu_get_process_at_pid(int pid)
{
    /*
        it return a reference to process by searching at that pid.
    */
    int i=pu_find_index_at_pid(pid);
    return &(g_pcb->processes[i]);

}

void pu_update_process (int prog_id,enum state curr_state, int runtime,int exec_time)
{
    /*
        update value of process at prog_id
        allwoed to update
            1-curr_stat
            2-runtime
            3-exec_time
    */
    struct process *tmp_p=pu_get_process_at_pid(prog_id);

    tmp_p->curr_state=curr_state;
    tmp_p->rum_time=runtime;
    tmp_p->exec_time=exec_time;

}

int pu_get_remaining_time(int pid)
{
    /*
        it calculates the remaining time to process
    */
    struct process *tmp_p=pu_get_process_at_pid(pid);
    //Remaining time= run_time - exec_time
    return (tmp_p->rum_time-tmp_p->exec_time);

}

int pu_get_waiting_time(int pid)
{
     /*
        it calculates the waiting time to process
    */
    struct process *tmp_p=pu_get_process_at_pid(pid);
    //wainting time = (current time - arrival time) -execution time
    return (100- tmp_p->arrival_time) -tmp_p->exec_time;

}

void pu_add_extra_exec_time(int pid, int extra)
{
    // if new extra time for process is spent on CPU, that extra should be passed to be added    
    struct process *tmp_p=pu_get_process_at_pid(pid);
    tmp_p->exec_time=tmp_p->exec_time+extra;
}

void pu_update_exec_time(int pid, int new_exec_time)
{
    // if you calc the new exec time on your own, jsut pass it and it will be updated
    struct process *tmp_p=pu_get_process_at_pid(pid);
    tmp_p->exec_time=new_exec_time;
}

