#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

#include "global.h"
//#include "entity.h"
#include "ball.h"

class RenderWindow
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Color color;
public:
    RenderWindow(const char* p_title, int p_w, int p_h);
    void renderBackground(Entity entity);
    void renderTowers(Entity entity);
    void renderBall(Ball ball);
    void renderText(Ball ball, Entity entity, TTF_Font* font);
    void renderIndicator(Ball ball, int x, int y);
    void cleanUp();
    void clear();
    void display();
};