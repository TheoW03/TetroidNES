#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
const int WIDTH = 800, HEIGHT = 600;
using namespace std;
int setup(char *title, int width, int height)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (window == NULL)
    {
        cout << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Event windowEvent;

    // vector<SDL_Vertex> verts =
    //     {
    //         {
    //             SDL_FPoint{400, 150},
    //             SDL_Color{255, 0, 0, 255},
    //             SDL_FPoint{0},
    //         },
    //         {
    //             SDL_FPoint{200, 450},
    //             SDL_Color{0, 0, 255, 255},
    //             SDL_FPoint{0},
    //         },
    //         {
    //             SDL_FPoint{600, 450},
    //             SDL_Color{0, 255, 0, 255},
    //             SDL_FPoint{0},
    //         },
    //     };

    while (1)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {

                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_SUCCESS;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        // SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
        SDL_RenderPresent(renderer);
    }
}
