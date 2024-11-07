#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include "vectors.h"
#include "KD_Tree.h"
#include "structs.h"
#ifndef PARTICLES_H
#define PARTICLES_H

void initialize_particles(particle* p, int n, int WINDOW_WIDTH, int WINDOW_HEIGHT, float u0, float v0){
    int rows = (int)sqrt(n);
    int cols = (n + rows - 1) / rows;

    int r = 25; // Assuming all particles have the same radius
    int diameter = 2 * r;

    // Calculate spacing to ensure no overlap
    int spacing_x = (WINDOW_WIDTH - (cols * diameter)) / (cols + 1);
    int spacing_y = (WINDOW_HEIGHT - (rows * diameter)) / (rows + 1);

    // Calculate initial offsets to center the grid
    int offset_x = spacing_x + r;
    int offset_y = spacing_y + r;

    // Position particles in a centered grid pattern
    int index = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (index >= n) break;
            p[index].m = 1.0;
            p[index].r = r;
            // Set particle position with offsets
            p[index].x_c = offset_x + col * (diameter + spacing_x);
            p[index].y_c = offset_y + row * (diameter + spacing_y);
            p[index].index = index;

            // Set other properties like mass and random velocity
            if (index == 0){
                p[index].velocity.x1 = u0;
                p[index].velocity.x2 = v0;
            }
            else{
                p[index].velocity.x1 = 0;
                p[index].velocity.x2 = 0;
            }
            index++;
        }
    }
}


bool pCollision(particle* b1, particle* b2){

    int radii_sum = b1->r + b2->r;
    double d_squared = pow((b1->x_c-b2->x_c),2)+pow((b1->y_c - b2->y_c),2);
    if(int(d_squared) <= radii_sum*radii_sum){
        return true;
    }
    return false;
}

void get_unit_normal(particle* p1, particle* p2, vector* x, int WINDOW_HEIGHT){
        x->x1 = p2->x_c - p1->x_c;
        x->x2 = p2->y_c- p1->y_c;
    float mag = sqrt(pow(x->x1,2)+pow(x->x2,2));
    x->x1 = x->x1/mag;
    x->x2 = x->x2/mag;
}

void get_unit_tangent(vector* ut, vector* un){
    ut->x1 = -un->x2;
    ut->x2 = un->x1;
}

void xy_to_nt(particle* p, float* v_n, float* v_t, vector un, vector ut){
    *v_n = dot_product(un, p->velocity); // find normal component of velocity for p1
    *v_t = dot_product(ut, p->velocity); // tangential component of velocity for p1
}

void get_new_normal_velocities(float v1_n, float* v1f_n, float v2_n, float* v2f_n, particle* p1, particle* p2){
    float m1 = p1->m;
    float m2 = p2->m;
    *v1f_n = (v1_n*(m1-m2)+ 2*m2*v2_n)/(m1+m2);
    *v2f_n = (v2_n*(m2-m1)+2*m1*v1_n)/(m1+m2);
}

void checkWallCollision(particle* p, int WINDOW_WIDTH, int WINDOW_HEIGHT){
    // Collision with the right wall
            if (p->x_c + p->r >= WINDOW_WIDTH) {
                p->x_c = WINDOW_WIDTH - p->r - 1;
                p->velocity.x1 = -p->velocity.x1;  // Reverse velocity
                // Move the p back so it is exactly at the boundary
            } 

            // Collision with the left wall
            if (p->x_c - p->r <= 0) {
                p->x_c = p->r + 1;
                p->velocity.x1 = -p->velocity.x1;  // Reverse velocity
                // Move the particle back so it is exactly at the boundary
            } 

            // Collision with the bottom wall
            if (p->y_c + p->r >= WINDOW_HEIGHT) {
                // Move the p back so it is exactly at the boundary
                p->y_c = WINDOW_HEIGHT - p->r- 1;
                p->velocity.x2 = -p->velocity.x2;  // Reverse velocity
            } 

            // Collision with the top wall
            if (p->y_c - p->r <= 0) {
                // Move the p back so it is exactly at the boundary
                p->y_c = p->r + 1;
                p->velocity.x2 = -p->velocity.x2;  // Reverse velocity   
            } 
}

