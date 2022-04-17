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

#include "SceneManager.h"
#include "Window.h"
#include "Graphics.h"
#include "private/Timer.h"

#ifdef __EMSCRIPTEN__
  #include <emscripten.h>
  #include <emscripten/html5.h>
#endif

struct SceneManager
{
    SDL_Event event;
    uint64_t lastPerformanceCounter;
    Window *window;
    Graphics *graphics;
    SDL_Renderer *renderer;

    SceneManager_CurrentScene newScene;

    struct
    {
        SceneManager_CurrentScene func;
        void *self;
    } scene;

    Timer *timer;
};

static void OverrideSceneFunctions(SceneManager_CurrentScene *func)
{
    *func = (SceneManager_CurrentScene) {
            .onNew = NULL,
            .onDelete = NULL,
            .onProcessEvent = NULL,
            .onUpdate = NULL,
            .onDraw = NULL,
        };
}

void SceneManager_InitScene(SceneManager * const self);
void SceneManager_Update(SceneManager * const self);
void SceneManager_Draw(SceneManager * const self);
bool SceneManager_MainLoop(SceneManager * const self);

SceneManager *SceneManager_New(Window *window, Graphics *graphics)
{
    SceneManager * const self = malloc(sizeof (SceneManager));

    self->window = window;
    self->graphics = graphics;
    self->renderer = Graphics_GetRenderer(graphics);
    self->lastPerformanceCounter = SDL_GetPerformanceCounter();

    OverrideSceneFunctions(&self->newScene);
    OverrideSceneFunctions(&self->scene.func);
    self->scene.self = NULL;

    self->timer = Timer_New();

    return self;
}

void SceneManager_Delete(SceneManager * const self)
{
    if (!self)
        return;

    Timer_Delete(self->timer);

    if (self->scene.func.onDelete)
        self->scene.func.onDelete(self->scene.self);

    free(self);
}

void SceneManager_GoTo(SceneManager * const self, const SceneManager_CurrentScene *scene)
{
    self->newScene = *scene;
}

void SceneManager_InitScene(SceneManager * const self)
{
    if (self->newScene.onNew)
    {
        if (self->scene.func.onDelete)
            self->scene.func.onDelete(self->scene.self);

        self->scene.func = self->newScene;
        OverrideSceneFunctions(&self->newScene);

        self->scene.self = self->scene.func.onNew(self);
    }
}

void SceneManager_AddTimer(SceneManager * const self, Uint32 interval, SceneManager_TimerCallback callback, void *userdata)
{
    Timer_Add(self->timer, interval, callback, userdata);
}

void SceneManager_ClearTimers(SceneManager * const self)
{
    Timer_Clear(self->timer);
}

bool SceneManager_MainLoop(SceneManager * const self)
{
    SceneManager_InitScene(self);

    while (SDL_PollEvent(&self->event))
    {
        if (self->event.type == SDL_QUIT || self->event.key.keysym.sym == SDLK_AC_BACK)
            return false;

        if (self->scene.func.onProcessEvent)
            self->scene.func.onProcessEvent(self->scene.self, &self->event);

        Timer_ProcessEvent(self->timer, self, &self->event);
    }

    Timer_Update(self->timer, self);

    SceneManager_Update(self);
    SceneManager_Draw(self);

    return true;
}

#ifdef __EMSCRIPTEN__
static int FrameLoop(double time, void *userData)
{
    (void)time;

    SceneManager * const self = userData;

    if (SceneManager_MainLoop(self))
        return EM_TRUE;

    return EM_FALSE;
}
#endif

void SceneManager_Run(SceneManager * const self)
{
#ifdef __EMSCRIPTEN__
    //emscripten_set_main_loop_arg(MainLoop, self, 0, 1);
    emscripten_request_animation_frame_loop(FrameLoop, self);
#else
    while (1)
        if (!SceneManager_MainLoop(self))
            return;
#endif
}

void SceneManager_Update(SceneManager * const self)
{
    Uint64 now = SDL_GetPerformanceCounter();
    double deltaTime = (double)(now - self->lastPerformanceCounter) / (double)SDL_GetPerformanceFrequency();
    self->lastPerformanceCounter = now;

    if (self->scene.func.onUpdate)
        self->scene.func.onUpdate(self->scene.self, deltaTime);
}

void SceneManager_Draw(SceneManager * const self)
{
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);

    if (self->scene.func.onDraw)
        self->scene.func.onDraw(self->scene.self);

    SDL_RenderPresent(self->renderer);
}

Window *SceneManager_Window(SceneManager * const self)
{
    return self->window;
}

Graphics *SceneManager_Graphics(SceneManager * const self)
{
    return self->graphics;
}
