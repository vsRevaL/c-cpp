#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node; 
struct Node { // Node "class"
    int data;
    Node* next;
};
Node* head; // global var, can be accessed anywhere

// Operations
void Insert (int data); // Inserts at the end of the list.
void InsertBeginning (int data); // Inserts Node with value x to the beginning.
void InsertAt (int data, int position); // Inserts Node with value `x` at a given position.
void DeleteAt (int position); // Deletes Node at at a given position.
void Print (); // Prints Linked List.
void ReversePrint(Node*); // Print elements of a linked list in forward and reverse order using recursion

// Reversing
Node* ReverseIterative();
Node* ReverseRecursion(Node*);

// For testing methods
void Test_Insert ();
void Test_InsertBeginning (); 
void Test_InsertAt ();
void Test_DeleteAt ();

int main ()
{
    Test_Insert();
    // Test_InsertBeginning();
    // Test_InsertAt();
    // Test_DeleteAt();
    // Node* reversed1 = ReverseIterative();
    head = ReverseRecursion(head);

    //ReversePrint(head);
    Print();

    return 0;
}

void Insert (int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    if (head == NULL) {
        head = newNode;
        return;
    }

    Node* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void InsertBeginning (int data) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = data;
    temp->next = head;
    head = temp;
}

void InsertAt (int data, int position) {
    Node* temp1 = (Node*)malloc(sizeof(Node)); // new Node() in C++
    temp1->data = data;
    temp1->next = NULL;
    if (position == 1) {
        temp1->next = head;
        head = temp1;
        return;
    }

    Node* temp2 = head;
    for (int i = 1; i <= position - 2; i++) {
        temp2 = temp2->next;
    }

    temp1->next = temp2->next;
    temp2->next = temp1;
}

void DeleteAt (int position) {
    Node* temp1 = head;
    if (position == 1) {
        head = temp1->next;
        free(temp1);
        return;
    }

    for (int i = 0; i < position - 2; i++) {
        temp1 = temp1->next;
    }
    Node* temp2 = temp1->next;
    temp1->next = temp2->next;
    free(temp2);
}

// Reversing
Node* ReverseIterative() {
    Node *current, *prev, *next;
    current = head;
    prev = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
    return head;
}

Node* ReverseRecursion(Node* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    Node* last = ReverseRecursion(head->next);
    head->next->next = head;
    head->next = NULL;
    return last;
}

// Test Methods:
void Print () {
    printf("List ist: ");
    Node* temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void ReversePrint(Node* head) {
    if (head == NULL) return;
    ReversePrint(head->next);
    printf("%d ", head->data);
}

void Test_Insert () {
    head = NULL;
    Insert(2);
    Insert(4);
    Insert(6);
    Insert(5);
    Print(); // List: [2, 4, 6, 5]
}

void Test_InsertBeginning () {
    head = NULL; // empty list
    printf("How many numbers?\n");
    int n, x;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        printf("Enter a number: \n", &x);
        scanf("%d", &x);
        InsertBeginning(x);
        Print(x);
    }
}

void Test_InsertAt() {
    head = NULL;    // empty list
    InsertAt(2, 1); // List: [2]
    InsertAt(3, 2); // List: [2, 3]
    InsertAt(4, 1); // List: [4, 2, 3]
    InsertAt(5, 2); // List: [4, 5, 2, 3]
    Print();
}

void Test_DeleteAt() {
    head = NULL;    // empty list
    Insert(2);
    Insert(4);
    Insert(6);
    Insert(5); 
    Print(); // List: [2, 4, 6, 5]
    int n;
    printf("Enter a position: \n");
    scanf("%d", &n);
    DeleteAt(n);
    Print();
}