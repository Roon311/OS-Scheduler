// A C program to demonstrate linked list based implementation of queue
#include <stdio.h>
#include <stdlib.h>
#include "../headers.h"

// A linked list (LL) node to store a queue entry
typedef struct QNode_ 
{
    process key;
    struct QNode_* next;
}QNode;
 
// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Queue {
    QNode *front, *rear;
};
 
// A utility function to create a new linked list node.
QNode* newNode(process k)
{
    QNode* temp = (QNode*)malloc(sizeof(QNode));
    temp->key = k;
    temp->next = NULL;
    return temp;
}
 
// A utility function to create an empty queue
struct Queue* createQueue()
{
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}
 
// The function to add a key k to q
void push(struct Queue* q, process k)
{
    // Create a new LL node
    QNode* temp = newNode(k);
 
    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
 
    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}
 
// Function to remove a key from given queue q
void pop(struct Queue* q)
{
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return;
 
    // Store previous front and move front one node ahead
    QNode* temp = q->front;
 
    q->front = q->front->next;
 
    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;
 
    free(temp);
}
process peek(struct Queue* q)
{
    return q->front->key;
}
