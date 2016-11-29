#include "InputHandler.h"
#include "SFML\Graphics.hpp"
#include "Player.h"
#include <math.h>

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

InputHandler::InputHandler(Player* player)
{
	owner = player;
	speed = 150.f;
}

void InputHandler::update(const sf::Time& elapsed, sf::RenderWindow& win)
{
	moveVec = { 0, 0 };
	float angle = 0;

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
	
	sf::Vector2i mousePos = sf::Mouse::getPosition(win);
	sf::Vector2f playerPos = owner->getShape().getPosition();

	float angle3 = atan2f((mousePos.y - playerPos.y), (mousePos.x - playerPos.x));

	angle3 *= RADTODEG;

	if (angle3 < 0)
	{
		angle3 = 360 - (-angle3);
	}

	//printf("Angle: %f\n", angle3);

	owner->getShape().setPosition(pos.x, pos.y);
	owner->getShape().setRotation(angle3);

}