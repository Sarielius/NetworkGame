#include "ServerGame.h"

#include <stdio.h>
#include <math.h>

#include "enet\enet.h"

#include "SFML\Graphics.hpp"

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

	// SFML Init

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);


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
		window.draw(shape);
		window.display();
		
		//running = false;

	}
	

	enet_deinitialize();
}