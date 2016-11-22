#pragma once
#include "SFML\Graphics.hpp"

class Player
{
public:
	Player();
	~Player();

	void update();
	void draw(sf::RenderWindow& win);
	void setPosition(const sf::Vector2f& pos);
	sf::CircleShape& getShape();

private:
	sf::CircleShape shape;

};

