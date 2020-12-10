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

#include "Footer.h"
#include "../base/Button.h"
#include "../base/Texture.h"

#include <malloc.h>

struct Footer
{
    SDL_Renderer *renderer;
    SceneGameRect *rect;

    Button *restartButton;
    Texture *copyrightText;
};

void Footer_CreateRestartButton(Footer *const this);
void Footer_CreateCopyrightText(Footer *const this);

Footer *Footer_New(SDL_Renderer *renderer, SceneGameRect *rect)
{
    Footer *const this = malloc(sizeof (Footer));

    this->renderer = renderer;
    this->rect = rect;

    Footer_CreateRestartButton(this);
    Footer_CreateCopyrightText(this);

    return this;
}

void Footer_Delete(Footer *const this)
{
    if (!this)
        return;

    Button_Delete(this->restartButton);
    Texture_Delete(this->copyrightText);
    free(this);
}

void Footer_ProcessEvent(Footer *const this, const SDL_Event *event)
{
    Button_ProcessEvent(this->restartButton, event);
}

void Footer_Draw(Footer *const this)
{
    Button_Draw(this->restartButton);
    Texture_Draw(this->copyrightText, NULL, NULL);
}

Button *Footer_GeRestartButton(Footer *const this)
{
    return this->restartButton;
}

void Footer_CreateRestartButton(Footer *const this)
{
    this->restartButton = Button_New(this->renderer);

    Button_SetText(this->restartButton, "Reiniciar", 16);

    int width = 110;
    int height = 32;
    int padding = 40;

    Button_SetRect(
                this->restartButton,
                (SDL_Rect) {
                    .x = this->rect->sidebar_w + ((this->rect->content_w - width) / 2),
                    .y = this->rect->window_h - height - padding,
                    .w = width,
                    .h = height
                });
}

void Footer_CreateCopyrightText(Footer *const this)
{
    this->copyrightText = Texture_New(this->renderer);

    const char text[] = "© 2020 Fábio Pichler                               www.fabiopichler.net";

    Texture_SetupText(this->copyrightText, text, 14, (SDL_Color) {30, 120, 120, 255});
    Texture_MakeText(this->copyrightText);

    int width = Texture_GetWidth(this->copyrightText);
    int height = Texture_GetHeight(this->copyrightText);
    int padding = 10;

    Texture_SetRect(this->copyrightText, (SDL_Rect) {
                        .x = this->rect->sidebar_w + ((this->rect->content_w - width) / 2),
                        .y = this->rect->window_h - height - padding,
                        .w = width,
                        .h = height
                    });
}
