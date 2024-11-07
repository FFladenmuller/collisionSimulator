#ifndef STRUCTS_H
#define STRUCTS_H
#define MAX_SIZE 300


typedef struct {
    float x1; // first vector component
    float x2; // second vector component
} vector; 

typedef struct {
    float m;
    int r;
    int x_c;
    int y_c;
    vector velocity;
    int index;
} particle;

typedef struct {
    int indices[MAX_SIZE];
    int front;
    int rear;
} Queue;


#endif