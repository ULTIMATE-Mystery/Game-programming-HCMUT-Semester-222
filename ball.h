#include <SDL2/SDL.h>

#include <iostream>

#include "global.h"
#include "entity.h"

class Ball
{
private:
    int posX, posY;
    int velocityX, velocityY;
    int radius, explosionRadius;
    int existFrame, travelFrame;
public:
    Ball();
    void setPos(int posX, int posY);
    void setVelocity(int velX, int velY);
    bool isFree();
    int getFrame()
    {
        return existFrame;
    }
    void exist();
    void explode(Entity & entities);
    void respawnBall();
    void damage(Entity & entities);
    void collision(Entity entities);
    friend class RenderWindow;
};