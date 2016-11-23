#include "InputHandler.h"
#include "SFML\Window\Keyboard.hpp"
#include "Player.h"

InputHandler::InputHandler(Player* player)
{
	owner = player;
	speed = 0.05f;
}

void InputHandler::update()
{
	moveVec = { 0, 0 };

	pos = owner->getShape().getPosition();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		moveVec.y -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		moveVec.x -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		moveVec.y += speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		moveVec.x += speed;
	}

	pos += moveVec;

	owner->getShape().setPosition(pos.x, pos.y);

}