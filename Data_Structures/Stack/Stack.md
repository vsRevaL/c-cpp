# Data Structures using C/C++

# Stack

**Main Reference:**

- **mycodeschool:** https://www.youtube.com/playlist?list=PL2_aWCzGMAwI3W_JlcBbtYTwiQSsOTa6P

## Operations

| operation | complexity
| --------- | ---------
| Push  |   O(1)
| Pop   |   O(1)
| Top   |   O(1)
| IsEmpty   |   O(1)

<br>

## Array implementation

```cpp
// Stack - Array based implementation
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 101
int A[MAX_SIZE];
int top = -1;

void Push(int x) {
    if (top == MAX_SIZE - 1) {
        printf("Error: stack overflow\n");
        return;
    }
    A[++top] = x;
}

void Pop() {
    if (top == -1) {
        printf("Error: No element to pop\n");
        return;
    }
    top--;    
}

void Print() {
    printf("Stack: ");
    for (int i = 0; i <= top; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

int main() {
    Push(2); Print(); // 2
    Push(5); Print(); // 2 5
    Push(10); Print();// 2 5 10
    Pop(); Print();   // 2 5
    Push(12); Print();// 2 5 12
    return 0;
}
```

<br>

## Linked List implementation of stacks

```cpp
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
```

## Reverse a string / array

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 101
char A[MAX_SIZE];
int top = -1;

void Push(char c) {
    if (top == MAX_SIZE - 1) {
        printf("Error: stack overflow\n");
        return;
    }
    A[++top] = c;
}

char Pop() {
    if (top == -1) {
        printf("Error: No element to pop\n");
    }
    else{
        int data = A[top];
        top--;
        return data;
    }   
}

void Reverse(char* str, int length) {
    // loop for push
    for (int i = 0; i < length; i++) {
        Push(str[i]);
    }

    // loop for pop
    for (int i = 0; i < length; i++) {
       str[i] = Pop();
    }
}

int main() {
    char str[] = "Hello";
    Reverse(str, strlen(str));
    printf("Reversed: %s\n", str);

    return 0;
}
```

## Check for balanced parentheses

```cpp
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Stack:
#define MAX_SIZE 101
char A[MAX_SIZE];
int top;
void Push(char);
void Pop();
void Print();
////////////////////

bool ArePair(char opening, char closing) {
	if(opening == '(' && closing == ')') return true;
	else if(opening == '{' && closing == '}') return true;
	else if(opening == '[' && closing == ']') return true;
	return false;
}

bool Check(char* str, int n) {
    for (int i = 0; i < n; i++){
        char c = str[i];
        if(str[i] == '(' || str[i] == '{' || str[i] == '[') {
			Push(str[i]);
        }    
        else if(str[i] = ']' || str[i] == ')' || str[i] == '}') {
            if (top == -1 || !ArePair(A[top], c)) {
                return false;
            }
            else{
                Pop();
            }
        }
    }
    return top == -1 ? true : false;
}

int main() {
    top = -1;
    char str[] = "[()]";
    printf("%d\n", Check(str, strlen(str))); // 1 => TRUE
    strcpy(str, "[(])");
    printf("%d\n", Check(str, strlen(str))); // 0 => FALSE
    top = -1;
    char str2[] = "[(){}[]]";
    printf("%d\n", Check(str2, strlen(str2))); // 1 => TRUE

    return 0;
}

// Stack operations:
void Push(char c) {
    if (top == MAX_SIZE - 1) return;
    top++;
    A[top] = c;
}

void Pop() {
    if (top == -1) return;
    top--;
}

void Print() {
    printf("Stack: ");
    for (int i = 0; i <= top; i++) {
        printf("%c ", A[i]);
    }
    printf("\n");
}
```

<br>
<br>
<br>
<br>
<br>