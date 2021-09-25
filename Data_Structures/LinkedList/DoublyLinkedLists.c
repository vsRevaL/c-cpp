#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
struct Node {
    int data;
    Node* next;
    Node* prev;
};
Node* head; // global var - pointer to head Node.

void InsertAtHead(int data);
Node* GetNewNode(int data);
void ReversePrint();

void Print(Node*);
int main() {
    head = NULL;
    InsertAtHead(2);
    InsertAtHead(4);
    InsertAtHead(5);
    InsertAtHead(6); // List: [6, 5, 4, 2]

    ReversePrint(); // [2, 4, 5, 6]
    //Print(head);

    return 0;
}

void InsertAtHead(int data) {
    Node* newNode = GetNewNode(data);
    if (head == NULL) {
        head = newNode;
        return;
    }

    head->prev = newNode;
    newNode->next = head;
    head = newNode;
}

Node* GetNewNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->prev = NULL;
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void ReversePrint() {
    Node* temp = head;
    if (temp == NULL) return;

    while (temp->next != NULL) { // Going to last Node
        temp = temp->next;
    }

    // Traversing backward using prev pointer
    printf("Reverse: ");
    while(temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->prev;
    }
    printf("\n");
}

void Print(Node* head) {
    if (head == NULL) return;
    printf("%d ", head->data);
    Print(head->next);
}