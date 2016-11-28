#include "InputHandler.h"
#include "SFML\Graphics.hpp"
#include "Player.h"
#include <math.h>
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

	float mouseVec = sqrt((mousePos.x*mousePos.x) + (mousePos.y * mousePos.y));
	float playerVec = sqrt((playerPos.x * playerPos.x) + (playerPos.y * playerPos.y));

	angle = atan2f(mouseVec, playerVec);

	// printf("\nAngle: %f\n", angle);

	owner->getShape().setPosition(pos.x, pos.y);

}