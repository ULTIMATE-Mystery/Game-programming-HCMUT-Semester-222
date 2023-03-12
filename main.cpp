#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <vector>

//#include "entity.h"
//#include "ball.h"
#include "renderwindow.h"

RenderWindow window("Fuck my life", WIDTH, HEIGHT);
Ball p_ball;
Entity entities = Entity();

bool gameRunning = true;
bool mouseDown = false;     // used to calculate force
bool mousePressed = false;

int frameCount,             // count number of frame in a sec
    timerFPS,               // duration of current frame
    lastFrame,              // time of the last frame
    fps;

int pos1x = 0, pos1y = 0, pos2x = 0, pos2y = 0;
int velX = 0, velY = 0;

SDL_Event event;
TTF_Font *font;

// called per frame
void update()
{
    // cap at 60 FPS
    frameCount++;
    timerFPS = SDL_GetTicks() - lastFrame;
    if(timerFPS < (1000 / 60))
        SDL_Delay((1000 / 60) - timerFPS);

    // get controls and events
    mousePressed = false;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            gameRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				mouseDown = true;
				mousePressed = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				mouseDown = false;
                // get released mouse' coordinate
                SDL_GetMouseState(&pos2x, &pos2y);
                if(p_ball.isFree() == true)
                {
                    int vx = (pos2x - pos1x) / 50;
                    int vy = (pos2y - pos1y) / 50;
                    p_ball.setVelocity(vx, vy);
                }
			}
			break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_LEFT:
                if(velX == 7) velX = 0;
                else velX = -7;
                std::cout << "left ";
                break;
            case SDLK_RIGHT:
                if(velX == -7) velX = 0;
                else velX = 7;
                std::cout << "right ";
                break;
            case SDLK_UP:
                if(velY == 7) velY = 0;
                else velY = -7;
                std::cout << "up ";
                break;
            case SDLK_DOWN:
                if(velY == -7) velY = 0;
                else velY = 7;
                std::cout << "down ";
                break;
            case SDLK_SPACE:
                if(velX != 0)
                {
                    if(velY != 0) p_ball.setVelocity(velX * 5 / 7, velY * 5 / 7);
                    else p_ball.setVelocity(velX, 0);
                }
                else p_ball.setVelocity(0, velY);
                std::cout << "out\n";
                velX = velY = 0;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    // get pressed mouse' coordinate
    if(mousePressed == true)
    {
        SDL_GetMouseState(&pos1x, &pos1y);
    }

    // calculate collision with defined entities
    p_ball.collision(entities);

    int frame = p_ball.getFrame();
    if(frame > 0) p_ball.exist();
    else if(frame == 0) p_ball.explode(entities);
    else p_ball.respawnBall();
}

void render()
{
    window.clear();
    window.renderBackground(entities);
    window.renderBall(p_ball);
    window.renderIndicator(p_ball, velX, velY);
    window.renderTowers(entities);
    window.renderText(p_ball, entities, font);
    window.display();
}

int main(int argv, char** args)
{
    // init library
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) 
        std::cout << "SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << '\n';
	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_init HAS FAILED. Error: " << SDL_GetError() << '\n';
    TTF_Init();

    font = TTF_OpenFont("res/font/Peepo.ttf", FONTSIZE);
    gameRunning = true;
    static int lastTime = 0;

    p_ball = Ball();

    while(gameRunning)
    {
        lastFrame = SDL_GetTicks();
        if(lastFrame >= (lastTime + 1000))
        {
            lastTime = lastFrame;
            fps = frameCount;
            frameCount = 0;
        }

        update();
        render();
    }

    TTF_CloseFont(font);
    window.cleanUp();
    SDL_Quit();
    return 1;
}