#include "ObjectsInMotion.hpp"


ObjectsInMotionEngine::ObjectsInMotionEngine(std::string config_file)
	:
	status(EXIT_SUCCESS), done(false), objects( std::unordered_map<std::string, object>() )
{
	std::ifstream fin;

	fin.open(config_file);
			
	if(fin.fail()) {
		done = true;
		fin.close();
		emsg = "\nERROR: File config.txt Not Found!!\n";
		status = EXIT_FAILURE;
		return;
	}
	
	//std::vector<std::string> objNames;
			
	std::string setting;
			
	while(!fin.eof())
	{
		fin >> setting;

		if(fin.fail())
		{
			done = true;
			fin.close();
			emsg = "\nFile I/O Error!!\n";
			status = EXIT_FAILURE;
			return;
		}
		
		if(setting == "Hexagon")
		{
			std::string name;
	
			float xpos, ypos;
			float sx, sy; // speed
			int R, G, B; // color
			float radius;
			
			fin >> name >> xpos >> ypos >> sx >> sy >> R >> G >> B >> radius;
			
			//objNames.push_back(name);
			
			objects[name] = object::createHexagon(name, xpos, ypos, sx, sy, R, G, B, radius);
		}
		else if(setting == "Triangle")
		{
			std::string name;
	
			float xpos, ypos;
			float sx, sy; // speed
			int R, G, B; // color
			float radius;
			
			fin >> name >> xpos >> ypos >> sx >> sy >> R >> G >> B >> radius;
			
			//objNames.push_back(name);
			
			objects[name] = object::createTriangle(name, xpos, ypos, sx, sy, R, G, B, radius);
		}
		else if(setting == "Circle")
		{
			std::string name;
	
			float xpos, ypos;
			float sx, sy; // speed
			int R, G, B; // color
			float radius;
			
			fin >> name >> xpos >> ypos >> sx >> sy >> R >> G >> B >> radius;
			
			//objNames.push_back(name);
			
			objects[name] = object::createCircle(name, xpos, ypos, sx, sy, R, G, B, radius);
		}
		else if(setting == "Rectangle")
		{
			std::string name;
	
			float xpos, ypos;
			float sx, sy; // speed
			int R, G, B; // color
			float w, h;
			
			fin >> name >> xpos >> ypos >> sx >> sy >> R >> G >> B >> w >> h;
			
			//objNames.push_back(name);
			
			objects[name] = object::createRectangle(name, xpos, ypos, sx, sy, R, G, B, w, h);
		}
		else if(setting == "Window")
			fin >> winWidth >> winHeight;
		else if(setting == "Font")
			fin >> font_file >> font_size >> fR >> fG >> fB;
				
		if(fin.fail())
		{
			done = true;
			fin.close();
			emsg = "\nFile I/O Error!!\n";
			status = EXIT_FAILURE;
			return;
		}
		
		fin >> std::ws;
	}
			
	fin.close();
	
	if ( !font.loadFromFile(font_file) )
	{
		done = true;
		emsg = "\nERROR: Unable Load Font File!!\n";
		status = EXIT_FAILURE;
		return;
	}
	
	for(auto& [n, o]: objects)
		o.setLabel(font, font_size, (uint8_t)fR, (uint8_t)fG, (uint8_t)fB);

	//updatePtr = &ObjectsInMotionEngine::update;
	
	/* int i = 0;
	
	objNamesUptr = std::make_unique<char*[]>(objNames.size());
	
	for(auto& n: objNames) {
		objNamesUptr[i] = new char[n.size() + 1];
		strcpy(objNamesUptr[i], n.c_str());
		i++;
	} */

	window.create(sf::VideoMode(winWidth, winHeight), "Objects In Motion");
	
		// done to surpress warning about ignoring function's return value
	if ( !ImGui::SFML::Init(window) )
	{
		done = true;
		emsg = "\nERROR: Unable To Initialize ImGui!!\n";
		status = EXIT_FAILURE;
	}
}

