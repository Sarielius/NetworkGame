#include "Player.h"


Player::Player(int playerNumber)
{
	shape.setRadius(25.f); // Pointcount has a default value of 30.
	shape.setOrigin(shape.getRadius(), shape.getRadius()); // Origin in the middle.
	shape.setFillColor(sf::Color::Blue); // Set Default color, prob need to make custom sprites for the players.
	id = playerNumber;
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

