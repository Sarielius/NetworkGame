#include "InputHandler.h"
#include "SFML\Graphics.hpp"
#include "Player.h"
#include <math.h>

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

InputHandler::InputHandler(Player* player) : owner(player), speed(150)
{
}

void InputHandler::update(const sf::Time& elapsed, sf::RenderWindow& win)
{
	

}