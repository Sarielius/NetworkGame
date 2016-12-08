#pragma once
#include "SFML\Graphics.hpp"
#include "ClientPlayer.h"

class ClientGame
{
public:
	ClientGame()
	{
		screenX = 800;
		screenY = 800;
	};
	~ClientGame()
	{};

	void run();

private:
	int screenX, screenY;

	sf::CircleShape arenaShape;
	sf::Texture arenaTex;
	sf::RectangleShape backgroundShape;
	sf::Texture backgroundTex;
};
