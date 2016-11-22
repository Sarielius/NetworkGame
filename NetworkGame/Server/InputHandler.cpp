#include "InputHandler.h"
#include "SFML\Window\Keyboard.hpp"
#include "Player.h"

InputHandler::InputHandler(Player* player)
{
	owner = player;
	speed = 0.05f;
}

void InputHandler::update() // Children, avert thine eyes.
{
	// PSEUDOA
	// bool w, a, s ,d = false;
	// if (is pressed (a))
	// {
	//		a = true;
	// }
	//  
	// HANDLING
	// if ( a && w )
	// if ( a && !w && !s)
	
	// How to not hax?

	pos = owner->getShape().getPosition();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		owner->getShape().setPosition(pos.x - speed, pos.y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		owner->getShape().setPosition(pos.x + speed, pos.y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		owner->getShape().setPosition(pos.x, pos.y + speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		owner->getShape().setPosition(pos.x, pos.y - speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		owner->getShape().setPosition(pos.x - speed, pos.y + speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		owner->getShape().setPosition(pos.x - speed, pos.y - speed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		owner->getShape().setPosition(pos.x + speed, pos.y + speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		owner->getShape().setPosition(pos.x + speed, pos.y - speed);
	}
}