void resolve_wall_collisions(particle* particles, int WINDOW_WIDTH, int WINDOW_HEIGHT, int n){
     for (int i = 0; i < n; i++) {
            checkWallCollision(&particles[i], WINDOW_WIDTH, WINDOW_HEIGHT);
        }
}

float get_y_coord(float y, int WINDOW_HEIGHT){
    return WINDOW_HEIGHT - y;
}

//calculate new velocities
void updateVelocities(particle* p1, particle* p2, int WINDOW_HEIGHT){
    vector un; vector ut; // define unit normal and tangential vectors
    float v1_n; float v2_n; float v1_t; float v2_t; float v1f_n; float v2f_n; float v1f_t; float v2f_t;

    get_unit_normal(p1, p2, &un, WINDOW_HEIGHT);
    
    get_unit_tangent(&ut, &un);

    xy_to_nt(p1, &v1_n, &v1_t, un, ut); // get normal and tangential velocities for particle 1 pre-collision
    xy_to_nt(p2, &v2_n, &v2_t, un, ut); // get normal and tangential velocities for particle 2 pre-collision

    get_new_normal_velocities(v1_n, &v1f_n, v2_n, &v2f_n, p1, p2);

    vector v1prime_n; vector v1prime_t; vector v2prime_n; vector v2prime_t;
    vector_scalar_multiplication(v1f_n, un, & v1prime_n); 
    vector_scalar_multiplication(v1_t, ut, &v1prime_t);
    vector_scalar_multiplication(v2f_n, un, &v2prime_n);
    vector_scalar_multiplication(v2_t, ut, &v2prime_t);
    
    vector v1prime; vector v2prime;
    vector_add(v1prime_n, v1prime_t, &v1prime); 
    vector_add(v2prime_n, v2prime_t, &v2prime); 

    p1->velocity=v1prime;
    p2->velocity=v2prime;

    p1->x_c -= round(un.x1);
    p1->y_c -= round(un.x2);
    p2->x_c += round(un.x1);
    p2->y_c += round(un.x2); 
}


void resolve_collisions(int n, particle* particles, int WINDOW_WIDTH, int WINDOW_HEIGHT){
    resolve_wall_collisions(particles,  WINDOW_WIDTH,  WINDOW_HEIGHT, n);
    Node* root = buildKdTree(particles, n);
    bool collisionResolved[n][n];

    // initialize matrix to record if a collision has already been resolved
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            collisionResolved[i][j] = 0;
        }
    }

    // iterate through all particles, resolving collisons found with range find on KD tree
    for (int i =0; i<n; i++){
         Queue* q = (Queue*)malloc(sizeof(Queue));
        initializeQueue(q);
        int collisionIndex;

        rangeFind(q, particles[i].x_c, particles[i].y_c, particles[i].r, root, 0, particles[i].index); // load the queue with all particles that collided with the current particle
        while(!isEmpty(q)){ // go through all the collisions in the queue
            collisionIndex = q->indices[q->front+1]; // get the index of the particle from particle array, stored in queue
            if (!collisionResolved[i][collisionIndex]){ // if this hasn't already been resolved, resolve it
                updateVelocities(&particles[i], &particles[collisionIndex], WINDOW_HEIGHT);
            }
            collisionResolved[i][collisionIndex] = 1; // mark collision as resolved
            collisionResolved[collisionIndex][i] = 1;
            dequeue(q); // move on to the next collision
        } 
        free(q);
    }

    free(root);
}

void updateParticlePosition(particle* p){
        p->x_c = p->x_c + (round(p->velocity.x1 * (1.0 / 120.0)));
        p->y_c = p->y_c + (round(p->velocity.x2 * (1.0 / 120.0)));
}

#endif