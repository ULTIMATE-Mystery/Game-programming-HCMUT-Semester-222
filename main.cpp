#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <vector>

#define WIDTH	500
#define HEIGHT	800
#define FONTSIZE    32

#define BALLEXISTFRAME      300
#define BALLSPAWNFRAME      60
#define BALLSIZE            8

using namespace std;

TTF_Font* font;
SDL_Color color;

bool gameRunning = true;
bool mouseDown = false;     // used to calculate force
bool mousePressed = false;

int frameCount,             // count number of frame in a sec
    timerFPS,               // duration of current frame
    lastFrame,              // time of the last frame
    fps;

int pos1x = 0, pos1y = 0, pos2x = 0, pos2y = 0;

SDL_Event event;

// define a general entity class
class Entity
{
private:
    SDL_Rect inner_yard;
    SDL_Rect towerA1, towerA2, towerA3;     // top towers
    SDL_Rect towerB1, towerB2, towerB3;     // bottom towers
public:
    Entity()
    {
        // initiate the inner yard
        inner_yard.x = 5;
        inner_yard.y = 5;
        inner_yard.w = WIDTH - 2 * inner_yard.x;
        inner_yard.h = HEIGHT - 2 * inner_yard.y;
        // initiate the towers's size of 2 teams
        towerA1.w = towerA3.w = towerB1.w = towerB3.w = 60;
        towerA1.h = towerA3.h = towerB1.h = towerB3.h = 60;
        towerA2.w = towerA2.h = towerB2.w = towerB2.h = 40; 
        // initiate the towers's location of 2 teams
        towerA1.x = towerB1.x = 50;
        towerA1.y = towerA3.y = 150;
        towerA3.x = towerB3.x = WIDTH - 110;
        towerB1.y = towerB3.y = HEIGHT - 210;
        towerA2.x = towerB2.x = 230;
        towerA2.y = 50;
        towerB2.y = 710;
    }
    bool isInside(int posX, int posY, SDL_Rect rect)
    {
        if((posX < rect.x) || (posX > rect.x + rect.w)) return false;
        if((posY < rect.y) || (posY > rect.y + rect.h)) return false;
        return true;
    }
    friend class RenderWindow;
    friend class Ball;
};

