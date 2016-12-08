#include "ClientGame.h"
#include "enet\enet.h"
#include <stdio.h>
#include <string.h>

#include "Input.h"
// Localhost IP = 127.0.0.1 try port 2302 or 8888

void ClientGame::run()
{	
	bool running = true;
	bool playerQuit = false;
	char IPaddress[20];
	
	sprintf_s(IPaddress, "127.0.0.1");

	if (enet_initialize() != 0)
	{
		printf("Fug\n");
	}

	ENetHost* client;
	client = enet_host_create(NULL, 1, 2, 0, 0);
	ENetAddress address;
	ENetPeer* server;
	
	enet_address_set_host(&address, IPaddress);
	address.port = 2302;

	server = enet_host_connect(client, &address, 2, 0);

	if (server == nullptr)
	{
		printf("No available peers for initiating an ENet connection.\n");
		//running = false;
	}

	// SFML

	sf::RenderWindow window(sf::VideoMode(screenX, screenY), "NetworkGame");
	sf::Clock clock;
	sf::Time elapsed = clock.getElapsedTime();

	// Shape initializations.
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

	// Player and enemy creation.

	ClientPlayer* player = new ClientPlayer(0);
	Input* input = new Input(player);
	ClientPlayer* enemy = new ClientPlayer(1);
	while (running)
	{
		// Receive Packet
		ENetEvent event;

		while (enet_host_service(client, &event, 0) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("Connected to server in location %s:%u.\n",
					IPaddress,
					event.peer->address.port);
				/* Store any relevant server information here. */
				event.peer->data = "Server information";
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
			}
		}
		// Update input gather data
		
		// Send Packet

		/* Create a reliable packet of size 7 containing "packet\0" */
		ENetPacket * packet = enet_packet_create("ping", strlen("ping") + 1, ENET_PACKET_FLAG_RELIABLE);

		//enet_packet_resize(packet, strlen("pingfoo") + 1);
		//strcpy_s((char*)&packet->data, strlen("pingfoo"), "foo");
		
		/* Send the packet to the peer over channel id 0. */
		/* One could also broadcast the packet by         */
		/* enet_host_broadcast (host, 0, packet);         */

		enet_peer_send(server, 0, packet);


		sf::Event sfEvent;

		while (window.pollEvent(sfEvent))
		{
			if (sfEvent.type == sf::Event::Closed)
			{
				window.close();
				running = false;
			}
		}

		window.clear();
		window.draw(backgroundShape);
		window.draw(arenaShape);

		player->draw(window);
		enemy->draw(window);

		window.display();

		player->update(elapsed);

		input->update(elapsed, window);

		elapsed = clock.restart();

		
		
	}
	
	delete input;

	enet_host_destroy(client);

	enet_deinitialize();

}
