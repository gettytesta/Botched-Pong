#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <time.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Frees media and closes SDL
void close();

//Prints the "net" to the screen
void createNet();

//Prints score to the screen
void printScore(int p1, int p2);

//Crap "net"
SDL_Rect net = { 319, 0, 2, 15 };

//Resets the position of the ball after a point is scored
void resetBall(struct Ball* ball);

//Prints the Ball to the screen
void createBall(struct Ball* circle);

//Changes the X & Y coords. of the ball
void moveBall(struct Ball* ball, int collisionCheck);

//Checks for collisions
int checkPhysics(struct Ball* ball, SDL_Rect* lPaddle, SDL_Rect* rPaddle);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//The surface rendered to the screen
SDL_Surface* gSurface = NULL;

//Movement offsets for ball
int mX = 2;
int mY = 2;

//A countdown for the ball to start moving after it respawns
int countDown;

//Frame gap between moving the ball
int ballDelay = 1;

//Counts how many times the ball hit the paddle
int hitCount;

//Number of hits before a change in speed
const int hitChange = 6;

//Player 1 & 2 score
int score1 = 0;
int score2 = 0;

bool winState = false;

struct Ball {
	int x, y;
	int r;
};

bool init()
{
	bool success = true;

	//Init. SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		success = false;
		printf("Failed to initialize the video! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window 
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			success = false;
			printf("The window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			gSurface = SDL_GetWindowSurface(gWindow);
			//Create renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				success = false;
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
			}
		}
	}

	return success;
}

void close()
{
	SDL_FreeSurface(gSurface);
	gSurface = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_DestroyRenderer(gRenderer);

	SDL_Quit();
}

void createNet()
{
	for (int i = 0; i < 25; i++)
	{	
		SDL_RenderFillRect(gRenderer, &net);
		net.y += 29;
	}
	net.y = 0;
}

