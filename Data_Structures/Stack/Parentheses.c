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