#include "enet\enet.h"
#include <iostream>
#include <string>


int main(int argc, char* argv[])
{

	if (enet_initialize() != 0)
	{
		std::cout << "fug" << std::endl;
	}

	ENetHost *entti;
	entti = enet_host_create(NULL, 1, 2, 0, 0);

	enet_host_destroy(entti);

	enet_deinitialize();


	return 0;
}