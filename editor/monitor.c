#include "monitor.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Event event;
static int32_t offset_x = 0;
static int32_t offset_y = 0;
static float scale = 1.f;
static int32_t quit = 0;

int32_t monitor_init(int32_t w, int32_t h)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("cannot init video\n");
        return -1;
    }

    window = SDL_CreateWindow(
        "Monitor", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        w, 
        h, 
        SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (!window)
    {
        printf("cannot create window\n");
        return -2;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        printf("cannot create renderer\n");
        return -3;
    }

    return 0;
}

void monitor_destroy()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void monitor_clear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void monitor_update()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            quit = 1;
        }
    }
}

void monitor_draw()
{  
    SDL_RenderPresent(renderer);
}

void monitor_set_viewport(int32_t x, int32_t y, float s)
{
    offset_x = (float)x;
    offset_y = (float)y;
    scale = s;
}

void monitor_draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    x1 += offset_x;
    y1 += offset_y;
    x2 += offset_x;
    y2 += offset_y;

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderDrawLine(renderer, x1 * 7, y1 * 7, x2 * 7, y2 * 7);
}

int32_t monitor_should_quit()
{
    return quit;
}