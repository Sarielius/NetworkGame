#pragma once
#include "SFML\Graphics.hpp"
#include "enet\enet.h"
#include <vector>

#include "ClientPlayer.h"
#include "Input.h"


struct PlayerData
{
	enet_uint8 id : 4;
	enet_uint8 left : 1;
	enet_uint8 right : 1;
	enet_uint8 up : 1;
	enet_uint8 down : 1;
	enet_uint8 attacking : 1;
	enet_uint8 sprinting : 1;
	float angle;
};

struct ServerData;

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
	void interpolate(ServerData& sData);

private:
	int screenX, screenY;
	ClientPlayer* player;
	ClientPlayer* enemy;
	Input* input;
	sf::CircleShape arenaShape;
	sf::Texture arenaTex;
	sf::RectangleShape backgroundShape;
	sf::Texture backgroundTex;
};
