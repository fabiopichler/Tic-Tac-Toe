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

typedef struct ButtonPressedEvent
{
    ButtonOnPressEvent function;
    void *userdata;
} ButtonPressedEvent;

struct Button
{
    SDL_Renderer *renderer;

    Texture *textTexture;
    Texture *imageTexture;
    SDL_Rect textureRect;

    SDL_Rect rect;

    SDL_Color color;
    SDL_Color colorHover;
    SDL_Color colorPressed;

    SDL_Color textColor;

    Rectangle *background;

    State state;

    ButtonPressedEvent pressedEvent;
};

void Button_Draw_(Button *const self);
bool Button_PointerIsHovering(Button *const self, const SDL_Event *event);
void Button_UpdateTextureRect(Button *const self);

Button *Button_New(SDL_Renderer *renderer)
{
    Button *const self = malloc(sizeof (Button));

    self->renderer = renderer;
    self->textTexture = NULL;
    self->imageTexture = NULL;
    self->textureRect = (SDL_Rect) { 0, 0, 0, 0 };

    self->color = (SDL_Color) {50, 140, 140, 255};
    self->colorHover = (SDL_Color) {30, 120, 120, 255};
    self->colorPressed = (SDL_Color) {60, 60, 60, 255};
    self->textColor = (SDL_Color) {255, 255, 255, 255};

    self->state = Normal;
    self->pressedEvent = (ButtonPressedEvent) {NULL, NULL};

    self->rect = (SDL_Rect) { .x = 0, .y = 0, .w = 60, .h = 40 };

    self->background = Rectangle_New(self->renderer, self->rect.w, self->rect.h);

    return self;
}

void Button_Delete(Button *const self)
{
    if (!self)
        return;

    Texture_Delete(self->textTexture);
    free(self);
}

void Button_SetBackgroundColor(Button *const self, const SDL_Color *color)
{
    if (color)
        self->color = *color;
}

void Button_SetBackgroundHoverColor(Button *const self, const SDL_Color *color)
{
    if (color)
        self->colorHover = *color;
}

void Button_SetBackgroundPressedColor(Button *const self, const SDL_Color *color)
{
    if (color)
        self->colorPressed = *color;
}

void Button_SetTextColor(Button *const self, const SDL_Color *color)
{
    if (color)
        self->textColor = *color;
}

bool Button_SetText(Button *const self, const char *text, int ptsize)
{
    if (!self->textTexture)
        self->textTexture = Texture_New(self->renderer);

    Texture_SetTextColor(self->textTexture, &self->textColor);
    Texture_SetTextSize(self->textTexture, ptsize);
    Texture_SetText(self->textTexture, text);

    if (Texture_MakeText(self->textTexture))
    {
        Button_UpdateTextureRect(self);
        return true;
    }

    return false;
}

void Button_SetImage(Button *const self, Texture *texture)
{
    if (texture)
    {
        self->imageTexture = texture;
        Button_UpdateTextureRect(self);
    }
}

void Button_SetRect(Button *const self, const SDL_Rect *rect)
{
    if (rect)
    {
        self->rect = *rect;

        Box_SetSize(Rectangle_Box(self->background), self->rect.w, self->rect.h);
        Box_SetPosition(Rectangle_Box(self->background), self->rect.x, self->rect.y);

        Button_UpdateTextureRect(self);
    }
}

void Button_SetOnPressEvent(Button *const self, ButtonOnPressEvent callback, void *user)
{
    self->pressedEvent.function = callback;
    self->pressedEvent.userdata = user;
}

void *Button_GetEventUserData(Button *const self)
{
    return self->pressedEvent.userdata;
}

void Button_ProcessEvent(Button *const self, const SDL_Event *event)
{
    if (event->button.button == SDL_BUTTON_LEFT && Button_PointerIsHovering(self, event))
    {
        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            self->state = Pressed;

            if (self->pressedEvent.function)
                self->pressedEvent.function(self, self->pressedEvent.userdata);

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

void Button_Draw(Button *const self)
{
    Button_Draw_(self);

    if (self->imageTexture)
        Texture_Draw(self->imageTexture, &self->textureRect);
}

void Button_DrawEx(Button *const self, const SDL_Rect *srcrect, const SDL_Rect *dstrect, const double angle)
{
    Button_Draw_(self);

    if (self->imageTexture)
    {
        // LEMBRETE: arrumar essa gambiarra (temporária)

        if (srcrect)
            Texture_SetSourceRect(self->imageTexture, *srcrect);

        Texture_SetAngle(self->imageTexture, angle);
        Texture_Draw(self->imageTexture, dstrect ? dstrect : &self->textureRect);
    }
}

void Button_Draw_(Button *const self)
{
    if (self->state == Hover)
        Rectangle_SetColor(self->background, self->colorHover);
    else if (self->state == Pressed)
        Rectangle_SetColor(self->background, self->colorPressed);
    else
        Rectangle_SetColor(self->background, self->color);

    Rectangle_Draw(self->background);

    if (self->textTexture)
        Texture_Draw(self->textTexture, &self->textureRect);
}

bool Button_PointerIsHovering(Button *const self, const SDL_Event *event)
{
    return event->button.x >= self->rect.x
            && event->button.x <= (self->rect.x + self->rect.w)
            && event->button.y >= self->rect.y
            && event->button.y <= (self->rect.y + self->rect.h);
}

void Button_UpdateTextureRect(Button *const self)
{
    Texture *texture = self->textTexture ? self->textTexture : self->imageTexture;

    if (texture)
    {
        int w = Texture_GetWidth(texture);
        int h = Texture_GetHeight(texture);

        self->textureRect = (SDL_Rect) {
            .x = self->rect.x + ((self->rect.w - w) / 2),
            .y = self->rect.y + ((self->rect.h - h) / 2),
            .w = w,
            .h = h
        };
    }
}
