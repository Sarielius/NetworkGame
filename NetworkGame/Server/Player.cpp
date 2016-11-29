#include "Player.h"


Player::Player(int playerNumber)
{
	shape.setRadius(25.f); // Pointcount has a default value of 30.
	shape.setOrigin(shape.getRadius(), shape.getRadius()); // Origin in the middle.
	shape.setFillColor(sf::Color::White); // Set Default color, prob need to make custom sprites for the players.
	id = playerNumber;

	// Quick hax to determine which texture to use, handled better when clientside. Maybe.

	if (id == 0)
	{
		if (!tex.loadFromFile("../Assets/blueplayer2.png"))
		{
			printf("Failed to load texture! Player ID: %d", id);
			return;
		}

		shape.setTexture(&tex);

		/*sprite.setTexture(tex);
		sprite.setOrigin(tex.getSize().x / 2, tex.getSize().y / 2);*/

	}
	else
	{
		if (!tex.loadFromFile("../Assets/redplayer2.png"))
		{
			printf("Failed to load texture! Player ID: %d", id);
			return;
		}
		/*sprite.setTexture(tex);
		sprite.setOrigin(tex.getSize().x / 2, tex.getSize().y / 2);*/
		shape.setTexture(&tex);
		shape.setRotation(180.f);
	}

}


Player::~Player()
{
}

void Player::update()
{
	sprite.setPosition(shape.getPosition());
	sprite.setRotation(shape.getRotation());
}

void Player::draw(sf::RenderWindow& win)
{
	win.draw(shape);
}

void Player::setPosition(const sf::Vector2f& pos)
{
	shape.setPosition(pos);
}

