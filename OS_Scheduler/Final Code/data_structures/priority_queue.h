/** @file priority_queue.h
 *  @brief fpriotiry queue used manily for two algorithms HPF, and SRTN
 * 
 *  @author Internet and edidted by Adel.
 *  @bug No known bugs.
 * 
*/
#ifndef priority_queue_H
#define priority_queue_H
/*++++++++Includes++++++++++*/
#include <stdio.h>
#include <stdlib.h>
#include "../headers.h"
 
 /*+++++++++++++++++++++++++++++++++++++++Data fields++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/**
 * represt node in priority queue
 * it include process, and priority, and pointer to next node
 * it include duplicate data, priority is defined twice in process adn int priotiry just to faster to execute.  
 */
typedef struct node_priority_
{
    process process;
    int priority;
    struct node_priority_* next;
} node_priority;

 /*+++++++++++++++++++++++++++++++++++++++functions header++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/** @brief create new node by memory allocation
 *
 *  @param prs process to be put inside new create node
 *  @param priority priority of new inserted node
 *  @return node_priority .
 */
node_priority* _new_priority_node(process prs, int priority);

/** @brief peek at the first element of priority queue
 *
 *  @param head head of priority queue
 *  @return process of max priority .
 */
process peek(node_priority** head);

/** @brief pop at the first element of priority queue
 *          and it free the memory
 *  @param head head of priority queue
 *  @return void .
 */
void pop(node_priority** head);

/** @brief push new element to priority queue
 *          and it free the memory
 *  @param prs process to be put inside new create node
 *  @param priority priority of new inserted node
 *  @return void .
 */
void push(node_priority** head,process prs,int priority);


/** @brief Function to check the queue is empty
 *  @param head head of priority queue
 *  @return 1 if empty 0 otherwise .
 */
int is_empty(node_priority** head);

 /*+++++++++++++++++++++++++++++++++++++++functions implmentations++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
node_priority* _new_priority_node(process prs, int priority)
{
    node_priority* temp = (node_priority*)malloc(sizeof(node_priority));
    temp->process = prs;
    temp->priority = priority;
    temp->next = NULL;
    return temp;
}

process peek(node_priority** head)
{
    return (*head)->process;
}

void pop(node_priority** head)
{
    node_priority* temp = *head;
    (*head) = (*head)->next;
    free(temp);
}

void pop_at_id(node_priority** head, int id)
{
    node_priority* temp = *head;
    node_priority* prev = NULL;
    if(temp !=NULL && temp->process.identity==id)
    {
        (*head) = (*head)->next;
        free(temp);
    }
    while (temp != NULL && temp->process.identity != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
        
    prev->next = temp->next;
    free(temp);

}


void push(node_priority** head,process prs,int priority)
{
    node_priority* start = (*head);
    node_priority* temp = _new_priority_node(prs, priority);
    if ((*head)->priority > priority)
    {
        temp->next = *head;
        (*head) = temp;
    }
    else
    {
        while (start->next != NULL && start->next->priority < priority)
        {
            start = start->next;
        }
        temp->next = start->next;
        start->next = temp;
    }
}


int is_empty(node_priority** head)
{
    return (*head) == NULL;
}

#endif