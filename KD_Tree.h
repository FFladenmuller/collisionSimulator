#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include "queue.h"
#include "structs.h"
#ifndef KD_TREE_H
#define KD_TREE_H

typedef struct  Node{
    particle* part;
    struct Node* left; // pointer to the left child Node
    struct Node* right; // pointer to the right child Node
} Node; 

Node* createNewNode(particle* p){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->part = p;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

int compare(Node* root, float x, float y, int depth){
    int rootX = root->part->x_c;
    int rootY = root->part->y_c; 

    if (depth%2 == 0){ // separation across vertical line
        if(x > rootX) {return 1;} 
        else if(x < rootX){return -1;} 
        else{return 0;}  
    }
    else {
        if(y > rootY) {return 1;} 
        else if(y < rootY){return -1;}
        else{return 0;} 
    } 
}

Node* insert(Node* root, particle* p, int depth){
    if(root==NULL){
        root = createNewNode(p);   
        return root;
    }
    int comparison = compare(root, p->x_c, p->y_c, depth);
    if (comparison == -1){
        root->left = insert(root->left, p, depth + 1);
    }
    else  {
        root->right = insert(root->right, p, depth + 1); 
    }
    return root;
}

Node* buildKdTree(particle* p, int n){
    Node* root;
    root = NULL; 

    // iterate through elements in particle array
    for (int i = 0; i < n; i++)
    {
        // insert 
        root = insert(root, &p[i], 0);
    }
    return root;
}

bool contains(int x_c, int y_c, int r, Node* currentNode){
    double d_squared = pow((x_c-currentNode->part->x_c),2)+pow((y_c - currentNode->part->y_c),2);
    if(int(d_squared) <= (pow(2*r,2))){
        return true;
    }
    else{
        return false;
    }
}

void rangeFind(Queue* q, int x_c, int y_c, int r, Node* root, int depth, int currentIndex) {
    if (root == NULL) { return; }

    // Check if the root node is within the range
    if (root->part->index != currentIndex && contains(x_c, y_c, r, root)) {
        enqueue(q, root->part->index);
    }

    // Calculate the region comparisons
    int cmpMin = compare(root, x_c - r, y_c - r, depth);
    int cmpMax = compare(root, x_c + r, y_c + r, depth);

    // Traverse the correct child nodes based on the comparisons
    if (cmpMin > 0 && cmpMax > 0) {
        // Entire range is to the right of root, search right subtree only
        rangeFind(q, x_c, y_c, r, root->right, depth + 1, currentIndex);
    } else if (cmpMin < 0 && cmpMax < 0) {
        // Entire range is to the left of root, search left subtree only
        rangeFind(q, x_c, y_c, r, root->left, depth + 1, currentIndex);
    } else {
        // Range overlaps, search both subtrees
        rangeFind(q, x_c, y_c, r, root->right, depth + 1, currentIndex);
        rangeFind(q, x_c, y_c, r, root->left, depth + 1, currentIndex);
    }
}


#endif