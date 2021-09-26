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