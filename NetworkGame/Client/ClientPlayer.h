#pragma once
#include "SFML\Graphics.hpp"

class ClientPlayer
{
public:
	ClientPlayer(int myID);
	~ClientPlayer();
	
	void update(const sf::Time& elapsed);
	void draw(sf::RenderWindow& win);
	void transform(float posX, float posY, float angle, bool attk);

	int getID()
	{
		return id;
	}

	void setID(int playerID)
	{
		id = playerID;
	}

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

