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

#include "Box.h"

typedef struct Box_UpdatedEvent
{
    Box_OnUpdateEvent function;
    void *userdata;
} Box_UpdatedEvent;

struct Box
{
    SDL_FRect rect;
    Box_UpdatedEvent updatedEvent;
};

void Box_CallUpdatedEvent(Box * const self);

Box *Box_New(float x, float y, float width, float height)
{
    Box * const self = malloc(sizeof (Box));

    self->rect = (SDL_FRect) {x, y, width, height};
    self->updatedEvent = (Box_UpdatedEvent) {NULL, NULL};

    return self;
}

void Box_Delete(Box * const self)
{
    if (!self)
        return;

    free(self);
}

void Box_SetOnPressEvent(Box * const self, Box_OnUpdateEvent callback, void *userdata)
{
    self->updatedEvent.function = callback;
    self->updatedEvent.userdata = userdata;
}

void *Box_GetEventUserData(Box * const self)
{
    return self->updatedEvent.userdata;
}

void Box_CallUpdatedEvent(Box * const self)
{
    if (self->updatedEvent.function)
        self->updatedEvent.function(self, self->updatedEvent.userdata);
}

void Box_SetSize(Box * const self, float w, float h)
{
    self->rect.w = w;
    self->rect.h = h;

    Box_CallUpdatedEvent(self);
}

void Box_SetPosition(Box * const self, float x, float y)
{
    self->rect.x = x;
    self->rect.y = y;

    Box_CallUpdatedEvent(self);
}

void Box_SetX(Box * const self, float x)
{
    self->rect.x = x;

    Box_CallUpdatedEvent(self);
}

void Box_SetY(Box * const self, float y)
{
    self->rect.y = y;

    Box_CallUpdatedEvent(self);
}

void Box_SetWidth(Box * const self, float w)
{
    self->rect.w = w;

    Box_CallUpdatedEvent(self);
}

void Box_SetHeight(Box * const self, float h)
{
    self->rect.h = h;

    Box_CallUpdatedEvent(self);
}

void Box_Move(Box * const self, float velX, float velY)
{
    self->rect.x += velX;
    self->rect.y += velY;

    Box_CallUpdatedEvent(self);
}

float Box_X(Box * const self)
{
    return self->rect.x;
}

float Box_Y(Box * const self)
{
    return self->rect.y;
}

float Box_Width(Box * const self)
{
    return self->rect.w;
}

float Box_Height(Box * const self)
{
    return self->rect.h;
}

const SDL_FRect *Box_Rect(Box * const self)
{
    return &self->rect;
}