void ObjectsInMotionEngine::processInputs()
{
	sf::Event event;
	
	getEvents = &sf::RenderWindow::pollEvent;

	//while (window.pollEvent(event))
	while( (window.*getEvents)(event) )
	{
		 ImGui::SFML::ProcessEvent(window, event);
		 
		 if(event.type == sf::Event::Closed) // window closed
		 	done = true;
		 else if(event.type == sf::Event::KeyPressed)
		 {
		 	if(event.key.code == sf::Keyboard::Key::Escape) { // quit if esc key has been pressed
				done = true;
				break;
			}
			else if(event.key.code == sf::Keyboard::Key::Space)
				getEvents = getEvents == &sf::RenderWindow::waitEvent ? &sf::RenderWindow::pollEvent : &sf::RenderWindow::waitEvent;
		 }
		 else if(event.type == sf::Event::Resized) // window resized
		 { 
		 	//oldWinWidth = winWidth;
		 	//oldWinHeight = winHeight;
		 	
		 	winWidth = event.size.width;
		 	winHeight = event.size.height;
		 	
		 	//window.create(sf::VideoMode(winWidth, winHeight), "Objects In Motion");
		 	
		 	//sf::Vector2u winSize = window.getSize();
		 	
		 	//winWidth = winSize.x;
		 	//winHeight = winSize.y;
		 	
		 	//updatePtr = &ObjectsInMotionEngine::resizeUpdate;
		 }
	}
}

void ObjectsInMotionEngine::handleImgui(sf::Time& delta)
{
	auto object_count = objects.size();
		
	int i{0};
		
	char* names[object_count];
	//std::unique_ptr<char[]> names[object_count];
		
	for(auto& [n, o]: objects)
	{
		names[i] = new char[n.size() + 1];
		//names[i] = std::make_unique<char[]>(n.size() + 1);
		strcpy(names[i], n.c_str());
		i++;
	}
		
	static int select = 1;
	static int xspeed = 0, yspeed = 0;
	
	ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreviewHalf
										| ImGuiColorEditFlags_NoOptions;
	//static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
	static ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		
	ImGui::SFML::Update(window, delta);

	ImGui::Begin("Control Panel");
		bool new_select = ImGui::Combo("##", &select, names, object_count);
		bool hide = ImGui::Button("Show/Hide Selected Shape");
		ImGui::Separator();
		bool xspeed_bttn_pressed = ImGui::Button("Adjust xspeed");
		ImGui::SliderInt("X", &xspeed, -8, 8);
		bool yspeed_bttn_pressed = ImGui::Button("Adjust yspeed");
			//ImGui::LabelText("##", "yspeed");
			//ImGui::SameLine();
		ImGui::SliderInt("Y", &yspeed, -8, 8);
		ImGui::Separator();
			//ImGui::LabelText("##", "Value");
		bool color_changed = ImGui::ColorEdit3("MyColor##1", (float*)&color, ImGuiColorEditFlags_DefaultOptions_);
		//ImGui::ColorPicker3("##MyColor##6", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
		//ImGui::ColorPicker3("##MyColor##6", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoAlpha);
		///ImGui::ColorPicker3("##MyColor##5", (float*)&color, ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
	ImGui::End();
	
	if(xspeed_bttn_pressed)
		objects[ std::string(names[select]) ].changeVelX( (float) xspeed);
	
	if(yspeed_bttn_pressed)
		objects[ std::string(names[select]) ].changeVelY( (float) yspeed);
	
	if(new_select) {
		xspeed = 0;
		yspeed = 0;
	}
	
	if(color_changed)
	{
		unsigned int R = (unsigned int) (color.x * 255);
		unsigned int G = (unsigned int) (color.y * 255);
		unsigned int B = (unsigned int) (color.z * 255);
		objects[ std::string(names[select]) ].setColor(R, G, B);
	}
		
	if(hide)
		objects[ std::string(names[select]) ].setVisible();
		
	for(int n = 0; n < object_count; n++)
		delete names[n];
}
