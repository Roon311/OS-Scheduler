#include "headers.h"
#include "process_generator.h"
#include "pcb.h"



node_priority* head;
scheduling_algorithms chosen_alg;
int quanta;
int main(int argc, char * argv[])
{
    /*++++++++++++++++++++DECLarations++++++++++++++++++++*/
    head=NULL;
    quanta=0;
    /*++++++++++++++++++++Signal handlers+++++++++++++++++*/
    signal(SIGINT, _clear_resources);
    signal(SIGSEGV, _clear_resources_seg_fault);
    /*++++++++++++++++++++Chosen algorithm +++++++++++++++*/
    chosen_alg =ask_for_alg();
    /*+++++++++++++++++++Read input files++++++++++++++++*/
    _read_input_file();
    /*++++++++++++++++++++Forking clock++++++++++++++++++*/
    _fork_clk();
    initClk();
    /*++++++++++++++++++++Forking schedular++++++++++++++++++*/
    schedular_prog_id= _fork_schedular();
    /*++++++++++++++++++++Sending process to schedular ++++++++++++++++++++*/
    while(true)
    {
        //printf("Current clock: %d in seconds\n",getClk());
        send_new_prs_to_sch();
        sleep(1);
    }

    destroyClk(true);
}


scheduling_algorithms ask_for_alg()
{

    int answer;
    scheduling_algorithms chosed_alg;
    answer = -1;
    while(!(answer ==1 || answer ==2 | answer ==3))
    {
        printf("Please specifiy an algorithm for sceduling. \n 1: HPF \n 2: SRTN\n 3: RR \n");
        scanf("%i", &answer);

        switch (answer)
        {
        case 1:
            printf("Working with HPF, highest priority first \n ");
            return HPF;
            break;

         case 2:
            printf("Working with SRTN, Shortest remaing time next \n");
            return SRTN;
            break;
        case 3:
            printf("Please specity quanta for RR!! \n");
            scanf("%i", &quanta);
            printf("Working with RR, Round robin with quanta equal to %d\n", quanta);
            return RR;
            break;
        
        default:
            continue;
        }
       
    }


}

void _read_input_file()
{
    static const char filename[] = "processes.txt";
    FILE * fPointer;
    fPointer = fopen(filename, "r");
    _read_each_row(fPointer);
    fclose(fPointer);

}

void _read_each_row(FILE * fPointer)
{
    char singleLine[BUFSIZ];
    int ID, arrival, runtime, priority;
    _skip_first_line(&fPointer,singleLine);//To skip first line

    while (fgets(singleLine, sizeof singleLine, fPointer) != NULL)
    {
        if ( sscanf(singleLine, "%d", &ID) == 1 
            && sscanf(singleLine, "%*d\t%d", &arrival) == 1 
            && sscanf(singleLine, "%*d\t%*d\t%d", &runtime) == 1 
            && sscanf(singleLine, "%*d\t%*d\t%*d\t%d", &priority) == 1
            )
        {
            push_to_pcb(&head,create_process(ID,arrival,runtime,priority), arrival);
            _total_count++;

        }
    }
}

void _skip_first_line(FILE **fPointer,char *singleLine)
{
    fgets(singleLine, sizeof singleLine, *fPointer) != NULL;
} 

void send_new_prs_to_sch()
{
   while(!is_empty(&head))
   {
       if(is_top_arvl_tm_now(&head,getClk()))
       {    
            process tmp_to_send=peek(&head);
            snd_prs_to_sch(tmp_to_send);
            pop(&head);

       }
       else
       {
           is_top_arvl_tm_now(&head,getClk());
           break; 
       }

   }
}
void snd_prs_to_sch(process prs)
{
    msg_buff msg;  
    msg.mtype=10;
    msg.prs=prs;
    int send_val=msgsnd(get_msg_q_id(), &msg, sizeof(msg.prs), IPC_NOWAIT);
    if(send_val == -1)//if no process is received
    {
        perror("Error in receiving"); 
        return;
    } 
    else
    {
        kill(schedular_prog_id,SIGUSR1);
    }
}




void _clear_resources(int signum)
{
    destroyClk(true);
    msgctl(get_msg_q_id(), IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

void _clear_resources_seg_fault(int signum)
{
    printf("SegFault \n");
    _clear_resources(signum);
}

void _fork_clk()
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


int _fork_schedular()
{
    int pid;
    pid= fork();
    if (pid ==0)
    {
        //Child
        char buffer_c[20]; 
        sprintf(buffer_c, "%d", _total_count);
        char buffer_alg[20]; 
        sprintf(buffer_alg, "%d", get_algo_num());
        char buffer_quanta[20]; 
        sprintf(buffer_quanta, "%d", quanta);
        char *args[] = {"./scheduler.out",buffer_c, buffer_alg,buffer_quanta, NULL};
        execvp(args[0],args);
    }
    else
    {
        return pid;
    }
}

int get_algo_num()
{
    if(chosen_alg ==HPF)
        return 0;
    if(chosen_alg ==SRTN)
        return 1;
    if(chosen_alg ==RR)
        return 2;


}


