#pragma once
#include "SFML\Graphics.hpp"

class Player;

class InputHandler
{
public:

	InputHandler(Player* player);
	
	void update();

private:

	sf::Vector2f pos;
	float speed;
	Player* owner;
};

