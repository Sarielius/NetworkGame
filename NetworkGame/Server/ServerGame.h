#pragma once
#include <vector>
#include <stdio.h>
#include <math.h>
#include "Player.h"
#include "InputHandler.h"

class ServerGame
{
public:
	ServerGame()
	{
		// SFML Init
		screenX = 800;
		screenY = 800;	
	}

	~ServerGame()
	{}

	void run();
	void updateState();

private:
	int screenX, screenY, playerCount;
	std::vector<Player*> playerContainer;
	sf::CircleShape arenaShape;
};

