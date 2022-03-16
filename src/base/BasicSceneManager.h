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

#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Window Window;
typedef struct Graphics Graphics;

typedef struct BasicSceneManager BasicSceneManager;

typedef void *(*BasicSceneManager_NewCallback)(BasicSceneManager *sceneManager);
typedef void (*BasicSceneManager_DeleteCallback)(void *const self);
typedef void (*BasicSceneManager_ProcessEventCallback)(void *const self, const SDL_Event *event);
typedef void (*BasicSceneManager_UpdateCallback)(void *const self, double deltaTime);
typedef void (*BasicSceneManager_DrawCallback)(void *const self);

typedef struct BasicSceneManager_CurrentScene
{
    BasicSceneManager_NewCallback onNew;
    BasicSceneManager_DeleteCallback onDelete;
    BasicSceneManager_ProcessEventCallback onProcessEvent;
    BasicSceneManager_UpdateCallback onUpdate;
    BasicSceneManager_DrawCallback onDraw;
} BasicSceneManager_CurrentScene;

BasicSceneManager *BasicSceneManager_New(Window *window, Graphics *graphics);
void BasicSceneManager_Delete(BasicSceneManager *const self);
void BasicSceneManager_GoTo(BasicSceneManager *const self, const BasicSceneManager_CurrentScene *scene);
void BasicSceneManager_Run(BasicSceneManager *const self);
Window *BasicSceneManager_Window(BasicSceneManager *const self);
Graphics *BasicSceneManager_Graphics(BasicSceneManager *const self);

#define GO_TO(MANAGER, SCENE_CLASS) \
    BasicSceneManager_GoTo(MANAGER, &(BasicSceneManager_CurrentScene) { \
        .onNew = (BasicSceneManager_NewCallback) SCENE_CLASS##_OnNew, \
        .onDelete = (BasicSceneManager_DeleteCallback) SCENE_CLASS##_OnDelete, \
        .onProcessEvent = (BasicSceneManager_ProcessEventCallback) SCENE_CLASS##_OnProcessEvent, \
        .onUpdate = (BasicSceneManager_UpdateCallback) SCENE_CLASS##_OnUpdate, \
        .onDraw = (BasicSceneManager_DrawCallback) SCENE_CLASS##_OnDraw, \
    });

#ifdef __cplusplus
}
#endif
