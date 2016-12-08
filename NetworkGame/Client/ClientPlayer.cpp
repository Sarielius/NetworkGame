#include "ClientPlayer.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f


ClientPlayer::ClientPlayer(int playerID) : id(playerID), attacking(false)
{
	shape.setRadius(25.f); // Pointcount has a default value of 30.
	shape.setOrigin(shape.getRadius(), shape.getRadius()); // Origin in the middle.
	shape.setFillColor(sf::Color::White); // Set Default color, prob need to make custom sprites for the players.

	// Quick hax to determine which texture to use, handled better when clientside. Maybe.

	if (id == 0)
	{
		if (!tex.loadFromFile("../Assets/blueplayer2.png"))
		{
			printf("Failed to load player texture! Player ID: %d", id);
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
			printf("Failed to load player texture! Player ID: %d", id);
			return;
		}
		/*sprite.setTexture(tex);
		sprite.setOrigin(tex.getSize().x / 2, tex.getSize().y / 2);*/
		shape.setTexture(&tex);
		shape.setRotation(180.f);
	}

	weaponShape.setSize(sf::Vector2f(2.f, 65.f));
	weaponShape.setOrigin(weaponShape.getSize().x / 2, weaponShape.getSize().y / 2);
	weaponShape.setFillColor(sf::Color::Magenta);

	if (!weaponTex.loadFromFile("../Assets/spear1.png"))
	{
		printf("Failed to load weapon texture! Player ID: %d", id);
	}

	weaponSprite.setTexture(weaponTex);
	weaponSprite.setOrigin(weaponTex.getSize().x / 2, weaponTex.getSize().y / 2);
}


ClientPlayer::~ClientPlayer()
{
}

void ClientPlayer::update(const sf::Time& elapsed)
{
	float dx, dy, angle, len, radius, rotation;
	radius = shape.getRadius();
	rotation = shape.getRotation();

	if (!attacking)
	{
		dx = radius * 0.9f * sinf((rotation * DEGTORAD));
		dy = radius * 0.9f * cosf((rotation * DEGTORAD));

		weaponShape.setPosition(shape.getPosition().x - dx, shape.getPosition().y + dy);
	}
	if (attacking)
	{
		angle = atan2f(-radius * 2, radius);

		angle += rotation * DEGTORAD;
		len = sqrt((radius*radius) + ((radius * 2) * (radius * 2)));

		dx = len * 0.9f * sinf(angle);
		dy = len * 0.9f * cosf(angle);

		weaponShape.setPosition(shape.getPosition().x - dx, shape.getPosition().y + dy);
	}

	weaponShape.setRotation(rotation + 90);
	weaponSprite.setPosition(weaponShape.getPosition());
	weaponSprite.setRotation(weaponShape.getRotation());

	spearTipPoint = weaponShape.getPoint(1);

	sf::Transform trans;
	trans = weaponShape.getTransform();

	spearTipPoint = trans.transformPoint(spearTipPoint);
}



void ClientPlayer::draw(sf::RenderWindow& win)
{
	win.draw(weaponSprite);
	win.draw(shape);
}
