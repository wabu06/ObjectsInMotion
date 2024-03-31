#pragma once


#include<stdint.h>

#include<iostream>
#include<memory>
#include<string>
//#include<utility>

#include<SFML/Graphics.hpp>


class object
{
	std::shared_ptr<sf::Shape> shape;
	
	std::string alias;
	sf::Color color; // original color

	std::string name;
	sf::Text text;
	
	float xvel, yvel;
	
	bool visible;
	
	static object& setProperties(object& obj, std::string& name, float xpos, float ypos, float sx, float sy, int R, int G, int B)
	{
		obj.visible = true;
		
		obj.display = &object::drawObject;

		obj.alias = "";
		obj.name = name;
	
		obj.color = sf::Color(R, G, B);
		
		obj.shape->setPosition(xpos, ypos);
		obj.shape->setFillColor(obj.color);
		obj.xvel = sx; obj.yvel = sy;
		
		return obj;
	}
	
	public:
		object() = default;
		~object() = default;
		
		void (object::* display)(sf::RenderWindow&);
		
		object(const object& obj)
			:
			shape(obj.shape), alias(obj.alias), color(obj.color), name(obj.name), xvel(obj.xvel), yvel(obj.yvel), visible(obj.visible),
			display(obj.display) {}

		object(object&& obj)
			:
			shape(obj.shape), alias(obj.alias), color(obj.color), name(obj.name), xvel(obj.xvel), yvel(obj.yvel), visible(obj.visible),
			display(obj.display) {}
		
		object& operator=(const object& obj)
		{
			this->shape = obj.shape;
			this->alias = obj.alias;
			this->color = obj.color;
			this->name = obj.name;
			this->xvel = obj.xvel;
			this->yvel = obj.yvel;
			this->visible = obj.visible;
			this->display = obj.display;
			
			return *this;
		}
		
		object& operator=(object&& obj)
		{
			this->shape = obj.shape;
			this->alias = obj.alias;
			this->color = obj.color;
			this->name = obj.name;
			this->xvel = obj.xvel;
			this->yvel = obj.yvel;
			this->visible = obj.visible;
			this->display = obj.display;
			
			return *this;
		}
		
		static object createCircle(std::string name, float xpos, float ypos, float sx, float sy, int R, int G, int B, float radius);
		
		static object createTriangle(std::string name, float xpos, float ypos, float sx, float sy, int R, int G, int B, float radius);
		
		static object createHexagon(std::string name, float xpos, float ypos, float sx, float sy, int R, int G, int B, float radius);
		
		static object createRectangle(std::string name, float xpos, float ypos, float sx, float sy, int R, int G, int B, float w, float h);
		
		void setLabel(sf::Font& font, unsigned int size, uint8_t R, uint8_t G, uint8_t B)
		{	
			text.setFont(font);
			text.setString(name);
			text.setCharacterSize(size);
			text.setFillColor(sf::Color(R, G, B));
		}
		
		void resetColor() {
			shape->setFillColor(color);
		}
		
		std::string getName() {
			return name;
		}
		
		void setName(std::string n) {
			name = n;
			text.setString(name);
		}
		
		void setAlias(std::string&& a) {
			alias = a;
			text.setString(name + " - " + alias);
		}
		
		void clearAlias() {
			text.setString(name);
		}
		
		bool setVisible() {
			visible = visible ? false : true;
			display = display == &object::drawObject ? &object::showLabel : &object::drawObject;
			return visible;
		}
		
		bool isVisible() {
			return visible;
		}
		
		sf::Vector2f getVelocity() {
			return sf::Vector2f(xvel, yvel);
		}
		
		void setVelocity(float xv, float yv) {
			xvel = xv;
			yvel = yv;
		}
		
		void changeVelX(float v) {
			float old_velx = xvel;
			xvel += v;
			
			xvel = xvel < 0 ? old_velx : xvel;
		}
		
		void changeVelY(float v)
		{
			float old_vely = yvel;
			yvel += v;
			
			yvel = yvel < 0 ? old_vely : yvel;
		}
		
		void resizeToScale(sf::Vector2u oldWinSize, sf::Vector2u WinSize)
		{
			float xscale = (float) WinSize.x / (float) oldWinSize.x;
			float yscale = (float) WinSize.y / (float) oldWinSize.y;
		
			shape->scale(xscale, yscale);
		}
		
		void resizeObject(float s, sf::Vector2f&& winSize)
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

		void setPosition(float xpos, float ypos) {
			shape.get()->setPosition(xpos, ypos);
		}
		
		const sf::Vector2f& getPosition() const {
			return shape.get()->getPosition();
		}
		
		void moveObject(sf::Time& delta, unsigned int winWidth, unsigned int winHeight);
		
		const sf::Color& getColor() {
			return shape->getFillColor();
		}
		
		void setColor(unsigned int R, unsigned int G, unsigned int B) {
			shape->setFillColor(sf::Color(R, G, B));
		}
		
		void drawObject(sf::RenderWindow& window) {
			window.draw(*shape);
			window.draw(text);
		}
		
		void showLabel(sf::RenderWindow& window) {
			window.draw(text);
		}
};

