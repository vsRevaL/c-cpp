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