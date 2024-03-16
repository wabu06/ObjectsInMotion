#include "objects.hpp"


object object::createCircle(std::string name, float xpos, float ypos, float sx, float sy, int R, int G, int B, float radius)
{
	object obj;
	
	obj.visible = true;

	obj.name = name;

	obj.shape = std::make_shared<sf::CircleShape>(radius);

	obj.shape.get()->setPosition(xpos, ypos);
	obj.shape.get()->setFillColor(sf::Color(R, G, B));
	obj.xvel = sx; obj.yvel = sy;

	return obj;
}

object object::createTriangle(std::string name, float xpos, float ypos, float sx, float sy, int R, int G, int B, float radius)
{
	object obj;
	
	obj.visible = true;

	obj.name = name;

	obj.shape = std::make_shared<sf::CircleShape>(radius, 3);

	obj.shape.get()->setPosition(xpos, ypos);
	obj.shape.get()->setFillColor(sf::Color(R, G, B));
	obj.xvel = sx; obj.yvel = sy;

	return obj;
}

object object::createHexagon(std::string name, float xpos, float ypos, float sx, float sy, int R, int G, int B, float radius)
{
	object obj;
	
	obj.visible = true;

	obj.name = name;

	obj.shape = std::make_shared<sf::CircleShape>(radius, 6);

	obj.shape.get()->setPosition(xpos, ypos);
	obj.shape.get()->setFillColor(sf::Color(R, G, B));
	obj.xvel = sx; obj.yvel = sy;

	return obj;
}

object object::createRectangle(std::string name, float xpos, float ypos, float sx, float sy, int R, int G, int B, float w, float h)
{
	object obj;
	
	obj.visible = true;

	obj.name = name;

	obj.shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(w, h));

	obj.shape.get()->setPosition(xpos, ypos);
	obj.shape.get()->setFillColor(sf::Color(R, G, B));
	obj.xvel = sx; obj.yvel = sy;

	return obj;
}

void object::moveObject(sf::Time& delta, unsigned int winWidth, unsigned int winHeight)
{
	//sf::Time delta = clock.restart();
	
	float d30 = delta.asSeconds() * 35;
	
	//std::cout << delta.asSeconds() << '\n';

	shape.get()->move(d30 * xvel, d30 * yvel);

	sf::FloatRect rect = shape.get()->getGlobalBounds();

	if(rect.left <= 0) { // object at left side of window
		xvel *= -1;
		shape.get()->move(d30 * xvel, 0);
	}

	if(rect.top <= 0) { // object at top of window
		yvel *= -1;
		shape.get()->move(0, d30 * yvel);
	}

	float right = rect.left + rect.width;

	if(right >= (float) winWidth) { // object at right side of window
		xvel *= -1;
		shape.get()->move(d30 * xvel, 0);
	}

	float bottom = rect.top + rect.height;

	if(bottom >= (float) winHeight) { // object at bottom of window
		yvel *= -1;
		shape.get()->move(0, d30 * yvel);
	}

		// calculate text position
	sf::FloatRect tRect = text.getGlobalBounds();
	
	float xoff = (rect.width - tRect.width) / 2;
	float yoff = (rect.height - tRect.height) / 2;
	
	sf::Vector2f sPos = shape->getPosition();
	
	sf::Vector2f tPos = sPos + sf::Vector2f(xoff, yoff) - sf::Vector2f(0, tRect.height / 2); //sf::Vector2f(tRect.width / 2, 0); 
	//sf::Vector2f tPos = sPos + sf::Vector2f(xoff, yoff) - sf::Vector2f((tRect.width / 2) * -1, tRect.height / 2);
	
	text.setPosition(tPos);
}