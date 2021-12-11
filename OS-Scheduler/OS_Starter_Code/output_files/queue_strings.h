// A C program to demonstrate linked list based implementation of Queue_str
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// A linked list (LL) node to store a Queue_str entry
struct QNode_str {
    char str [100];
    struct QNode_str* next;
};
 
// The Queue_str, front stores the front node of LL and rear stores the
// last node of LL
struct Queue_str {
    struct QNode_str *front, *rear;
};
 
// A utility function to create a new linked list node.
struct QNode_str* newNode_str(char k [])
{
    struct QNode_str* temp = (struct QNode_str*)malloc(sizeof(struct QNode_str));
    strcpy(temp->str, k);
    temp->next = NULL;
    return temp;
}
 
// A utility function to create an empty Queue_str
struct Queue_str* createQueue_str()
{
    struct Queue_str* q = (struct Queue_str*)malloc(sizeof(struct Queue_str));
    q->front = q->rear = NULL;
    return q;
}
 
// The function to add a key k to q
void enQueue_str(struct Queue_str* q, char k[])
{
    // Create a new LL node
    struct QNode_str* temp = newNode_str(k);
 
    // If Queue_str is empty, then new node is front and rear both
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
 
    // Add the new node at the end of Queue_str and change rear
    q->rear->next = temp;
    q->rear = temp;
}
 
// Function to remove a key from given Queue_str q
char* deQueue_str(struct Queue_str* q)
{
    // If Queue_str is empty, return NULL.
    if (q->front == NULL)
        return "";
 
    // Store previous front and move front one node ahead
    struct QNode_str* temp = q->front;
 
    q->front = q->front->next;
 
    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;
    return &(temp->str);
    //free(temp);
}