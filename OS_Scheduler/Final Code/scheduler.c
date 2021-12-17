#include "headers.h"
#include "scheduler.h"
#include "data_structures/priority_queue.h"
#include "data_structures/circular_queue.h"


node_priority* ready_priority_q;
process prs_to_run;
process prs_currently_running;
int quanta;
int rem_quanta;
FILE * pFile;
int main(int argc, char * argv[])
{
    
    pFile = fopen("debugging.txt", "w");   

    initClk();
    /*+++++++++++++++++++++++++ recieving some values from process generator+++++++++++++++++++++++++++*/
    total_count_prss= atoi(argv[1]);
    alg_num = atoi(argv[2]);
    quanta= atoi(argv[3]);
    set_alg_enum(alg_num);
    /*++++++++++++++++++++++Intializations+++++++++++++++++*/
    prss_completed=0;
    ready_priority_q=NULL;
    /*++++++++++++++++++++++++++siganls handlers+++++++++++++++++++++++++*/
    signal(SIGUSR1, rec_handler);
    signal(SIGINT, interrupt_handler);
    /*+++++++++++++++++++++++apply specified algorith++++++++++++++++++*/
    if(chosen_alg ==HPF)
        apply_hpf();
    if(chosen_alg ==SRTN)
        apply_srtn();
    if(chosen_alg ==RR)
        apply_rr();

    destroyClk(true);
}
void  interrupt_handler(int signum)
{
    fclose(pFile);
    exit(EXIT_FAILURE);
}

void set_alg_enum(int alg_num)
{
    if (alg_num==0)
        chosen_alg=HPF;
    if (alg_num==1)
        chosen_alg=SRTN;
    if (alg_num==2)
        chosen_alg=RR;
}

void rec_handler(int signum)
{
    process tmp_p=rec_one_procss();
    while(tmp_p.arrival_time != -1)
    {   
        if(chosen_alg ==HPF)
            insert_prs_to_hpf(tmp_p);
        if(chosen_alg ==SRTN)
            insert_prs_to_srtn(tmp_p);
        if(chosen_alg ==RR)
            insert_prs_to_rr(tmp_p);

        tmp_p=rec_one_procss();
    }

}
void insert_prs_to_hpf(process prs)
{
    printf("Process %d received at time: %d \n", prs.identity, getClk());
    fprintf(pFile, "Process %d received at time: %d \n", prs.identity, getClk());
    push_to_pri_q(&ready_priority_q, prs, prs.priority);
}

void insert_prs_to_srtn(process prs)
{
    printf("Process %d received at time: %d , rem time is %d\n", prs.identity, getClk(), get_remaining_time(prs));
    fprintf(pFile, "Process %d received at time: %d , rem time is %d\n", prs.identity, getClk(), get_remaining_time(prs));
    push_to_pri_q(&ready_priority_q, prs, get_remaining_time(prs));

}

void insert_prs_to_rr(process prs)
{
    printf("Process %d received at time: %d , rem time is %d\n", prs.identity, getClk(), get_remaining_time(prs));
    fprintf(pFile, "Process %d received at time: %d , rem time is %d\n", prs.identity, getClk(), get_remaining_time(prs));
    cq_enqueue(prs);
}
int get_remaining_time(process prs)
{
    return prs.run_time-prs.exec_time;
}
process rec_one_procss()
{
    msg_buff message; // create a message
    int rec_val; 
    rec_val = msgrcv(get_msg_q_id(), &message, sizeof(message.prs), 0, IPC_NOWAIT); // receive a message No waiting if a message is not found
    if (rec_val == -1)                                                            // if no process is received
    {
        process s={.arrival_time=-1}; //Indication that it's an error
        return s;
    }
    else
    {
        process *ptbp = (process *)malloc(sizeof(process)); 
        *ptbp = message.prs; 
        ptbp->curr_state = WAITING;
        return *ptbp;
    }

}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++HPF++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void apply_hpf()
{
    while(true)
    {
        if(!is_empty(&ready_priority_q))
        {
            prs_to_run=peek(&ready_priority_q);
            execute_process_hpf(prs_to_run);
            prs_finished(prs_to_run);
            pop_at_id(&ready_priority_q, prs_to_run.identity);
        }
        if(prss_completed==total_count_prss)
        {
            killpg(getpgrp(), SIGINT);
        }
        //sleep(1);
       
    }
}

