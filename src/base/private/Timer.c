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

#include "Timer.h"
#include "../LinkedList.h"
#include "../SceneManager.h"

#include <stdlib.h>

#ifdef __EMSCRIPTEN__
  #include <emscripten.h>
#else
  #include <SDL2/SDL_timer.h>
#endif

typedef struct TimerData
{
    SceneManager_TimerCallback callback;
    void *userdata;

#ifdef __EMSCRIPTEN__
    Uint64 time;
#else
    SDL_TimerID timerId;
#endif

} TimerData;

#ifndef __EMSCRIPTEN__
static const int TimerEventCode = 42000;

static Uint32 TimerCallback(Uint32 interval, void *param)
{
    (void)interval;

    SDL_Event event = {.type = SDL_USEREVENT};
    event.user = (SDL_UserEvent) {
        .type = SDL_USEREVENT,
        .code = TimerEventCode,
        .data1 = param,
    };

    SDL_PushEvent(&event);

    return 0;
}
#endif

struct Timer
{
    LinkedList *timers;
};

Timer *Timer_New()
{
    Timer * const self = malloc(sizeof (Timer));

    self->timers = LinkedList_New();

    return self;
}

void Timer_Delete(Timer * const self)
{
    if (!self)
        return;

    Timer_Clear(self);
    LinkedList_Delete(self->timers);

    free(self);
}

void Timer_Clear(Timer * const self)
{
#ifndef __EMSCRIPTEN__
    for (LinkedListNode *iterator = LinkedList_GetFirst(self->timers); iterator != NULL;)
    {
        TimerData *data = LinkedList_GetValuePtr(self->timers, iterator);

        SDL_RemoveTimer(data->timerId);
        free(data);
        LinkedList_Next(self->timers, &iterator);
    }
#endif

    LinkedList_Clear(self->timers);
}

void Timer_Add(Timer * const self, Uint32 interval, Timer_TimerCallback callback, void *userdata)
{
    TimerData *data = malloc(sizeof (TimerData));

    data->callback = callback;
    data->userdata = userdata;

#ifdef __EMSCRIPTEN__
    data->time = SDL_GetTicks64() + interval;

    LinkedList_PushPtr(self->timers, data);
#else
    data->timerId = SDL_AddTimer(interval, TimerCallback, data);

    LinkedList_PushPtr(self->timers, data);
#endif
}

void Timer_ProcessEvent(Timer * const self, SceneManager *sceneManager, SDL_Event *event)
{
#ifndef __EMSCRIPTEN__
    if (event->type == SDL_USEREVENT && event->user.type == SDL_USEREVENT)
    {
        if (event->user.code == TimerEventCode)
        {
            TimerData *data = event->user.data1;

            data->callback(sceneManager, data->userdata);

            SDL_RemoveTimer(data->timerId);
            LinkedList_RemoveFromValuePtr(self->timers, data);
            free(data);
        }
    }
#endif
}

void Timer_Update(Timer * const self, SceneManager *sceneManager)
{
#ifdef __EMSCRIPTEN__
    for (LinkedListNode *iterator = LinkedList_GetFirst(self->timers); iterator != NULL;)
    {
        TimerData *data = LinkedList_GetValuePtr(self->timers, iterator);

        if (SDL_GetTicks64() >= data->time)
        {
            data->callback(sceneManager, data->userdata);

            free(data);
            LinkedList_Remove(self->timers, &iterator);
        }
        else
        {
            LinkedList_Next(self->timers, &iterator);
        }
    }
#endif
}
