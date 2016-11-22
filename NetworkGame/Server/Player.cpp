#include "Player.h"


Player::Player()
{
	shape.setRadius(25.f); // Pointcount has a default value of 30.
	shape.setOrigin(shape.getRadius(), shape.getRadius()); // Origin in the middle.
	shape.setFillColor(sf::Color::Blue); // Set Default color, prob need to make custom sprites for the players.
}


Player::~Player()
{
}

void Player::update()
{

}

void Player::draw(sf::RenderWindow& win)
{
	win.draw(shape);
}

void Player::setPosition(const sf::Vector2f& pos)
{
	shape.setPosition(pos);
}


sf::CircleShape& Player::getShape()
{
	return shape;
}