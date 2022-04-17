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

#include "Button.h"
#include "Rectangle.h"
#include "Box.h"

#include <malloc.h>

typedef enum State
{
    Normal,
    Pressed,
    Hover
} State;

typedef struct Button_PressedEvent
{
    Button_OnPressEvent function;
    void *userdata;
} Button_PressedEvent;

struct Button
{
    SDL_Renderer *renderer;

    Texture *textTexture;
    Texture *iconTexture;

    Box *box;

    SDL_Color color;
    SDL_Color colorHover;
    SDL_Color colorPressed;

    SDL_Color textColor;

    Rectangle *background;

    State state;

    Button_PressedEvent pressedEvent;
};

bool Button_PointerIsHovering(Button * const self, const SDL_Event *event);
void Button_OnUpdateBox(Button * const self);
void Button_CallPressedEvent(Button * const self);
void Button_BoxOnUpdateEvent(Box * const box, void *userdata);

Button *Button_New(SDL_Renderer *renderer)
{
    Button * const self = malloc(sizeof (Button));

    self->renderer = renderer;
    self->textTexture = NULL;
    self->iconTexture = NULL;

    self->color = (SDL_Color) {50, 140, 140, 255};
    self->colorHover = (SDL_Color) {30, 120, 120, 255};
    self->colorPressed = (SDL_Color) {60, 60, 60, 255};
    self->textColor = (SDL_Color) {255, 255, 255, 255};

    self->state = Normal;
    self->pressedEvent = (Button_PressedEvent) {NULL, NULL};

    self->box = Box_New(0.f, 0.f, 60.f, 40.f);

    self->background = Rectangle_New(self->renderer, Box_Width(self->box), Box_Height(self->box));

    Box_SetOnPressEvent(self->box, Button_BoxOnUpdateEvent, self);

    return self;
}

void Button_Delete(Button * const self)
{
    if (!self)
        return;

    Texture_Delete(self->textTexture);
    free(self);
}

void Button_SetBackgroundColorRGB(Button * const self, uint8_t r, uint8_t g, uint8_t b)
{
    self->color.r = r;
    self->color.g = g;
    self->color.b = b;
    self->color.a = 255;
}

void Button_SetBackgroundColorRGBA(Button * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    self->color.r = r;
    self->color.g = g;
    self->color.b = b;
    self->color.a = a;
}

void Button_SetBackgroundHoverColorRGB(Button * const self, uint8_t r, uint8_t g, uint8_t b)
{
    self->colorHover.r = r;
    self->colorHover.g = g;
    self->colorHover.b = b;
    self->colorHover.a = 255;
}

void Button_SetBackgroundHoverColorRGBA(Button * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    self->colorHover.r = r;
    self->colorHover.g = g;
    self->colorHover.b = b;
    self->colorHover.a = a;
}

void Button_SetBackgroundPressedColorRGB(Button * const self, uint8_t r, uint8_t g, uint8_t b)
{
    self->colorPressed.r = r;
    self->colorPressed.g = g;
    self->colorPressed.b = b;
    self->colorPressed.a = 255;
}

void Button_SetBackgroundPressedColorRGBA(Button * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    self->colorPressed.r = r;
    self->colorPressed.g = g;
    self->colorPressed.b = b;
    self->colorPressed.a = a;
}

void Button_SetTextColorRGB(Button * const self, uint8_t r, uint8_t g, uint8_t b)
{
    self->textColor.r = r;
    self->textColor.g = g;
    self->textColor.b = b;
    self->textColor.a = 255;
}

void Button_SetTextColorRGBA(Button * const self, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    self->textColor.r = r;
    self->textColor.g = g;
    self->textColor.b = b;
    self->textColor.a = a;
}

bool Button_SetText(Button * const self, const char *text, int ptsize)
{
    if (!self->textTexture)
        self->textTexture = Texture_New(self->renderer);

    Texture_SetTextColor(self->textTexture, &self->textColor);
    Texture_SetTextSize(self->textTexture, ptsize);
    Texture_SetText(self->textTexture, text);

    if (Texture_MakeText(self->textTexture))
    {
        Button_OnUpdateBox(self);
        return true;
    }

    return false;
}

void Button_SetIcon(Button * const self, Texture *texture)
{
    self->iconTexture = texture;
    Button_OnUpdateBox(self);
}

void Button_SetOnPressEvent(Button * const self, Button_OnPressEvent callback, void *userdata)
{
    self->pressedEvent.function = callback;
    self->pressedEvent.userdata = userdata;
}

void *Button_GetEventUserData(Button * const self)
{
    return self->pressedEvent.userdata;
}

void Button_CallPressedEvent(Button * const self)
{
    if (self->pressedEvent.function)
        self->pressedEvent.function(self, self->pressedEvent.userdata);
}

void Button_ProcessEvent(Button * const self, const SDL_Event *event)
{
    if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP)
    {
        if (event->button.button == SDL_BUTTON_LEFT && Button_PointerIsHovering(self, event))
        {
            if (event->type == SDL_MOUSEBUTTONDOWN)
            {
                self->state = Pressed;

                Button_CallPressedEvent(self);

                return;
            }
            else if (self->state == Pressed && event->type != SDL_MOUSEBUTTONUP)
            {
                return;
            }
        }

        if (Button_PointerIsHovering(self, event))
            self->state = Hover;
        else
            self->state = Normal;
    }
}

void Button_Draw(Button * const self)
{
    Button_OnUpdateBox(self);

    if (self->state == Hover)
        Rectangle_SetColor(self->background, self->colorHover);
    else if (self->state == Pressed)
        Rectangle_SetColor(self->background, self->colorPressed);
    else
        Rectangle_SetColor(self->background, self->color);

    Rectangle_Draw(self->background);

    if (self->iconTexture)
        Texture_Draw(self->iconTexture);

    if (self->textTexture)
        Texture_Draw(self->textTexture);
}

bool Button_PointerIsHovering(Button * const self, const SDL_Event *event)
{
    const SDL_FRect *rect = Box_Rect(self->box);

    return event->button.x >= rect->x
            && event->button.x <= (rect->x + rect->w)
            && event->button.y >= rect->y
            && event->button.y <= (rect->y + rect->h);
}

void Button_OnUpdateBox(Button * const self)
{
    Texture *texture = self->textTexture ? self->textTexture : self->iconTexture;
    const SDL_FRect *rect = Box_Rect(self->box);

    if (texture)
    {
        Box *box = Texture_Box(texture);
        int w = Texture_GetWidth(texture);
        int h = Texture_GetHeight(texture);

        Box_SetSize(box, w, h);
        Box_SetPosition(box, rect->x + ((rect->w - w) / 2), rect->y + ((rect->h - h) / 2));
    }
}

void Button_BoxOnUpdateEvent(Box * const box, void *userdata)
{
    Button * const self = userdata;

    Box_SetSize(Rectangle_Box(self->background), Box_Width(self->box), Box_Height(self->box));
    Box_SetPosition(Rectangle_Box(self->background), Box_X(self->box), Box_Y(self->box));

    Button_OnUpdateBox(self);
}

Box *Button_Box(Button * const self)
{
    return self->box;
}

Texture *Button_Icon(Button * const self)
{
    return self->iconTexture;
}
