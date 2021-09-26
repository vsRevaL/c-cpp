#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 101
int A[MAX_SIZE];
int front = -1;
int end = -1;

// To check whether Queue is empty or not
bool IsEmpty() {
    return front == -1 && end == -1;
}

// To check whether Queue is full or not
bool IsFull() {
	return (end + 1) % MAX_SIZE == front ? true : false;
}    

// Inserts an element in queue at rear end
void Enqueue(int data) {
    if (IsFull()) {
        printf("Error: Queue if Full\n");
        return;
    }
    if (IsEmpty()) {
        front = end = 0;
    }
    else {
        end = (end+1) % MAX_SIZE;
    }
    A[end] = data;
}

// Removes an element in Queue from front end.
void Dequeue() {
    if (IsEmpty()) {
        printf("Error: Queue is Empty\n");
        return;
    }
    
    if (front == end) {
        end = front = -1;
    }
    else {
        front = (front + 1) % MAX_SIZE;
    }
}

void Print() {
    // Finding number of elements in queue
    int count = (end + MAX_SIZE - front)  % MAX_SIZE + 1;
    printf("Queue: ");
    for (int i = 0; i < count; i++) {
        int index = (front + i) % MAX_SIZE; // Index of element while traversing circularly from front
        printf("%d ", A[index]);
    }
}

int main() {
    Enqueue(2);
    Enqueue(5);
    Enqueue(4);
    Dequeue();
    Enqueue(6);
    Print(); // 5 4 6

    return 0;
}