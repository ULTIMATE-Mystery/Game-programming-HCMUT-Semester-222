#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <vector>

#define WIDTH	800
#define HEIGHT	600
#define FONTSIZE    32

using namespace std;

SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* font;
SDL_Color color;

bool gameRunning = true;
bool mouseDown = false;     // used to calculate force
bool mousePressed = false;

int frameCount,             // count number of frame in a sec
    timerFPS,               // duration of current frame
    lastFrame,              // time of the last frame
    fps;

SDL_Rect t_rect, b_rect, l_rect, r_rect;
SDL_Rect inner_yard;

float ballX = 400, ballY = 300, 
      ballRadius = 8;       // also the explosion radius
float velX = 0, velY = 0;
int pos1x = 0, pos1y = 0, pos2x = 0, pos2y = 0;

SDL_Event event;

// Draw ball and explosion
void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    if(radius <= 0) return;
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

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

// initiate statistics of the yard, vision-blocking blocks
void blockInit()
{  
    // top
    t_rect.x = t_rect.y = 0;
    t_rect.w = WIDTH;
    t_rect.h = HEIGHT / 12 - 5;
    // bottom
    b_rect.x = 0;
    b_rect.y = HEIGHT - t_rect.h;
    b_rect.w = WIDTH;
    b_rect.h = t_rect.h;
    // left
    l_rect.x = l_rect.y = 0;
    l_rect.w = WIDTH / 4 - 5;
    l_rect.h = HEIGHT;
    // right
    r_rect.x = WIDTH - l_rect.w;
    r_rect.y = 0;
    r_rect.h = HEIGHT;
    r_rect.w = l_rect.w;
    // inner yard
    inner_yard.x = WIDTH / 4;
    inner_yard.y = HEIGHT / 12;
    inner_yard.w = WIDTH / 2;
    inner_yard.h = HEIGHT * 10 / 12;
}

// Collision with 4 edges of the yard
void collision()
{
    // top & bottom edge collision
    if ((ballY <= inner_yard.y + ballRadius) || (ballY >= inner_yard.y + inner_yard.h - ballRadius)) 
    {
        velY = -velY;
        ballY += velY / 69.0;   // push away immediately to prevent bugs
    }
    // left & right edge collitsion
    if ((ballX <= inner_yard.x + ballRadius) || (ballX >= inner_yard.x + inner_yard.w - ballRadius))
    {
        velX = -velX;
        ballX += velX / 69.0;;   // push away immediately to prevent bugs
    }
}

// called per frame
void update()
{
    static int ballTravelFrame = -1;
    mousePressed = false;
    // get controls and events
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
                // get the force vector
                SDL_GetMouseState(&pos2x, &pos2y);
                velX = pos2x - pos1x;
                velY = pos2y - pos1y;
                // set number of ball existing frames
                ballTravelFrame = 180;
			}
			break;
        default:
            break;
        }
    }

    // ball physics
    if(mousePressed == true)
    {
        SDL_GetMouseState(&pos1x, &pos1y);
    }

    collision();

    // ball is traveling
    if (ballTravelFrame > 0)
    {
        ballX += velX / 50.0 * (ballTravelFrame + 20) / 200;
        ballY += velY / 50.0 * (ballTravelFrame + 20) / 200;
        ballTravelFrame--;
    }
    // ball is exploding
    if(ballTravelFrame == 0)
    {
        ballRadius += 3;
        if(ballRadius > 150)
        {
            ballTravelFrame = -1;
            ballX = 400;
            ballY = 300;
            ballRadius = 8;
        }
    }

}

void render()
{
    // render a white background, what remains of it will be the border of the yard
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    frameCount++;
    timerFPS = SDL_GetTicks() - lastFrame;
    if(timerFPS < (1000 / 60))
        SDL_Delay((1000/60) - timerFPS);

    // draw inner yard
    color.r = 0;
    color.g = 0;
    color.b = 0;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &inner_yard);

    // draw the ball
    color.r = 255;
    color.g = 0;
    color.b = 0;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    DrawCircle(renderer, ballX, ballY, ballRadius);

    // draw outer rectangle to block the visual of the explosion
    color.r = 0;
    color.g = 0;
    color.b = 0;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &t_rect);
    SDL_RenderFillRect(renderer, &b_rect);
    SDL_RenderFillRect(renderer, &l_rect);
    SDL_RenderFillRect(renderer, &r_rect);

    SDL_RenderPresent(renderer);
}

int main(int argv, char** args)
{
    // init library
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) 
        cout << "SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << endl;
	if (!(IMG_Init(IMG_INIT_PNG)))
		cout << "IMG_init HAS FAILED. Error: " << SDL_GetError() << endl;
//	if (!(TTF_Init()))
//		cout << "TTF_init HAS FAILED. Error: " << SDL_GetError() << endl;
    if(SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0)
        cout << "FAILED TO CREATE WINDOW. Error: " << SDL_GetError() << endl;

//    font = TTF_OpenFont("Peepo.ttf", FONTSIZE);
    
    gameRunning = true;
    static int lastTime = 0;

    blockInit();

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

//  TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}