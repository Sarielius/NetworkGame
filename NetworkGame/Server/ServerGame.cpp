#include "ServerGame.h"

#include "enet\enet.h"
#include "SFML\Graphics.hpp"

#include <stdio.h>
#include <stdlib.h>

/////////////// THE GRAND LIST OF TODO ///////////////
/*
	_GAME_
	Refactor for network usage, endianness etc.
	Mouse tracking and shape rotation. Done
	Weapon mechanics 
	Death Done
	Points Done
	Graphics, as in player, arena and background textures.

	_NETWORK_
	Connection Done
	Serialization // Just throw similar structs
	Packet send rate and client/server update rate // Limit bandwidth on server, send packet each frame on client.


	_THOUGHTS_
	Player creation function that sends an ID packet to new connections.
	Max players 2, ignore new connections after 2 confirmed ones.
	

*/

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

enum PacketType
{
	INIT = 0,	// ID of the player, confirmation of connection
	DATA,		// Updated positions and such from the server
	MESSAGE		// Messages such as player deaths and current score
};

struct ServerData
{
	enet_uint8 type = PacketType::DATA;

	enet_uint8 p1Attk : 1; // Whether or not the player is attacking
	enet_uint8 p2Attk : 1;

	float p1PosX; // Positions and where the player is looking.
	float p1PosY;
	float p1Angle;

	float p2PosX;
	float p2PosY;
	float p2Angle;
};

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


// Could probably do different kind of structs with a template but effort.

struct InitData
{
	enet_uint8 type = PacketType::INIT;
	enet_uint8 id;
};

struct MessageData
{
	enet_uint8 type = PacketType::MESSAGE;
	char message[50];
};



ServerGame::ServerGame() : speed(150)
{
	// SFML Init
	screenX = 800;
	screenY = 800;
}

void ServerGame::run()
{
	bool running = true;

	std::vector<ENetPeer*> peers;

	InitData initData;
	MessageData messageData;
	PlayerData playerData;
	ServerData serverData;



	// ENet init
	
	if (enet_initialize() != 0)
	{
		return;
	}

	ENetAddress adder;

	adder.host = ENET_HOST_ANY;
	adder.port = 2302;

	ENetHost *servu = enet_host_create(&adder, 32, 2, 0, 0);
	ENetEvent event;

	// Rendering stuff

	sf::RenderWindow window(sf::VideoMode(screenX, screenY), "NetworkGame");
	window.setFramerateLimit(60);
	sf::Clock clock;
	sf::Time elapsed = clock.getElapsedTime();

	
	arenaShape.setRadius(300.f);
	arenaShape.setPointCount(60);
	arenaShape.setFillColor(sf::Color::White);
	arenaShape.setOutlineColor(sf::Color::Black);
	arenaShape.setOutlineThickness(10.f);
	arenaShape.setOrigin(arenaShape.getRadius(), arenaShape.getRadius());
	arenaShape.setPosition(screenX / 2, screenY / 2);
	
	if (!arenaTex.loadFromFile("../Assets/arena.png"))
	{
		printf("Arena texture loading failed!");
	}

	arenaShape.setTexture(&arenaTex);

	backgroundShape.setSize(sf::Vector2f(screenX, screenY));
	backgroundShape.setFillColor(sf::Color::White);

	if (!backgroundTex.loadFromFile("../Assets/background3.png"))
	{
		printf("Background texture loading failed!");
	}

	backgroundShape.setTexture(&backgroundTex);

	reset();
	
	while (running)
	{
		while (enet_host_service(servu, &event, 0) > 0)
		{
			// Handle ENet events and inbound packets first on every frame.
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				/* Store any relevant client information here. */
				event.peer->data = "Client information";

				peers.push_back(event.peer);

				ENetPacket* packet;
				if (createPlayer())
				{
					
					initData.id = (id); // Better id management one day
					packet = enet_packet_create(&initData, sizeof(initData), 0);
					enet_peer_send(event.peer, 0, packet);
					id++;

					// !!! SEND INITIAL POSITION !!! 

					//updateNetworkData(playerData, serverData, elapsed);

					/*MessageData data;
					sprintf_s(data.message, "This is a test message.");
					packet = enet_packet_create(&data, sizeof(data), 0);
					enet_peer_send(event.peer, 0, packet);*/
				}
				else
				{
					sprintf_s(messageData.message, "Maximum number of players reached");
					packet = enet_packet_create(&messageData, sizeof(messageData), 0);
					enet_peer_send(event.peer, 0, packet);
				}
				reset();
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				/*printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);
				*/
				
				memcpy(&playerData, event.packet->data, event.packet->dataLength);
				updateNetworkData(playerData, serverData, elapsed);

				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%s disconnected.\n", event.peer->data);
				/* Reset the peer's client information. */
				event.peer->data = NULL;
				break;
			}
		}

		// Rest of the game loop. 

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				running = false;
			}
		}

		window.clear();
		window.draw(backgroundShape);
		window.draw(arenaShape);

		/*if (playerData.id == 0 || playerData.id == 1)
		{
			updateNetworkData(playerData, serverData, elapsed);
		}*/

		for (auto &player : playerContainer)
		{
			player->update(elapsed);
			player->draw(window);

		}

		window.display();

		/*for (auto &handler : handlers)
		{
			handler->update(elapsed, window);
		}*/

		if (!peers.empty())
		{
			for (auto &peer : peers)
			{
				ENetPacket* packet = enet_packet_create(&serverData, sizeof(serverData), 0);
				enet_peer_send(peer, 0, packet);
			}
		}

		updateState();

		elapsed = clock.restart();
		
	}

	if (!playerContainer.empty())
	{
		for (auto &player : playerContainer)
		{
			delete player;
		}

		playerContainer.clear();
	}

	if (!handlers.empty())
	{
		for (auto &handler : handlers)
		{
			delete handler;
		}

		handlers.clear();
	}

	enet_deinitialize();
}

