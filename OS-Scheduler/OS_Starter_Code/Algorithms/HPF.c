//-----------------------------------------------Includes----------------------------------------------------------//
#include "../headers.h"
#include "../Data_Structures/priority_queue.h"
#include "../output_files/output_utils.h"
#include "../output_files/queue_strings.h"
//-----------------------------------------------Variables----------------------------------------------------------//

/////+++++++ msg queue id is in header int msgqupid=152001;
int completed = 0;    // number of completed processes
int current_time = 0; // will be setted with the clock
int t_count;
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
    return p.run_time - p.exec_time;
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
        *ptbp = message.p; // store the process received in the allocated space by dereferencing and equating
        // TODO delete that line
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

        // HeapPush(readyQ, ptbp->mRemainTime, ptbp); //push the  pointer to readyQ and sort by RemainingTime"Priority"
        // printf("\n\n\n\n To HPF from PG: %d, %d, %d\n\n\n\n ", ptbp->identity, ptbp->exec_time, ptbp->arrival_time);
        return 1;
    }
}

void rec_many_process()
{
    // printf("in many \n");
    while (recieve_single_process());
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

    char new_str1[100]; // Runtime int-->string
    sprintf(new_str1, "At time %d process %d started arr %d total %d remain %d wait %d\n",
            getClk(), CuP.identity, CuP.arrival_time, CuP.run_time, CuP.run_time, getClk() - CuP.arrival_time);
    enQueue_str(q_strs, new_str1);

    pause();
    CuP.finish_time = getClk();
    char new_str2[100]; // Runtime int-->string
    sprintf(new_str2, "At time %d process %d finished arr %d total %d remain %d wait %f TA %f WTA %f\n",
    getClk(), CuP.identity, CuP.arrival_time, CuP.run_time, 0, get_waiting_time(CuP), get_turn_around_time(CuP), get_weighted_turn_around(CuP));
    enQueue_str(q_strs, new_str2);

    printf("Executing Parent\n");
    CuP.finish_time = getClk();
    add_new_finished_pros(CuP);
    // pause();
}
//------------------------------------------------------------------------------------------------------------------//

//**********************************************Applying_Algo*******************************************************//
void ApplyingAlgo()
{
    // printf("\nIF Queue is not null we are applying\n\n");
    // CP // current process working
    // our queue has 3 arriving at time 3 we will check the highest and get the highest to working s
    if (ready_q != NULL)
    {
        // printf("\nApplying \n\n");
        // printf("\n 5azan a7zan \n\n");
        CuP = peek(&ready_q);
        // printf("\nPeek successful\n\n");

        // CP->identity=peek(&ready_q).identity;
        // printf("\n The current process identity is %d \n\n",CuP.identity);
        CuP.curr_state = START;
        pop(&ready_q); // curently working on the one with highest priority
        printf("\n----------Starting Process%d , start time is %d \n\n", CuP.identity, getClk());
        Execute_Process(); // process is now executing when it is over it will break
        printf("\n-----------------Process%d is Done, exit time is %d \n\n", CuP.identity, getClk());
        completed++;
        CuP.curr_state = FINISHED;
        if (done_q == NULL)
        {
            done_q = newNode(CuP, completed); // 3
            // printf("\n-----------------Process%d is Done, exit time is %d \n\n",peek(&done_q).identity,getClk());
        }
        else
        {
            push(&done_q, completed, CuP);
            // printf("\n-----------------Process%d is Done, exit time is %d \n\n",CuP.identity,getClk());
        }
    }
}
//------------------------------------------------------------------------------------------------------------------//

//**********************************************Print Output********************************************************//
void print_output()
{
    FILE *pFile;
    pFile = fopen("scheduler.log", "w");
    char *var = deQueue_str(q_strs);
    while (var != "")
    {
        fprintf(pFile, var);
        var = deQueue_str(q_strs);
    }
    fclose(pFile);
}
//------------------------------------------------------------------------------------------------------------------//

//*************************************************Main*************************************************************//
int main(int argc, char *argv[])
{
    t_count = atoi(argv[1]);
    q_strs = createQueue_str();
    signal(SIGUSR1, ReceiveHandler);
    signal(SIGCHLD, ChildHandler);
    initClk();
    while (true)
    {
        printf("clk is: %d seconds\n\n", getClk());
        // rec_many_process();
        ApplyingAlgo();
        if (completed == t_count)
        {
            do_calculations();
            print_output();
            printf("\n Scheduling and Executing Done!!!\n\n");
            // To Do:call the log function

            destroyClk(true);
            // To Do:clear the resources
            exit(getpid());
        }
        sleep(1);
    }
}
//------------------------------------------------------------------------------------------------------------------//