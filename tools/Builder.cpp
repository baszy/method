#include <iostream>

#include <SDL2/SDL.h>

#include "Vector.hpp"

using namespace method;

class Map {
public:
    Vec2 * points;
    unsigned int points_length;

public:
    Map() : points_length(0) {
        points = static_cast<Vec2 *>(malloc(256));
    }

    void add_point(Vec2 point) {
        points[points_length] = point;
        points_length++;
    }
};


int main(int argc, char * argv[]) {
    SDL_Window * window = SDL_CreateWindow("Builder",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600,
        SDL_WINDOW_OPENGL);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Vec2 point_list[100];
    int num_points = 0;

    bool running = true;
    while (running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        for (int i = 0; i < num_points - 1; i++) {
            SDL_RenderDrawLine(renderer, point_list[i].x, point_list[i].y, point_list[i + 1].x, point_list[i + 1].y);
        }

        SDL_RenderPresent(renderer);
 
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                Vec2 pos = Vec2(event.button.x, event.button.y);
                point_list[num_points] = pos;
                num_points++;
                break;
            }
        }
    }

    return 0;
}
