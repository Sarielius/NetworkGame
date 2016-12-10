#include "Input.h"
#include "ClientGame.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f


Input::Input(ClientPlayer* player) : owner(player), speed(150)
{
}


Input::~Input()
{
}

void Input::update(const sf::Time& elapsed, sf::RenderWindow& win, PlayerData& data)
{
	moveVec = { 0, 0 };
	float angle = 0;

	data = { 0, 0, 0, 0, 0 };

	data.id = owner->getID();

	float time = elapsed.asSeconds();

	pos = owner->getShape().getPosition();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		moveVec.y -= speed * time;
		data.up = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		moveVec.x -= speed * time;
		data.left = 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		moveVec.y += speed * time;
		
		if (data.up) // Nullify up input.
		{
			data.up = 0;
		}
		else
		{
			data.down = 1;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		moveVec.x += speed * time;

		if (data.left) // Nullify left input.
		{
			data.left = 0;
		}
		else
		{
			data.right = 1;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		data.attacking = 1;
	}
	else
	{
		data.attacking = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		pos += moveVec * 2.0f;
		data.sprinting = 1;
	}
	else
	{
		pos += moveVec;
		data.sprinting = 0;
	}

	sf::Vector2i mousePos = sf::Mouse::getPosition(win);
	sf::Vector2f playerPos = owner->getShape().getPosition();

	angle = atan2f((mousePos.y - playerPos.y), (mousePos.x - playerPos.x));

	angle *= RADTODEG;

	if (angle < 0)
	{
		angle = 360 - (-angle);
	}

	data.angle = angle;

	owner->getShape().setPosition(pos.x, pos.y);
	owner->getShape().setRotation(angle);
}