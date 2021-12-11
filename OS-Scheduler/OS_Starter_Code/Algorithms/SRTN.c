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
bool PauseFlag=false;    //for the pausing 
node_priority *ready_q = NULL; // Ready Queue
node_priority *done_q = NULL;  // Queue containing completed processes
process CuP;                   // current process si that we fork the processes
struct Queue_str *q_strs;
//------------------------------------------------------------------------------------------------------------------//


//----------------------------------------Functions' implementations------------------------------------------------//
/*Content
1-Receive Process
2-
*/

//********************************************ReceiveProcess*********************************************************//
// Receive the sent process
int get_remaining_time(process p)
{
    return p.remaining_time;
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
        ptbp->remaining_time=ptbp->run_time;//the remaining time at the very beginning is the run_time
        if (ready_q == NULL)
        {
            ready_q = newNode(*ptbp,ptbp->remaining_time);//create new node to hold the queue
        }
        else
        {
            push(&ready_q, ptbp->remaining_time, *ptbp);//push to the queue
        }
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
    if(CuP.identity==0)  //if there is no current process return and don't compare
    {
        return;
    }
    /*  We will now calculate the remaining time to make the comparison with the current process and the the process in 
        the ready queue waiting time = (current time - arrival time) -execution time...Remaining time= run_time - exec_time
    */
    CuP.remaining_time = CuP.run_time - (getClk() - (CuP.arrival_time +CuP.waiting_time));/*runtime-(current time-arrival-waiting)*/
    if (peek(&ready_q).remaining_time < CuP.remaining_time)//if the least remaining time in the ready queue is less than the remaining time of the executing process, 
    {
        if (kill(CuP.prog_id, SIGTSTP) == -1) // stop the executing process
        {
            perror("Stopping Failed");
        }
        CuP.finish_time=getClk();
        char new_str2[100]; // Runtime int-->string
        sprintf(new_str2, "At time %d process %d stopped arr %d total %d remain %d wait %f \n",
        getClk(), CuP.identity, CuP.arrival_time, CuP.run_time, 0, get_waiting_time(CuP));
        enQueue_str(q_strs, new_str2);
        printf("\n Process %d stopped at time %d \n\n",CuP.identity,getClk());
        CuP.last_stop=getClk();//saving the last stop to use it to update the waiting time
        printf("\nTest1\n\n");
        PauseFlag = false;   //Pause the main
        printf("\nTest2\n\n");
        push(&ready_q,CuP.remaining_time,CuP);
        CuP.curr_state=WAITING;//because it is stopped
        //printf("\npeeking\n\n");
        //CuP=peek(&ready_q);pop(&ready_q);
        //printf("\nCurrent Process is process %d\n\n",CuP.identity);
    }

}

void ChildHandler(int signum)
{
    if (waitpid(CuP.prog_id, NULL, WNOHANG)==0) //process did not return
    {
        return;
    }  
    else
    {
        //process terminated
        PauseFlag=false;
        CuP.curr_state=FINISHED;
        printf("\n --------Finished Process %d\n\n",CuP.identity);
        CuP.remaining_time=0;
        completed++;
        CuP.finish_time=getClk();
        char new_str2[100]; // Runtime int-->string
        sprintf(new_str2, "At time %d process %d finished arr %d total %d remain %d wait %f TA %f WTA %f\n",
        getClk(), CuP.identity, CuP.arrival_time, CuP.run_time, 0, get_waiting_time(CuP), get_turn_around_time(CuP), get_weighted_turn_around(CuP));
        enQueue_str(q_strs, new_str2);
        add_new_finished_pros(CuP);
    }  
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
            //printf("Error resuming process %d", CuP.identity);
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
    // printf("\nIF Queue is not null we are applying\n\n");
    // CP // current process working
    // our queue has 3 arriving at time 3 we will check the highest and get the highest to working s
    if (ready_q != NULL)//if the ready_q has elements
    {
        // printf("\nApplying \n\n");
        // printf("\n 5azan a7zan \n\n");


        CuP = peek(&ready_q); //get the element to the current processs
        // printf("\nPeek successful\n\n");

        // CP->identity=peek(&ready_q).identity;
        // printf("\n The current process identity is %d \n\n",CuP.identity);
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
       /* completed++;
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
        */
    }
}

//------------------------------------------------------------------------------------------------------------------//

//**********************************************Print Output********************************************************//
//------------------------------------------------------------------------------------------------------------------//

//*************************************************Main*************************************************************//

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

int main(int argc, char *argv[])
{
    //printf("\n This is the CuP ID %d\n\n",CuP.identity);
    t_count = atoi(argv[1]);
    q_strs = createQueue_str();
    signal(SIGUSR1, ReceiveHandler);//handler to when sending occurs so we can receive the process
    signal(SIGCHLD, ChildHandler); //handler when process stops 
    initClk();//initiate the clk
    while (true)
    {
        printf("clk is: %d seconds\n\n", getClk());
        // rec_many_process();
        ApplyingAlgo();//apply the algorithm
       
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
