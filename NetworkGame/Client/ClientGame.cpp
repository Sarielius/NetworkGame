#include "ClientGame.h"
#include "enet\enet.h"
#include <stdio.h>

#include "Input.h"
// Localhost IP = 127.0.0.1 try port 2302 or 8888

void ClientGame::run()
{	
	bool running = true;
	Input* input = new Input();
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
		running = input->update();
		// Send Packet
		
	}
	
	delete input;

	enet_host_destroy(client);

	enet_deinitialize();

}