void execute_process_hpf(process prs)
{
    fork_new_prs(&prs);
    //parent
    int status;
    fprintf(pFile, "process %d started at %d \n", prs.identity,getClk());
    if(wait(&status))
    {
        //Done
        //printf("process %d finished at %d \n", prs.identity,getClk());
        fprintf(pFile, "process %d finished at %d \n", prs.identity,getClk());

    }

}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++SRTN++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void apply_srtn()
{
    prs_currently_running=create_process(-1,-1,-1,-1);//Indication nothing started yet
    while(true)
    {
        if(prss_completed==total_count_prss)
        {
            killpg(getpgrp(), SIGINT);

        }
        if(!is_empty(&ready_priority_q))
        {
            if(prs_currently_running.identity ==-1)
            {
                process prs=get_shortest_in_rem_time(ready_priority_q);
                execute_process_srtn(&prs);
                //continue;
            }
            else
            {
                
                minus_1_sec(&prs_currently_running); 

                process prs=get_shortest_in_rem_time(ready_priority_q);
                if(get_remaining_time(prs_currently_running)==0)  
                {
                    printf("process %d finished at time %d \n", prs_currently_running.identity, getClk());
                    fprintf(pFile, "process %d finished at time %d \n", prs_currently_running.identity, getClk());

                    prs_finished(prs_currently_running);
                    pop_at_id(&ready_priority_q, prs_currently_running.identity);
                    prs_currently_running=create_process(-1,-1,-1,-1);//Indication nothing is running now yet
                    
                    continue;
                }
                if(prs.identity ==prs_currently_running.identity)
                {
                    //pass
                }
                
                else
                {
                    kill(prs_currently_running.prog_id, SIGSTOP);
                    printf("process %d will paused now at time %d\n", prs_currently_running.identity, getClk());
                    fprintf(pFile, "process %d will paused now at time %d\n", prs_currently_running.identity, getClk());
                    prs_currently_running=prs;
                    execute_process_srtn(&prs);

                }
                
            }
        }

        sleep_1_sec();
        fprintf(pFile, "current time : %d\n", getClk());
        printf("current time : %d\n", getClk());
    }
}
process get_shortest_in_rem_time(node_priority* head)
{

    return peek(&ready_priority_q);
    
}
void execute_process_srtn(process* prs)
{
    if(prs->prog_id ==-1)
    {
        printf("process %d start at time %d\n", prs->identity, getClk());
        fprintf(pFile, "process %d start at time %d\n", prs->identity, getClk());
        fork_new_prs(prs);
        prs_currently_running=*prs;
    }
    else
    {
        //forked and just needs to continue
        kill(prs->prog_id, SIGCONT); 
        prs_currently_running=*prs;
        printf("process %d resumed at time %d\n", prs->identity, getClk());
        fprintf(pFile, "process %d resumed at time %d\n", prs->identity, getClk());
    }

}

