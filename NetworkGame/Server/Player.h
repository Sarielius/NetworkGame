#pragma once
#include "SFML\Graphics.hpp"

class Player
{
public:
	Player(int playerNumber);
	~Player();

	void update();
	void draw(sf::RenderWindow& win);
	void setPosition(const sf::Vector2f& pos);

	sf::CircleShape& getShape()
	{
		return shape;
	}
	
	sf::RectangleShape& getWeaponShape()
	{
		return weaponShape;
	}

	int getId()
	{
		return id;
	}
	
	void setAttackState(bool state)
	{
		attacking = state;
	}

	sf::Vector2f& getSpearTipPoint()
	{
		return spearTipPoint;
	}

private:

	int id;
	bool attacking;

	// Player
	//sf::Sprite sprite;
	sf::Texture tex;
	sf::CircleShape shape;

	// Weapon
	sf::RectangleShape weaponShape;
	sf::CircleShape debugShape;
	sf::Texture weaponTex;
	sf::Sprite weaponSprite;
	
	sf::Vector2f spearTipPoint;
};

