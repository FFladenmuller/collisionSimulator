#include "structs.h"
#ifndef VECTORS_H
#define VECTORS_H

float dot_product(vector v1, vector v2){
    return v1.x1 * v2.x1 + v1.x2*v2.x2;
}

void vector_add(vector v1, vector v2, vector* new_vector){
    new_vector->x1 = v1.x1 + v2.x1;
    new_vector->x2 = v1.x2 + v2.x2;
}

void vector_scalar_multiplication(float scalar, vector v, vector* newVector){
    newVector->x1 = scalar * v.x1;
    newVector->x2 = scalar * v.x2;
}

#endif
