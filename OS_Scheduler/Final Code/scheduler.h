#ifndef scheduler_H
#define scheduler
#include "headers.h"
#include "data_structures/priority_queue.h"


int total_count_prss;
int alg_num;
int prss_completed;
scheduling_algorithms chosen_alg;
//---------------------------------------------Push to priority Queue--------------------------------------------------------//
/** @brief push to pcb and intialize it if null
 *          made to create priority queue easier.
 * 
 *  @param head could be null-> we will intialize it for you
 *  @param priority priority of new inserted node
 *  @return void .
 */
void push_to_pri_q(node_priority** head,process prs,int priority)
{
    if(*head ==NULL)
    {
        (*head) = _new_priority_node(prs,priority);
    }
    else
    {
        push(head, prs, priority);
    }
}
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------Receive Handler-------------------------------------------------------------//

/** @brief signal handler for SIGUSR1
 *  @author Noureldin.
 *  @return Void.
 */
void rec_handler(int signum);
//--------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------alg_num Setter-------------------------------------------------------------//

/** @brief set chosen algorthm num in enum 
 *          to better read
 *  @return Void.
 */

void set_alg_enum(int alg_num);
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------Applying HPF-------------------------------------------------------------//
/** @brief apply hightest priority first
 *  @return Void.
 */
void apply_hpf();
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------Applying SRTN---------------------------------------------------------------//
/** @brief apply shortest remaining time next
 *  @author Noureldin and edited by adel.
 *  @return Void.
 */
void apply_srtn();
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------Applying Round Robin--------------------------------------------------------//
/** @brief apply round robin
 *  @author Noureldin and edited by adel.
 *  @return Void.
 */
void apply_rr();
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------Receive 1 Process-----------------------------------------------------------//
/** @brief recieve one single process from msg queue
 *          if nothing has come it returns process with arrival time = -1
 *          as indication of error
 *  @author Noureldin and edited by adel.
 *  @return process .
 */
process rec_one_procss();
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------pushng into prority Queue HPF-----------------------------------------------//
/** @brief insert the new coming process to appropiate data strucutre for HPF
 *  @author Noureldin andd adel turned it to a function by Adel.
 *  @return int .
 */
void insert_prs_to_hpf(process prs);
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------pushng into prority Queue SRTN----------------------------------------------//
/** @brief insert the new coming process to appropiate data strucutre for SRTN
 *  @author Adel.
 *  @return int .
 */
void insert_prs_to_srtn(process prs);
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------pushng into circular Queue RR-----------------------------------------------//
/** @brief insert the new coming process to appropiate data strucutre for RR
 *  @author Adel.
 *  @return int .
 */
void insert_prs_to_rr(process prs);
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------HPF Execute Process---------------------------------------------------------//
/** @brief fires a new process 
 *  @author Adel.
 *  @return int .
 */
void execute_process_hpf(process prs);
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------SRTN Execute Process--------------------------------------------------------//
/** @brief executre process, but should be used only by srtn algorithm
 *          as it does extra work for SRTN algorith
 *  @author Noureldin.
 *  @return void .
 */
void execute_process_srtn(process* prs);
//--------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------Remaining time getter-------------------------------------------------------//
/** @brief calcuate the remaining time for process
 *  @author Noureldin.
 *  @return remaining time for process .
 */
int get_remaining_time(process prs);
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------Shortest remaining time getter----------------------------------------------//
/** @brief just peek but redifined it for calrity
 *  @author Noureldin and placed n a function by adel.
 *  @return process .
 */
process get_shortest_in_rem_time(node_priority* head);
//--------------------------------------------------------------------------------------------------------------------------//


//----------------------------------------------Fork new process------------------------------------------------------------//
/** @brief it just forks new process 
 *  @author Noureldin and placed n a function by adel.
 *  @return void .
 */
void fork_new_prs(process* prs);
//--------------------------------------------------------------------------------------------------------------------------//


/** @brief it adds one second to exec time to 
 *          currenly running process, update its priority
 * 
 *  @return void .
 */
void minus_1_sec(process *prs);
//--------------------------------------------------------------------------------------------------------------------------//


/** @brief must be called when process finishs
 *         it adds one to prss_complmted to used later 
 *          as indication for all processes are done
 *  @author Adel.
 *  @return void .
 */
void prs_finished(process prs);
//--------------------------------------------------------------------------------------------------------------------------//


/** @brief like minus_1_sec but it does on data strucutre related to RR
 *          circular queue 
 *  @return void .
 */
void cq_minus_1_sec(process *prs);
//--------------------------------------------------------------------------------------------------------------------------//


/** @brief suspend process, just sends SIGSTOP, and enqueu the process again to circular queue
 *  @return void .
 */
void cq_suspend_process(process *prs);
//--------------------------------------------------------------------------------------------------------------------------//


/** @brief run the top of circular queue
 *  @return void .
 */
void cq_run_top();
//--------------------------------------------------------------------------------------------------------------------------//


/** @brief for Debugging, i print output to output.txt. might be removed when i'm done debuggint
 *          it just closes the file opened to write output in it.
 *  @return void .
 */
void  interrupt_handler(int signum);
//--------------------------------------------------------------------------------------------------------------------------//


void sleep_1_sec();
//--------------------------------------------------------------------------------------------------------------------------//

#endif