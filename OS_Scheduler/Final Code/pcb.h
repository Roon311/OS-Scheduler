#ifndef pcb_H
#define pcb_H

 /*+++++++++++++++++++++++++++++++++++++++Includes++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#include "headers.h"
#include "data_structures/priority_queue.h"
/** @brief push to pcb and intialize it if null
 *          made to create priority queue easier.
 * 
 *  @param head could be null-> we will intialize it for you
 *  @param priority priority of new inserted node
 *  @return void .
 */
void push_to_pcb(node_priority** head,process prs,int priority)
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
/** @brief check if top of priority queue arrival time is eqution to current time
 *          made to create priority queue easier.
 * 
 *  @param current_time by getClk()
 *  @return bool.
 */
bool is_top_arvl_tm_now(node_priority** head,int current_time)
{
    if(is_empty(head)) return false;
    if(head ==NULL) return false;
    //why not ==?
    //because if at any second, the the prcess gen coldn't work it will now send current process and that will create huge problem
    if(peek(head).arrival_time<=current_time)
    {
        return true;
    }
    return false;
}


#endif