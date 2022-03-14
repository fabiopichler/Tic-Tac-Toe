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
#include "base/BasicSceneManager.h"
#include "scene_game/SceneGame.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif

struct App
{
    Window *window;
    Graphics *graphics;
    SDL_Renderer *renderer;
    BasicSceneManager *sceneManager;
};

void App_InitSDL();

App *App_New()
{
    App *const self = malloc(sizeof (App));

    App_InitSDL();

    int width = 640;
    int height = 480;

    self->window = Window_New(width, height);
    self->graphics = Graphics_New(self->window);
    self->renderer = Graphics_GetRenderer(self->graphics);
    self->sceneManager = BasicSceneManager_New(self->renderer);

    SceneGame *sceneGame = SceneGame_New(self->renderer, Window_GetRect(self->window));

    GO_TO(self->sceneManager, SceneGame, sceneGame);

    return self;
}

void App_Delete(App *const self)
{
#ifndef __EMSCRIPTEN__
    if (!self)
        return;

    BasicSceneManager_Delete(self->sceneManager);
    Graphics_Delete(self->graphics);
    Window_Delete(self->window);
    free(self);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
#endif
}

void App_Run(App *const self)
{
    BasicSceneManager_Run(self->sceneManager);
}

void App_InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        exit(-1);
    }

#ifndef __EMSCRIPTEN__
    int imgFlags = IMG_INIT_PNG;

    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        exit(-1);
    }
#endif

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        exit(-1);
    }
}
