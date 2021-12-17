#ifndef circular_queue_H
#define circular_queue_H

#include "../headers.h"
#include<stdio.h>
#include<stdlib.h>

struct node
{
    process info;
    struct node *link;
}*rear=NULL;

void cq_enqueue(process item);
process cq_dequeue();
void cq_display();
int cq_is_empty();
process cq_peek();

void cq_enqueue(process item)
{
    struct node *tmp;
    tmp=(struct node *)malloc(sizeof(struct node));
    tmp->info=item;
    if(tmp==NULL)
    {
            printf("\nMemory not available\n");
            return;
    }

    if( cq_is_empty() ) /*If queue is empty */
    {
            rear=tmp;
            tmp->link=rear;
    }
    else
    {
            tmp->link=rear->link;
            rear->link=tmp;
            rear=tmp;
    }
}

process cq_dequeue()
{
        process item;
        struct node *tmp;
        if( cq_is_empty() )
        {
                printf("\nQueue underflow\n");
                exit(1);
        }
        if(rear->link==rear)  /*If only one element*/
        {
                tmp=rear;
                rear=NULL;
        }
        else
        {
                tmp=rear->link;
                rear->link=rear->link->link;
        }
        item=tmp->info;
        free(tmp);
        return item;
}

process q_cq_peek()
{
        if( cq_is_empty() )
        {
                printf("\nQueue underflow\n");
                exit(1);
        }
        return rear->link->info;
}

int cq_is_empty()
{
        if( rear == NULL )
                return 1;
        else
                return 0;
}


void cq_display()
{
        struct node *p;
        if(cq_is_empty())
        {
                printf("\nQueue is empty\n");
                return;
        }
        printf("\nQueue is :\n");
        p=rear->link;
        do
        {
                printf("%d ",p->info.identity);
                p=p->link;
        }while(p!=rear->link);
        printf("\n");
}


#endif