void printScore(int p1, int p2)
{
	SDL_Rect scoreRect = { 0, 30, 6, 0 };

	//Thanks for wasting 251 lines
	switch (p1)
	{
		case 0:
			scoreRect = { 160, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 183, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;

		case 1:
			scoreRect = { 183, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;

		case 2:
			scoreRect = { 160, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 183, 30, 6, 23 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 53, 6, 27 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;

		case 3:
			scoreRect = { 160, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 183, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;

		case 4:
			scoreRect = { 160, 30, 6, 23 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 183, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;

		case 5:
			scoreRect = { 160, 30, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 30, 6, 23 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 183, 53, 6, 27 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;

		case 6:
			scoreRect = { 160, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 183, 53, 6, 27 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;

		case 7:
			scoreRect = { 183, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;

		case 8:
			scoreRect = { 160, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 183, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;

		case 9:
			scoreRect = { 160, 30, 6, 23 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 183, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;

		case 10:
			scoreRect = { 160, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 183, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 160, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 143, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;

		case 11:
			scoreRect = { 153, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 183, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	}

	switch (p2)
	{
		case 0:
			scoreRect = { 451, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 474, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	
		case 1:
			scoreRect = { 474, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	
		case 2:
			scoreRect = { 451, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 474, 30, 6, 23 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 53, 6, 27 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	
		case 3:
			scoreRect = { 451, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 474, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	
		case 4:
			scoreRect = { 451, 30, 6, 23 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 474, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	
		case 5:
			scoreRect = { 451, 30, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 30, 6, 23 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 474, 53, 6, 27 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	
		case 6:
			scoreRect = { 451, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 474, 53, 6, 27 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	
		case 7:
			scoreRect = { 474, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	
		case 8:
			scoreRect = { 451, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 474, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	
		case 9:
			scoreRect = { 451, 30, 6, 23 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 474, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 53, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	
		case 10:
			scoreRect = { 451, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 474, 30, 6, 50 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 30, 23, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 451, 80, 29, 6 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 434, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	
		case 11:
			scoreRect = { 451, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			scoreRect = { 474, 30, 6, 56 };
			SDL_RenderFillRect(gRenderer, &scoreRect);
			break;
	}
}

void resetBall(Ball* ball)
{
	ball->x = 320;
	ball->y = 240;
	ball->r = 8;

	countDown = 300;
	hitCount = 0;
	if (mX > 0)
	{
		mX = 2;
	}
	else
	{
		mX = -2;
	}
}

void createBall(Ball* circle)
{
	for (int i = circle->r; i >= 0; i--)
	{
		for (int j = circle->r; j >= 0; j--)
		{
			double hDist = sqrt(pow(i, 2) + pow(j, 2));
			if (hDist <= (circle->r + 0.5))
			{
				SDL_RenderDrawPoint(gRenderer, circle->x - i, circle->y - j);
				SDL_RenderDrawPoint(gRenderer, circle->x + i, circle->y - j);
				SDL_RenderDrawPoint(gRenderer, circle->x - i, circle->y + j);
				SDL_RenderDrawPoint(gRenderer, circle->x + i, circle->y + j);
			}
		}
	}
}

void moveBall(Ball* ball, SDL_Rect* leftPaddle, SDL_Rect* rightPaddle)
{
	if (ballDelay == 0)
	{
		ball->x += mX;
		ball->y += mY;
		ballDelay = 1;
	}
	else
	{
		ballDelay--;
		return;
	}

	time_t t;
	srand((unsigned) time(&t));
	int randOffset = (rand() % 2) + 1;

	switch (checkPhysics(ball, leftPaddle, rightPaddle))
	{
		//Ball made no collisions 
		case 0:
			break;

		//Ball made collision with ceiling
		case 1:
			mY = -mY;
			ball->y = ball->r;
			break;

		//Ball made collision with floor
		case 2:
			mY = -mY;
			ball->y = WINDOW_HEIGHT - ball->r;
			break;

		//Ball made collision with left wall
		case 3:
			resetBall(ball);
			score2++;
			break;

		//Ball made collision with right wall
		case 4:
			resetBall(ball);
			score1++;
			break;

		//Ball made collision with left paddle
		case 5:
			mX = -mX;
			(mY > 0) ? mY = randOffset : mY = -randOffset;
			ball->x = (leftPaddle->x + leftPaddle->w) + ball->r;
			hitCount++;
			break;

		//Ball made collision with right paddle
		case 6:
			mX = -mX;
			(mY > 0) ? mY = randOffset : mY = -randOffset;
			ball->x = rightPaddle->x - ball->r;
			hitCount++;
			break;

		//Ball made collision with a wall after a score of 11 is reached
		case 7:
			mX = -mX;
	}

	return;
}

int checkPhysics(Ball* ball, SDL_Rect* lPaddle, SDL_Rect* rPaddle)
{
	if (ball->x <= ball->r)
	{
		if (!winState)
		{
			return 3;
		}

		return 7;
	}
	else if (ball->x >= WINDOW_WIDTH - ball->r)
	{
		if (!winState)
		{
			return 4;
		}

		return 7;
	}
	else if (ball->y <= ball->r)
	{
		return 1;
	}
	else if (ball->y >= WINDOW_HEIGHT - ball->r)
	{
		return 2;
	}
	else
	{
		if (!winState)
		{
			int cY;

			if (ball->x + ball->r >= rPaddle->x && ball->x < rPaddle->x)
			{
				if (ball->y + ball->r < rPaddle->y || ball->y - ball->r > rPaddle->y + rPaddle->h)
				{
					return 0;
				}
				else if (ball->y + ball->r >= rPaddle->y)
				{
					cY = rPaddle->y;
				}
				else if (ball->y - ball->r <= rPaddle->y + rPaddle->h)
				{
					cY = rPaddle->y + rPaddle->h;
				}
				else
				{
					cY = ball->y;
				}

				if (ball->r <= sqrt(pow(rPaddle->x - ball->x, 2) + pow(cY - ball->y, 2)))
				{
					return 6;
				}
			}
			else if (ball->x - ball->r <= lPaddle->x + lPaddle->w && ball->x > lPaddle->x + lPaddle->w)
			{
				if (ball->y + ball->r < lPaddle->y || ball->y - ball->r > lPaddle->y + lPaddle->h)
				{
					return 0;
				}
				else if (ball->y + ball->r >= lPaddle->y)
				{
					cY = lPaddle->y;
				}
				else if (ball->y - ball->r <= lPaddle->y + lPaddle->h)
				{
					cY = lPaddle->y + lPaddle->h;
				}
				else
				{
					cY = ball->y;
				}

				if (ball->r <= sqrt(pow((lPaddle->x + lPaddle->w) + ball->x, 2) + pow(cY - ball->y, 2)))
				{
					return 5;
				}
			}
		}

		return 0;
	}
}

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
		return 1;
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		int paddleSpeed = 1;

		//Creates left paddle
		SDL_Rect leftPaddle = { 15, 190, 17, 100 };
		SDL_RenderFillRect(gRenderer, &leftPaddle);
		SDL_RenderPresent(gRenderer);

		//Creates right paddle
		SDL_Rect rightPaddle = { 608, 190, 17, 100 };
		SDL_RenderFillRect(gRenderer, &rightPaddle);
		SDL_RenderPresent(gRenderer);

		//Creates ball
		Ball pongBall = {320, 240, 8};

		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				//Checks if user quits
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			//Check keyboard inputs using key states
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			//Check left inputs
			if (currentKeyStates[SDL_SCANCODE_W])
			{
				if (leftPaddle.y - paddleSpeed < 0)
				{
					leftPaddle.y = 0;
				}
				else
				{
					leftPaddle.y -= paddleSpeed;
				}
			}
			else if (currentKeyStates[SDL_SCANCODE_S])
			{
				if (leftPaddle.y + paddleSpeed > 380)
				{
					leftPaddle.y = 380;
				}
				else
				{
					leftPaddle.y += paddleSpeed;
				}
			}

			//Check right inputs
			if (currentKeyStates[SDL_SCANCODE_UP])
			{
				if (rightPaddle.y - paddleSpeed < 0)
				{
					rightPaddle.y = 0;
				}
				else
				{
					rightPaddle.y -= paddleSpeed;
				}
			}
			else if (currentKeyStates[SDL_SCANCODE_DOWN])
			{
				if (rightPaddle.y + paddleSpeed > 380)
				{
					rightPaddle.y = 380;
				}
				else
				{
					rightPaddle.y += paddleSpeed;
				}
			}

			if (countDown == 0)
			{
				moveBall(&pongBall, &leftPaddle, &rightPaddle);
			}
			else
			{
				countDown--;
			}

			if (score1 > 10 || score2 > 10)
			{
				winState = true;
			}

			//Clears screen to black
			SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(gRenderer);

			//Changes color to white
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			if (!winState)
			{
				SDL_RenderFillRect(gRenderer, &leftPaddle);
				SDL_RenderFillRect(gRenderer, &rightPaddle);
			}
			createNet();
			printScore(score1, score2);
			createBall(&pongBall);
			SDL_RenderPresent(gRenderer);

			if (mX > 0)
			{
				mX = 2 + (int) hitCount/hitChange;
			}
			else
			{
				mX = -2 - (int) hitCount / hitChange;
			}
			
			SDL_Delay(2);
		}
	}

	return 0;
}