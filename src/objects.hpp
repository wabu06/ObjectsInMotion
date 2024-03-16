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
	
	std::string name;
	sf::Text text;
	
	float xvel, yvel;
	
	bool visible;
	
	public:
		object() = default;
		~object() = default;
		
		object(const object& obj) : shape(obj.shape), name(obj.name), xvel(obj.xvel), yvel(obj.yvel), visible(obj.visible) {}

		object(const object&& obj) : shape(obj.shape), name(obj.name), xvel(obj.xvel), yvel(obj.yvel), visible(obj.visible) {}
		
		object& operator=(const object& obj)
		{
			this->shape = obj.shape;
			this->name = obj.name;
			this->xvel = obj.xvel;
			this->yvel = obj.yvel;
			this->visible = obj.visible;
			
			return *this;
		}
		
		object& operator=(const object&& obj)
		{
			this->shape = obj.shape;
			this->name = obj.name;
			this->xvel = obj.xvel;
			this->yvel = obj.yvel;
			this->visible = obj.visible;
			
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
		
		std::string getName() {
			return name;
		}
		
		void setName(std::string n) {
			name = n;
			text.setString(name);
		}
		
		bool setVisible() {
			visible = visible ? false : true;
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
			xvel += v;
		}
		
		void changeVelY(float v) {
			yvel += v;
		}
		
		void resizeToScale(sf::Vector2u oldWinSize, sf::Vector2u WinSize)
		{
			float xscale = (float) WinSize.x / (float) oldWinSize.x;
			float yscale = (float) WinSize.y / (float) oldWinSize.y;
		
			shape->scale(xscale, yscale);
		}

		void setPosition(float xpos, float ypos) {
			shape.get()->setPosition(xpos, ypos);
		}
		
		const sf::Vector2f& getPosition() const {
			return shape.get()->getPosition();
		}
		
		void moveObject(sf::Time& delta, unsigned int winWidth, unsigned int winHeight);
		
		void setColor(int R, int G, int B) {
			shape.get()->setFillColor(sf::Color(R, G, B));
		}
		
		void drawObject(sf::RenderWindow& window) {
			window.draw(*shape);
			window.draw(text);
		}
};