void minus_1_sec(process *prs)
{
    prs->exec_time=prs->exec_time+1;
    pop_at_id(&ready_priority_q, prs->identity);
    push_to_pri_q(&ready_priority_q, *prs, get_remaining_time(*prs));
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++RR++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void prs_finished(process prs)
{
    prs.finish_time=getClk();
    prs.exec_time=prs.run_time;
    prs.curr_state=FINISHED;
    prss_completed ++;
    printf("process %d finished at time %d\n", prs.identity, getClk());
    fprintf(pFile, "process %d finished at time %d\n", prs.identity, getClk());
    printf("%d completed\n", prss_completed);
    fprintf(pFile, "%d completed\n", prss_completed);
}
void apply_rr()
{
    
    initClk();
    printf("I'm in RR at time %d \n", getClk());
    prs_currently_running=create_process(-1,-1,-1,-1);//Indication nothing started yet
    rem_quanta=quanta;
    while(true)
    {
        if(prss_completed==total_count_prss)
        {
             killpg(getpgrp(), SIGINT);
        }
        if(cq_is_empty() && get_remaining_time(prs_currently_running)==0)
        {
            prs_finished(prs_currently_running);
            prs_currently_running=create_process(-1,-1,-1,-1);
        }
        if(cq_is_empty()&& rem_quanta == -1)
        {
            rem_quanta=quanta;
        }
        if(!cq_is_empty())
        {
            if(prs_currently_running.identity ==-1)
            {
                rem_quanta=quanta;
                cq_run_top();
                
            }
            else
            {
                if(rem_quanta==0)
                {
                    fprintf(pFile, "remtime =0 \n");
                    if(get_remaining_time(prs_currently_running)==0)
                    {
                        //it should finish now
                        prs_finished(prs_currently_running);
                        //prs_currently_running=create_process(-1,-1,-1,-1);
                        cq_run_top();
                    }
                    else
                    {
                        fprintf(pFile, "else \n");
                        //quanta is finihsed but process still needs some time.
                        cq_suspend_process(&prs_currently_running);
                        cq_run_top();
                        //prs_currently_running=create_process(-1,-1,-1,-1);
                    }
                }
                else if(get_remaining_time(prs_currently_running)==0)
                {
                    //prs finished before qunata finishes
                    prs_finished(prs_currently_running);
                    cq_run_top();
                    //prs_currently_running=create_process(-1,-1,-1,-1);
                }

            }
        }
        
        sleep_1_sec();
        fprintf(pFile, "current time : %d\n", getClk());
        printf("current time : %d\n", getClk());
        rem_quanta=rem_quanta-1;
        fprintf(pFile, "rem_quanta =%d \n", rem_quanta);
        cq_minus_1_sec(&prs_currently_running);
    }
}

void cq_minus_1_sec(process *prs)
{
    prs->exec_time =prs->exec_time+1;
}

void cq_suspend_process(process *prs)
{
    fprintf(pFile, "prs %d stopped at time %d \n", prs->identity, getClk());
    kill(prs->prog_id, SIGSTOP);
    cq_enqueue(*prs);
}

void cq_run_top()
{
   
    prs_currently_running=cq_dequeue();
    rem_quanta=quanta;
    execute_process_srtn(&prs_currently_running);
    fprintf(pFile, "run top, process %d started at time%d \n",prs_currently_running.identity, getClk() );
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Common++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void fork_new_prs(process* prs)
{
    int pid;
    pid=fork();
    if(pid==0)
    {
        //Child
        char buffer[15]; // Runtime int-->string
        sprintf(buffer, "%d", prs_to_run.run_time);
        char *argv[] = {"process.out", buffer, NULL};
        //printf("process %d started at %d \n", prs->identity,getClk());
        fprintf(pFile, "process %d started at %d \n", prs->identity,getClk());
        execv("process.out", argv);
        exit(EXIT_SUCCESS);
    }
    else
    {
        fprintf(pFile, "program id is updated to %d\n", pid);
        prs->prog_id=pid;
    }
}



void ignore_all_sgnls()
{
    signal(SIGHUP, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
    signal(SIGILL, SIG_IGN);
    signal(SIGTRAP,SIG_IGN);
    signal(SIGABRT,SIG_IGN);
    signal(SIGBUS, SIG_IGN);
    signal(SIGFPE, SIG_IGN);
    signal(SIGKILL,SIG_IGN);
    signal(SIGUSR1,SIG_IGN);
    signal(SIGSEGV,SIG_IGN);
    signal(SIGUSR2, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGALRM, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGSTKFLT, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGCONT, SIG_IGN);
    signal(SIGSTOP, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGURG, SIG_IGN);
    signal(SIGXCPU, SIG_IGN);
    signal(SIGXFSZ, SIG_IGN);
    signal(SIGVTALRM, SIG_IGN);
    signal(SIGPROF, SIG_IGN);
    signal(SIGWINCH, SIG_IGN);
    signal(SIGIO, SIG_IGN);
    signal(SIGPWR, SIG_IGN);
    signal(SIGSYS, SIG_IGN);
    signal(SIGRTMIN, SIG_IGN);
    signal(SIGRTMIN+1, SIG_IGN);
    signal(SIGRTMIN+2, SIG_IGN);
    signal(SIGRTMIN+3, SIG_IGN);
    signal(SIGRTMIN+9, SIG_IGN);
    signal(SIGRTMIN+10, SIG_IGN);
    signal(SIGRTMIN+11, SIG_IGN);
    signal(SIGRTMIN+12, SIG_IGN);
    signal(SIGRTMIN+13, SIG_IGN);
    signal(SIGRTMIN+14, SIG_IGN);
    signal(SIGRTMIN+15, SIG_IGN);
    signal(SIGRTMAX-14, SIG_IGN);
    signal(SIGRTMAX-13, SIG_IGN);
    signal(SIGRTMAX-12, SIG_IGN);
    signal(SIGRTMAX-11, SIG_IGN);
    signal(SIGRTMAX-10, SIG_IGN);
    signal(SIGRTMAX-9, SIG_IGN);
    signal(SIGRTMAX-8, SIG_IGN);
    signal(SIGRTMAX-7, SIG_IGN);
    signal(SIGRTMAX-6, SIG_IGN);
    signal(SIGRTMAX-5, SIG_IGN);
    signal(SIGRTMAX-3, SIG_IGN);
    signal(SIGRTMAX-2, SIG_IGN);
    signal(SIGRTMAX-1, SIG_IGN);
    signal(SIGRTMAX, SIG_IGN);
    
}

void accept_all_signals()
{
    signal(SIGHUP, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT,SIG_DFL);
    signal(SIGILL, SIG_DFL);
    signal(SIGTRAP,SIG_DFL);
    signal(SIGABRT,SIG_DFL);
    signal(SIGBUS, SIG_DFL);
    signal(SIGFPE, SIG_DFL);
    signal(SIGKILL,SIG_DFL);
    signal(SIGUSR1,SIG_DFL);
    signal(SIGSEGV,SIG_DFL);
    signal(SIGUSR2, SIG_DFL);
    signal(SIGPIPE, SIG_DFL);
    signal(SIGALRM, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGSTKFLT, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);
    signal(SIGCONT, SIG_DFL);
    signal(SIGSTOP, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGURG, SIG_DFL);
    signal(SIGXCPU, SIG_DFL);
    signal(SIGXFSZ, SIG_DFL);
    signal(SIGVTALRM, SIG_DFL);
    signal(SIGPROF, SIG_DFL);
    signal(SIGWINCH, SIG_DFL);
    signal(SIGIO, SIG_DFL);
    signal(SIGPWR, SIG_DFL);
    signal(SIGSYS, SIG_DFL);
    signal(SIGRTMIN, SIG_DFL);
    signal(SIGRTMIN+1, SIG_DFL);
    signal(SIGRTMIN+2, SIG_DFL);
    signal(SIGRTMIN+3, SIG_DFL);
    signal(SIGRTMIN+9, SIG_DFL);
    signal(SIGRTMIN+10, SIG_DFL);
    signal(SIGRTMIN+11, SIG_DFL);
    signal(SIGRTMIN+12, SIG_DFL);
    signal(SIGRTMIN+13, SIG_DFL);
    signal(SIGRTMIN+14, SIG_DFL);
    signal(SIGRTMIN+15, SIG_DFL);
    signal(SIGRTMAX-14, SIG_DFL);
    signal(SIGRTMAX-13, SIG_DFL);
    signal(SIGRTMAX-12, SIG_DFL);
    signal(SIGRTMAX-11, SIG_DFL);
    signal(SIGRTMAX-10, SIG_DFL);
    signal(SIGRTMAX-9, SIG_DFL);
    signal(SIGRTMAX-8, SIG_DFL);
    signal(SIGRTMAX-7, SIG_DFL);
    signal(SIGRTMAX-6, SIG_DFL);
    signal(SIGRTMAX-5, SIG_DFL);
    signal(SIGRTMAX-3, SIG_DFL);
    signal(SIGRTMAX-2, SIG_DFL);
    signal(SIGRTMAX-1, SIG_DFL);
    signal(SIGRTMAX, SIG_DFL);
    
}
void sleep_1_sec()
{

    signal(SIGUSR1, SIG_IGN);
    signal(SIGINT, SIG_IGN);

    sleep(1);

    signal(SIGUSR1, SIG_DFL);
    signal(SIGINT, SIG_DFL);

    signal(SIGUSR1, rec_handler);
    signal(SIGINT, interrupt_handler);
    rec_handler(-1);
    printf("completed: %d\n" ,prss_completed);
    printf("Holla!!!!!!!\n");
}



