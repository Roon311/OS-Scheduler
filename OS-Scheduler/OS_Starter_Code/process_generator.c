/** @file process_generator.c
 *  @brief file to manage sendgin processes to message queue.
 *          it reads the file and fills pcb and then sends data to schedular.
 *  @author Ahmed Adel 
 *  @bug No known bugs.
 * 
*/
#include "Data_Structures/pcb_linked_list.h"
#include "headers.h"
/**
 * ALgorithms implemented.
 * */
typedef enum scheduling_algorithms_ {HPF, SRTN,RR } scheduling_algorithms;


///==============================
/*Implicit declerations*/
//  /==============================

/** @brief clear ipc, and destroys the clock
 *
 *  @param sig_num input from catching error
 *  @return Void.
 */
void clear_resources(int sig_num);


/** @brief managing segmentation fault
 *
 *  @param sig_num input from catching error
 *  @return Void.
 */
void clear_resources_seg_fault(int);  //To catch segmentation faults

/** @brief forks the clock 
 *  @return Void.
 */
void fork_clk();

/** @brief ask the used for their preferred algorithm
 *
 *  @return enum of chosed algorithm.
 */
scheduling_algorithms ask_for_alg();

/** @brief read input file
 *          and fill pcb
 *  @return Void.
 */
void read_input_file();

/** @brief start communication to schedular
 *          using msg queue
 *  @return Void.
 */
void init_comm_to_alg(scheduling_algorithms chosen_alg);

/** @brief fork schedular algorithm
 *  @param chosen_alg chosed algorithm, is to be used to determine which algoritm to fire.

 *  @return Void.
 */
int fork_schdular_alg(scheduling_algorithms chosen_alg);

/** @brief send process at which their arrival time is equal to current time.

 *  @return Void.
 */
void send_new_process_to_schedular(int cur_time,int FPID);
/**
 * global variable to connect to message quque
*/
int msgqid;
int t_count=0;



int main(int argc, char * argv[])
{
    //initClk();
    signal(SIGINT, clear_resources);
    signal(SIGSEGV, clear_resources_seg_fault);

    scheduling_algorithms chosen_alg =ask_for_alg();

    int AlgoPIID;
    read_input_file();
    init_comm_to_alg(chosen_alg);
    AlgoPIID=fork_schdular_alg(chosen_alg);
    fork_clk();
    initClk();
    while(true)
    {
        //printf("curr_clk: %d \n",getClk());

        send_new_process_to_schedular(getClk(),AlgoPIID);
        sleep(1);
    }

    destroyClk(true);
}

void clear_resources(int signum)
{
    destroyClk(true);
    msgctl(set_msg_q_id(), IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

void clear_resources_seg_fault(int signum)
{
    printf("SegFault \n");
    clear_resources(signum);
}

void fork_clk()
{
    int pid;
    pid= fork();
    if (pid ==0)
    {
        //Child
        char *args[]= {"./clk.out",NULL};
        execvp(args[0],args);

    }
}

scheduling_algorithms ask_for_alg()
{

    int answer;
    scheduling_algorithms chosed_alg;

    while(!(answer ==1 || answer ==2 | answer ==3))
    {
        printf("Please specifiy an algorithm for sceduling.\n 1: HPF\n 2: SRTN\n 3: RR \n\n");
        scanf("%i", &answer);

        if(answer ==1)
        {
            printf("Working with HPF, highest priority first \n ");
            return HPF;
        }

        if(answer ==2)
        {
            printf("Working with SRTN, Shortest remaing time next \n");
            return SRTN;
        }
        if(answer ==3)
        {
           
            return RR;
        }
    }


}
void read_input_file()
{
    static const char filename[] = "processes.txt";
    FILE * fPointer;
    fPointer = fopen(filename, "r");
    char singleLine[BUFSIZ];

    if(fPointer != NULL)
    {
        int ID, arrival, runtime, priority;
        if ( fgets(singleLine, sizeof singleLine, fPointer) != NULL )
        {
            //Just to get rid of the first line in the file.
        }

        while ( fgets(singleLine, sizeof singleLine, fPointer) != NULL )
        {
            if ( sscanf(singleLine, "%d", &ID) == 1 && sscanf(singleLine, "%*d\t%d", &arrival) == 1 && sscanf(singleLine, "%*d\t%*d\t%d", &runtime) == 1 && sscanf(singleLine, "%*d\t%*d\t%*d\t%d", &priority) == 1)
            {
                insert_end(create_process(ID,arrival,runtime,priority));
                t_count++;

            }
        }
    }
    else
    {
        perror(filename);
    }
    fclose(fPointer);


    printf("Reading all processes from input file into pcb: \n");
    printf("█ 25%% Done\n\n");
    printf("█ █  50%% Done\n\n");
    printf("█ █ █  75%% Done\n\n");
    printf("█ █ █ █  100%% Done\n\n");
    printf("ALl file has been read into memory let's execute them by your specified algorithm \n");
}


void init_comm_to_alg(scheduling_algorithms chosed_alg)
{

    //set_msg_q_id();
}

int fork_schdular_alg(scheduling_algorithms chosen_alg)
{
    //char *arg_ 
    if (chosen_alg == HPF)
    {
            int pid;
            pid= fork();
            if (pid ==0)
            {
                //Child
                //printf("\nforkied HPF sucessfully count: %d\n", t_count);
                char buffer[20]; // Runtime int-->string
                sprintf(buffer, "%d", t_count);
                char *args[] = {"./HPF.out", buffer, NULL};
                execvp(args[0],args);
                printf("\nforkied HPF sucessfully \n");
            }
            return pid;
    }
    if (chosen_alg == SRTN)
    {
            int pid;
            pid= fork();
            if (pid ==0)
            {
                 //Child
                //printf("\nforkied SRTN sucessfully \n");
                char buffer[20]; // Runtime int-->string
                sprintf(buffer, "%d", t_count);
                char *args[] = {"./SRTN.out", buffer, NULL};
                execvp(args[0],args);
                printf("\nforkied SRTN sucessfully \n");
            }
            return pid;
    }
    if (chosen_alg == RR)
    {
            int pid;
            pid= fork();
            if (pid ==0)
            {
                 //Child
                printf("\nforkied RR sucessfully \n");
                char buffer[20]; // Runtime int-->string
                sprintf(buffer, "%d", t_count);
                char *args[] = {"./RR.out", buffer,NULL};
                execvp(args[0],args);
                printf("\nforkied RR sucessfully \n");
            }
            return pid;
    }
  
}


