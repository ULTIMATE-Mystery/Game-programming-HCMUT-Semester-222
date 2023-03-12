#include "entity.h"

Entity::Entity()
{
    // initiate the side score board
    side_board.w = 95;
    side_board.h = HEIGHT - 10;
    side_board.x = WIDTH - side_board.w - 5;
    side_board.y = 5;
    // initiate the inner yard
    inner_yard.x = 5;
    inner_yard.y = 5;
    inner_yard.w = WIDTH - 3 * inner_yard.x - side_board.w;
    inner_yard.h = HEIGHT - 2 * inner_yard.y;
    // initiate the towers's size of 2 teams
    towerA1.w = towerA3.w = towerB1.w = towerB3.w = 60;
    towerA1.h = towerA3.h = towerB1.h = towerB3.h = 60;
    towerA2.w = towerA2.h = towerB2.w = towerB2.h = 40; 
    // initiate the towers's location of 2 teams
    towerA1.x = towerB1.x = 50;
    towerA1.y = towerA3.y = 150;
    towerA3.x = towerB3.x = WIDTH - 110 - side_board.w;
    towerB1.y = towerB3.y = HEIGHT - 210;
    towerA2.x = towerB2.x = 230;
    towerA2.y = 50;
    towerB2.y = 710;
    // 2 teams score
    scoreA = scoreB = 0;
    // text frames
    timerFrame.x = 540;
    timerFrame.y = HEIGHT / 2;
    scoreFrameA.x = scoreFrameB.x = 540;
    scoreFrameA.y = HEIGHT / 8;
    scoreFrameB.y = HEIGHT / 8 * 7;
}

bool Entity::isInside(int posX, int posY, SDL_Rect rect)
{
    if((posX < rect.x) || (posX > rect.x + rect.w)) return false;
    if((posY < rect.y) || (posY > rect.y + rect.h)) return false;
    return true;
}