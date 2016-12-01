#include "Input.h"
#include "SFML\Graphics.hpp"

Input::Input()
{
}


Input::~Input()
{
}

bool Input::update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		return false;
	}

	return true;
}