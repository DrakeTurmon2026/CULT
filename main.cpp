#include <SDL.h>
#include <iostream>
#include "ObjectClasses.cpp"

void DoRenderStuff(person ToDraw[], SDL_Renderer* rend, int length, int col1[], int col2[], SDL_Rect Camera, SDL_Texture* Sprite) {
	//clear render
	SDL_RenderClear(rend);

	//render background
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderFillRect(rend, NULL);

	//Render Main shapes
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
	for (size_t i = 0; i < length; i++)
	{
		if (ToDraw[i].Health > 0)
		{
			ToDraw[i].RenderAsSprite(rend, ToDraw[i].team == 1 ? col1 : col2, Camera, Sprite);
		}
	}
	SDL_RenderPresent(rend);
}

int main(int argc, char* argv[]) {
	//Create the window and renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("CULT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	//For rendering characters as sprites
	SDL_Surface* CultistSprite = SDL_LoadBMP("Cultist.bmp");
	SDL_SetColorKey(CultistSprite, SDL_TRUE, (0,0,0));
	SDL_Texture* CultistTexture = SDL_CreateTextureFromSurface(renderer, CultistSprite);

	//Literally Just Trees

	//Set the window BG color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_RenderClear(renderer);

	//Draw the background color to the window's renderer
	SDL_RenderPresent(renderer);

	//Fixes issues with time
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	//fixes issues with random
	srand(time(NULL));

	//Team Colors
	int col1[] = { 255,0,0 };
	int col2[] = { 0,0,255 };

	//Camera rect
	SDL_Rect camerarect = {0,0,1600,900};

	//Mouse Camera Drag
	int MDX = 0;
	int MDY = 0;

	bool IsCameraMove = false;

	//Make 100 Characters
	person OgList[100];
	for (size_t i = 0; i < sizeof(OgList) / sizeof(person); i++)
	{
		OgList[i] = person(rand() % 1600 - 800, rand() % 900 - 450, 32, 32, i % 2);
	}

	//Main Loop
	bool running = true;
	while (running) {
		//Handle events
		SDL_Event curevent;

		while (SDL_PollEvent(&curevent) != 0) {
			switch (curevent.type)
			{
			case SDL_QUIT:
				//Quit ALL code when window closed
				running = false;
				return 0;
			case SDL_MOUSEBUTTONUP:
				if (curevent.button.button == 3)
				{
					IsCameraMove = false;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (curevent.button.button == 3)
				{
					IsCameraMove = true;
				}
				break;
			default:
				break;
			}
		}

		//Delta time
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

		//Mouse Delta (For Camera)
		SDL_GetRelativeMouseState(&MDX, &MDY);

		if (IsCameraMove)
		{
			camerarect.x -= MDX;
			camerarect.y -= MDY;
		}

		person templist[sizeof(OgList) / sizeof(person)];
		//RenderMyGarbage.RFE
		for (size_t i = 0; i < sizeof(OgList) / sizeof(person); i++)
		{
			templist[i] = OgList[i].Update(deltaTime / 1000, OgList, sizeof(OgList) / sizeof(person));
			OgList[i] = templist[i];

		}

		DoRenderStuff(OgList, renderer, sizeof(OgList) / sizeof(person), col1, col2, camerarect, CultistTexture);

		//WAIT FOR THE THING BECAUSE FPS IS BAD RIP GPU HAHAHA
		SDL_Delay(16);
	}

	return 0;
}