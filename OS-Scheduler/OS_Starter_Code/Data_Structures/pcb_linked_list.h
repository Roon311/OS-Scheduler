/** @file pcb_linked_list.h
 *  @brief An initial Program contro block that is used by process generator
 *
 *  PCB uses linked list as mainly data strcuture.
 *  Why Linked list?
 *  1-the size of incoming processes is not known.
 *  2-there will be no overhead while looping over all the list
 *      a) as i will search in all of it.    
 *
 *  @author Ahmed Adel 
 *  @bug No known bugs.
 * 
 */
#ifndef pcb_linked_list_H
#define pcb_linked_list_H

/*-- Includes --*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

/* for all gloobal shard variables intialized there */
#include "../headers.h"

/* For NULL definition*/
#include <stddef.h>


/**
*   represent node in linked list containg value of current process and pointer for next element.
**/
struct node
{
    process info; //Value
    struct node *next; //Next pointer
};
/*
*
*/
struct node *start=NULL;

/** @brief  Display current data of linked list
 *          Mainly used in testing only .
 *  @return Void.
 */
void display()
{
    struct node *ptr;
    if(start==NULL)
    {
        //TODO delete that line after debugging
        printf("\nList is empty:\n");
        return;
    }
    else
    {
        ptr=start;
        //TODO delete that line after debugging
        printf("\nThe List elements are:\n");
        while(ptr!=NULL)
        {
            printf("%d \n",ptr->info.identity );
            ptr=ptr->next ;
        }
    }
}

/** @brief insert new node at begining of linked list
 *  @param p process to be inserted at end of linked list.
 *  @return Void.
 */
void insert_begin(process p)
{
    struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    if(temp==NULL)
    {
        //TODO delete that line after debugging
        printf("nOut of Memory Space:n");
        return;
    }
    temp->info=p;
    temp->next =NULL;
    if(start==NULL)
    {
        // head is not intialized yer
        start=temp;
    }
    else
    {
        // set head to gived process
        temp->next=start;
        start=temp;
    }
}

/** @brief insert new node at end of linked list
 *  @param p process to be inserted at end of linked list.
 *  @return Void.
 */
void insert_end(process p)
{
    struct node *temp,*ptr;
    temp=(struct node *)malloc(sizeof(struct node));
    if(temp==NULL)
    {
        //TODO delete that line after debugging
        printf("nOut of Memory Space:n");
        return;
    }
    temp->info=p;
    temp->next =NULL;
    if(start==NULL)
    {
        //head is not intialized yet
        start=temp;
    }
    else
    {
        ptr=start;
        while(ptr->next !=NULL) // find last element
        {
            ptr=ptr->next ;
        }
        ptr->next =temp;
    }
}

/*
*       delete the first element of linked list
*/
/** @brief delete the first element of linked list
 *  @return Void.
 */
void delete_begin()
{
    struct node *ptr;
    if(ptr==NULL)
    {
        //TODO delete that line after debugging
        printf("nList is Empty:n");
        return;
    }
    else
    {
        ptr=start;
        start=start->next ;
        //TODO delete that line after debugging
        printf("nThe deleted element is :%dt",ptr->info.identity);
        free(ptr);
    }
}

/** @brief insert new node at certain position
 *  @param pos position at which you need to delete node.
 *  @return Void.
 */
void delete_pos(int pos)
{
    int i;
    struct node *temp,*ptr;
    if(start==NULL)
    {
        //TODO delete that line after debugging
        printf("nThe List is Empty:n");
        exit(0);
    }
    else
    {
        if(pos==0)
        {
            ptr=start;
            start=start->next ;
            //TODO delete that line after debugging
            printf("\nThe deleted element is:%d \n",ptr->info.identity  );
            free(ptr);
        }
        else
        {
            ptr=start;
            for(i=0; i<pos; i++)
            {
                temp=ptr;
                ptr=ptr->next ;
                if(ptr==NULL)
                {
                    printf("\nPosition not Found:\n");
                    return;
                }
            }
            temp->next =ptr->next ;
            //TODO delete that line after debugging
            printf("\nThe deleted element is:%d\n",ptr->info.identity );
            free(ptr);
        }
    }
}



/** @brief delete at thbe end of linked list 
 *  @return Void.
 */
void delete_end()
{
    struct node *temp,*ptr;
    if(start==NULL)
    {
        //TODO delete that line after debugging
        printf("\nList is Empty:");
        exit(0);
    }
    else if(start->next ==NULL)
    {
        ptr=start;
        start=NULL;
        //TODO delete that line after debugging
        printf("\nThe deleted element is:%d \n",ptr->info.identity);
        free(ptr);
    }
    else
    {
        ptr=start;
        while(ptr->next!=NULL)
        {
            temp=ptr;
            ptr=ptr->next;
        }
        temp->next=NULL;
        //TODO delete that line after debugging
        printf("\nThe deleted element is:%d \n",ptr->info.identity);
        free(ptr);
    }
}



/** @brief send process at current time to schedular
 *         through a mesg queue.
 *  @param cur_time current time given by clk
 *  @param AlgoPID Algorithm PID
 *  @return Void.
 */
void send_new_process_to_schedular(int cur_time,int AlgoPID)
{
    struct node *ptr;
    if(start==NULL)
    {   
        //TODO delete that line after debugging
        printf("\nList is empty:\n");
        return;
    }
    else
    {
        ptr=start;
        //TODO delete that line after debugging
        //printf("\nThe List elements are:\n");
        //TODO delete that line after debugging
        //printf("curr time from pcb: %d \n",cur_time);
        int pos=0;
        while(ptr!=NULL)
        {

            if(ptr->info.arrival_time==cur_time)
            {
                //TODO delete that line after debugging
                printf("\nProcess %d sent at time: %d \n\n",ptr->info.identity, cur_time);
                msg_buff msg;
                msg.mtype=10;
                msg.p=ptr->info;
                //TODO Delete that line
                //printf("\n\n\n\n\n msg info: %d \n\n\n\n\n", msg.p.arrival_time);
                int send_val=msgsnd(set_msg_q_id(), &msg, sizeof(msg.p), IPC_NOWAIT);
                if(send_val == -1)//if no process is received
                {
                    perror("Error in receiving"); //for debugging, print error in receiving
                    return;
                }
                else
                {
                    kill(AlgoPID,SIGUSR1);
                }
                //TODO Delete that line
                //printf("\n\n\n\n sended %d \n\n\n\n", send_val);
                //TODO: test if deleting causes any problem
            }
            //pos=pos +1;
            ptr=ptr->next ;
        }
        //printf("\n Deleting\n\n");
        //delete_pos(pos); //Etakd en mfysh seneh 3
        


    }
}

#endif