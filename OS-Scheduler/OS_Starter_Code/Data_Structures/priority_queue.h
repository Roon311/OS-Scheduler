/** @file priority_queue.h
 *  @brief fpriotiry queue used manily for two algorithms HPF, and SRTN
 * 
 *  @author Internet and edidted by Adel.
 *  @bug No known bugs.
 * 
*/

/*-- Includes --*/
#ifndef PriorityQueue_H
#define PriorityQueue_H

#include <stdio.h>
#include <stdlib.h>

/* -- for all global variables --*/
#include "../headers.h"

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

/** @brief create new node by memory allocation
 *
 *  @param prs process to be put inside new create node
 *  @param priority priority of new inserted node
 *  @return node_priority .
 */
node_priority* newNode(process prs, int priority)
{
    node_priority* temp = (node_priority*)malloc(sizeof(node_priority));
    temp->process = prs;
    temp->priority = priority;
    temp->next = NULL;
    return temp;
}

/** @brief peek at the first element of priority queue
 *
 *  @param head head of priority queue
 *  @return process of max priority .
 */
process peek(node_priority** head)
{
    //printf("\n inside the peek\n\n");
    return (*head)->process;
}
/** @brief pop at the first element of priority queue
 *          and it free the memory
 *  @param head head of priority queue
 *  @return void .
 */
void pop(node_priority** head)
{
    node_priority* temp = *head;
    (*head) = (*head)->next;
    free(temp);
}

/** @brief push new element to priority queue
 *          and it free the memory
 *  @param prs process to be put inside new create node
 *  @param priority priority of new inserted node
 *  @return void .
 */
void push(node_priority** head, int priority,process prs)
{
   //printf("\nentered null \n");

    node_priority* start = (*head);
    node_priority* temp = newNode(prs, priority);
    if ((*head)->priority > priority)
    {
       //printf("created new node22222\n");
        temp->next = *head;
        (*head) = temp;
    }
    else
    {
       //printf("created new node33333\n");
        while (start->next != NULL && start->next->priority < priority)
        {
            start = start->next;
        }
        // Either at the ends of the list
        // or at required position
        temp->next = start->next;
        start->next = temp;
       // printf("created new node\n");
    }
}
/** @brief Function to check the queue is empty
 *  @param head head of priority queue
 *  @return 1 if empty 0 otherwise .
 */
int isEmpty(node_priority** head)
{
    return (*head) == NULL;
}

#endif
