#include "ClientGame.h"

#include <stdio.h>
#include <string.h>


// Localhost IP = 127.0.0.1 try port 2302 or 8888

enum PacketType
{
	INIT = 0,	// ID of the player, confirmation of connection
	DATA,		// Updated positions and such from the server
	MESSAGE		// Messages such as player deaths and current score
};

struct InitData
{
	enet_uint8 type = PacketType::INIT;
	enet_uint8 id;
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

struct MessageData
{
	enet_uint8 type = PacketType::MESSAGE;
	char message[256];
};



void ClientGame::run()
{	
	bool running = true;
	char IPaddress[25];

	PlayerData outboundData;
	
	InitData initData;
	ServerData serverData;
	MessageData messageData;

	NetworkInfo netInfo;

	/*printf("Enter IP:\n> ");
	gets_s(netInfo.IP);*/
	
	sprintf_s(netInfo.IP, "127.0.0.1");

	if (enet_initialize() != 0)
	{
		printf("Fug\n");
	}
	
	initDone = initialize(netInfo);

	//ENetHost* client;
	//client = enet_host_create(NULL, 1, 2, 0, 0);
	//ENetAddress address;
	//ENetPeer* server;
	//
	//enet_address_set_host(&address, IPaddress);
	//address.port = 2302;

	//server = enet_host_connect(client, &address, 2, 0);

	//if (server == nullptr)
	//{
	//	printf("\nNo available peers for initiating an ENet connection.\n");
	//	//running = false;
	//}

	// SFML

	sf::RenderWindow window(sf::VideoMode(screenX, screenY), "NetworkGame");
	window.setFramerateLimit(60);
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
		printf("\nArena texture loading failed!\n");
	}

	arenaShape.setTexture(&arenaTex);

	backgroundShape.setSize(sf::Vector2f(screenX, screenY));
	backgroundShape.setFillColor(sf::Color::White);

	if (!backgroundTex.loadFromFile("../Assets/background3.png"))
	{
		printf("\nBackground texture loading failed!\n");
	}

	backgroundShape.setTexture(&backgroundTex);

	if (initDone)
	{
		input = new Input(players[0]);

		while (running)
		{
			int type;

			// Receive Packet
			ENetEvent event;

			while (enet_host_service(netInfo.client, &event, 0) > 0)
			{
				switch (event.type)
				{
				//case ENET_EVENT_TYPE_CONNECT:
				//	printf("\nConnected to server in location %s:%u.\n",
				//		IPaddress,
				//		event.peer->address.port);
				//	/* Store any relevant server information here. */
				//	event.peer->data = "Server information";
				//	break;
				case ENET_EVENT_TYPE_RECEIVE:

					type = *event.packet->data;

					//printf("Type: %d\n", type);

					switch (type)
					{
					//case PacketType::INIT:
					//	//printf("Type: INIT\n");
					//	memcpy(&initData, event.packet->data, event.packet->dataLength);

					//	player->setID(initData.id);

					//	if (initData.id == 0)
					//	{
					//		enemy->setID(1);
					//	}
					//	else
					//	{
					//		enemy->setID(0);
					//	}

					//	printf("My ID:%d\n", player->getID());

					//	break;
					case PacketType::DATA:
						//printf("Type: DATA\n");
						memcpy(&serverData, event.packet->data, event.packet->dataLength);

						//Interpolate with old position!
						// Currently hard setting server values, not enough time to properly interpolate.
						interpolate(serverData);

						break;
					case PacketType::MESSAGE:
						//printf("Type: MESSAGE\n");
						memcpy(&messageData, event.packet->data, event.packet->dataLength);
						printf("%s\n", messageData.message);

						break;
					default:
						break;
					}

					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy(event.packet);

					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					printf("\n%s disconnected.\n", event.peer->data);
					/* Reset the peer's client information. */
					event.peer->data = NULL;
				}
			}

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

			for (auto &player : players)
			{
				player->draw(window);
				player->update(elapsed);
			}

			window.display();

			// Update input and gather data

			input->update(elapsed, window, outboundData);

			elapsed = clock.restart();

			// Send Packet
			ENetPacket* packet = enet_packet_create(&outboundData, sizeof(outboundData), ENET_PACKET_FLAG_RELIABLE);

			enet_peer_send(netInfo.server, 0, packet);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				running = false;
			}
		}
	}
	

	if (!players.empty())
	{
		for (auto &player : players)
		{
			delete player;
		}

		players.clear();

		delete input;
	}

	enet_host_destroy(netInfo.client);

	enet_deinitialize();

}


void ClientGame::interpolate(ServerData& sData)
{
	ClientPlayer* player = players[0];
	ClientPlayer* enemy = players[1];


	if (player->getID() == 0)
	{
		player->transform(sData.p1PosX, sData.p1PosY, sData.p1Angle, sData.p1Attk);
		enemy->transform(sData.p2PosX, sData.p2PosY, sData.p2Angle, sData.p2Attk);
	}
	else
	{
		enemy->transform(sData.p1PosX, sData.p1PosY, sData.p1Angle, sData.p1Attk);
		player->transform(sData.p2PosX, sData.p2PosY, sData.p2Angle, sData.p2Attk);
	}
}

bool ClientGame::initialize(NetworkInfo& netInfo)
{
	int type;
	InitData initData;

	netInfo.client = enet_host_create(NULL, 1, 2, 0, 0);
	enet_address_set_host(&netInfo.address, netInfo.IP);
	netInfo.address.port = 2302;
	netInfo.server = enet_host_connect(netInfo.client, &netInfo.address, 2, 0);


	if (netInfo.server == nullptr)
	{
		printf("No available peers for initiating an ENet connection.\n");
		return false;
	}

	ENetEvent event;

	if (enet_host_service(netInfo.client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	{
		printf("Connected to server in location %s:%u.\n",
			netInfo.IP,
			event.peer->address.port);

		if (enet_host_service(netInfo.client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_RECEIVE)
		{
			type = *event.packet->data;

			switch (type)
			{
			case PacketType::INIT:
				//printf("Type: INIT\n");
				memcpy(&initData, event.packet->data, event.packet->dataLength);

				//player->setID(initData.id);
				players.push_back(new ClientPlayer(initData.id));

				if (initData.id == 0)
				{
					players.push_back(new ClientPlayer(1));
				}
				else
				{
					players.push_back(new ClientPlayer(0));
				}

				printf("My ID:%d\n", initData.id);
			}

			enet_packet_destroy(event.packet);
		}
	}
	else
	{
		printf("Could not connect!\n");
		enet_peer_reset(netInfo.server);
		return false;
	}

	return true;
}