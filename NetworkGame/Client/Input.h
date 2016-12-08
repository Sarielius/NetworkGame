#pragma once
#include "SFML\Graphics.hpp"
#include "ClientPlayer.h"

class Input
{
public:
	Input(ClientPlayer* player);
	~Input();

	bool update(const sf::Time& elapsed, sf::RenderWindow& win); // Bool for now...

private:
	sf::Vector2f pos;
	sf::Vector2f moveVec;
	ClientPlayer* owner;
	float speed;
};

