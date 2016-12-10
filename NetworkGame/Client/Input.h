#pragma once
#include "SFML\Graphics.hpp"
#include "ClientPlayer.h"

struct PlayerData;


class Input
{
public:
	Input(ClientPlayer* player);
	~Input();

	void update(const sf::Time& elapsed, sf::RenderWindow& win, PlayerData& data); // Bool for now...
	
private:
	sf::Vector2f pos;
	sf::Vector2f moveVec;
	ClientPlayer* owner;
	float speed;
};

