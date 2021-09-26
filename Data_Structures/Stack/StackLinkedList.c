// Linked List implementation of stacks
#include <stdlib.h>
#include <stdio.h>

typedef struct Node Node;
struct Node {
    int data;
    Node* next;
};
Node* top = NULL;

// The added Node to the stack will be always the first Node
void Push(int data) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = data;
    temp->next = top;
    top = temp;
}

void Pop() {
    struct Node* temp;
    if (top == NULL) return;
    temp = top;
    top = temp->next;
    free(temp);
}

void Print();
int main() {

    Push(5);  // 5
    Push(3);  // 3 5
    Push(2);  // 2 3 5
    Pop();    // 3 5
    Push(10); // 10 3 5
    Print();
    Pop(); Pop();
    Print();
    
    return 0;
}

void Print() {
    Node* temp = top;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}