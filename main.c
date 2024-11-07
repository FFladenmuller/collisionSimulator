#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "particles.h"
#include "SDL_helpers.h"
#include <stdbool.h>
#define WINDOW_WIDTH (1000)
#define WINDOW_HEIGHT (600)

int main(int argc, char *args[]) {
    int n = 300; // number of balls
    int close_requested = 0;
    particle* particles = (particle*)calloc(n, sizeof(particle)); // initialize array for particles or balls
    float u0 = 2000;
    float v0 = -2000;

    initialize_particles(particles, n, WINDOW_WIDTH, WINDOW_HEIGHT, u0, v0);
    initializeSDL(WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_Window* win = create_window( WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_Renderer* rend = create_renderer(win); 
    
    while (!close_requested) {
        check_window_close(&close_requested);

        resolve_collisions(n, particles, WINDOW_WIDTH, WINDOW_HEIGHT);
        clear_window(rend);

        for (int i = 0; i < n; i++) {
            updateParticlePosition(&particles[i]);
            DrawCircle(rend, particles[i].x_c, particles[i].y_c, particles[i].r);
        }
        render_window(rend); 
    }

    quit(rend, win, particles); // close out SDL, free memory
    return 0;
}