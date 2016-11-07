#include "SDL\SDL.h"
#include "enet\enet.h"
#include <iostream>
#include <string>


int main(int argc, char* argv[])
{

	if (enet_initialize() != 0)
	{
		std::cout << "fug" << std::endl;
	}

	ENetHost *entti;
	entti = enet_host_create(NULL, 1, 2, 0, 0);
	

	// SDL_Init initializes specified subsystems, returns 0 if successful. 
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Create window.
	SDL_Window *win = SDL_CreateWindow("Test", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Create renderer.
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (ren == nullptr)
	{
		// Cleanup window.
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Load bitmap image.
	std::string filePath = "../Assets/hello.bmp";
	SDL_Surface *bmp = SDL_LoadBMP(filePath.c_str());

	if (bmp == nullptr)
	{
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTExtureFromSurface Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == nullptr)
	{
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	for (int i = 0; i < 3; ++i)
	{
		//First clear the renderer
		SDL_RenderClear(ren);
		//Draw the texture
		SDL_RenderCopy(ren, tex, NULL, NULL);
		//Update the screen
		SDL_RenderPresent(ren);
		//Take a quick break after all that hard work
		SDL_Delay(1000);
	}

	enet_host_destroy(entti);

	enet_deinitialize();

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}