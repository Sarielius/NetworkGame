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


struct NetworkInfo
{
	ENetHost* client;
	ENetAddress address;
	ENetPeer* server;
	char IP[25];
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
	bool initialize(NetworkInfo& netInfo);

private:
	int screenX, screenY, myID;
	bool initDone;

	std::vector<ClientPlayer*> players;
	
	Input* input;
	sf::CircleShape arenaShape;
	sf::Texture arenaTex;
	sf::RectangleShape backgroundShape;
	sf::Texture backgroundTex;
};
