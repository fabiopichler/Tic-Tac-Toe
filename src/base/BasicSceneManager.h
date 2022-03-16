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

typedef struct Window Window;
typedef struct Graphics Graphics;

typedef struct BasicSceneManager BasicSceneManager;

typedef void (*BasicSceneManager_DeleteCallback)(void *const self);
typedef void (*BasicSceneManager_ProcessEventCallback)(void *const self, const SDL_Event *event);
typedef void (*BasicSceneManager_UpdateCallback)(void *const self, double deltaTime);
typedef void (*BasicSceneManager_DrawCallback)(void *const self);

typedef struct BasicSceneManager_CurrentScene
{
    void *self;
    BasicSceneManager_DeleteCallback deleteCallback;
    BasicSceneManager_ProcessEventCallback processEventCallback;
    BasicSceneManager_UpdateCallback updateCallback;
    BasicSceneManager_DrawCallback drawCallback;
} BasicSceneManager_CurrentScene;

BasicSceneManager *BasicSceneManager_New(Window *window, Graphics *graphics);
void BasicSceneManager_Delete(BasicSceneManager *const self);
void BasicSceneManager_GoTo(BasicSceneManager *const self, const BasicSceneManager_CurrentScene *scene);
void BasicSceneManager_Run(BasicSceneManager *const self);

#define GO_TO(SCENE_MANAGER, SCENE_CLASS, CURRENT_SCENE)\
    BasicSceneManager_GoTo(SCENE_MANAGER, &(BasicSceneManager_CurrentScene) {\
        .self = CURRENT_SCENE,\
        .deleteCallback = (BasicSceneManager_DeleteCallback) SCENE_CLASS##_Delete,\
        .processEventCallback = (BasicSceneManager_ProcessEventCallback) SCENE_CLASS##_ProcessEvent,\
        .updateCallback = (BasicSceneManager_UpdateCallback) SCENE_CLASS##_Update,\
        .drawCallback = (BasicSceneManager_DrawCallback) SCENE_CLASS##_Draw,\
    });
