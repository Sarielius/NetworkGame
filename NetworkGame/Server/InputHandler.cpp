#include "InputHandler.h"
#include "SFML\Graphics.hpp"
#include "Player.h"

InputHandler::InputHandler(Player* player)
{
	owner = player;
	speed = 150.f;
}

void InputHandler::update(const sf::Time& elapsed)
{
	moveVec = { 0, 0 };

	float time = elapsed.asSeconds();

	pos = owner->getShape().getPosition();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		moveVec.y -= speed * time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		moveVec.x -= speed * time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		moveVec.y += speed * time;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		moveVec.x += speed * time;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		pos += moveVec * 2.0f;
	}
	else
	{
		pos += moveVec;
	}
	

	owner->getShape().setPosition(pos.x, pos.y);

}