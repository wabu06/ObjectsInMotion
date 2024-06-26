#include "objects.hpp"


void object::resizeObject(float s, sf::Vector2f&& winSize)
{
	float winWidth = winSize.x;
			
	float winHeight = winSize.y;
			
	shape->scale(s, s);
			
	sf::FloatRect rect = shape->getGlobalBounds();
			
	bool tooBig = (rect.width >= winWidth) || (rect.height >= winHeight);
			
	if(tooBig) {
		shape->scale(1.0f / s, 1.0f / s);
		return;
	}
			
	if(rect.top <= 0) // object at top of window
		shape->setPosition(rect.left, 1);
			
	rect = shape->getGlobalBounds();
			
	if(rect.left <= 0) // object at left side of window
		shape->setPosition(1, rect.top);
			
	rect = shape->getGlobalBounds();
			
	float right = rect.left + rect.width;

	if(right >= winWidth) // object at right side of window
		shape->setPosition(winWidth - rect.width - 3.0f, rect.top);
			
	rect = shape->getGlobalBounds();
			
	float bottom = rect.top + rect.height;

	if(bottom >= winHeight) // object at bottom of window
		shape->setPosition(rect.left, winHeight - rect.height);
}

void object::moveObject(sf::Time& delta, unsigned int winWidth, unsigned int winHeight)
{
	//if(factor > 1.0f) std::cout << factor << '\n'; //sf::Time delta = clock.restart();
	
	float d30 = delta.asSeconds() * 35;

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
	
	//float xoff = (rect.width - tRect.width) / 2;
	//float yoff = (rect.height - tRect.height) / 2;
	
	//sf::Vector2f sPos = shape->getPosition();
	
	//sf::Vector2f tPos = sPos + sf::Vector2f(xoff, yoff) - sf::Vector2f(0, tRect.height / 2);
	
	//text.setPosition(tPos);
	
	float tTop = rect.top - (tRect.height + tRect.height / 2);
	
	float leftOff = (rect.width - tRect.width) / 2;
	
	text.setPosition(sf::Vector2f(rect.left + leftOff, tTop));
}
