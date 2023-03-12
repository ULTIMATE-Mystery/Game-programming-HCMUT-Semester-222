#include <SDL2/SDL.h>

#include "global.h"

class Entity
{
private:
    SDL_Rect inner_yard, side_board;
    SDL_Rect towerA1, towerA2, towerA3;     // top towers
    SDL_Rect towerB1, towerB2, towerB3;     // bottom towers
    SDL_Rect scoreFrameA, scoreFrameB;
    int scoreA, scoreB;
    SDL_Rect timerFrame;
public:
    Entity();
    bool isInside(int posX, int posY, SDL_Rect rect);
    friend class RenderWindow;
    friend class Ball;
};