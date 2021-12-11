//-----------------------------------------------Includes----------------------------------------------------------//
#include "../headers.h"
#include "../Data_Structures/priority_queue.h"
#include "../output_files/output_utils.h"
#include "../output_files/queue_strings.h"
//-----------------------------------------------Variables----------------------------------------------------------//

/////+++++++ msg queue id is in header int msgqupid=152001;
int completed = 0;    // number of completed processes
int current_time = 0; // will be setted with the clock
int quanta=0;
int tracer=0;
int t_count;
bool PauseFlag=false;    //for the pausing 
node_priority *ready_q = NULL; // Ready Queue
node_priority *done_q = NULL;  // Queue containing completed processes
process CuP;                   // current process si that we fork the processes
struct Queue_str *q_strs;
//------------------------------------------------------------------------------------------------------------------//

//----------------------------------------Functions' implementations------------------------------------------------//
/*Content
1-Receive Process
2-Handlers
3-Execute_Process
4-Applying Algo
5-Print Output
6-Main
*/
//********************************************ReceiveProcess*********************************************************//
// Receive the sent process
int get_remaining_time(process p)
{
    return (p.run_time - p.exec_time);
}

int recieve_single_process()
{
    msg_buff message; // create a message
    int rec_val;      // int for the receive value "-1" means nothing received
    // TODO delete that after debugging
    printf("\n Receiving...\n\n");
    rec_val = msgrcv(set_msg_q_id(), &message, sizeof(message.p), 0, IPC_NOWAIT); // receive a message No waiting if a message is not found
    if (rec_val == -1)                                                            // if no process is received
    {
        perror("Error in receiving"); // for debugging, print error in receiving
        return 0;
    }
    else // a process is received
    {
        // printf("\nProcess received \n\n");                  // for debugging, print that the process is received
        process *ptbp = (process *)malloc(sizeof(process)); // ptbp process to be pushed
        while (!ptbp)
        {
            perror("Allocation failed");
            ptbp = (process *)malloc(sizeof(process));
        }
        *ptbp = message.p; 
        printf("Process %d received at time: %d \n", ptbp->identity, getClk());
        ptbp->curr_state = WAITING;
        if (ready_q == NULL)
        {
            ready_q = newNode(*ptbp, ptbp->priority);
        }
        else
        {
            push(&ready_q, ptbp->priority, *ptbp);
        }
        return 1;
    }
}

void rec_many_process()
{
    while (recieve_single_process());//if a process is received perform receiving one more time to check if anything more is received at same time
}

//***********************************************Handlers************************************************************//
void ReceiveHandler(int signum)
{
    rec_many_process();
}
void ChildHandler(int signum)
{
}
//------------------------------------------------------------------------------------------------------------------//

//*********************************************Execute_Process******************************************************//
void Execute_Process()
{

    printf("\n In execute\n\n");
    int PID;
    if(CuP.remaining_time==CuP.run_time)// process was not forked before
    {
        char new_str1[100]; // Runtime int-->string
        sprintf(new_str1, "At time %d process %d started arr %d total %d remain %d wait %d\n",
        getClk(), CuP.identity, CuP.arrival_time, CuP.run_time, CuP.run_time, getClk() - CuP.arrival_time);
        enQueue_str(q_strs, new_str1);

        PID = fork();
        while (PID == -1)
        {
            perror("Forking Failed!");
            printf("Retrying :!\n");
            PID = fork();
        }
        CuP.prog_id = PID;
        if (CuP.prog_id == 0)
        {
        // printf("In Child: %d \n", getClk());
            char buffer[15]; // Runtime int-->string
            sprintf(buffer, "%d", CuP.run_time);
            char *argv[] = {"process.out", buffer, NULL};
            execv("process.out", argv);
            perror("Execution Failed!");
            printf("Process running \n");
            printf("Out of child = %d\n", getClk());
            exit(EXIT_SUCCESS);
        }
        CuP.waiting_time = getClk() - CuP.arrival_time;
        CuP.curr_state = START;
        printf("Process %d is now executing\n", CuP.identity);
    }
    else
    {
        char new_str2[100]; // Runtime int-->string
        sprintf(new_str2, "At time %d process %d resumed arr %d total %d remain %d wait %d\n",
        getClk(), CuP.identity, CuP.arrival_time, CuP.run_time, CuP.run_time, getClk() - CuP.arrival_time);
        enQueue_str(q_strs, new_str2);

         if (kill(CuP.prog_id, SIGCONT) == -1) // stop the executing process
        {
            perror(NULL);
            return;
        }
        else
        {
            CuP.waiting_time = CuP.waiting_time+(getClk() - CuP.last_stop);  //increment waiting time
            CuP.curr_state=RUNNING;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------//

//**********************************************Applying_Algo*******************************************************//
void ApplyingAlgo()
{
    if (ready_q != NULL)//if the ready_q has elements
    {
        
        CuP = peek(&ready_q); 
        CuP.curr_state = START;// and set its state to start
        pop(&ready_q); //and pop it out of the ready_q
        printf("\n----------Starting Process%d , start time is %d \n\n", CuP.identity, getClk());
        Execute_Process(); // process is now executing when it is over it will break
        printf("\nThis is tha pause state2  %hi \n\n",PauseFlag);
        sleep(1);
        PauseFlag=true;
        while(PauseFlag==true)
        {
            printf("\n Pausing\n\n");
            printf("\nThis is tha pause state3  %hi at time %d\n\n",PauseFlag,getClk());
            pause();
        }// out of pause if a process with smaller remaining time arrives or if the process ends and a new one needs to work
        printf("\nThis is tha pause state4  %hi at time %d\n\n",PauseFlag,getClk());
        printf("\n RESUMING! \n\n");
    }
}

//------------------------------------------------------------------------------------------------------------------//
int main(int argc, char *argv[])
{
    initClk;
    quanta = atoi(argv[2]); //get quanta from command line argument and store it   
    printf("\n This is the quanta  %d\n\n",quanta);
}