#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Node Node;
struct Node {
    int data;
    Node* next;
};
Node* front = NULL;
Node* end = NULL;

bool IsEmpty() {
    return front == NULL && end == NULL;
}

void Enqueue(int x) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = x;
    temp->next = NULL;
    if (IsEmpty()) {
        front = end = temp;
        return;
    }
    end->next = temp;
    end = temp;
}

void Dequeue() {
    Node* temp = front;
    if (front == end) {
        front = end = NULL;
    }
    else {
        front = front->next;   
    }
    free(temp);
}

void Print() {
    Node* temp = front;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
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