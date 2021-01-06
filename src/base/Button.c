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

    State state;

    ButtonPressedEvent pressedEvent;
};

void Button_Draw_(Button *const this);
bool Button_PointerIsHovering(Button *const this, const SDL_Event *event);
void Button_UpdateTextureRect(Button *const this);

Button *Button_New(SDL_Renderer *renderer)
{
    Button *const this = malloc(sizeof (Button));

    this->renderer = renderer;
    this->textTexture = NULL;
    this->imageTexture = NULL;
    this->textureRect = (SDL_Rect) { 0, 0, 0, 0 };

    this->color = (SDL_Color) {50, 140, 140, 255};
    this->colorHover = (SDL_Color) {30, 120, 120, 255};
    this->colorPressed = (SDL_Color) {60, 60, 60, 255};
    this->textColor = (SDL_Color) {255, 255, 255, 255};

    this->state = Normal;
    this->pressedEvent = (ButtonPressedEvent) {NULL, NULL};

    this->rect = (SDL_Rect) { .x = 0, .y = 0, .w = 60, .h = 40 };

    return this;
}

void Button_Delete(Button *const this)
{
    if (!this)
        return;

    Texture_Delete(this->textTexture);
    free(this);
}

void Button_SetBackgroundColor(Button *const this, const SDL_Color *color)
{
    if (color)
        this->color = *color;
}

void Button_SetBackgroundHoverColor(Button *const this, const SDL_Color *color)
{
    if (color)
        this->colorHover = *color;
}

void Button_SetBackgroundPressedColor(Button *const this, const SDL_Color *color)
{
    if (color)
        this->colorPressed = *color;
}

void Button_SetTextColor(Button *const this, const SDL_Color *color)
{
    if (color)
        this->textColor = *color;
}

bool Button_SetText(Button *const this, const char *text, int ptsize)
{
    if (!this->textTexture)
        this->textTexture = Texture_New(this->renderer);

    Texture_SetTextColor(this->textTexture, &this->textColor);
    Texture_SetTextSize(this->textTexture, ptsize);
    Texture_SetText(this->textTexture, text);

    if (Texture_MakeText(this->textTexture))
    {
        Button_UpdateTextureRect(this);
        return true;
    }

    return false;
}

void Button_SetImage(Button *const this, Texture *texture)
{
    if (texture)
    {
        this->imageTexture = texture;
        Button_UpdateTextureRect(this);
    }
}

void Button_SetRect(Button *const this, const SDL_Rect *rect)
{
    if (rect)
    {
        this->rect = *rect;
        Button_UpdateTextureRect(this);
    }
}

void Button_SetOnPressEvent(Button *const this, ButtonOnPressEvent callback, void *user)
{
    this->pressedEvent.function = callback;
    this->pressedEvent.userdata = user;
}

void *Button_GetEventUserData(Button *const this)
{
    return this->pressedEvent.userdata;
}

void Button_ProcessEvent(Button *const this, const SDL_Event *event)
{
    if (event->button.button == SDL_BUTTON_LEFT && Button_PointerIsHovering(this, event))
    {
        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            this->state = Pressed;

            if (this->pressedEvent.function)
                this->pressedEvent.function(this, this->pressedEvent.userdata);

            return;
        }
        else if (this->state == Pressed && event->type != SDL_MOUSEBUTTONUP)
        {
            return;
        }
    }

    if (Button_PointerIsHovering(this, event))
        this->state = Hover;
    else
        this->state = Normal;
}

void Button_Draw(Button *const this)
{
    Button_Draw_(this);

    if (this->imageTexture)
        Texture_Draw(this->imageTexture, NULL, &this->textureRect);
}

void Button_DrawEx(Button *const this, const SDL_Rect *srcrect, const SDL_Rect *dstrect, const double angle)
{
    Button_Draw_(this);

    if (this->imageTexture)
        Texture_DrawEx(this->imageTexture, srcrect, dstrect ? dstrect : &this->textureRect, angle);
}

void Button_Draw_(Button *const this)
{
    if (this->state == Hover)
    {
        SDL_SetRenderDrawColor(this->renderer, this->colorHover.r,
                               this->colorHover.g, this->colorHover.b, this->colorHover.a);
    }
    else if (this->state == Pressed)
    {
        SDL_SetRenderDrawColor(this->renderer, this->colorPressed.r,
                               this->colorPressed.g, this->colorPressed.b, this->colorPressed.a);
    }
    else
    {
        SDL_SetRenderDrawColor(this->renderer, this->color.r, this->color.g, this->color.b, this->color.a);
    }

    SDL_RenderFillRect(this->renderer, &this->rect);

    if (this->textTexture)
        Texture_Draw(this->textTexture, NULL, &this->textureRect);
}

bool Button_PointerIsHovering(Button *const this, const SDL_Event *event)
{
    return event->button.x >= this->rect.x
            && event->button.x <= (this->rect.x + this->rect.w)
            && event->button.y >= this->rect.y
            && event->button.y <= (this->rect.y + this->rect.h);
}

void Button_UpdateTextureRect(Button *const this)
{
    Texture *texture = this->textTexture ? this->textTexture : this->imageTexture;

    if (texture)
    {
        int w = Texture_GetWidth(texture);
        int h = Texture_GetHeight(texture);

        this->textureRect = (SDL_Rect) {
            .x = this->rect.x + ((this->rect.w - w) / 2),
            .y = this->rect.y + ((this->rect.h - h) / 2),
            .w = w,
            .h = h
        };
    }
}
