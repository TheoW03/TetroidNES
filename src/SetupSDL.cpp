#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "../src/emulator/Memory.h"
#include <pthread.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <unistd.h>
// #include <Windows.h>

#include <SDL2/SDL_pixels.h>
const int WIDTH = 800, HEIGHT = 600;
using namespace std;
void handle_keybinds(SDL_Event e, SDL_Window *window)
{

    while (SDL_PollEvent(&e) != 0)
    {

        if (e.type == SDL_KEYDOWN)
        {

            switch (e.key.keysym.sym)
            {
            case SDLK_w:
                write_8bit(0xff, 0x77);
                break;
            case SDLK_a:
                write_8bit(0xff, 0x61);
                break;
            case SDLK_s:
                write_8bit(0xff, 0x73);
                break;
            case SDLK_d:
                write_8bit(0xff, 0x64);
                break;
            case SDLK_ESCAPE:
                SDL_DestroyWindow(window);
                SDL_Quit();
                exit(EXIT_SUCCESS);
            }
        }

        // _sleep(1);
    }
}
SDL_Color getColorFromByte(uint8_t byte)
{
    // printf("%d \n", byte);
    switch (byte)
    {
    case 0:
        return {0, 0, 0}; // BLACK
    case 1:
        return {255, 255, 255}; // WHITE
    case 2:
        return {128, 128, 128}; // GREY
    case 9:
        return {128, 128, 128}; // GREY
    case 3:
        return {255, 0, 0}; // RED
    case 10:
        return {255, 0, 0}; // RED
    case 4:
        return {0, 255, 0}; // GREEN
    case 11:
        return {0, 255, 0}; // GREEN
    case 5:
        return {0, 0, 255}; // BLUE
    case 12:
        return {0, 0, 255}; // BLUE
    case 6:
        return {255, 0, 255}; // MAGENTA
    case 13:
        return {255, 0, 255}; // MAGENTA
    case 7:
        return {255, 255, 0}; // YELLOW
    case 14:
        return {255, 255, 0}; // YELLOW
    default:
        return {0, 255, 255}; // CYAN
    }
}
bool update_texture(uint8_t pixels[32 * 32 * 3])
{
    // cout << "test: " << static_cast<int>(color.r) << endl;
    int frame_idx = 0;
    bool update = false;

    for (int i = 0x0200; i < 0x0600; i++)
    {
        SDL_Color c = getColorFromByte(read_8bit(i));
        // if (pixels[frame_idx] != static_cast<uint8_t>(c.r))
        //     cout << "doesnt equal" << endl;

        if (pixels[frame_idx] != static_cast<uint8_t>(c.r)         // m
            || pixels[frame_idx + 1] != static_cast<uint8_t>(c.g)  // s
            || pixels[frame_idx + 2] != static_cast<uint8_t>(c.b)) // s
        {
            pixels[frame_idx] = static_cast<uint8_t>(c.r);
            pixels[frame_idx + 1] = static_cast<uint8_t>(c.g);
            pixels[frame_idx + 2] = static_cast<uint8_t>(c.b);
            update = true;
        }
        frame_idx += 3;
    }
    return update;
}
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
    SDL_Event e;
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 32, 32);
    uint8_t pixels[32 * 32 * 3]; // meow

    while (1)
    {

        if (SDL_PollEvent(&windowEvent) == 1)
        {
            if (windowEvent.type == SDL_QUIT)
            {

                SDL_DestroyWindow(window);
                SDL_Quit();
                exit(EXIT_SUCCESS);
                return EXIT_SUCCESS;
            }
        }
        write_8bit(0xfe, ((uint8_t)rand() % 16 + 1));
        handle_keybinds(e, window);
        if (update_texture(pixels))
        {
            SDL_UpdateTexture(texture, nullptr, pixels, 32 * 3);

            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            // SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }
        // this_thread::sleep_for(std::chrono::milliseconds(1));

        // sleep(1);

        // sleep(600);
        // usleep(10);
        // Sleep(1000);
        // SDL_Delay(1);
        // SDLK_SLEEP()
    }
}
