//#include "headers.h"
#include "pcb.h"

enum scheduling_algorithms {HPF, SRTN,RR };

struct msgbuff
{
    long mtype;
    struct process *p;
};

struct pcb *global_pcb;


void clearResources(int);
void clearResources_1(int);

void fork_clk();

enum scheduling_algorithms ask_for_alg();

void read_input_file();

void init_comm_to_alg(enum scheduling_algorithms chosen_alg);

void init_schdular_alg(enum scheduling_algorithms chosen_alg);

void send_new_process_to_schedular();  

int g_q_key;
int msgqid;

int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    signal(SIGSEGV, clearResources_1);

    enum scheduling_algorithms chosen_alg =ask_for_alg();

    global_pcb = pu_create_pcb_10();

    read_input_file();

    init_comm_to_alg(chosen_alg);
    init_schdular_alg(chosen_alg);
    fork_clk();
    initClk();
    while(true)
    {
        //printf("%d \n",getClk());
        send_new_process_to_schedular(); 
        sleep(1);       
    }

    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    

   

    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
    destroyClk(true);
    
}
void clearResources_1(int signum)
{
    //TODO Clears all resources in case of interruption
    printf("SegFault \n");
    clearResources(signum);
}

void fork_clk()
{
    int pid;
    pid= fork();
    if (pid ==0)
    { //Child
        char *args[]={"./clk.out",NULL};
        execvp(args[0],args);

    }
}

enum scheduling_algorithms ask_for_alg()
{
    
    int answer;
    enum  scheduling_algorithms chosed_alg;

    while(!(answer ==1 || answer ==2 | answer ==3))
    {
        printf("Please specifiy an algorithm for sceduling. 1: HPF, 2: SRTN, 3: RR \n");
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
            printf("Working with RR, Round robin \n");   
            return RR;
        }
    }
    

}

void read_input_file()
{
    printf("Reading all processes from input file into pcb: \n");
    printf("█ 25%% Done\n\n");
    printf("█ █  50%% Done\n\n");
    printf("█ █ █  75%% Done\n\n");
    printf("█ █ █ █  100%% Done\n\n");
    printf("ALl file has been read into memory let's execute them by your specified algorithm \n");
    // TODO sakke should be replacing it.
    pu_add_process(pu_create_process(1,1,10, 8));
    pu_add_process(pu_create_process(2,2,20, 1));
    pu_add_process(pu_create_process(3,2,30, 7));
    pu_add_process(pu_create_process(4,6,40, 23));
    pu_add_process(pu_create_process(5,6,50, 4));
    pu_add_process(pu_create_process(6,8,60, 4));

}


void init_comm_to_alg(enum scheduling_algorithms chosed_alg)
{
    /*
        We will use message queue
        one message queue for all algorithms, and only one will use it.
    */
    //int msgqid=msgget(g_q_key, IPC_CREAT);

}

void init_schdular_alg(enum scheduling_algorithms chosen_alg)
{
    /*
        shall be forking and execvp
    */

}




void send_new_process_to_schedular()
{
    printf("Checking if there is any new processes at time");
    printf("%d\n", getClk());
    int cur_time=getClk();
    struct pcb *tmp_pcb;
    int *actually_held;
    tmp_pcb = pu_get_processes_at_arvl_time(getClk(), actually_held);
    if((*actually_held) >0)
    {
        printf("There are new proceses, i will send them now.\n");
        printf("count: %d \n", *actually_held);      
        for (int i = 0; i< *actually_held; i++)
        {   
            printf("MAIN %d, ",tmp_pcb->processes[i].prog_id);
            struct msgbuff msg;
            msg.p=&(tmp_pcb->processes[i]);
            //int send_val=msgsnd(msgqid, &msg, sizeof(msg.p),!IPC_NOWAIT); 
            //printf("index \n");
            //struct process *cur_proc;
            //cur_proc->exec_time=1148;

            //cur_proc=malloc(sizeof *cur_proc);
            //cur_proc=(tmp_pcb->processes[i]);
            //cur_proc =malloc(sizeof *cur_proc);
            //=tmp_pcb->processes[i];
            
            
            //printf("MAIN %d, ", cur_proc->exec_time);
            //printf("%d, ",cur_proc.arrival_time);
            //printf("%d, ",cur_proc.priority);
            //msg.p= malloc(sizeof msg.p);
            
        }
    }
        
}