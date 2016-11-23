#include "ServerGame.h"

#include "enet\enet.h"
#include "SFML\Graphics.hpp"

#include <stdio.h>
#include <stdlib.h>

/////////////// THE GRAND LIST OF TODO ///////////////
/*
	_GAME_
	Mouse tracking and shape rotation.
	Weapon mechanics
	Death
	Points
	Graphics, as in player, arena and background textures.

	_NETWORK_
	Everything xd
	Connection
	Serialization
	Packet send rate and client/server update rate

*/


void ServerGame::run()
{
	bool running = true;

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

	sf::RenderWindow window(sf::VideoMode(screenX, screenY), "NetworkGame");
	
	// Other initializations.
	arenaShape.setRadius(300.f);
	arenaShape.setPointCount(60);
	arenaShape.setFillColor(sf::Color::Green);
	arenaShape.setOrigin(arenaShape.getRadius(), arenaShape.getRadius());
	arenaShape.setPosition(screenX / 2, screenY / 2);

	Player* player = new Player(playerCount++);
	player->getShape().setPosition(screenX*.25f, screenY / 2);
	InputHandler* handler = new InputHandler(player);

	playerContainer.push_back(player);

	Player* dummy = new Player(playerCount++);
	dummy->getShape().setPosition(screenX*.75f, screenY/2);
	dummy->getShape().setFillColor(sf::Color::Red);

	playerContainer.push_back(dummy);
	

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
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);
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
		window.draw(arenaShape);

		for (auto &player : playerContainer)
		{
			player->update();
			player->draw(window);
		}

		window.display();

		updateState();

		handler->update();
	}
	
	for (auto &player : playerContainer)
	{
		delete player;
	}

	playerContainer.clear();
	delete handler;

	enet_deinitialize();
}

void ServerGame::updateState()
{
	float distance;
	sf::Vector2f playerPos; // Player position
	sf::Vector2f arenaPos = arenaShape.getPosition();

	for (auto &player : playerContainer)
	{
		playerPos = player->getShape().getPosition();

		distance = sqrt((playerPos.x - arenaPos.x) * (playerPos.x - arenaPos.x) + (playerPos.y - arenaPos.y) * (playerPos.y - arenaPos.y));
		
		if (distance > (arenaShape.getRadius() + player->getShape().getRadius()))
		{
			// Kill player

			printf("Player ID:%d out of range!\n", player->getId());
		}
	}
}