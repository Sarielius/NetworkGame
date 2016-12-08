#pragma once
#include "SFML\Graphics.hpp"

class ClientPlayer
{
public:
	ClientPlayer(int playerID);
	~ClientPlayer();
	
	void update(const sf::Time& elapsed);
	void draw(sf::RenderWindow& win);

	sf::CircleShape& getShape()
	{
		return shape;
	}

private:
	int id;
	sf::Texture tex;
	sf::CircleShape shape;
	bool attacking;
	// Weapon
	sf::RectangleShape weaponShape;
	sf::Texture weaponTex;
	sf::Sprite weaponSprite;
	sf::Vector2f spearTipPoint;
};

