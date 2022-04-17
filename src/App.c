//-------------------------------------------------------------------------------
// Copyright (c) 2020-2022 Fábio Pichler
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
#include "base/DataZipFile.h"
#include "base/Window.h"
#include "base/Graphics.h"
#include "base/SceneManager.h"
#include "scene_game/SceneGame.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

struct App
{
    Window *window;
    Graphics *graphics;
    SceneManager *sceneManager;
};

static void InitSDL();

App *App_New()
{
    srand(time(NULL));

#ifdef USE_DATA_ZIP
    if (!DataZipFile_Init())
        return NULL;
#endif

    InitSDL();

    App * const self = malloc(sizeof (App));

    self->window = Window_New(640, 480, "Tic Tac Toe");
    self->graphics = Graphics_New(self->window);
    self->sceneManager = SceneManager_New(self->window, self->graphics);

    Window_SetWindowIcon(self->window, "images/player_1.png");

    SCENE_MANAGER_GOTO(self->sceneManager, SceneGame);

    return self;
}

void App_Delete(App * const self)
{
#ifndef __EMSCRIPTEN__
    if (!self)
        return;

    SceneManager_Delete(self->sceneManager);
    Graphics_Delete(self->graphics);
    Window_Delete(self->window);

    free(self);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

#ifdef USE_DATA_ZIP
    DataZipFile_Close();
#endif
#endif
}

void App_Run(App * const self)
{
    SceneManager_Run(self->sceneManager);
}

void InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        exit(-1);
    }

#ifndef __EMSCRIPTEN__
    const int imgFlags = IMG_INIT_PNG;

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