void ServerGame::updateState()
{
	float distance;
	bool death = false;
	sf::Vector2f playerPos; // Player position
	sf::Vector2f arenaPos = arenaShape.getPosition();
	sf::Vector2f spearPos;
	sf::Vector2f otherPos;

	

	for (auto &player : playerContainer)
	{
		playerPos = player->getShape().getPosition();

		distance = sqrt((playerPos.x - arenaPos.x) * (playerPos.x - arenaPos.x) + 
			(playerPos.y - arenaPos.y) * (playerPos.y - arenaPos.y));
		
		if (distance > (arenaShape.getRadius() + player->getShape().getRadius()))
		{
			// Kill player

			death = true;

			if (player->getId() == 0)
			{
				score.player2++;
			}
			else
			{
				score.player1++;
			}

			printf(" Player ID:%d has fallen out of the arena!\n", player->getId());
			printf(" Current score:\n Player 1: %d\n Player 2: %d\n\n", score.player1, score.player2);
		}

		if (!death) // Don't check spear collision if the other player is already out of range and dead.
		{
			spearPos = player->getSpearTipPoint();

			for (int i = 0; i < playerContainer.size(); i++)
			{
				if (playerContainer[i]->getId() != player->getId())
				{
					otherPos = playerContainer[i]->getShape().getPosition();
				}
			}

			distance = sqrt((spearPos.x - otherPos.x) * (spearPos.x - otherPos.x) +
				(spearPos.y - otherPos.y) * (spearPos.y - otherPos.y));


			if (distance < player->getShape().getRadius())
			{
				death = true;

				if (player->getId() == 0)
				{
					score.player1++;
				}
				else
				{
					score.player2++;
				}

				printf(" Player ID:%d impaled his foe!\n", player->getId());
				printf(" Current score:\n Player 1: %d\n Player 2: %d\n\n", score.player1, score.player2);
			}
		}
		
	}

	if (death)
	{
		reset();
	}
}

bool ServerGame::createPlayer()
{
	if (playerCount <= 1)
	{
		playerContainer.push_back(new Player(id));
		playerCount++;
	}
	else
	{
		return false;
	}
	return true;
}


void ServerGame::reset()
{
	// Move players back to default positions.

	for (int i = 0; i < playerContainer.size(); i++)
	{
		if (playerContainer[i]->getId() == 0)
		{
			playerContainer[i]->getShape().setPosition(screenX*.25f, screenY / 2);
		}
		else
		{
			playerContainer[i]->getShape().setPosition(screenX*.75f, screenY / 2);
		}
	}
}

void ServerGame::updateNetworkData(PlayerData& pData, ServerData& sData, const sf::Time& elapsed)
{
	sf::Vector2f moveVec = { 0, 0 };
	float angle = pData.angle;
	float time = elapsed.asSeconds();
	Player* player1 = playerContainer[pData.id];
	Player* player2 = nullptr;

	if (player1->getId() == 0 && playerContainer.size() > 1)
	{
		player2 = playerContainer[1];
	}
	else if (playerContainer.size() > 1)
	{
		player2 = playerContainer[0];
	}


	sf::Vector2f pos = player1->getShape().getPosition();

	if (pData.up)
	{
		moveVec.y -= speed * time;
	}
	if (pData.down)
	{
		moveVec.y += speed * time;
	}
	if (pData.left)
	{
		moveVec.x -= speed * time;
	}
	if (pData.right)
	{
		moveVec.x += speed * time;
	}
	if (pData.sprinting)
	{
		pos += moveVec * 2.0f;
	}
	else
	{
		pos += moveVec;
	}

	if (pData.attacking)
	{
		if (player1->playerCanAttack())
		{
			player1->setAttackState(true);
			if (pData.id == 0)
			{
				sData.p1Attk = 1;
			}
			else
			{
				sData.p2Attk = 1;
			}	
		}
		else
		{
			if (pData.id == 0)
			{
				sData.p1Attk = 0;
			}
			else
			{
				sData.p2Attk = 0;
			}
		}
	}
	
	player1->getShape().setPosition(pos.x, pos.y);
	player1->getShape().setRotation(angle);

	if (pData.id == 0)
	{
		sData.p1Angle = player1->getShape().getRotation();
		sData.p1PosX = player1->getShape().getPosition().x;
		sData.p1PosY = player1->getShape().getPosition().y;

		if (player2 != nullptr)
		{
			sData.p2Angle = player2->getShape().getRotation();
			sData.p2PosX = player2->getShape().getPosition().x;
			sData.p2PosY = player2->getShape().getPosition().y;
		}
		
	}
	else
	{
		sData.p2Angle = player1->getShape().getRotation();
		sData.p2PosX = player1->getShape().getPosition().x;
		sData.p2PosY = player1->getShape().getPosition().y;

		if (player2 != nullptr)
		{
			sData.p1Angle = player2->getShape().getRotation();
			sData.p1PosX = player2->getShape().getPosition().x;
			sData.p1PosY = player2->getShape().getPosition().y;
		}
	}
}
