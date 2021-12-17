#ifndef headers_H
#define headers_H

#include <stdio.h>      //if you don't use scanf/printf change this include
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef short bool;
#define true 1
#define false 0

#define SHKEY 300


///==============================
//don't mess with this variable//
int * shmaddr;                 //
//===============================

typedef enum scheduling_algorithms_ {HPF, SRTN, RR } scheduling_algorithms;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Process related data fields and functions+++++++++++++++++++++++++++++++++++++++++*/

typedef enum state_ {RUNNING, WAITING, START, FINISHED,OFF} state;

typedef struct process_
{
    /* representation for process in memory */
    int         identity;       /*As prog_id is determined by schedular, there must be a datafield where it's determined before sending to schedular*/
    int         prog_id;        /*process id id, you can access it using getpid()*/
    int         arrival_time;   /*Arrival time at which process arrived*/
    state       curr_state;     /*state of process*/
    int         run_time;       /*Time process that should be spent on CPU, source: file*/
    int         exec_time;      /*Exection time. time already spent on CPU(Time elapsed)*/
    int         finish_time;    /*time at which algorithm finish*/
    int         priority;       /*Priority used mainly in HPF algorithm*/
    /*
        The waiting time and remaining time should are to be caluculed in fuctions
        Remaining time= run_time - exec_time
        waiting time = (current time - arrival time) -execution time
    */
} process;


process create_process(int identity,int arrival_time, int runtime, int priority)
{
    process *tmp_p;
    tmp_p=(process*)malloc(sizeof *tmp_p); /*Intializing the variable*/

    tmp_p->identity=identity;
    tmp_p->prog_id=-1;
    tmp_p->arrival_time=arrival_time;
    tmp_p->curr_state=OFF;  /*As intially was waiting not running*/
    tmp_p->run_time=runtime;
    tmp_p->exec_time=0;         /*As it didn't run at all*/
    tmp_p->finish_time=-1;      /*should be edited later to represent exact finish time.*/
    tmp_p->priority=priority;
    return *tmp_p;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Message queues +++++++++++++++++++++++++++++++++++++++++*/

#define MSG_Q_ID 152001
int msg_queue_id;
/*
    message buffer used in messege queues while sending data between processes
    it included type, and process while includes all necessary data
*/
typedef struct msg_buff_
{
    long mtype;
    process prs;
}msg_buff;


/** @brief create messeage queue for certain key
 *          if already created, it return the id
 *  @return id of msg queue.
 * 
 */
int get_msg_q_id()
{
    msg_queue_id=msgget(MSG_Q_ID, IPC_CREAT | 0644) ;
     if(msg_queue_id == -1)// Exception Handling, if the ID was already used
    {
        perror("Error in creating up queue");
        exit(-1);//exit the program
    }
    else
    {
        
        return msg_queue_id;
    }
        
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Clock+++++++++++++++++++++++++++++++++++++++++*/
int getClk()
{
    return *shmaddr;
}


/*
 * All process call this function at the beginning to establish communication between them and the clock module.
 * Again, remember that the clock is only emulation!
*/
void initClk()
{
    int shmid = shmget(SHKEY, 4, 0444);
    while ((int)shmid == -1)
    {
        //Make sure that the clock exists
        //printf("Wait! The clock not initialized yet!\n");
        sleep(1);
        shmid = shmget(SHKEY, 4, 0444);
    }
    shmaddr = (int *) shmat(shmid, (void *)0, 0);
}


/*
 * All process call this function at the end to release the communication
 * resources between them and the clock module.
 * Again, Remember that the clock is only emulation!
 * Input: terminateAll: a flag to indicate whether that this is the end of simulation.
 *                      It terminates the whole system and releases resources.
*/

void destroyClk(bool terminateAll)
{
    shmdt(shmaddr);
    if (terminateAll)
    {
        killpg(getpgrp(), SIGINT);
    }
}

#endif
