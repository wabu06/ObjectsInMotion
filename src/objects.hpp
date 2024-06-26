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
	
	sf::Color color; // original color

	std::string alias;
	std::string name;
	sf::Text text;
	
	float xvel, yvel;
	
	bool visible;
	
	public:
		object() = default;
		~object() = default;
		
		void (object::* display)(sf::RenderWindow&);
		
		object(const object& obj)
			:
			shape(obj.shape), color(obj.color), alias(obj.alias), name(obj.name), text(obj.text), xvel(obj.xvel), yvel(obj.yvel),
			visible(obj.visible),
			display(obj.display) {}

		object(object&& obj)
			:
			shape(obj.shape), color(obj.color), alias(obj.alias), name(obj.name), text(obj.text), xvel(obj.xvel), yvel(obj.yvel),
			visible(obj.visible),
			display(obj.display) {}
		
		object& operator=(const object& obj)
		{
			this->shape = obj.shape;
			this->alias = obj.alias;
			this->color = obj.color;
			this->name = obj.name;
			this->text = obj.text;
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
			this->text = obj.text;
			this->xvel = obj.xvel;
			this->yvel = obj.yvel;
			this->visible = obj.visible;
			this->display = obj.display;
			
			return *this;
		}
		
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
		
		void setAlias(std::string&& a)
		{
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
		
		void resizeObject(float s, sf::Vector2f&& winSize);

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
		
		class Creator;
};

class object::Creator
{
	object obj;

	sf::Color color = sf::Color(255, 255, 255); // original color

	std::string alias = "";
	std::string name = "";
	sf::Text text;
	
	float xvel{0.0f}, yvel{0.0f};
			
	float xpos{0.0f}, ypos{0.0f};
	
	bool visible{true};
	
	void setPositionAndColor() {
		obj.shape->setPosition(xpos, ypos);
		obj.shape->setFillColor(color);
	}
			
	void (object::* display)(sf::RenderWindow&) = &object::drawObject;

	public:
		Creator() = default;
		virtual ~Creator() = default;
				
		Creator& setColor(const sf::Color&& color) {
			this->color = color;
			return *this;
		}
			
		Creator& setColor(const sf::Color& color) {
			this->color = color;
			return *this;
		}
				
		Creator& setName(const std::string& name) {
			this->name = name;
			return *this;
		}
			
		Creator& setName(const std::string&& name) {
			this->name = name;
			return *this;
		}
				
		Creator& setText(const sf::Text& text) {
			this->text = text;
			return *this;
		}
				
		Creator& setVelocity(float xvel, float yvel) {
			this->xvel = xvel;
			this->yvel = yvel;
			return *this;
		}
				
		Creator& setPosition(float xpos, float ypos) {
			this->xpos = xpos;
			this->ypos = ypos;
			return *this;
		}
			
		Creator& finalize()
		{
			obj.color = color;

			obj.alias = alias;
			obj.name = name;
	
			obj.xvel = xvel;
			obj.yvel = yvel;
				
				//obj.xpos = xpos;
				//obj.ypos = ypos;
				
			obj.visible = visible;
				
			obj.display = display;
				
			return *this;
		}

		object createRectangle(float w = 1.0f, float h = 1.0f)
		{
			obj.shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(w, h));

			setPositionAndColor();
			
			return obj;
		}
		
		object createCircle(float radius)
		{
			obj.shape = std::make_shared<sf::CircleShape>(radius);
			
			setPositionAndColor();

			return obj;
		}
		
		object createHexagon(float length)
		{
			obj.shape = std::make_shared<sf::CircleShape>(length, 6);

			setPositionAndColor();

			return obj;
		}
		
		object createTriangle(float length)
		{
			obj.shape = std::make_shared<sf::CircleShape>(length, 3);

			setPositionAndColor();

			return obj;
		}
};

