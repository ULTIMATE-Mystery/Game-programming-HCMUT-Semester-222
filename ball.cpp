#include "ball.h"

long int sqrD(int x1, int y1, int x2, int y2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

Ball::Ball()
{
    this->posX = 250;
    this->posY = HEIGHT / 2;
    this->velocityX = this->velocityY = 0;
    this->radius = BALLSIZE;
    this->explosionRadius = 70;
    this->existFrame = BALLEXISTFRAME;
    this->travelFrame = 0;
}

void Ball::setPos(int posX, int posY)
{
    this->posX = posX;
    this->posY = posY;
}

void Ball::setVelocity(int velX, int velY)
{
    if(existFrame <= 0) return;
    this->velocityX = velX;
    this->velocityY = velY;
    this->travelFrame = 120;
}

bool Ball::isFree()
{
    if(existFrame > 0) return true;
    else return false;
}

void Ball::exist()
{
    // timer 7 secs
    if(existFrame <= 0) return;
    existFrame--;
    travelFrame--;
    // reset velocity when exploding
    if(existFrame == 0) travelFrame = 0;
    if(travelFrame == 0) velocityX = velocityY = 0;
    // ball's traveling
    if((velocityX == 0) && (velocityY == 0)) return;
    posX += velocityX;
    posY += velocityY;
}

void Ball::explode(Entity & entities)
{
    if(existFrame != 0) return;
    radius += 4;
    if(radius > explosionRadius)
    {
        existFrame = -1;
        radius = 0;
        damage(entities);
    }
}

void Ball::respawnBall()
{
    // timer 1 sec
    if(existFrame >= 0) return;
    existFrame--;
    // spawn the ball but can't be controled
    if(-existFrame == BALLSPAWNFRAME / 2)
    {
        posX = rand() % 300 + 101;     // random in range 101 - 400
        posY = HEIGHT / 2;
    }
    if(-existFrame > BALLSPAWNFRAME / 2)
    {
        radius = BALLSIZE + (BALLSPAWNFRAME + existFrame) / 3;
    }
    // free the ball, increase explosion radius
    if(-existFrame > BALLSPAWNFRAME)
    {
        radius = BALLSIZE;
        explosionRadius = (int)(1.3 * explosionRadius);
        existFrame = BALLEXISTFRAME;
    }
}

void Ball::damage(Entity &entities)
{
    if(explosionRadius > 800)
    {
        entities.scoreA += 4;
        entities.scoreB += 4;
        return;
    }
    // squared distance
    long int d;
    // tower A1, A3, B1, B3
    if(sqrD(posX, posY, entities.towerA1.x + 30, entities.towerA1.y + 30) < explosionRadius * explosionRadius) entities.scoreB += 1;
    if(sqrD(posX, posY, entities.towerA3.x + 30, entities.towerA3.y + 30) < explosionRadius * explosionRadius) entities.scoreB += 1;
    if(sqrD(posX, posY, entities.towerB1.x + 30, entities.towerB1.y + 30) < explosionRadius * explosionRadius) entities.scoreA += 1;
    if(sqrD(posX, posY, entities.towerB3.x + 30, entities.towerB3.y + 30) < explosionRadius * explosionRadius) entities.scoreA += 1;
    // tower A2, B2
    if(sqrD(posX, posY, entities.towerA2.x + 20, entities.towerA2.y + 20) < explosionRadius * explosionRadius) entities.scoreB += 2;
    if(sqrD(posX, posY, entities.towerB2.x + 20, entities.towerB2.y + 20) < explosionRadius * explosionRadius) entities.scoreA += 2;
}

void Ball::collision(Entity entities)
{
    if(velocityX == 0 && velocityY == 0) return;
    int nextX = posX + velocityX;
    int nextY = posY + velocityY;
    // left & right edges of inner yard collision
    if(entities.isInside(nextX, posY, entities.inner_yard) == false) velocityX = -velocityX;

    // with towers
    if(posY < 120)
    {
        // top edge of inner yard collision
        if(entities.isInside(posX, nextY, entities.inner_yard) == false) velocityY = -velocityY;
        // towerA2 collision
        if(entities.isInside(posX, nextY, entities.towerA2) == true) velocityY = -velocityY;
        if(entities.isInside(nextX, posY, entities.towerA2) == true) velocityX = -velocityX;
        return;
    }
    if(posY >= 120 && posY < 250)
    {
        // towerA1 collision
        if(entities.isInside(posX, nextY, entities.towerA1) == true) velocityY = -velocityY;
        if(entities.isInside(nextX, posY, entities.towerA1) == true) velocityX = -velocityX;
        // towerA3 collision
        if(entities.isInside(posX, nextY, entities.towerA3) == true) velocityY = -velocityY;
        if(entities.isInside(nextX, posY, entities.towerA3) == true) velocityX = -velocityX;
    }
    if(posY >= 550 && posY < 680)
    {
        // towerB1 collision
        if(entities.isInside(posX, nextY, entities.towerB1) == true) velocityY = -velocityY;
        if(entities.isInside(nextX, posY, entities.towerB1) == true) velocityX = -velocityX;
        // towerB3 collision
        if(entities.isInside(posX, nextY, entities.towerB3) == true) velocityY = -velocityY;
        if(entities.isInside(nextX, posY, entities.towerB3) == true) velocityX = -velocityX;
    }
    if(posY >= 680)
    {
        // bottom edge of inner yard collision
        if(entities.isInside(posX, nextY, entities.inner_yard) == false) velocityY = -velocityY;
        // towerB2 collision
        if(entities.isInside(posX, nextY, entities.towerB2) == true) velocityY = -velocityY;
        if(entities.isInside(nextX, posY, entities.towerB2) == true) velocityX = -velocityX;
        return;
    }
}