// define class Ball
class Ball
{
private:
    int posX, posY;
    int velocityX, velocityY;
    int radius;
    int explosionRadius;
    int existFrame;             
public:
    Ball()
    {
        this->posX = WIDTH / 2;
        this->posY = HEIGHT / 2;
        this->velocityX = this->velocityY = 0;
        this->radius = BALLSIZE;
        this->explosionRadius = 70;
        this->existFrame = BALLEXISTFRAME;
    }
    void setPos(int posX, int posY)
    {
        this->posX = posX;
        this->posY = posY;
    }
    void setVelocity(int velX, int velY)
    {
        this->velocityX = velX;
        this->velocityY = velY;
    }
    bool isFree()
    {
        if(existFrame > 0) return true;
        else return false;
    }
    int getFrame()
    {
        return existFrame;
    }
    void travel()
    {
        // timer 7 secs
        if(existFrame <= 0) return;
        existFrame--;
        // reset velocity when exploding
        if(existFrame == 0)
        {
            velocityX = velocityY = 0;
            std::cout << "explode\n";
        }
        // ball's traveling
        if((velocityX == 0) && (velocityY == 0)) return;
        posX += velocityX;
        posY += velocityY;
    }
    void explode()
    {
        if(existFrame != 0) return;
        radius += 3;
        if(radius > explosionRadius)
        {
            existFrame = -1;
            radius = 0;
            std::cout << "respawn\n";
        }
    }
    void respawnBall()
    {
        // timer 1 sec
        if(existFrame >= 0) return;
        existFrame--;
        // spawn the ball but can't be controled
        if(-existFrame == BALLSPAWNFRAME / 2)
        {
            posX = rand() % (WIDTH - 100 * 2) + 101;     // random in range 101 - 400
            posY = HEIGHT / 2;
            std::cout << "random\n";
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
            std::cout << "free\n";
        }
    }
    void collision(Entity entity)
    {
        if(velocityX == 0 && velocityY == 0) return;
        int nextX = posX + velocityX;
        int nextY = posY + velocityY;
        // left & right edges of inner yard collision
        if(entity.isInside(nextX, posY, entity.inner_yard) == false) velocityX = -velocityX;

        // with towers
        if(posY < 120)
        {
            // top edge of inner yard collision
            if(entity.isInside(posX, nextY, entity.inner_yard) == false) velocityY = -velocityY;
            // towerA2 collision
            if(entity.isInside(posX, nextY, entity.towerA2) == true) velocityY = -velocityY;
            if(entity.isInside(nextX, posY, entity.towerA2) == true) velocityX = -velocityX;
            return;
        }
        if(posY >= 120 && posY < 250)
        {
            // towerA1 collision
            if(entity.isInside(posX, nextY, entity.towerA1) == true) velocityY = -velocityY;
            if(entity.isInside(nextX, posY, entity.towerA1) == true) velocityX = -velocityX;
            // towerA3 collision
            if(entity.isInside(posX, nextY, entity.towerA3) == true) velocityY = -velocityY;
            if(entity.isInside(nextX, posY, entity.towerA3) == true) velocityX = -velocityX;
        }
        if(posY >= 550 && posY < 680)
        {
            // towerB1 collision
            if(entity.isInside(posX, nextY, entity.towerB1) == true) velocityY = -velocityY;
            if(entity.isInside(nextX, posY, entity.towerB1) == true) velocityX = -velocityX;
            // towerB3 collision
            if(entity.isInside(posX, nextY, entity.towerB3) == true) velocityY = -velocityY;
            if(entity.isInside(nextX, posY, entity.towerB3) == true) velocityX = -velocityX;
        }
        if(posY >= 680)
        {
            // bottom edge of inner yard collision
            if(entity.isInside(posX, nextY, entity.inner_yard) == false) velocityY = -velocityY;
            // towerB2 collision
            if(entity.isInside(posX, nextY, entity.towerB2) == true) velocityY = -velocityY;
            if(entity.isInside(nextX, posY, entity.towerB2) == true) velocityX = -velocityX;
            return;
        }
    }
    friend class RenderWindow;
};

// define class RenderWindow
class RenderWindow
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
public:
    RenderWindow(const char* p_title, int p_w, int p_h)
    {
    	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            std::cout << "FAILED TO CREATE WINDOW. ERROR: " << SDL_GetError() << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
    void renderBackground(Entity entity)
    {
        // render a white background, what remains of it will be the border of the yard
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // render the inner yard
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &(entity.inner_yard));
    }
    void renderTowers(Entity entity)
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
    void renderBall(Ball ball)
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
    void cleanUp()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
    void clear()
    {
        SDL_RenderClear(renderer);
    }
    void display()
    {
        SDL_RenderPresent(renderer);
    }
};

RenderWindow window("Fuck my life", WIDTH, HEIGHT);
Ball p_ball;
Entity entities = Entity();

// called per frame
void update()
{
    // cap at 60 FPS
    frameCount++;
    timerFPS = SDL_GetTicks() - lastFrame;
    if(timerFPS < (1000 / 60))
        SDL_Delay((1000/60) - timerFPS);

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
                    int velX = (pos2x - pos1x) / 50;
                    int velY = (pos2y - pos1y) / 50;
                    p_ball.setVelocity(velX, velY);
                }
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
    if(frame > 0) p_ball.travel();
    else if(frame == 0) p_ball.explode();
    else p_ball.respawnBall();
}

void render()
{
    window.clear();
    window.renderBackground(entities);
    window.renderBall(p_ball);
    window.renderTowers(entities);
    window.display();
}

int main(int argv, char** args)
{
    // init library
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) 
        std::cout << "SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << endl;
	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_init HAS FAILED. Error: " << SDL_GetError() << endl;
//	if (!(TTF_Init()))
//		cout << "TTF_init HAS FAILED. Error: " << SDL_GetError() << endl;
//    font = TTF_OpenFont("Peepo.ttf", FONTSIZE);
    
    gameRunning = true;
    static int lastTime = 0;
    int count = 0;

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

        count = (count + 1) % 20;
        if(count == 0)
        {
            std::cout << p_ball.getFrame() << endl;
        }

        update();
        render();
    }

//  TTF_CloseFont(font);
    window.cleanUp();
    SDL_Quit();
}