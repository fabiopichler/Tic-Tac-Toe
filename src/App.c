//-------------------------------------------------------------------------------
// Copyright (c) 2020 Fábio Pichler
/*-------------------------------------------------------------------------------

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-------------------------------------------------------------------------------*/

#include "App.h"
#include "base/Window.h"
#include "base/Graphics.h"
#include "scene_game/SceneGame.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>

struct App
{
    Window *window;
    Graphics *graphics;
    SDL_Renderer *renderer;
    SceneGame *sceneGame;

    uint64_t lastPerformanceCounter;
};

void App_Update(App *const this);
void App_Draw(App *const this);
void App_InitSDL();

App *App_New()
{
    App *const this = malloc(sizeof (App));

    App_InitSDL();

    this->window = Window_New(640, 480);
    this->graphics = Graphics_New(this->window);
    this->renderer = Graphics_GetRenderer(this->graphics);
    this->sceneGame = SceneGame_New(this->renderer, Window_GetRect(this->window));

    this->lastPerformanceCounter = SDL_GetPerformanceCounter();

    return this;
}

void App_Delete(App *const this)
{
    if (!this)
        return;

    SceneGame_Delete(this->sceneGame);
    Graphics_Delete(this->graphics);
    Window_Delete(this->window);
    free(this);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void App_Run(App *const this)
{
    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;

            SceneGame_ProcessEvent(this->sceneGame, &event);
        }

        App_Update(this);
        App_Draw(this);
    }
}

void App_Update(App *const this)
{
    Uint64 now = SDL_GetPerformanceCounter();
    double deltaTime = (double)((now - this->lastPerformanceCounter) * 1000ul) / (double)SDL_GetPerformanceFrequency();
    this->lastPerformanceCounter = now;

    SceneGame_Update(this->sceneGame, deltaTime);
}

void App_Draw(App *const this)
{
    SDL_SetRenderDrawColor(this->renderer, 190, 225, 225, 255);
    SDL_RenderClear(this->renderer);

    SceneGame_Draw(this->sceneGame);

    SDL_RenderPresent(this->renderer);
}

void App_InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        exit(-1);
    }

    int imgFlags = IMG_INIT_PNG;

    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        exit(-1);
    }

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        exit(-1);
    }
}
