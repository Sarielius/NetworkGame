#pragma once
#include <vector>
#include <stdio.h>
#include <math.h>
#include "Player.h"
#include "InputHandler.h"

struct Score
{
	int player1 = 0;
	int player2 = 0;
};


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
	void reset();

private:
	int screenX, screenY;
	Score score;
	std::vector<Player*> playerContainer;
	std::vector<InputHandler*> handlers;
	sf::CircleShape arenaShape;
};

