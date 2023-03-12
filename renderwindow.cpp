#include <string>

#include "renderwindow.h"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        std::cout << "FAILED TO CREATE WINDOW. ERROR: " << SDL_GetError() << std::endl;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

//    if (!(TTF_Init()))
//		std::cout << "TTF_init HAS FAILED. Error: " << TTF_GetError() << '\n';
//    TTF_Init();

    color.r = 255;
    color.b = 255;
    color.g = 255;
    color.a = 255;
}

void RenderWindow::renderBackground(Entity entity)
{
    // render a white background, what remains of it will be the border of the yard
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // render the inner yard
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &(entity.inner_yard));
}

void RenderWindow::renderTowers(Entity entity)
{
    // render all towers with white color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &(entity.towerA1));
    SDL_RenderFillRect(renderer, &(entity.towerA2));
    SDL_RenderFillRect(renderer, &(entity.towerA3));
    SDL_RenderFillRect(renderer, &(entity.towerB1));
    SDL_RenderFillRect(renderer, &(entity.towerB2));
    SDL_RenderFillRect(renderer, &(entity.towerB3));
}

void RenderWindow::renderBall(Ball ball)
{
    if(ball.radius <= 0) return;
    const int32_t diameter = (ball.radius * 2);
    int32_t x = (ball.radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);
    // set ball color
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, ball.posX + x, ball.posY - y);
        SDL_RenderDrawPoint(renderer, ball.posX + x, ball.posY + y);
        SDL_RenderDrawPoint(renderer, ball.posX - x, ball.posY - y);
        SDL_RenderDrawPoint(renderer, ball.posX - x, ball.posY + y);
        SDL_RenderDrawPoint(renderer, ball.posX + y, ball.posY - x);
        SDL_RenderDrawPoint(renderer, ball.posX + y, ball.posY + x);
        SDL_RenderDrawPoint(renderer, ball.posX - y, ball.posY - x);
        SDL_RenderDrawPoint(renderer, ball.posX - y, ball.posY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }
        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void RenderWindow::renderText(Ball ball, Entity entity, TTF_Font* font)
{
    SDL_Surface* surface;
    SDL_Texture* texture;
    // render the side score board
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &(entity.side_board));
    // 2 teams score
    char c[2];
    color.b = 255;
    color.g = 255;
    // team A
    sprintf(c, "%d", (entity.scoreA % 100));
    surface = TTF_RenderText_Solid(font, c, color);
    if(entity.scoreA >= 10) entity.scoreFrameA.x = 530;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    entity.scoreFrameA.w = surface->w;
    entity.scoreFrameA.h = surface->h;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &(entity.scoreFrameA));

    // team B
    sprintf(c, "%d", (entity.scoreB % 100));
    surface = TTF_RenderText_Solid(font, c, color);
    if(entity.scoreA >= 10) entity.scoreFrameB.x = 530;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    entity.scoreFrameB.w = surface->w;
    entity.scoreFrameB.h = surface->h;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &(entity.scoreFrameB));

    // Ball timer
    if(ball.existFrame > 0)
    {
        int timerBall = ball.existFrame / 60 + 1;
        // color settings
        if(timerBall < 3) 
        {
            color.g = 0;
            color.b = 0;
        }
        char t[4];
        sprintf(t, "%d", timerBall);
        surface = TTF_RenderText_Solid(font, t, color);
        entity.timerFrame.x = 540;
    }
    else 
    {
        color.g = 0;
        color.b = 0;
        surface = TTF_RenderText_Solid(font, "BUZZ", color);
        entity.timerFrame.x = 510;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    entity.timerFrame.w = surface->w;
    entity.timerFrame.h = surface->h;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &(entity.timerFrame));
    SDL_DestroyTexture(texture);
}

void RenderWindow::renderIndicator(Ball ball, int vx, int vy)
{
    if(vx == 0 && vy == 0) return;
    int x = ball.posX, y = ball.posY;
//    SDL_RenderDrawPoint(renderer, x + 2 * vx, y + 2 * vy);
    SDL_RenderDrawLine(renderer, x + vx, y + vy, x + 2 * vx, y + 2 * vy);
}

void RenderWindow::cleanUp()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}