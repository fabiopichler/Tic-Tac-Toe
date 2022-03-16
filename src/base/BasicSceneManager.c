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

#include "BasicSceneManager.h"
#include "Window.h"
#include "Graphics.h"

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif

struct BasicSceneManager
{
    SDL_Event event;
    uint64_t lastPerformanceCounter;
    Window *window;
    Graphics *graphics;
    SDL_Renderer *renderer;
    BasicSceneManager_CurrentScene currentScene;
};

static void BasicSceneManager_Update(BasicSceneManager *const self);
static void BasicSceneManager_Draw(BasicSceneManager *const self);

BasicSceneManager *BasicSceneManager_New(Window *window, Graphics *graphics)
{
    BasicSceneManager *const self = malloc(sizeof (BasicSceneManager));

    self->window = window;
    self->graphics = graphics;
    self->renderer = Graphics_GetRenderer(graphics);
    self->lastPerformanceCounter = SDL_GetPerformanceCounter();

    self->currentScene = (BasicSceneManager_CurrentScene) {
        .self = NULL,
        .processEventCallback = NULL,
        .updateCallback = NULL,
        .drawCallback = NULL,
    };

    return self;
}

void BasicSceneManager_Delete(BasicSceneManager *const self)
{
    if (self->currentScene.deleteCallback)
        self->currentScene.deleteCallback(self->currentScene.self);

    free(self);
}

void BasicSceneManager_GoTo(BasicSceneManager *const self, const BasicSceneManager_CurrentScene *scene)
{
    if (self->currentScene.deleteCallback)
        self->currentScene.deleteCallback(self->currentScene.self);

    self->currentScene = *scene;
}

bool main_loop(BasicSceneManager *const self)
{
    while (SDL_PollEvent(&self->event))
    {
        if (self->event.type == SDL_QUIT || self->event.key.keysym.sym == SDLK_AC_BACK)
            return false;

        if (self->currentScene.processEventCallback)
            self->currentScene.processEventCallback(self->currentScene.self, &self->event);
    }

    BasicSceneManager_Update(self);
    BasicSceneManager_Draw(self);

    return true;
}

#ifdef __EMSCRIPTEN__
int frame_loop(double time, void *userData)
{
    BasicSceneManager *const self = userData;

    if (main_loop(self))
        return EM_TRUE;

    return EM_FALSE;
}
#endif

void BasicSceneManager_Run(BasicSceneManager *const self)
{
#ifdef __EMSCRIPTEN__
    //emscripten_set_main_loop_arg(main_loop, self, 60, 1);
    emscripten_request_animation_frame_loop(frame_loop, self);
#else
    while (1)
        if (!main_loop(self))
            return;
#endif
}

void BasicSceneManager_Update(BasicSceneManager *const self)
{
    Uint64 now = SDL_GetPerformanceCounter();
    double deltaTime = (double)(now - self->lastPerformanceCounter) / (double)SDL_GetPerformanceFrequency();
    self->lastPerformanceCounter = now;

    if (self->currentScene.updateCallback)
        self->currentScene.updateCallback(self->currentScene.self, deltaTime);
}

void BasicSceneManager_Draw(BasicSceneManager *const self)
{
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);

    if (self->currentScene.drawCallback)
        self->currentScene.drawCallback(self->currentScene.self);

    SDL_RenderPresent(self->renderer);
}

Window *BasicSceneManager_Window(BasicSceneManager *const self)
{
    return self->window;
}

Graphics *BasicSceneManager_Graphics(BasicSceneManager *const self)
{
    return self->graphics;
}
