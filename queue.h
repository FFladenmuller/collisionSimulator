#include "structs.h"
#include <stdio.h>
#ifndef QUEUE_H
#define QUEUE_H
// https://www.geeksforgeeks.org/queue-in-c/

// Function to initialize the queue 
void initializeQueue(Queue* q)
{
    q->front = -1;
    q->rear = 0;
}

// Function to check if the queue is empty
bool isEmpty(Queue* q) { return (q->front == q->rear - 1); }

// Function to check if the queue is full
bool isFull(Queue* q) { return (q->rear == MAX_SIZE); }

// Function to add an element to the queue (Enqueue
// operation)
void enqueue(Queue* q, int index)
{
    if (isFull(q)) {
        printf("Queue is full\n");
        return;
    }
    q->indices[q->rear] = index;
    q->rear++;
}

// Function to remove an element from the queue (Dequeue
// operation)
void dequeue(Queue* q)
{
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    q->front++;
}

#endif