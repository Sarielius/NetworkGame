#pragma once
#include "SFML\Graphics.hpp"

class Player
{
public:
	Player(int playerNumber);
	~Player();

	void update();
	void draw(sf::RenderWindow& win);
	void setPosition(const sf::Vector2f& pos);

	sf::CircleShape& getShape()
	{
		return shape;
	}

	int getId()
	{
		return id;
	}

private:
	sf::CircleShape shape;
	int id;
};

