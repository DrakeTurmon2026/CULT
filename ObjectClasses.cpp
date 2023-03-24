#include <SDL.h>
#include <cmath>
#include <iostream>
#include <random>

class Object {
public:
	SDL_Rect Transform;

	void render(SDL_Renderer* rend) {
		SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
		SDL_RenderFillRect(rend, &Transform);
	}

	Object(int IntX, int IntY, int IntH, int IntW) {
		Transform.x = IntX;
		Transform.y = IntY;
		Transform.h = IntH;
		Transform.w = IntW;
	}

	Object() = default;
};

class person {
public:
	Object GameObject;
	SDL_Rect MathedRect;
	int Health;
	int Happiness;
	int speed;
	int Goalx;
	int Goaly;
	int countdown = 0;
	int team;

	void RenderAsSprite(SDL_Renderer* rend, int col[], SDL_Rect Camera_Rect, SDL_Texture* Sprite) {
		//Translate the rect by the camera
		MathedRect.x = GameObject.Transform.x - Camera_Rect.x + (int)round(Camera_Rect.w / 2);
		MathedRect.y = GameObject.Transform.y - Camera_Rect.y + (int)round(Camera_Rect.h / 2);

		MathedRect.w = GameObject.Transform.w;
		MathedRect.h = GameObject.Transform.h;
		//Draw the sprite
		SDL_SetTextureColorMod(Sprite, col[0], col[1], col[2]);
		SDL_RenderCopy(rend, Sprite, NULL, &MathedRect);
	}

	void RenderAsSquare(SDL_Renderer* rend, int col[], SDL_Rect Camera_Rect) {
		//Translate the rect by the camera
		MathedRect.x = GameObject.Transform.x - Camera_Rect.x + (int)round(Camera_Rect.w / 2);
		MathedRect.y = GameObject.Transform.y - Camera_Rect.y + (int)round(Camera_Rect.h / 2);

		MathedRect.w = GameObject.Transform.w;
		MathedRect.h = GameObject.Transform.h;
		//Render The new translated Rect
		SDL_SetRenderDrawColor(rend, col[0], col[1], col[2], 255);
		SDL_RenderFillRect(rend, &MathedRect);
	}

	person Update(double DT, person plist[], int length) {
		//Set Goal pos to brazil
		if (Health > 0)
		{
			person* CurrentTarget = NULL;
			float LastDist = INFINITY;
			float dist = 0;
			for (size_t i = 0; i < length; i++)
			{
				if (plist[i].team != team && plist[i].Health > 0)
				{
					if (CurrentTarget == NULL)
					{
						int Xdiff = GameObject.Transform.x - plist[i].GameObject.Transform.x;
						int Ydiff = GameObject.Transform.y - plist[i].GameObject.Transform.y;
						dist = sqrt(abs(Xdiff) ^ 2 + abs(Ydiff) * 2);
						CurrentTarget = &plist[i];

						LastDist = dist;
					}
					else {
						int Xdiff = GameObject.Transform.x - plist[i].GameObject.Transform.x;
						int Ydiff = GameObject.Transform.y - plist[i].GameObject.Transform.y;
						dist = sqrt(abs(Xdiff) ^ 2 + abs(Ydiff) * 2);

						if (dist < LastDist && plist[i].Health > 0)
						{
							LastDist = dist;
							CurrentTarget = &plist[i];
						}
					}
				}
			}
			//MOVE TO THE ENEMY!
			if (CurrentTarget != NULL)
			{
				Goalx = CurrentTarget->GameObject.Transform.x;
				Goaly = CurrentTarget->GameObject.Transform.y;
			}
			else
			{
				if (countdown <= 0)
				{
					Goalx = GameObject.Transform.x + rand() % 200 - 100;
					Goaly = GameObject.Transform.y + rand() % 200 - 100;
					countdown = rand() % 3000 + 500;
				}
				else {
					countdown -= (DT * 1000);
				}
			}
			
			if (abs(dist) > 3 or CurrentTarget == NULL)
			{
				//X movement
				int differencex = Goalx - GameObject.Transform.x;
				if (differencex != 0)
				{
					GameObject.Transform.x += (int)round((differencex / abs(differencex)) * round(speed * DT));
				}
				//y movement
				int differencey = Goaly - GameObject.Transform.y;
				if (differencey != 0)
				{
					GameObject.Transform.y += (int)round((differencey / abs(differencey)) * round(speed * DT));
				}
			}

			if (CurrentTarget != NULL && abs(dist) <= 3)
			{
				CurrentTarget->Health -= 20;
			}
		}
		return *this;
	}

	person(int x, int y, int w, int h, int t) {
		//setup Game Object
		GameObject.Transform.x = x;
		GameObject.Transform.y = y;
		GameObject.Transform.w = w;
		GameObject.Transform.h = h;

		//Setup Char
		Health = 100;
		Happiness = 50;
		speed = 32;
		Goalx = 800;
		Goaly = 450;
		team = t;
	}

	person() = default;
};