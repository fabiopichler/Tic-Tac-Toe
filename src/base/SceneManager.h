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

#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Window Window;
typedef struct Graphics Graphics;

typedef struct SceneManager SceneManager;

typedef void *(*SceneManager_NewCallback)(SceneManager *sceneManager);
typedef void (*SceneManager_DeleteCallback)(void * const self);
typedef void (*SceneManager_ProcessEventCallback)(void * const self, const SDL_Event *event);
typedef void (*SceneManager_UpdateCallback)(void * const self, double deltaTime);
typedef void (*SceneManager_DrawCallback)(void * const self);
typedef void (*SceneManager_TimerCallback)(void * const manager, void *userdata);

typedef struct SceneManager_CurrentScene
{
    SceneManager_NewCallback onNew;
    SceneManager_DeleteCallback onDelete;
    SceneManager_ProcessEventCallback onProcessEvent;
    SceneManager_UpdateCallback onUpdate;
    SceneManager_DrawCallback onDraw;
} SceneManager_CurrentScene;

SceneManager *SceneManager_New(Window *window, Graphics *graphics);
void SceneManager_Delete(SceneManager * const self);
void SceneManager_GoTo(SceneManager * const self, const SceneManager_CurrentScene *scene);
void SceneManager_AddTimer(SceneManager * const self, Uint32 interval, SceneManager_TimerCallback callback, void *userdata);
void SceneManager_ClearTimers(SceneManager * const self);
void SceneManager_Run(SceneManager * const self);
Window *SceneManager_Window(SceneManager * const self);
Graphics *SceneManager_Graphics(SceneManager * const self);

#define SCENE_MANAGER_GOTO(MANAGER, SCENE_CLASS) \
    SceneManager_GoTo(MANAGER, &(SceneManager_CurrentScene) { \
        .onNew = (SceneManager_NewCallback) SCENE_CLASS##_OnNew, \
        .onDelete = (SceneManager_DeleteCallback) SCENE_CLASS##_OnDelete, \
        .onProcessEvent = (SceneManager_ProcessEventCallback) SCENE_CLASS##_OnProcessEvent, \
        .onUpdate = (SceneManager_UpdateCallback) SCENE_CLASS##_OnUpdate, \
        .onDraw = (SceneManager_DrawCallback) SCENE_CLASS##_OnDraw, \
    });

#ifdef __cplusplus
}
#endif
