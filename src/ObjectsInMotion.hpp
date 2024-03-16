#pragma once


//#include<iostream>
#include<fstream>
#include<unordered_map>
#include<vector>

#include<imgui.h>
#include "imgui-SFML.h"

#include "objects.hpp"


//#include<string>
//#include<utility>

//#include<SFML/Graphics.hpp>


class ObjectsInMotionEngine
{
	std::string emsg;
	
	int status; // shutdown status
	
	bool done;
	
	//bool paused;
	
	sf::Font font;
	
	std::string font_file;
	unsigned int font_size;
	unsigned int fR, fG, fB; // font color
	
	unsigned int oldWinWidth, oldWinHeight;
	unsigned int winWidth, winHeight;
	
	//sf::RenderWindow window(sf::VideoMode(800, 600), "Objects In Motion");
	sf::RenderWindow window;
	
	//std::vector<std::string> objNames;
	std::unique_ptr<char*[]> objNamesUptr;
	
	std::unordered_map<std::string, object> objects;
	
	bool (sf::RenderWindow::*getEvents)(sf::Event& event);
	
	void processInputs();
	
	void processImguiEvents() {}
	
	void (ObjectsInMotionEngine::*updatePtr) (sf::Time&);
	
	void resizeUpdate(sf::Time& delta)
	{
		for(auto& [n, o]: objects)
			o.resizeToScale(sf::Vector2u(oldWinWidth, oldWinHeight), sf::Vector2u(winWidth, winHeight));
		
		for(auto& [n, o]: objects)
			o.moveObject(delta, winWidth, winHeight);
		
		updatePtr = &ObjectsInMotionEngine::update;
	}
	
	void update(sf::Time& delta) {
		for(auto& [n, o]: objects)
			o.moveObject(delta, winWidth, winHeight);
	}
	
	void handleImgui(sf::Time& delta)
	{
		ImGui::SFML::Update(window, delta);
		
		char** names = objNamesUptr.get();
		
		//std::cout << IM_ARRAYSIZE(items) << '\n';
		
		static int select = 1;
		
		//char* elem = objNamesUptr[2];

		ImGui::Begin("Control Panel");
			ImGui::Combo("##", &select, names, objects.size()); //IM_ARRAYSIZE(items));
			bool hide = ImGui::Button("Show/Hide Selected Shape");
		ImGui::End();
		
		if(hide)
			objects[ std::string(names[select]) ].setVisible();
	}
	
	void render()
	{
		window.clear(sf::Color::Black);

		for(auto& [n, o]: objects)
		{
			if( o.isVisible() )
				o.drawObject(window);
		}
		
		ImGui::SFML::Render(window);
		
		window.display();
	}
	
	public:
		ObjectsInMotionEngine(std::string config_file = "assets/config.txt");

		~ObjectsInMotionEngine() {
			if( window.isOpen() ) window.close();
		}
		
		ObjectsInMotionEngine& run()
		{
			sf::Clock clock;
			sf::Time delta = clock.restart();

			while (!done)
			{
				processInputs();
				//processImguiEvents();
				
				delta = clock.restart();
				
				handleImgui(delta);

				update(delta);
				//(this->*updatePtr)(delta);
				
				render();
			}
			
			return *this;
		}
		
		int shutdown()
		{
			ImGui::SFML::Shutdown();

			if( window.isOpen() ) window.close();
			
			if(status == EXIT_FAILURE)
				std::cerr << emsg << std::endl;
			
			return status;
		}
};

