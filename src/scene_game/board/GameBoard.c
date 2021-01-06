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

#include "GameBoard.h"
#include "../../base/Button.h"
#include "../../base/Texture.h"
#include "board_util.h"

typedef struct GameEvent
{
    GameEventHandler function;
    void *userdata;
} GameEvent;

struct GameBoard
{
    const int item_size;
    const int board_space;
    const int board_x;
    const int board_y;
    const SDL_Rect board_rect;

    SDL_Renderer *renderer;
    const SceneGameRect *rect;

    Player player;
    Player gameResult;
    int round;
    BoardItem board[3][3];

    GameEvent gameEvent;

    Texture *player1Texture;
    Texture *player2Texture;

    double p1Angle;
};

void GameBoard_SetupBoard(GameBoard *const this);
void GameBoard_OnItemPress(Button *const button, void *user);
void GameBoard_Check(GameBoard *const this, BoardItem *item);
Player GameBoard_CheckWinner(GameBoard *const this);

GameBoard *GameBoard_New(SDL_Renderer *renderer, SceneGameRect *rect)
{
    GameBoard *const this = malloc(sizeof (GameBoard));

    const int board_size = 304;

    *(int *)&this->item_size = 98;
    *(int *)&this->board_space = 5;
    *(int *)&this->board_x = rect->sidebar_w + ((rect->content_w - board_size) / 2);
    *(int *)&this->board_y = (rect->window_h - board_size) / 2;
    *(SDL_Rect *)&this->board_rect = (SDL_Rect) {this->board_x, this->board_y, board_size, board_size};

    this->renderer = renderer;
    this->rect = rect;
    this->player = Player_1;
    this->gameResult = None;
    this->round = 0;
    this->gameEvent = (GameEvent) {NULL, NULL};
    this->player1Texture = Texture_New(renderer);
    this->player2Texture = Texture_New(renderer);
    this->p1Angle = 0.0;

    Texture_LoadImageFromFile(this->player1Texture, "images/player_1.png");
    Texture_LoadImageFromFile(this->player2Texture, "images/player_2.png");

    GameBoard_SetupBoard(this);

    return this;
}

void GameBoard_Delete(GameBoard *const this)
{
    if (!this)
        return;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            Button_Delete(this->board[i][j].button);

    Texture_Delete(this->player1Texture);
    Texture_Delete(this->player2Texture);
    free(this);
}

void GameBoard_ProcessEvent(GameBoard *const this, const SDL_Event *event)
{
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            Button_ProcessEvent(this->board[row][col].button, event);
}

void GameBoard_Update(GameBoard *const this, double deltaTime)
{
    this->p1Angle = this->p1Angle + 0.03 * deltaTime;

    if (this->p1Angle > 360)
        this->p1Angle = 0;
}

void GameBoard_Draw(GameBoard *const this)
{
    SDL_SetRenderDrawColor(this->renderer, 80, 160, 160, 255);
    SDL_RenderFillRect(this->renderer, &this->board_rect);

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            BoardItem *item = &this->board[row][col];

            Button_DrawEx(item->button, NULL, NULL, item->player == Player_1 ? this->p1Angle : 0);
        }
    }
}

void GameBoard_SetGameEvent(GameBoard *const this, GameEventHandler callback, void *user)
{
    this->gameEvent.function = callback;
    this->gameEvent.userdata = user;
}

int GameBoard_GetCurrentPlayer(GameBoard *const this)
{
    return this->player;
}

int GameBoard_GetGameResult(GameBoard *const this)
{
    return this->gameResult;
}

void GameBoard_SetupBoard(GameBoard *const this)
{
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            BoardItem *item = &this->board[row][col];
            *item = (BoardItem) { .player = 0, .button = Button_New(this->renderer) };

            Button_SetRect(
                        item->button,
                        &(SDL_Rect) {
                            .x = this->board_x + (col * this->item_size) + (col * this->board_space),
                            .y = this->board_y + (row * this->item_size) + (row * this->board_space),
                            .w = this->item_size,
                            .h = this->item_size
                        });

            Button_SetOnPressEvent(item->button, GameBoard_OnItemPress, this);
            Button_SetBackgroundColor(item->button, &(SDL_Color) {210, 240, 240, 255});
            Button_SetBackgroundHoverColor(item->button, &(SDL_Color) {225, 255, 255, 255});
            Button_SetBackgroundPressedColor(item->button, &(SDL_Color) {180, 230, 230, 255});
        }
    }
}

void GameBoard_OnItemPress(Button *const button, void *user)
{
    GameBoard *this = user;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            BoardItem *item = &this->board[row][col];

            if (item->button == button)
            {
                GameBoard_Check(this, item);
                return;
            }
        }
    }
}

void GameBoard_Check(GameBoard *const this, BoardItem *item)
{
    if (item->player != None || this->gameResult != None)
        return;

    item->player = this->player;

    this->gameResult = GameBoard_CheckWinner(this);

    this->player = this->player == Player_1 ? Player_2 : Player_1;

    if (this->gameEvent.function)
        this->gameEvent.function(this, this->gameEvent.userdata);

    Button_SetImage(item->button, item->player == Player_1 ? this->player1Texture : this->player2Texture);

    this->round++;
}

Player GameBoard_CheckWinner(GameBoard *const this)
{
    Player player = CheckBoardRows(3, this->board);

    if (player == None)
    {
        BoardItem board[3][3];
        TransposeBoard(this->board, board);
        player = CheckBoardRows(3, board);
    }

    if (player == None)
        player = CheckBoardDiagonals(this->board);

    if (player > None)
        return player;

    if (player == None && this->round == 8)
        return Tied;

    return None;
}
