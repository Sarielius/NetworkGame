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

struct ServerData;
struct PlayerData;

class ServerGame
{
public:
	ServerGame();

	~ServerGame()
	{}

	void run();
	void updateState();
	bool createPlayer();
	void reset();
	void updateNetworkData(PlayerData& pData, ServerData& sData, const sf::Time& elapsed);

private:
	int screenX, screenY, playerCount, id;
	float speed;
	Score score;
	std::vector<Player*> playerContainer;
	std::vector<InputHandler*> handlers;
	sf::CircleShape arenaShape;
	sf::Texture arenaTex;
	sf::RectangleShape backgroundShape;
	
	sf::Texture backgroundTex;
};

