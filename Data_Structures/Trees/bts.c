#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Binary Search Tree
typedef struct Node Node;
struct Node {
    int data;
    Node* left;
    Node* right;
};

void Preorder(Node* root) {
    if (root == NULL) return;
    printf("%d ", root->data); // Print data
    Preorder(root->left); // Visit left subtree
    Preorder(root->right); // Visit right subtree
}

void Inorder(Node* root) {
    if (root == NULL) return;
    Inorder(root->left); // Visit left subtree
    printf("%d ", root->data); // Print data
    Inorder(root->right); // Visit right subtree
}

void Postorder(Node* root) {
    if (root == NULL) return;
    Postorder(root->left); // Visit left subtree
    Postorder(root->right); // Visit right subtree
    printf("%d ", root->data); // Print data
}

Node* GetNewNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// To insert data in BST, returns address of root node 
Node* Insert(Node* root, int data) {
    if (root == NULL) { // empty tree
        root = GetNewNode(data);
    }
    // if data to be inserted is lesser, insert in left subtree.
    else if (data <= root->data) {
        root->left = Insert(root->left, data);
    }
    // else, insert in right subtree.
    else {
        root->right = Insert(root->right, data);
    }
    return root;
}

//To search an element in BST, returns true if element is found
bool Search(Node* root, int data) {
    if (root == NULL) return false;
    else if (data == root->data) return true;
    else if (data <= root->data) return Search(root->left, data);
    else return Search(root->right, data);
}

// Find the minimum value in the BTS - iterative
int FindMin(Node* root) { // root is a local variable here
    // Node* current = root;
    if (root == NULL) {
        printf("Error: Tree is empty\n");
        return -1;
    }
    while (root->left != NULL) {
        root = root->left;
    }
    return root->data;
}

// Find the maximum value in the BTS - recursive
int FindMax(Node* root) {
    if (root == NULL) {
        printf("Error: Tree is empty\n");
        return -1;
    }
    else if (root->right == NULL) {
        return root->data;
    }

    // Search in right subtree.
    return FindMax(root->right);
}

int FindHeight(Node* root) {
    if (root == NULL) {
        return -1;
    }
    int a = FindHeight(root->left); // leftHeight
    int b = FindHeight(root->right); // rightHeight
    return (a > b ? a : b) + 1; // max(a, b) + 1
}

int main()
{
    Node* root = NULL; // Creating an empty tree
    /* Code to tes the logic*/
    root = Insert(root, 15);
    root = Insert(root, 10);
    root = Insert(root, 20);
    root = Insert(root, 25);
    root = Insert(root, 8);
    root = Insert(root, 12);

    int number = 25; // Found!
    // If number is found, print "Found"
    if (Search(root, number)) { printf("Found!\n"); }
    else { printf("Not found\n"); }  

    printf("Min element: %d\n", FindMin(root));
    printf("Max element: %d\n", FindMax(root));
    printf("Height of the tree: %d\n", FindHeight(root)); // 2

    //              15
    //            /   \
    //          10     20
    //         /  \      \       
    //        8    12     25

    printf("\nPreorder: ");
    Preorder(root); // 15 10 8 12 20 25
    
    printf("\nInorder: ");
    Inorder(root); // 8 10 15 20 25 [SORTED!]
    
    printf("\nPostorder: ");
    Postorder(root); // 8 12 10 25 20 15
}