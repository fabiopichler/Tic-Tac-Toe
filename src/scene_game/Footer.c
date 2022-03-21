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
#include "../base/Box.h"

#include <malloc.h>

struct Footer
{
    SDL_Renderer *renderer;
    SceneGameRect *sceneGameRect;

    Button *restartButton;
    Texture *copyrightText;
};

void Footer_CreateRestartButton(Footer *const self);
void Footer_CreateCopyrightText(Footer *const self);

Footer *Footer_New(SDL_Renderer *renderer, SceneGameRect *sceneGameRect)
{
    Footer *const self = malloc(sizeof (Footer));

    self->renderer = renderer;
    self->sceneGameRect = sceneGameRect;

    Footer_CreateRestartButton(self);
    Footer_CreateCopyrightText(self);

    return self;
}

void Footer_Delete(Footer *const self)
{
    if (!self)
        return;

    Button_Delete(self->restartButton);
    Texture_Delete(self->copyrightText);

    free(self);
}

void Footer_ProcessEvent(Footer *const self, const SDL_Event *event)
{
    Button_ProcessEvent(self->restartButton, event);
}

void Footer_Draw(Footer *const self)
{
    Button_Draw(self->restartButton);
    Texture_Draw(self->copyrightText);
}

Button *Footer_GetRestartButton(Footer *const self)
{
    return self->restartButton;
}

void Footer_CreateRestartButton(Footer *const self)
{
    self->restartButton = Button_New(self->renderer);

    Button_SetText(self->restartButton, "Reiniciar", 16);

    int width = 110;
    int height = 32;
    int padding = 40;

    Box_SetSize(Button_Box(self->restartButton), width, height);
    Box_SetPosition(Button_Box(self->restartButton),
                    self->sceneGameRect->sidebar_w + ((self->sceneGameRect->content_w - width) / 2),
                    self->sceneGameRect->window_h - height - padding);
}

void Footer_CreateCopyrightText(Footer *const self)
{
    self->copyrightText = Texture_New(self->renderer);

    const char text[] = "© 2020 Fábio Pichler                               www.fabiopichler.net";

    Texture_SetupText(self->copyrightText, text, 14, &(SDL_Color) {30, 120, 120, 255});
    Texture_MakeText(self->copyrightText);

    int width = Texture_GetWidth(self->copyrightText);
    int height = Texture_GetHeight(self->copyrightText);
    int padding = 10;

    Box_SetSize(Texture_Box(self->copyrightText), width, height);
    Box_SetPosition(Texture_Box(self->copyrightText),
                    self->sceneGameRect->sidebar_w + ((self->sceneGameRect->content_w - width) / 2),
                    self->sceneGameRect->window_h - height - padding);
}
